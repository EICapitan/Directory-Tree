
# Directory-diSplay (DS)

A simple directory tree display application was created to make getting information about a folder easier.

DS as the package is in the arch repository: [raw.githubusercontent.com](https://raw.githubusercontent.com/EICapitan/at-repository/master/x86_64/)

### Installation

```sh
mkdir -p directory-display && cd directory-display
wget https://raw.githubusercontent.com/at-elcapitan/Directory-diSplay/master/PKGBUILD
makepkg -i
```

### Description

Directory-diSplay (DS) is a simple yet powerful command-line application that displays the current directory structure. It provides various options to customize the output, such as displaying file sizes, and access options for easier readability in different terminal environments.

### Usage

To display the current directory structure, use the following command:

```sh
ds [OPTION] [DIRECTORY]
```

### Options

- `-а, --all`          : Display all directory content
- `-f, --full-name`    : Disable file name shortening
- `-s, --size`         : Display file sizes
- `-A, --access`       : Display access options
- `--version`          : Display current program version
- `--help`             : Display help message

### Example

Just displaying directory tree:

```sh
ds
```
![image](https://github.com/user-attachments/assets/49404764-3adc-478f-b0e4-3e2dd14f1f21)

Displaying directory tree with file sizes and full names:

```sh
ds -sf
```

or

```sh
ds --size --full-name
```
![image](https://github.com/user-attachments/assets/c5418248-031d-465b-bda3-7c7c6f543a4a)

