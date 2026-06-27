use std::fs;
use std::path::{Path, PathBuf};

use crate::dataset_loader::LanguageDataset;

fn is_authentic_submission(name: &str) -> bool {
    name.starts_with("auth") && name.ends_with(".cpp") && !name.contains("_plag")
}

fn is_plagiarized_submission(name: &str) -> bool {
    name.contains("_plag") && name.ends_with(".cpp")
}

fn auth_path_for_plag(task_dir: &Path, plag_name: &str) -> Option<PathBuf> {
    let stem = plag_name.strip_suffix(".cpp")?;
    let auth_stem = stem.split("_plag").next()?;
    let auth_path = task_dir.join(format!("{auth_stem}.cpp"));
    auth_path.is_file().then_some(auth_path)
}

fn all_pairs<T: Ord + Clone>(mut items: Vec<T>) -> Vec<(T, T)> {
    items.sort();
    items.dedup();
    let mut result = Vec::new();
    for i in 0..items.len() {
        for j in (i + 1)..items.len() {
            result.push((items[i].clone(), items[j].clone()));
        }
    }
    result
}

fn is_cpp_file(path: impl AsRef<Path>) -> bool {
    path.as_ref()
        .extension()
        .map(|ext| ext == "cpp" || ext == "hpp")
        .unwrap_or(false)
}

/// Load the self-sourced-llm C++ dataset from `dataset_root/src/task-*/`.
///
/// - **Authentic pairs:** every unordered pair of `auth*.cpp` files across all tasks.
/// - **Plagiarized pairs:** each `authN_plagM.cpp` paired with its source `authN.cpp`.
pub fn load_self_sourced_llm_dataset(
    dataset_root: impl AsRef<Path>,
) -> Result<LanguageDataset, String> {
    let dataset_root = dataset_root.as_ref();
    let src_root = dataset_root.join("src");

    if !src_root.is_dir() {
        return Err(format!(
            "Self-sourced dataset src directory not found: {}",
            src_root.display()
        ));
    }

    let mut parsed_dataset = LanguageDataset::new("cpp");
    let mut authentic_paths = Vec::new();

    let task_entries = fs::read_dir(&src_root).map_err(|e| {
        format!(
            "Failed to read self-sourced dataset src directory {}: {e}",
            src_root.display()
        )
    })?;

    for task_entry in task_entries {
        let task_entry =
            task_entry.map_err(|e| format!("Failed to read task directory entry: {e}"))?;
        let task_dir = task_entry.path();
        if !task_dir.is_dir() {
            continue;
        }

        let file_entries = fs::read_dir(&task_dir)
            .map_err(|e| format!("Failed to read task directory {}: {e}", task_dir.display()))?;

        for file_entry in file_entries {
            let file_entry =
                file_entry.map_err(|e| format!("Failed to read submission file entry: {e}"))?;
            let path = file_entry.path();
            if !path.is_file() || !is_cpp_file(&path) {
                continue;
            }

            let Some(name) = path.file_name().and_then(|s| s.to_str()) else {
                continue;
            };

            if is_authentic_submission(name) {
                authentic_paths.push(path);
            } else if is_plagiarized_submission(name) {
                let Some(auth_path) = auth_path_for_plag(&task_dir, name) else {
                    return Err(format!(
                        "Plagiarized submission {} has no matching authentic source in {}",
                        path.display(),
                        task_dir.display()
                    ));
                };
                parsed_dataset.add_plagiarized_pair(auth_path.clone(), path.clone());
            }
        }
    }

    if authentic_paths.is_empty() {
        return Err(format!(
            "No authentic submissions found under {}",
            src_root.display()
        ));
    }

    for (left_path, right_path) in all_pairs(authentic_paths) {
        parsed_dataset.add_authentic_pair(left_path, right_path);
    }

    Ok(parsed_dataset)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn loads_authentic_and_plagiarized_pairs() {
        let tempdir = tempfile::tempdir().unwrap();
        let root = tempdir.path();
        let task_dir = root.join("src").join("task-1");
        fs::create_dir_all(&task_dir).unwrap();

        fs::write(task_dir.join("auth1.cpp"), "int main(){}\n").unwrap();
        fs::write(task_dir.join("auth2.cpp"), "int main(){}\n").unwrap();
        fs::write(task_dir.join("auth1_plag1.cpp"), "int main(){}\n").unwrap();
        fs::write(task_dir.join("auth1_plag2.cpp"), "int main(){}\n").unwrap();

        let task_dir2 = root.join("src").join("task-2");
        fs::create_dir_all(&task_dir2).unwrap();
        fs::write(task_dir2.join("auth1.cpp"), "int main(){}\n").unwrap();

        let dataset = load_self_sourced_llm_dataset(root).unwrap();

        assert_eq!(dataset.plagiarized_pairs.len(), 2);
        assert!(dataset
            .plagiarized_pairs
            .iter()
            .all(|pair| pair.left_path.file_name().unwrap() == "auth1.cpp"));

        // C(3, 2) authentic cross-task pairs
        assert_eq!(dataset.authentic_pairs.len(), 3);
    }

    #[test]
    fn rejects_plag_without_matching_auth() {
        let tempdir = tempfile::tempdir().unwrap();
        let root = tempdir.path();
        let task_dir = root.join("src").join("task-1");
        fs::create_dir_all(&task_dir).unwrap();
        fs::write(task_dir.join("auth9_plag1.cpp"), "int main(){}\n").unwrap();

        let err = load_self_sourced_llm_dataset(root).unwrap_err();
        assert!(err.contains("no matching authentic source"));
    }
}
