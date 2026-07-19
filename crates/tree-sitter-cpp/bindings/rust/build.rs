fn main() {
    let src_dir = std::path::Path::new("src");

    let mut c_config = cc::Build::new();
    c_config
        .std("c11")
        .include(src_dir)
        .warnings(false)
        .flag_if_supported("-Wno-incompatible-pointer-types");

    #[cfg(target_env = "msvc")]
    c_config.flag("-utf-8");

    if std::env::var("TARGET")
        .map(|target| target.starts_with("wasm32-unknown"))
        .unwrap_or(false)
    {
        let wasm_headers = std::env::var("DEP_TREE_SITTER_LANGUAGE_WASM_HEADERS")
            .expect("DEP_TREE_SITTER_LANGUAGE_WASM_HEADERS must be set by tree-sitter-language");

        c_config
            .include(&wasm_headers)
            .define("static_assert(...)", "");
    }

    let parser_path = src_dir.join("parser.c");
    c_config.file(&parser_path);
    println!("cargo:rerun-if-changed={}", parser_path.to_str().unwrap());

    let scanner_path = src_dir.join("scanner.c");
    c_config.file(&scanner_path);
    println!("cargo:rerun-if-changed={}", scanner_path.to_str().unwrap());

    c_config.compile("tree-sitter-cpp");
}
