import init, {
  analyze_importance,
  compare_embeddings,
  embed_source,
  plagiarism_threshold_value,
} from "./pkg/decider_wasm.js";

const source1 = document.getElementById("source-1");
const source2 = document.getElementById("source-2");
const highlight1 = document.getElementById("highlight-1");
const highlight2 = document.getElementById("highlight-2");
const scoreValue = document.getElementById("score-value");
const decisionValue = document.getElementById("decision-value");
const thresholdLabel = document.getElementById("threshold-label");
const parseWarning = document.getElementById("parse-warning");
const analyzeBtn = document.getElementById("analyze-btn");
const statusBar = document.getElementById("status-bar");
const upload1 = document.getElementById("upload-1");
const upload2 = document.getElementById("upload-2");

let wasmReady = false;
let compareTimer = null;
let compareGeneration = 0;
let importanceInFlight = false;
let lastScore = null;
let lastDecision = null;
let highlightsActive = false;
let importanceBySide = { 1: [], 2: [] };
const embeddingCache = {
  1: { content: null, embedding: null },
  2: { content: null, embedding: null },
};

function escapeHtml(text) {
  return text
    .replaceAll("&", "&amp;")
    .replaceAll("<", "&lt;")
    .replaceAll(">", "&gt;");
}

function renderPlainHighlight(side, text) {
  const target = side === 1 ? highlight1 : highlight2;
  target.innerHTML = escapeHtml(text || "");
}

function normalizeImportanceForDisplay(spans1, spans2) {
  const allWeights = [...spans1, ...spans2].map((span) => span.weight);
  if (allWeights.length === 0) {
    return { 1: spans1, 2: spans2 };
  }

  const min = Math.min(...allWeights);
  const max = Math.max(...allWeights);
  const range = max - min;

  const toDisplayRange = (spans) => {
    if (range < 1e-9) {
      return spans.map((span) => ({ ...span, weight: 0 }));
    }
    return spans.map((span) => ({
      ...span,
      weight: (span.weight - min) / range,
    }));
  };

  return {
    1: toDisplayRange(spans1),
    2: toDisplayRange(spans2),
  };
}

function renderImportanceHighlight(side, text, spans) {
  const target = side === 1 ? highlight1 : highlight2;
  if (!text) {
    target.innerHTML = "";
    return;
  }

  const sorted = [...spans].sort((a, b) => a.start_byte - b.start_byte);
  let html = "";
  let cursor = 0;

  for (const span of sorted) {
    const start = Math.max(0, Math.min(span.start_byte, text.length));
    const end = Math.max(start, Math.min(span.end_byte, text.length));
    if (start > cursor) {
      html += escapeHtml(text.slice(cursor, start));
    }
    const alpha = 0.15 + span.weight * 0.55;
    html += `<mark class="importance" style="background: rgba(248, 113, 113, ${alpha.toFixed(
      3,
    )})">${escapeHtml(text.slice(start, end))}</mark>`;
    cursor = end;
  }

  if (cursor < text.length) {
    html += escapeHtml(text.slice(cursor));
  }

  target.innerHTML = html;
}

function syncEditorScroll(side) {
  const textarea = side === 1 ? source1 : source2;
  const highlight = side === 1 ? highlight1 : highlight2;
  highlight.scrollTop = textarea.scrollTop;
  highlight.scrollLeft = textarea.scrollLeft;
}

function invalidateEmbeddingCache(side = null) {
  if (side === 1 || side === null) {
    embeddingCache[1] = { content: null, embedding: null };
  }
  if (side === 2 || side === null) {
    embeddingCache[2] = { content: null, embedding: null };
  }
}

function clearHighlights() {
  highlightsActive = false;
  importanceBySide = { 1: [], 2: [] };
  renderPlainHighlight(1, source1.value);
  renderPlainHighlight(2, source2.value);
}

function setScore(score, decision) {
  lastScore = score;
  lastDecision = decision;
  scoreValue.textContent = score.toFixed(4);
  if (decision === "plagiarism") {
    decisionValue.textContent = "Likely plagiarism";
    decisionValue.className = "mt-2 text-sm font-medium text-plagiarism";
  } else {
    decisionValue.textContent = "Likely original";
    decisionValue.className = "mt-2 text-sm font-medium text-original";
  }
}

function showParseWarning(message) {
  parseWarning.textContent = message;
  parseWarning.classList.remove("hidden");
}

