import init, {
  analyze_importance,
  compare_sources,
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
let lastScore = null;
let lastDecision = null;
let highlightsActive = false;
let importanceBySide = { 1: [], 2: [] };

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
  compareTimer = setTimeout(runCompare, 250);
}

function runCompare() {
  const left = source1.value;
  const right = source2.value;

  if (!left.trim() || !right.trim()) {
    scoreValue.textContent = "—";
    decisionValue.textContent = "Waiting for input";
    decisionValue.className = "mt-2 text-sm font-medium text-slate-400";
    lastScore = null;
    lastDecision = null;
    hideParseWarning();
    return;
  }

  const result = compare_sources(left, right);
  if (result.error) {
    showParseWarning("Parse issue on last edit — score unchanged");
    return;
  }

  hideParseWarning();
  setScore(result.score, result.decision);
  statusBar.textContent = `Similarity updated at ${new Date().toLocaleTimeString()}`;
}

async function runImportanceAnalysis() {
  if (!wasmReady) {
    return;
  }

  analyzeBtn.disabled = true;
  statusBar.textContent = "Computing AST influence…";

  const result = analyze_importance(source1.value, source2.value);
  analyzeBtn.disabled = false;

  if (result.error) {
    showParseWarning(result.error);
    statusBar.textContent = "AST influence analysis failed";
    return;
  }

  hideParseWarning();
  setScore(result.score, result.decision);
  highlightsActive = true;
  importanceBySide = {
    1: result.file_1.importance,
    2: result.file_2.importance,
  };
  updateHighlights();
  statusBar.textContent = "AST influence highlighting applied";
}

function handleInput() {
  clearHighlights();
  hideParseWarning();
  scheduleCompare();
}

function wireUpload(input, textarea) {
  input.addEventListener("change", async () => {
    const file = input.files?.[0];
    if (!file) {
      return;
    }
    textarea.value = await file.text();
    input.value = "";
    handleInput();
  });
}

function wireEditor(textarea, side) {
  textarea.addEventListener("input", handleInput);
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
wireUpload(upload1, source1);
wireUpload(upload2, source2);
analyzeBtn.addEventListener("click", runImportanceAnalysis);

boot();
