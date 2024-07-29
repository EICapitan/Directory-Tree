use std::fs;
use std::path::{Path, PathBuf};
use std::{
    env::{self},
    os::{
        linux::fs::MetadataExt,
        unix::fs::{FileTypeExt, PermissionsExt},
    },
};

const MASK_ALL: i8 = 1;
const MASK_SIZE: i8 = 1 << 2;
const MASK_ACCESS: i8 = 1 << 3;
const MASK_FULL_NAME: i8 = 1 << 4;

fn display_help() {
    println!("Usage: ds [OPTION] [DIRECTORY]");
    println!();
    println!("Display directory contents");
    println!();
    println!("Options:");
    println!("  -a, --all             display all directory content");
    println!("  -s, --size            display files size");
    println!("  -f, --full-name       disable file name shortening");
    println!("  -A, --access          display access options");
    println!("      --version         display current program version");
    println!("      --help            display this message)");
    println!();
    println!("Directory diSplay home page: <https://github.com/at-elcapitan/Directory-diSplay>");
}

fn format_size(size: u64) -> (f64, String) {
    let size = size as f64;
    const KIB: f64 = 1024.0;
    const MIB: f64 = 1024.0 * KIB;
    const GIB: f64 = 1024.0 * MIB;

    if size < KIB {
        (size, " B".to_string())
    } else if size < MIB {
        (size / KIB, "KB".to_string())
    } else if size < GIB {
        (size / MIB, "MB".to_string())
    } else {
        (size / GIB, "GB".to_string())
    }
}

