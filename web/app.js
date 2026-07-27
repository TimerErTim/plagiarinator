import {
  analyzeImportance,
  compareEmbeddings,
  embedSource,
  initWasmWorker,
} from "./wasm-client.js";

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
let analyzeGeneration = 0;
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
  syncEditorScroll(side);
}

function buildByteToCharLookup(text) {
  const lookup = new Uint32Array(new TextEncoder().encode(text).length + 1);
  let byteIndex = 0;
  let charIndex = 0;
  lookup[0] = 0;
  const encoder = new TextEncoder();

  for (const char of text) {
    const charBytes = encoder.encode(char);
    for (let i = 0; i < charBytes.length; i += 1) {
      lookup[byteIndex + i] = charIndex;
    }
    byteIndex += charBytes.length;
    charIndex += char.length;
    lookup[byteIndex] = charIndex;
  }

  return lookup;
}

function mergeSpansToRuns(text, spans) {
  if (!text || spans.length === 0) {
    return [];
  }

  const lookup = buildByteToCharLookup(text);
  const weights = new Float64Array(text.length);

  for (const span of spans) {
    const startByte = Math.max(0, Math.min(span.start_byte, lookup.length - 1));
    const endByte = Math.max(startByte, Math.min(span.end_byte, lookup.length - 1));
    const start = lookup[startByte];
    const end = lookup[endByte];
    const weight = Number(span.weight);
    if (!Number.isFinite(weight)) {
      continue;
    }

    for (let i = start; i < end; i += 1) {
      weights[i] = Math.max(weights[i], weight);
    }
  }

  const runs = [];
  let index = 0;
  while (index < text.length) {
    const weight = weights[index];
    let next = index + 1;
    while (next < text.length && weights[next] === weight) {
      next += 1;
    }
    if (weight > 0) {
      runs.push({ start: index, end: next, weight });
    }
    index = next;
  }

  return runs;
}

function normalizeWeightsForDisplay(weights) {
  if (weights.length === 0) {
    return [];
  }

  const sorted = [...weights].sort((a, b) => a - b);
  const min = sorted[0];
  const max = sorted[sorted.length - 1];
  if (max - min < 1e-9) {
    return weights.map(() => 0);
  }

  const lowIndex = Math.floor((sorted.length - 1) * 0.1);
  const highIndex = Math.floor((sorted.length - 1) * 0.9);
  const floor = sorted[lowIndex];
  const ceiling = sorted[highIndex];
  const range = Math.max(ceiling - floor, 1e-9);

  return weights.map((weight) => {
    const stretched = (weight - floor) / range;
    const clamped = Math.max(0, Math.min(1, stretched));
    return Math.sqrt(clamped);
  });
}

function normalizeImportanceForDisplay(spans1, spans2, text1, text2) {
  const runs1 = mergeSpansToRuns(text1, spans1);
  const runs2 = mergeSpansToRuns(text2, spans2);
  const allWeights = [...runs1, ...runs2].map((run) => run.weight);
  const displayWeights = normalizeWeightsForDisplay(allWeights);

  let cursor = 0;
  const withDisplayWeights = (runs) =>
    runs.map((run) => ({
      ...run,
      weight: displayWeights[cursor++],
    }));

  return {
    1: withDisplayWeights(runs1),
    2: withDisplayWeights(runs2),
  };
}

function renderImportanceHighlight(side, text, runs) {
  const target = side === 1 ? highlight1 : highlight2;
  if (!text) {
    target.innerHTML = "";
    return;
  }

  let html = "";
  let cursor = 0;

  for (const run of runs) {
    const start = Math.max(0, Math.min(run.start, text.length));
    const end = Math.max(start, Math.min(run.end, text.length));
    if (start > cursor) {
      html += escapeHtml(text.slice(cursor, start));
    }
    const alpha = 0.08 + run.weight * 0.62;
    html += `<mark class="importance" style="background: rgba(248, 113, 113, ${alpha.toFixed(
      3,
    )})">${escapeHtml(text.slice(start, end))}</mark>`;
    cursor = end;
  }

  if (cursor < text.length) {
    html += escapeHtml(text.slice(cursor));
  }

  target.innerHTML = html;
  syncEditorScroll(side);
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

  const result = await embedSource(content);
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

    const result = await compareEmbeddings(embedding1, embedding2);
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

  const generation = ++analyzeGeneration;
  importanceInFlight = true;
  analyzeBtn.disabled = true;
  statusBar.textContent = "Computing AST influence…";

  try {
    const result = await analyzeImportance(source1.value, source2.value);
    if (generation !== analyzeGeneration) {
      return;
    }

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
      source1.value,
      source2.value,
    );
    updateHighlights();
    statusBar.textContent = "AST influence highlighting applied";
  } catch (error) {
    if (generation !== analyzeGeneration) {
      return;
    }
    showParseWarning(error.message);
    statusBar.textContent = "AST influence analysis failed";
  } finally {
    if (generation === analyzeGeneration) {
      importanceInFlight = false;
      analyzeBtn.disabled = false;
    }
  }
}

function handleInput(side) {
  analyzeGeneration += 1;
  if (importanceInFlight) {
    importanceInFlight = false;
    analyzeBtn.disabled = false;
    statusBar.textContent = "AST influence cancelled — source changed";
  }
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
    const { threshold } = await initWasmWorker();
    wasmReady = true;
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
