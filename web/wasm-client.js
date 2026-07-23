let worker = null;
let readyPromise = null;
let nextId = 0;
const pending = new Map();

function handleWorkerMessage(event) {
  const message = event.data;

  if (message.type === "ready") {
    return;
  }

  if (message.type === "error") {
    for (const { reject } of pending.values()) {
      reject(new Error(message.error));
    }
    pending.clear();
    return;
  }

  const request = pending.get(message.id);
  if (!request) {
    return;
  }

  pending.delete(message.id);
  if (message.error) {
    request.reject(new Error(message.error));
  } else {
    request.resolve(message.result);
  }
}

function callWorker(type, payload) {
  if (!worker) {
    return Promise.reject(new Error("WASM worker is not initialized"));
  }

  const id = ++nextId;
  return new Promise((resolve, reject) => {
    pending.set(id, { resolve, reject });
    worker.postMessage({ id, type, payload });
  });
}

export function initWasmWorker() {
  if (readyPromise) {
    return readyPromise;
  }

  readyPromise = new Promise((resolve, reject) => {
    worker = new Worker(new URL("./wasm-worker.js", import.meta.url), {
      type: "module",
    });

    worker.onmessage = (event) => {
      const message = event.data;
      if (message.type === "ready") {
        resolve({ threshold: message.threshold });
        return;
      }
      if (message.type === "error") {
        reject(new Error(message.error));
        return;
      }
      handleWorkerMessage(event);
    };

    worker.onerror = (error) => {
      reject(error);
    };

    worker.onmessageerror = () => {
      reject(new Error("Failed to deserialize WASM worker message"));
    };
  });

  return readyPromise;
}

export function embedSource(content) {
  return callWorker("embed_source", { content });
}

export function compareEmbeddings(embedding1, embedding2) {
  return callWorker("compare_embeddings", { embedding1, embedding2 });
}

export function analyzeImportance(file1, file2) {
  return callWorker("analyze_importance", { file1, file2 });
}
