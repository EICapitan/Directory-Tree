
# Directory-diSplay (DS)

Simple directory tree display application, created to make it easier to get information about a folder.

DS as the package is in the arch repository: [raw.githubusercontent.com](https://raw.githubusercontent.com/EICapitan/at-repository/master/x86_64/)

### Installation

```sh
mkdir -p directory-display && cd directory-display
wget https://raw.githubusercontent.com/at-elcapitan/Directory-diSplay/master/PKGBUILD
makepkg -i
```

### Description

Directory-diSplay (DS) is a simple yet powerful command-line application designed to display the current directory structure. It provides various options to customize the output, such as displaying file sizes, access options, and disabling color for easier readability in different terminal environments.

### Usage

To display the current directory structure, use the following command:

```sh
ds [OPTION] [DIRECTORY]
```

### Options

- `-а, --all`          : Display all directory content
- `-f, --full-version` : Disable file name shortening
- `-s, --size`         : Display file sizes
- `-A, --access`       : Display access options
- `--version`          : Display current program version
- `--help`             : Display help message

### Example

To display the current directory structure with all details and file sizes:

```sh
ds -as
```
