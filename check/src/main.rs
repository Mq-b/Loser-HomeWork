use std::{
    path::{Path, PathBuf},
    sync::atomic::{AtomicBool, Ordering},
};

use comrak::{
    nodes::{AstNode, NodeValue},
    Arena, Options,
};

static FAILED: AtomicBool = AtomicBool::new(false);

fn main() {
    let root = Path::new(".").canonicalize().unwrap();
    let files = get_file_paths(&root);
    let others = get_extension_paths(&files, &["tex", "cpp"]);
    let mds = get_extension_paths(&files, &["md"]);
    check_utf8(&others);
    check_utf8(&mds);
    check_md(&mds);
    if FAILED.load(Ordering::Relaxed) {
        std::process::exit(1);
    }
}

fn get_file_paths(root: &Path) -> Vec<PathBuf> {
    let mut out = Vec::<PathBuf>::new();
    for res in std::fs::read_dir(root).unwrap() {
        let path = res.unwrap().path();
        if path.is_dir() {
            out.append(&mut get_file_paths(&path));
        }
        if path.is_file() {
            out.push(path);
        }
    }
    out
}

fn get_extension_paths<'a>(paths: &'a Vec<PathBuf>, extension: &[&str]) -> Vec<&'a PathBuf> {
    paths
        .iter()
        .filter(|path| match path.extension() {
            Some(ext) => {
                let ext = ext.to_str().unwrap();
                extension.contains(&ext)
            }
            None => false,
        })
        .collect()
}

fn check_utf8(paths: &Vec<&PathBuf>) {
    for path in paths {
        let data = std::fs::read(path).unwrap();
        let Err(_) = String::from_utf8(data) else {
            continue;
        };
        println!("({}) 无效的UTF-8", path.display());
        FAILED.store(true, Ordering::Relaxed);
    }
}

fn check_md(paths: &Vec<&PathBuf>) {
    for path in paths {
        let data = std::fs::read(path).unwrap();
        let Ok(str) = String::from_utf8(data) else {
            continue;
        };
        let arena = Arena::new();
        let options = Options::default();
        let root = comrak::parse_document(&arena, &str, &options);
        iter_nodes(root, &|node| match &node.data.borrow().value {
            NodeValue::Text(str) => {
                let chars = str.chars().collect::<Vec<_>>();
                for window in chars.windows(2) {
                    if (is_a2z(window[0]) && is_cjk(window[1]))
                        || (is_cjk(window[0]) && is_a2z(window[1]))
                    {
                        println!(
                            "({}) 两个字之间没有空格:{}{}",
                            path.display(),
                            window[0],
                            window[1],
                        );
                        FAILED.store(true, Ordering::Relaxed);
                    }
                }
            }
            _ => {}
        });
    }
}

fn is_a2z(ch: char) -> bool {
    matches!(ch, 'A'..='Z' | 'a'..='z')
}

fn is_cjk(ch: char) -> bool {
    matches!(ch, '\u{4E00}'..='\u{9FA5}')
}

fn iter_nodes<'a, F>(node: &'a AstNode<'a>, f: &F)
where
    F: Fn(&'a AstNode<'a>),
{
    f(node);
    for c in node.children() {
        iter_nodes(c, f);
    }
}
