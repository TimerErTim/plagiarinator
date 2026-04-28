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

pub struct PlagiarismTestSetLoader<B: Backend> {
    plagiarized_pairs: Vec<FilePair>,
    authentic_pairs: Vec<FilePair>,
    device: B::Device,
}

impl<B: Backend> PlagiarismTestSetLoader<B> {
    pub fn iter<'a>(&'a self) -> impl Iterator<Item = PlagiarismTrainItem<B>> + use<'a, B> {
        let all_pairs = self
            .plagiarized_pairs
            .iter()
            .map(|pair| (pair, true))
            .chain(self.authentic_pairs.iter().map(|pair| (pair, false)));
        all_pairs.filter_map(|(pair, label)| {
            Some(PlagiarismTrainItem {
                graph_1: Graph::from_treesitter_ast(
                    parse_cpp_to_tree(BufReader::new(File::open(pair.left_path.clone()).expect(
                        &format!("Failed to open file: {}", pair.left_path.display()),
                    ))),
                    &self.device,
                )?,
                graph_2: Graph::from_treesitter_ast(
                    parse_cpp_to_tree(BufReader::new(File::open(pair.right_path.clone()).expect(
                        &format!("Failed to open file: {}", pair.right_path.display()),
                    ))),
                    &self.device,
                )?,
                label,
            })
        })
    }
}

pub fn make_testset_loader<B: Backend>(
    plagiarized_pairs: Vec<FilePair>,
    authentic_pairs: Vec<FilePair>,
    device: &B::Device,
) -> PlagiarismTestSetLoader<B> {
    PlagiarismTestSetLoader {
        plagiarized_pairs,
        authentic_pairs,
        device: device.clone(),
    }
}

pub fn make_trainset_loader<B: Backend>(
    plagiarized_pairs: Vec<FilePair>,
    authentic_pairs: Vec<FilePair>,
    mut rng: impl Rng + Send + 'static + UnwindSafe,
    device: &B::Device,
) -> impl Iterator<Item = PlagiarismTrainItem<B>> + Sized + Send + 'static + UnwindSafe
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
    let items = std::iter::repeat_with(move || {
        // Pad both lists to be equal in length by repeating elements as necessary
        let max_len = plagiarized_pairs.len().max(authentic_pairs.len());

        let padded_plagiarized = (0..max_len)
            .map(|i| (&plagiarized_pairs[i % plagiarized_pairs.len()], true));

        let padded_authentic = (0..max_len)
            .map(|i| (&authentic_pairs[i % authentic_pairs.len()], false));

        let mut all_pairs = padded_plagiarized
            .chain(padded_authentic)
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
    items
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

pub trait IterUtilsExt<T>: Iterator<Item = T> {
    fn prefetch(self, buffer_size: usize) -> PrefetchIterator<Self, T>
    where
        Self: Send + 'static + UnwindSafe + Sized,
        T: Send + 'static,
    {
        PrefetchIterator::new(self, buffer_size)
    }

    fn chunked(self, chunk_size: usize) -> impl Iterator<Item = Vec<T>>
    where
        Self: Sized + 'static,
        T: 'static,
    {
        chunked_iter(self, chunk_size)
    }
}

pub fn chunked_iter<T>(
    mut iterator: impl Iterator<Item = T>,
    chunk_size: usize,
) -> impl Iterator<Item = Vec<T>>
where
    T: 'static,
{
    let mut chunk = Vec::with_capacity(chunk_size);
    std::iter::from_fn(move || {
        while let Some(item) = iterator.next() {
            chunk.push(item);
            if chunk.len() == chunk_size {
                let chunk = std::mem::replace(&mut chunk, Vec::with_capacity(chunk_size));
                return Some(chunk);
            }
        }
        None
    })
}
