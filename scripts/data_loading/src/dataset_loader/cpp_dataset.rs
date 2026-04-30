use std::fs;
use std::path::{Path, PathBuf};

use fxhash::{FxBuildHasher, FxHashMap, FxHashSet};
use itertools::Itertools;

use crate::dataset_loader::LanguageDataset;

/// Reads the ground-truth-static-anon.txt format into a map of assignment -> groups of student ids.
fn read_ground_truth_static(
    gt_path: impl AsRef<Path>,
) -> Result<FxHashMap<String, Vec<Vec<String>>>, String> {
    let gt_path = gt_path.as_ref();
    let content = fs::read_to_string(gt_path).map_err(|e| {
        format!(
            "Failed to read ground truth file {}: {}",
            gt_path.display(),
            e
        )
    })?;

    let mut assignment_to_groups: FxHashMap<String, Vec<Vec<String>>> =
        FxHashMap::with_hasher(FxBuildHasher::new());
    let mut current_assignment: Option<String> = None;

    for raw_line in content.lines() {
        let line = raw_line.trim();
        if line.is_empty() {
            continue;
        }
        if let Some(stripped) = line.strip_prefix('-') {
            let header = stripped.trim().to_string();
            current_assignment = Some(header.clone());
            assignment_to_groups.entry(header).or_default();
            continue;
        }
        if let Some(curr) = &current_assignment {
            let students: Vec<String> = line
                .split(',')
                .map(|s| s.trim())
                .filter(|s| !s.is_empty())
                .map(|s| s.to_string())
                .collect();
            if students.len() >= 2 {
                assignment_to_groups
                    .entry(curr.clone())
                    .or_default()
                    .push(students);
            }
        }
    }

    Ok(assignment_to_groups)
}

fn assignment_dir(dataset_root: &Path, assignment_id: &str) -> PathBuf {
    let mut p = dataset_root.to_path_buf();
    p.push("src");
    for part in assignment_id.split('/') {
        p.push(part);
    }
    p
}

/// Find files named like "studentXXXX.*" in the assignment directory.
fn find_student_files(assignment_dir: impl AsRef<Path>) -> FxHashMap<String, PathBuf> {
    let assignment_dir = assignment_dir.as_ref();
    let mut id_to_path: FxHashMap<String, PathBuf> = FxHashMap::default();
    let Ok(entries) = fs::read_dir(assignment_dir) else {
        return id_to_path;
    };
    for entry in entries.flatten() {
        let path = entry.path();
        if !path.is_file() {
            continue;
        }
        if let Some(name) = path.file_name().and_then(|s| s.to_str()) {
            if name.starts_with("student") {
                if let Some(stem) = name.split('.').next() {
                    id_to_path.insert(stem.to_string(), path);
                }
            }
        }
    }
    id_to_path
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

pub fn load_cpp_dataset(dataset_root: impl AsRef<Path>) -> Result<LanguageDataset, String> {
    let mut parsed_dataset = LanguageDataset::new("cpp");
    let dataset_root = dataset_root.as_ref();
    let gt_path = dataset_root.join("ground-truth-static-anon.txt");
    let gt = read_ground_truth_static(gt_path)?;

    for (assignment_id, plagiarized_groups) in gt {
        let assignment_dir = assignment_dir(dataset_root, &assignment_id);
        let student_id_to_path = find_student_files(assignment_dir);

        let plagiarized_group_paths = plagiarized_groups
            .into_iter()
            .map(|plagiarized_group| {
                plagiarized_group
                    .into_iter()
                    .map(|student_id| student_id_to_path.get(&student_id).cloned())
                    .flatten()
                    .collect_vec()
            })
            .collect_vec();
        let plagiarzed_paths_set =
            FxHashSet::from_iter(plagiarized_group_paths.iter().flatten().cloned());

        let plagiarized_path_pairs = plagiarized_group_paths
            .iter()
            .cloned()
            .flat_map(|group| {
                let mut pairs: Vec<_> = group
                    .iter()
                    .map(|path| (path.clone(), path.clone()))
                    .collect();
                pairs.extend(all_pairs(group));
                pairs
            })
            .collect_vec();

        let unplagiarized_paths_set = student_id_to_path
            .values()
            .filter(|k| !plagiarzed_paths_set.contains(*k))
            .cloned()
            .collect::<FxHashSet<_>>();

        let unplagiarzed_paths =
            fastrand::choose_multiple(unplagiarized_paths_set, plagiarzed_paths_set.len());

        let authentic_path_pairs = all_pairs(unplagiarzed_paths);

        for (left_path, right_path) in plagiarized_path_pairs {
            if !is_cpp_file(&left_path) || !is_cpp_file(&right_path) {
                continue;
            }
            parsed_dataset.add_plagiarized_pair(left_path, right_path);
        }

        for (left_path, right_path) in authentic_path_pairs {
            if !is_cpp_file(&left_path) || !is_cpp_file(&right_path) {
                continue;
            }
            parsed_dataset.add_authentic_pair(left_path, right_path);
        }
    }

    Ok(parsed_dataset)
}

fn is_cpp_file(path: impl AsRef<Path>) -> bool {
    let path = path.as_ref();
    path.extension()
        .map(|ext| ext == "cpp" || ext == "hpp")
        .unwrap_or(false)
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::io::Write;

    #[test]
    fn parses_ground_truth_and_finds_pairs() {
        let tempdir = tempfile::tempdir().unwrap();
        let root = tempdir.path();

        // ground-truth-static-anon.txt
        let gt = root.join("ground-truth-static-anon.txt");
        let mut f = fs::File::create(&gt).unwrap();
        writeln!(f, "- A2016/Z1/Z1").unwrap();
        writeln!(f, "student001,student002").unwrap();
        writeln!(f, "student003,student004,student005").unwrap();

        // files under src/A2016/Z1/Z1
        let assign_dir = root.join("src").join("A2016").join("Z1").join("Z1");
        fs::create_dir_all(&assign_dir).unwrap();
        for sid in [
            "student001.cpp",
            "student002.cpp",
            "student003.cpp",
            "student004.cpp",
            "student005.cpp",
            "student006.cpp",
            "student007.cpp",
        ] {
            fs::write(assign_dir.join(sid), "int main(){}\n").unwrap();
        }

        let dataset = load_cpp_dataset(root).unwrap();
        assert!(!dataset.plagiarized_pairs.is_empty());
        assert!(!dataset.authentic_pairs.is_empty());
        for e in dataset
            .plagiarized_pairs
            .iter()
            .chain(dataset.authentic_pairs.iter())
        {
            assert!(e.left_path.is_file());
            assert!(e.right_path.is_file());
        }
    }
}
