use std::{
    path::{Path, PathBuf},
    sync::atomic::{AtomicBool, Ordering},
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
        match String::from_utf8(data) {
            Ok(_) => {}
            Err(e) => {
                let pos = e.utf8_error().valid_up_to();
                println!("({}) 无效的UTF-8:at {} bytes", path.display(), pos);
                FAILED.store(true, Ordering::Relaxed);
            }
        }
    }
}

fn check_md(paths: &Vec<&PathBuf>) {
    for path in paths {
        let data = std::fs::read(path).unwrap();
        let Ok(str) = String::from_utf8(data) else {
            continue;
        };
        let chars = str.chars().collect::<Vec<_>>();
        for (index, window) in chars.windows(2).enumerate() {
            if (is_a2z(window[0]) && is_cjk(window[1])) || (is_cjk(window[0]) && is_a2z(window[1]))
            {
                let mut x = chars.split_at(index).0.into_iter().collect::<Vec<_>>();
                x.reverse();
                let mut x = x.into_iter().take(80).collect::<Vec<_>>();
                x.reverse();
                let at = String::from_iter(x);
                println!(
                    "({}) 两个字之间没有空格:{}{} 上下文：\n{}\n",
                    path.display(),
                    window[0],
                    window[1],
                    at
                );
                FAILED.store(true, Ordering::Relaxed);
            }
        }
    }
}

fn is_a2z(ch: char) -> bool {
    matches!(ch, 'A'..='Z' | 'a'..='z')
}

fn is_cjk(ch: char) -> bool {
    matches!(ch, '\u{4E00}'..='\u{9FA5}')
}