fn main() -> std::io::Result<()> {
    let mut args: Vec<String> = env::args().collect();
    let mut dir_objects: Vec<(String, String, u64, u32)> = Vec::new();
    let mut max_length: usize = 0;
    let mut flags: i8 = 0;
    let mut dir_arg: String = String::new();

    args.remove(0);

    for mut arg in args {
        match arg.as_str() {
            "--help" => {
                display_help();
                return Ok(());
            }
            "--version" => {
                println!("ds (Directory diSplay) 2.0.0");
                println!();
                println!(
                    "This is free software; see the source for copying conditions. There is NO"
                );
                println!(
                    "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE."
                );
                println!();
                println!("Written by Vladislav 'ElCapitan' Nazarov");
                return Ok(());
            }
            "--access" => flags = flags | MASK_ACCESS,
            "--all" => flags = flags | MASK_ALL,
            "--size" => flags = flags | MASK_SIZE,
            "--full-name" => flags = flags | MASK_FULL_NAME,
            &_ => {
                if !arg.starts_with("-") {
                    let path = Path::new(arg.as_str());
                    if !path.exists() {
                        println!("Cannot access: no such file or directory");
                        return Ok(());
                    }

                    if path.is_file() {
                        println!("{} is not a directory", arg.as_str());
                        return Ok(());
                    }

                    if !fs::File::open(arg.as_str()).is_ok() {
                        println!("Cannot access: Permission Denied");
                        return Ok(());
                    }

                    dir_arg = arg.clone();
                    continue;
                }
                assert_eq!(arg.remove(0), '-');

                for c in arg.chars() {
                    match c {
                        'h' => {
                            display_help();
                            return Ok(());
                        }
                        'A' => flags = flags | MASK_ACCESS,
                        'a' => flags = flags | MASK_ALL,
                        's' => flags = flags | MASK_SIZE,
                        'f' => flags = flags | MASK_FULL_NAME,
                        '-' => {
                            println!(
                                "Unknown argument -{}. Use -h or --help for arguments list",
                                arg
                            );
                            return Ok(());
                        }
                        _ => {
                            println!(
                                "Unknown argument -{}. Use -h or --help for arguments list",
                                c
                            );
                            return Ok(());
                        }
                    }
                }
            }
        }
    }

    let mut current_dir: PathBuf;

    if dir_arg.is_empty() {
        current_dir = match std::env::current_dir() {
            Ok(data) => data,
            Err(err) => {
                panic!("Error {err} while getting working directory");
            }
        };
    } else {
        current_dir = PathBuf::new();
        current_dir.push(dir_arg)
    }

    if current_dir.read_dir()?.next().is_none() {
        return Ok(());
    }

    if let Ok(objects) = current_dir.read_dir() {
        for obj in objects {
            match obj {
                Ok(data) => {
                    let mut filename = data
                        .file_name()
                        .into_string()
                        .expect("Error while converting OsString to String");

                    if filename.starts_with('.') && flags & MASK_ALL == 0 {
                        continue;
                    }

                    let length = filename.len();
                    let fmode = data.metadata()?.permissions().mode();

                    let filetype = match data.file_type() {
                        Ok(file_type) => {
                            if file_type.is_dir() {
                                String::from("DIR")
                            } else if file_type.is_file()
                                && data.metadata()?.permissions().mode() & 0o111 != 0
                            {
                                String::from("EXEC")
                            } else if file_type.is_file() {
                                String::from("FILE")
                            } else if file_type.is_symlink() {
                                String::from("SYMLINK")
                            } else if file_type.is_block_device() {
                                String::from("BLOCK")
                            } else if file_type.is_char_device() {
                                String::from("CHAR")
                            } else {
                                String::from("UNKNOWN")
                            }
                        }
                        Err(err) => panic!(
                            "Unexpected error occurred while getting {filename} arguments. {err}"
                        ),
                    };

                    let size = if filetype == "FILE" || filetype == "EXEC" {
                        data.metadata().unwrap().st_size()
                    } else {
                        0
                    };

                    if flags & MASK_FULL_NAME == 0 && length > 40 {
                        filename.truncate(
                            filename
                                .char_indices()
                                .take_while(|(i, _)| *i <= 38)
                                .last()
                                .map(|(i, _)| i)
                                .unwrap_or(0),
                        );
                        filename.push_str("..");
                        if filetype == "DIR" {
                            filename.push('/')
                        }
                        max_length = 40;
                        dir_objects.push((filename, filetype, size, fmode));
                        continue;
                    }

                    max_length = if length > max_length {
                        length
                    } else {
                        max_length
                    };

                    if filetype == "DIR" {
                        filename.push('/')
                    }
                    dir_objects.push((filename, filetype, size, fmode))
                }
                Err(err) => panic!("Unexpected error {err}"),
            }
        }
    } else {
        panic!("Error while reading directory")
    }

    let mut additional: String = String::new();

    if flags & MASK_SIZE != 0 {
        additional.push_str("   SIZE\t\t")
    }

    if flags & MASK_ACCESS != 0 {
        additional.push_str("PUSER\tGROUP\tOTHER")
    }

    if let Some(dir_name) = current_dir.file_name() {
        let dir_name_len = dir_name.len();

        max_length = if dir_name.len() > max_length {
            dir_name_len
        } else {
            max_length
        };

        let spaces = max_length + 7 - dir_name_len;
        println!(
            "{}/{}\tTYPE\t\t{}",
            dir_name.to_string_lossy(),
            " ".repeat(spaces),
            additional
        );
    } else {
        println!("./{}  TYPE{}", " ".repeat(max_length + 6), additional);
    }

    println!("  │");
    let mut total_size: u64 = 0;

    for (i, obj) in dir_objects.iter().enumerate() {
        let (name, obj_type, size, mode) = obj;

        let color = match obj_type.as_str() {
            "DIR" => "\x1b[34;1m",
            "SYMLINK" => "\x1b[31;1m",
            "BLOCK" => "\x1b[33;1m",
            "CHAR" => "\x1b[37;1m",
            "EXEC" => "\x1b[32;1m",
            &_ => "",
        };
        let mut flags_data = String::new();

        if flags & MASK_SIZE != 0 && (obj_type == "FILE" || obj_type == "EXEC") {
            total_size += size;
            let (fsize, val_type) = format_size(*size);
            flags_data.push_str(&format!("{:.2}\t{}\t", fsize, val_type.as_str()));
        } else if flags & MASK_SIZE != 0 {
            flags_data.push_str("\t -\t")
        }

        if flags & MASK_ACCESS != 0 {
            // Owner permissions
            flags_data.push(' ');
            flags_data.push(if mode & 0o400 != 0 { 'r' } else { '-' });
            flags_data.push(if mode & 0o200 != 0 { 'w' } else { '-' });
            flags_data.push(if mode & 0o100 != 0 { 'x' } else { '-' });
            flags_data.push('\t');

            // Group permissions
            flags_data.push(' ');
            flags_data.push(if mode & 0o040 != 0 { 'r' } else { '-' });
            flags_data.push(if mode & 0o020 != 0 { 'w' } else { '-' });
            flags_data.push(if mode & 0o010 != 0 { 'x' } else { '-' });
            flags_data.push('\t');

            // Others permissions
            flags_data.push(' ');
            flags_data.push(if mode & 0o004 != 0 { 'r' } else { '-' });
            flags_data.push(if mode & 0o002 != 0 { 'w' } else { '-' });
            flags_data.push(if mode & 0o001 != 0 { 'x' } else { '-' });
        }

        if i == dir_objects.len() - 1 {
            println!(
                "  └ {color}{:<width$}\x1b[0m\t{}\t\t{flags_data}",
                name,
                obj_type,
                width = max_length + 4
            );
            break;
        }
        println!(
            "  ├ {color}{:<width$}\x1b[0m\t{}\t\t{flags_data}",
            name,
            obj_type,
            width = max_length + 4
        );
    }

    if flags & MASK_SIZE != 0 {
        let (total_size, dtype) = format_size(total_size);
        println!("Total size: {:.2} {}", total_size, dtype);
    }

    Ok(())
}