function hideParseWarning() {
  parseWarning.textContent = "";
  parseWarning.classList.add("hidden");
}

function updateHighlights() {
  if (!highlightsActive) {
    renderPlainHighlight(1, source1.value);
    renderPlainHighlight(2, source2.value);
    return;
  }

  renderImportanceHighlight(1, source1.value, importanceBySide[1]);
  renderImportanceHighlight(2, source2.value, importanceBySide[2]);
}

function scheduleCompare() {
  if (!wasmReady) {
    return;
  }

  clearTimeout(compareTimer);
  compareTimer = setTimeout(() => {
    void runCompare();
  }, 250);
}

async function getEmbeddingForSide(side, content) {
  const cache = embeddingCache[side];
  if (cache.content === content && cache.embedding) {
    return cache.embedding;
  }

  const result = embed_source(content);
  if (result.error) {
    invalidateEmbeddingCache(side);
    throw new Error(result.error);
  }

  cache.content = content;
  cache.embedding = result.embedding;
  return cache.embedding;
}

async function runCompare() {
  if (!wasmReady) {
    return;
  }

  const generation = ++compareGeneration;
  const left = source1.value;
  const right = source2.value;

  if (!left.trim() || !right.trim()) {
    if (generation !== compareGeneration) {
      return;
    }
    scoreValue.textContent = "—";
    decisionValue.textContent = "Waiting for input";
    decisionValue.className = "mt-2 text-sm font-medium text-slate-400";
    lastScore = null;
    lastDecision = null;
    hideParseWarning();
    invalidateEmbeddingCache();
    return;
  }

  try {
    const embedding1 = await getEmbeddingForSide(1, left);
    const embedding2 = await getEmbeddingForSide(2, right);
    if (generation !== compareGeneration) {
      return;
    }

    const result = compare_embeddings(embedding1, embedding2);
    if (generation !== compareGeneration) {
      return;
    }

    if (result.error) {
      showParseWarning("Parse issue on last edit — score unchanged");
      return;
    }

    hideParseWarning();
    setScore(result.score, result.decision);
    statusBar.textContent = `Similarity updated at ${new Date().toLocaleTimeString()}`;
  } catch {
    if (generation !== compareGeneration) {
      return;
    }
    showParseWarning("Parse issue on last edit — score unchanged");
  }
}

async function runImportanceAnalysis() {
  if (!wasmReady || importanceInFlight) {
    return;
  }

  importanceInFlight = true;
  analyzeBtn.disabled = true;
  statusBar.textContent = "Computing AST influence…";
  await new Promise((resolve) => setTimeout(resolve, 0));

  const result = analyze_importance(source1.value, source2.value);

  importanceInFlight = false;
  analyzeBtn.disabled = false;

  if (result.error) {
    showParseWarning(result.error);
    statusBar.textContent = "AST influence analysis failed";
    return;
  }

  hideParseWarning();
  highlightsActive = true;
  importanceBySide = normalizeImportanceForDisplay(
    result.file_1.importance,
    result.file_2.importance,
  );
  updateHighlights();
  statusBar.textContent = "AST influence highlighting applied";
}

function handleInput(side) {
  clearHighlights();
  hideParseWarning();
  invalidateEmbeddingCache(side);
  scheduleCompare();
}

function wireUpload(input, textarea, side) {
  input.addEventListener("change", async () => {
    const file = input.files?.[0];
    if (!file) {
      return;
    }
    textarea.value = await file.text();
    input.value = "";
    handleInput(side);
  });
}

function wireEditor(textarea, side) {
  textarea.addEventListener("input", () => handleInput(side));
  textarea.addEventListener("scroll", () => syncEditorScroll(side));
}

async function boot() {
  try {
    await init();
    wasmReady = true;
    const threshold = plagiarism_threshold_value();
    thresholdLabel.textContent = `Decision threshold: ${threshold.toFixed(2)}`;
    analyzeBtn.disabled = false;
    statusBar.textContent = "Model loaded. Start typing or upload two C++ files.";
    renderPlainHighlight(1, "");
    renderPlainHighlight(2, "");
  } catch (error) {
    statusBar.textContent = `Failed to load WASM module: ${error}`;
    console.error(error);
  }
}

wireEditor(source1, 1);
wireEditor(source2, 2);
wireUpload(upload1, source1, 1);
wireUpload(upload2, source2, 2);
analyzeBtn.addEventListener("click", () => {
  void runImportanceAnalysis();
});

boot();
