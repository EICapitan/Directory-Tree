use std::path::PathBuf;
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
    println!("  -A, --access          display access options (not impl)");
    println!("      --version         display current program version");
    println!("      --help            display this message)");
    println!();
    println!("Directory diSplay home page: <https://github.com/at-elcapitan/Directory-diSplay>");
}

fn main() -> std::io::Result<()> {
    let mut args: Vec<String> = env::args().collect();
    let mut dir_objects: Vec<(String, String, u64)> = Vec::new();
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
                println!("ds (Directory diSplay) 2.0.0-beta.2");
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
                    let length = filename.len();

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
                        filename.truncate(38);
                        filename.push_str("..");
                        if filetype == "DIR" {
                            filename.push('/')
                        }
                        max_length = 40;
                        dir_objects.push((filename, filetype, size));
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
                    dir_objects.push((filename, filetype, size))
                }
                Err(err) => panic!("Unexpected error {err}"),
            }
        }
    } else {
        panic!("Error while reading directory")
    }

    let mut additional: String = String::new();

    if flags & MASK_SIZE != 0 {
        additional.push_str("\t\t  SIZE")
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
            "{}/{}\tTYPE{}",
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
        let (name, obj_type, size) = obj;

        if name.starts_with('.') && flags & MASK_ALL == 0 {
            continue;
        }

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
            flags_data.push_str("\t\t");
            total_size += size;
            flags_data.push_str(size.to_string().as_str());
            flags_data.push_str("\tB")
        } else if flags & MASK_SIZE != 0 {
            flags_data.push_str("\t\t\t-")
        }

        if i == dir_objects.len() - 1 {
            println!(
                "  └ {color}{:<width$}\x1b[0m\t{}{flags_data}",
                name,
                obj_type,
                width = max_length + 4
            );
            break;
        }
        println!(
            "  ├ {color}{:<width$}\x1b[0m\t{}{flags_data}",
            name,
            obj_type,
            width = max_length + 4
        );
    }

    if flags & MASK_SIZE != 0 {
        println!("Total size: {}\tB", total_size);
    }

    Ok(())
}
