extern crate cbindgen;

use std::env;

fn main() {
    let crate_dir = env::var("CARGO_MANIFEST_DIR").unwrap();
    let mut config: cbindgen::Config = Default::default();
    config.language = cbindgen::Language::Cxx;
    config.namespace = Some("subnite::rust".to_string());
    cbindgen::generate_with_config(&crate_dir, config)
      .unwrap()
      .write_to_file("target/include/rust-bindings.h");
}
