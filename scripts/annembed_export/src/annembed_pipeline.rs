use annembed::fromhnsw::kgraph::KGraph;
use annembed::fromhnsw::kgraph::kgraph_from_hnsw_all;
use annembed::prelude::*;
use hnsw_rs::prelude::*;

pub fn reduce_dimensions(vectors: &[Vec<f32>], dim: usize) -> Vec<Vec<f64>> {
    let n = vectors.len();
    assert!(n > 0, "cannot reduce dimensions of an empty dataset");

    let max_nb_connection = 32.min(n);
    let ef_c = 50;
    let nb_layer = 16.min((n as f32).ln().trunc() as usize);

    let hnsw = Hnsw::<f32, DistL2>::new(max_nb_connection, n, nb_layer, ef_c, DistL2 {});
    let data_with_id: Vec<(&Vec<f32>, usize)> = vectors.iter().zip(0..n).collect();
    hnsw.parallel_insert(&data_with_id);

    let knbn = 8.min(n.saturating_sub(1).max(1));
    let kgraph: KGraph<f32> = kgraph_from_hnsw_all(&hnsw, knbn).expect("failed to build k-graph from HNSW");

    let mut embed_params = EmbedderParams::default();
    embed_params.set_dim(dim);
    embed_params.set_nb_gradient_batch(15);

    let mut embedder = Embedder::new(&kgraph, embed_params);
    embedder.embed().expect("annembed embedding failed");

    let embedded = embedder.get_embedded_reindexed();

    embedded
        .rows()
        .into_iter()
        .map(|row| row.iter().map(|&v| v as f64).collect())
        .collect()
}
