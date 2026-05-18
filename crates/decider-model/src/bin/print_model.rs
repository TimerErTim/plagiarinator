use decider_model::init_model;

pub fn main() {
    type B = burn::backend::ndarray::NdArray;
    let device = burn::backend::ndarray::NdArrayDevice::default();
    let model = init_model::<B>(&device);
    println!("{}", model);
}