from __future__ import annotations

import os
from pathlib import Path

import PySimpleGUI as sg
from decider_python import PlagiarismModel

DEFAULT_WEIGHTS = os.environ.get(
    "MODEL_WEIGHTS_PATH",
    "out/deep_learning/run_20260511_160716/checkpoints/model_20260511_160716_2300.bpk",
)


def _read_file(path: str) -> str:
    return Path(path).read_text(encoding="utf-8", errors="replace")


def main() -> None:
    sg.theme("SystemDefault")

    layout = [
        [
            sg.Text("Model weights:", size=(14, 1)),
            sg.Input(DEFAULT_WEIGHTS, key="-WEIGHTS-", expand_x=True),
            sg.FileBrowse("Browse", file_types=(("Burnpack", "*.bpk"), ("All", "*.*"))),
            sg.Button("Load model", key="-LOAD-MODEL-"),
        ],
        [
            sg.Text("Plagiarism score:", size=(14, 1)),
            sg.Text("—", key="-SCORE-", font=("Helvetica", 16, "bold"), expand_x=True),
        ],
        [
            sg.Text("File 1:", size=(8, 1)),
            sg.Input(key="-PATH1-", expand_x=True),
            sg.FileBrowse("Browse", key="-BROWSE1-", file_types=(("C++", "*.cpp"), ("All", "*.*"))),
        ],
        [
            sg.Text("File 2:", size=(8, 1)),
            sg.Input(key="-PATH2-", expand_x=True),
            sg.FileBrowse("Browse", key="-BROWSE2-", file_types=(("C++", "*.cpp"), ("All", "*.*"))),
        ],
        [
            sg.Multiline(
                "",
                key="-LEFT-",
                size=(55, 28),
                disabled=True,
                expand_x=True,
                expand_y=True,
                font=("Consolas", 10),
            ),
            sg.VSeparator(),
            sg.Multiline(
                "",
                key="-RIGHT-",
                size=(55, 28),
                disabled=True,
                expand_x=True,
                expand_y=True,
                font=("Consolas", 10),
            ),
        ],
        [
            sg.Button("Analyze", key="-ANALYZE-", bind_return_key=True),
            sg.Button("Reload files", key="-RELOAD-"),
            sg.Push(),
            sg.Button("Exit"),
        ],
        [sg.StatusBar("Load a model, pick two C++ files, then Analyze.", key="-STATUS-", expand_x=True)],
    ]

    window = sg.Window(
        "Plagiarinator",
        layout,
        resizable=True,
        finalize=True,
        size=(1200, 720),
    )

    model: PlagiarismModel | None = None

    def load_model() -> bool:
        nonlocal model
        weights = window["-WEIGHTS-"].get().strip()
        if not weights:
            window["-STATUS-"].update("Set a model weights path first.")
            return False
        try:
            model = PlagiarismModel(weights)
        except OSError as exc:
            window["-STATUS-"].update(f"Failed to load model: {exc}")
            model = None
            return False
        window["-STATUS-"].update(f"Loaded model from {weights}")
        return True

    def reload_file_contents() -> None:
        path1 = window["-PATH1-"].get().strip()
        path2 = window["-PATH2-"].get().strip()
        left = _read_file(path1) if path1 and Path(path1).is_file() else ""
        right = _read_file(path2) if path2 and Path(path2).is_file() else ""
        window["-LEFT-"].update(left)
        window["-RIGHT-"].update(right)

    def run_analysis() -> None:
        if model is None and not load_model():
            return

        path1 = window["-PATH1-"].get().strip()
        path2 = window["-PATH2-"].get().strip()
        if not path1 or not path2:
            window["-STATUS-"].update("Select two C++ files to compare.")
            return
        if not Path(path1).is_file() or not Path(path2).is_file():
            window["-STATUS-"].update("Both paths must point to existing files.")
            return

        reload_file_contents()
        window["-STATUS-"].update("Running analysis…")
        window.refresh()

        try:
            result = model.analyze(path1, path2)
        except ValueError as exc:
            window["-STATUS-"].update(f"Analysis failed: {exc}")
            return

        score = float(result["plagiarism_score"])
        window["-SCORE-"].update(f"{score:.4f}")
        window["-LEFT-"].update(result["file_1"]["file_content"])
        window["-RIGHT-"].update(result["file_2"]["file_content"])
        window["-STATUS-"].update(f"Compared {path1} and {path2}")

    while True:
        event, values = window.read()
        if event in (sg.WIN_CLOSED, "Exit"):
            break
        if event == "-LOAD-MODEL-":
            load_model()
        elif event in ("-BROWSE1-", "-BROWSE2-", "-PATH1-", "-PATH2-", "-RELOAD-"):
            reload_file_contents()
        elif event == "-ANALYZE-":
            run_analysis()

    window.close()


if __name__ == "__main__":
    main()
