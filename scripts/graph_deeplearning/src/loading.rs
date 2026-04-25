use std::{
    any::Any,
    fs::File,
    io::{BufReader, Read},
    marker::PhantomData,
    panic::{catch_unwind, UnwindSafe},
    sync::{
        atomic::{AtomicUsize, Ordering},
        mpsc::Receiver,
    },
    thread::{self, JoinHandle},
};

use burn::prelude::Backend;
use data_loading::dataset_loader::FilePair;
use rand::{seq::SliceRandom, Rng, RngExt};

use crate::model::{Graph, PlagiarismTrainItem};

pub fn parse_cpp_to_tree(reader: impl Read) -> tree_sitter::Tree {
    let mut parser = tree_sitter::Parser::new();
    parser
        .set_language(&tree_sitter_cpp::LANGUAGE.into())
        .expect("Failed to set language to C++");
    let tree = parser
        .parse(
            reader
                .bytes()
                .map(|b| b.expect("Failed to read bytes"))
                .collect::<Vec<u8>>()
                .as_slice(),
            None,
        )
        .expect("Failed to parse C++ code");
    tree
}

pub fn make_trainset_loader<B: Backend>(
    plagiarized_pairs: Vec<FilePair>,
    authentic_pairs: Vec<FilePair>,
    batch_size: usize,
    mut rng: impl Rng + Send + 'static + UnwindSafe,
    device: &B::Device,
) -> impl Iterator<Item = Vec<PlagiarismTrainItem<B>>> + Sized + Send + 'static + UnwindSafe
where
    B::Device: Send + 'static + UnwindSafe,
{
    fn shuffle_pair(pair: &FilePair, rng: &mut impl Rng) -> FilePair {
        if rng.random_bool(0.5) {
            FilePair {
                left_path: pair.right_path.clone(),
                right_path: pair.left_path.clone(),
            }
        } else {
            pair.clone()
        }
    }
    let device = device.clone();
    let mut items = std::iter::repeat_with(move || {
        let mut all_pairs = plagiarized_pairs
            .iter()
            .map(|pair| (pair, true))
            .chain(authentic_pairs.iter().map(|pair| (pair, false)))
            .map(|(pair, label)| (shuffle_pair(&pair, &mut rng), label))
            .collect::<Vec<_>>();
        all_pairs.shuffle(&mut rng);
        all_pairs
    })
    .flatten()
    .filter_map(move |(pair, label)| {
        Some(PlagiarismTrainItem {
            graph_1: Graph::from_treesitter_ast(
                parse_cpp_to_tree(BufReader::new(File::open(pair.left_path.clone()).expect(
                    &format!("Failed to open file: {}", pair.left_path.display()),
                ))),
                &device,
            )?,
            graph_2: Graph::from_treesitter_ast(
                parse_cpp_to_tree(BufReader::new(File::open(pair.right_path.clone()).expect(
                    &format!("Failed to open file: {}", pair.right_path.display()),
                ))),
                &device,
            )?,
            label: label,
        })
    });
    let mut batch = Vec::with_capacity(batch_size);
    std::iter::from_fn(move || {
        while let Some(item) = items.next() {
            batch.push(item);
            if batch.len() == batch_size {
                let batch = std::mem::replace(&mut batch, Vec::with_capacity(batch_size));
                return Some(batch);
            }
        }
        None
    })
}

static PREFETCH_COUNTER: AtomicUsize = AtomicUsize::new(0);

pub struct PrefetchIterator<I: Iterator<Item = T> + Send + 'static + UnwindSafe, T: Send + 'static>
{
    handle: JoinHandle<()>,
    receiver: Receiver<Result<T, Box<dyn Any + Send>>>,
    _marker_i: PhantomData<I>,
}

impl<I: Iterator<Item = T> + Send + 'static + UnwindSafe, T: Send + 'static>
    PrefetchIterator<I, T>
{
    pub fn new(iterator: I, buffer_size: usize) -> Self {
        let (sender, receiver) = std::sync::mpsc::sync_channel(buffer_size);
        let handle = thread::Builder::new()
            .name(format!(
                "prefetch_{}",
                PREFETCH_COUNTER.fetch_add(1, Ordering::Relaxed)
            ))
            .spawn(move || {
                let panic_result = catch_unwind(|| {
                    for item in iterator {
                        if sender.send(Ok(item)).is_err() {
                            // The receiver has been dropped
                            break;
                        }
                    }
                });
                if let Err(panic) = panic_result {
                    let _ = sender.send(Err(panic));
                }
            })
            .unwrap();
        Self {
            handle,
            receiver,
            _marker_i: PhantomData,
        }
    }
}

impl<I: Iterator<Item = T> + Send + 'static + UnwindSafe, T: Send + 'static> Iterator
    for PrefetchIterator<I, T>
{
    type Item = T;

    fn next(&mut self) -> Option<Self::Item> {
        self.receiver.recv().ok().map(Result::unwrap)
    }
}

pub trait IterUtilsExt<T>: Iterator<Item = T> + Sized {
    fn prefetch(self, buffer_size: usize) -> PrefetchIterator<Self, T>
    where
        Self: Send + 'static + UnwindSafe,
        T: Send + 'static,
    {
        PrefetchIterator::new(self, buffer_size)
    }
}
