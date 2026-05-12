use burn::backend::NdArray;
use burn::tensor::{Device, backend::Backend as _};
use graph_deeplearning::get_model_config;

fn main() {
    // Choose the backend (e.g., Wgpu, NdArray, LibTorch, etc.)
    // Here, we'll use NdArray as an example.
    type B = NdArray<f32>;

    // Set the device (CPU or GPU)
    let device = Device::<B>::default();

    // Initialize the model from config on the specified backend and device
    let model = get_model_config().init::<B>(&device);

    // Print model summary/debug info
    println!("{}", model);
}
