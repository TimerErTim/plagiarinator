use burn::backend::Flex;
use burn::tensor::{backend::Backend as _, Device};
use graph_deeplearning::get_model_config;

fn main() {
    // Choose the backend (e.g., Wgpu, Flex, LibTorch, etc.)
    // Here, we'll use Flex as an example.
    type B = Flex;

    // Set the device (CPU or GPU)
    let device = Device::<B>::default();

    // Initialize the model from config on the specified backend and device
    let model = get_model_config().init::<B>(&device);

    // Print model summary/debug info
    println!("{}", model);
}
