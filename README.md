# simple-ls

Minimal ls clone in C++

## Dependencies

- [argparse](https://github.com/p-ranav/argparse): For parsing CLI arguments. MIT Licensed

## Installation

1. Clone the repository

```sh
git clone https://github.com/ppaabo/simple-ls.git
```

2. Build

```sh
cd simple-ls
make
```

## Usage

```sh
# Basic usage
./build/simple-ls [options] [path]

# If no path is given, uses current '.' by default
./build/simple-ls

# Multiple flags can be combined e.g.
./build/simple-ls -alg

# -h or --help to view instructions and available options:
./build/simple-ls --help
Usage: simple-ls [--help] [--version] [--all] [-l] [-p] [--group-directories-first] path

Positional arguments:
  path                           Directory path [nargs=0..1] [default: "."]

Optional arguments:
  -h, --help                     shows help message and exits
  -v, --version                  prints version information and exits
  -a, --all                      Show files starting with '.'
  -l                             Print entries using long format
  -p                             Add '/' indicator to directories
  -g, --group-directories-first  Group directories before files

```