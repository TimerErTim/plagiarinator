# Self-Sourced LLM C++ Plagiarism Dataset

Single-file C++ exercise submissions with authentic, plagiarized, and labeled pairs for training/evaluation.

## Layout

```
self-sourced-llm/
  task-{1..20}/task.md          # exercise spec
  task-{1..20}/auth{1..8}.cpp   # independent authentic solutions
  task-{1..20}/auth{N}_plag{1..4}.cpp  # plagiarized variants of authN
  src/task-{1..20}/             # mirror of all .cpp submissions (loader path)
  ground-truth-static-anon.txt  # plagiarism clusters (IEEE-style)
  labels.json                   # pair-level labels with blatancy metadata
  manifest.json                 # dataset counts
```

## Counts

| Item | Count |
|------|-------|
| Tasks | 20 |
| Authentic per task | 8 |
| Plagiarized per authentic | 4 |
| Total submissions | 800 (160 auth + 640 plag) |
| Plagiarism clusters | 160 |
| Labeled pairs | 2800 (2240 plagiarized + 560 authentic) |

## Plagiarism variants

Each `authN_plagM.cpp` is derived from `authN.cpp` on the same task (never cross-auth).

| Variant | Typical edits |
|---------|----------------|
| **plag1** | Subtle: field/local renames, include reorder, `'\n'` ↔ `endl`, `push_back` ↔ `emplace_back` |
| **plag2** | Moderate: helper extract/inline, loop style change, Allman braces, filter logic reshaped |
| **plag3** | Blatant: heavy renames, casual comment (`// from lab partner`), equivalent API swaps |
| **plag4** | Very blatant: near copy-paste, shuffled headers, minimal renames |

## Ground truth (`ground-truth-static-anon.txt`)

Compatible with the existing C++ loader format. Each cluster groups one authentic source with its four plagiarized copies:

```
- task-1/auth1
auth1,auth1_plag1,auth1_plag2,auth1_plag3,auth1_plag4
```

Assignment path for loader: `src/task-N/` (files named `auth*.cpp`).

## Pair labels (`labels.json`)

Each entry has:

- `left`, `right` — paths relative to dataset root
- `label` — `"plagiarized"` or `"authentic"`
- `cluster` — plagiarism lineage (e.g. `task-3/auth2`)
- `source` — authentic file stem
- `variant` — 1–4 for plag copies (plagiarized pairs only)
- `blatancy` — `subtle` | `moderate` | `blatant` | `very_blatant`

**Plagiarized pairs:** every unordered pair within each 5-file cluster (auth + 4 plag), bidirectional.

**Authentic pairs:** cross-auth pairs on the same task (different clusters), balanced to ~560 pairs total.

## C++ feature coverage

See task specs. Features are spread across tasks: Rule of Three/Five, templates, smart pointers, virtual/CRTP, SFINAE, concurrency, etc.
