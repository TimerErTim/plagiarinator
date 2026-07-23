import init, {
  analyze_importance,
  compare_embeddings,
  embed_source,
  plagiarism_threshold_value,
} from "./pkg/decider_wasm.js";

let ready = false;

async function boot() {
  await init(new URL("./pkg/decider_wasm_bg.wasm", import.meta.url));
  ready = true;
  self.postMessage({
    type: "ready",
    threshold: plagiarism_threshold_value(),
  });
}

self.onmessage = (event) => {
  const { id, type, payload } = event.data;

  if (!ready) {
    self.postMessage({ id, error: "WASM worker is not ready yet" });
    return;
  }

  try {
    let result;
    switch (type) {
      case "embed_source":
        result = embed_source(payload.content);
        break;
      case "compare_embeddings":
        result = compare_embeddings(payload.embedding1, payload.embedding2);
        break;
      case "analyze_importance":
        result = analyze_importance(payload.file1, payload.file2);
        break;
      default:
        throw new Error(`Unknown worker request: ${type}`);
    }
    self.postMessage({ id, result });
  } catch (error) {
    self.postMessage({
      id,
      error: error instanceof Error ? error.message : String(error),
    });
  }
};

boot().catch((error) => {
  self.postMessage({
    type: "error",
    error: error instanceof Error ? error.message : String(error),
  });
});
