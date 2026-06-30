use decider_model::init_model;

pub fn main() {
    type B = burn::backend::flex::Flex;
    let device = burn::backend::flex::FlexDevice::default();
    let model = init_model::<B>(&device);
    println!("{}", model);
}
