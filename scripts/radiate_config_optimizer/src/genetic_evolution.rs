use radiate::prelude::*;

use crate::plagiarism_runner::{Config, MetricWeight};

pub struct ConfigCodec;

impl Codec<FloatChromosome, Config> for ConfigCodec {
    fn encode(&self) -> Genotype<FloatChromosome> {
        Genotype::new(vec![
            FloatChromosome::from((3, (1.0..4.0), (1.0..8.0))),
            FloatChromosome::from((5, (0.0..1.0), (0.0..1.0))),
            FloatChromosome::from((5, (0.1..5.0), (0.1..10.0))),
            FloatChromosome::from((5, (0.1..5.0), (0.1..7.5))),
        ])
    }

    fn decode(&self, genotype: &Genotype<FloatChromosome>) -> Config {
        let pqgram_p = *genotype.get(0).unwrap().get(0).allele() as i32;
        let pqgram_q = *genotype.get(0).unwrap().get(1).allele() as i32;
        let wl_subtree_jaccard_rounds = *genotype.get(0).unwrap().get(2).allele() as i32;

        let mut metric_weights = vec![];
        for i in 0..5 {
            metric_weights.push(MetricWeight {
                x_scale: *genotype.get(1).unwrap().get(i).allele() as f64,
                exponent: *genotype.get(2).unwrap().get(i).allele() as f64,
                weight: *genotype.get(3).unwrap().get(i).allele() as f64,
            });
        }
        let pqgram_weight = metric_weights[0].clone();
        let ted_weight = metric_weights[1].clone();
        let node_cosine_weight = metric_weights[2].clone();
        let bigram_cosine_weight = metric_weights[3].clone();
        let wl_subtree_jaccard_weight = metric_weights[4].clone();

        Config {
            pqgram_p,
            pqgram_q,
            wl_subtree_jaccard_rounds,
            pqgram_weight,
            ted_weight,
            node_cosine_weight,
            bigram_cosine_weight,
            wl_subtree_jaccard_weight,
        }
    }
}
