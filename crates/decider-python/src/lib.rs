use std::path::Path;

use burn::backend::{
    autodiff::Autodiff,
    ndarray::{NdArray, NdArrayDevice},
};
use burn::store::BurnpackStore;
use burn_store::ModuleSnapshot;
use decider_model::{
    data::{analyze_plagiarism, parse_cpp_file, PlagiarismInsights},
    init_model, PlagiarismDecider,
};
use pyo3::exceptions::{PyIOError, PyValueError};
use pyo3::prelude::*;
use pyo3::types::{PyDict, PyList};

#[pyclass(unsendable)]
pub struct PlagiarismModel {
    model: PlagiarismDecider<Autodiff<NdArray>>,
}

#[pymethods]
impl PlagiarismModel {
    /// Load model weights from a Burnpack (`.bpk`) checkpoint file.
    #[new]
    fn new(weights_path: String) -> PyResult<Self> {
        if !Path::new(&weights_path).is_file() {
            return Err(PyIOError::new_err(format!(
                "model weights file not found: {weights_path}"
            )));
        }

        let device = NdArrayDevice::default();
        let mut store = BurnpackStore::from_file(&weights_path);
        let mut model = init_model::<Autodiff<NdArray>>(&device);
        model.load_from(&mut store).map_err(|e| {
            PyIOError::new_err(format!(
                "failed to load model weights from {weights_path}: {e}"
            ))
        })?;

        Ok(Self { model })
    }

    /// Parse two C++ files and run end-to-end plagiarism analysis.
    fn analyze<'py>(
        &self,
        py: Python<'py>,
        path1: &str,
        path2: &str,
    ) -> PyResult<Bound<'py, PyAny>> {
        let file1 = parse_cpp_file(path1);
        let file2 = parse_cpp_file(path2);
        let insights = analyze_plagiarism(file1, file2, self.model.clone()).map_err(|e| {
            PyValueError::new_err(e.to_string())
        })?;
        insights_to_python(py, insights)
    }
}

fn insights_to_python(py: Python<'_>, insights: PlagiarismInsights) -> PyResult<Bound<'_, PyAny>> {
    let result = PyDict::new(py);
    result.set_item("plagiarism_score", insights.plagiarism_score)?;

    result.set_item("file_1", analyzed_file_to_dict(py, insights.file_1)?)?;
    result.set_item("file_2", analyzed_file_to_dict(py, insights.file_2)?)?;

    Ok(result.into_any())
}

fn analyzed_file_to_dict(
    py: Python<'_>,
    file: decider_model::data::AnalyzedFile,
) -> PyResult<Bound<'_, PyDict>> {
    let dict = PyDict::new(py);
    dict.set_item(
        "file_path",
        file.file_path.to_string_lossy().into_owned(),
    )?;
    dict.set_item("file_content", file.file_content)?;

    let importance = PyList::empty(py);
    for (weight, range) in file.importance {
        let entry = PyDict::new(py);
        entry.set_item("weight", weight)?;
        entry.set_item("start_line", range.start_line)?;
        entry.set_item("start_column", range.start_column)?;
        entry.set_item("end_line", range.end_line)?;
        entry.set_item("end_column", range.end_column)?;
        entry.set_item("start_byte", range.start_byte)?;
        entry.set_item("end_byte", range.end_byte)?;
        importance.append(entry)?;
    }
    dict.set_item("importance", importance)?;

    Ok(dict)
}

#[pymodule]
fn decider_python(m: &Bound<'_, PyModule>) -> PyResult<()> {
    m.add_class::<PlagiarismModel>()?;
    Ok(())
}
