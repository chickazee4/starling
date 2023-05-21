# libstarling / starling2csv

libstarling is a free and open source C library for reading and decoding [Starling](https://starling.rinet.ru/index2.php?lan=en) [linguistic databases](https://starling.rinet.ru/downl.php). It is capable of transcoding Starling-encoded text into standard UTF-8, reading Starling databases into C structs, and converting the resulting structs into plain text CSV format data.

starling2csv is a free and open source C command-line program for macOS, Linux, and *BSD that allows human users to easily interface with libstarling and turn Starling databases into human-readable CSV files. It is included in this project and built by default.

libstarling and starling2csv are designed to be used on Unix-like systems and are not guaranteed to compile or work properly on Windows (although they may, with some tweaks). They currently compile and function on Linux (using gcc) and macOS (using appleclang). BSDs should work, but are not actively tested. On a standard Unix-like system, the only prerequisites for compilation and installation of libstarling/starling2csv are a libc, a C compiler, and CMake. libintl/gettext is also required to display messages in a language other than English (currently only Russian is supported) — this is probably preinstalled if you use Linux or BSD, but you will likely need to install it separately on macOS. 

## Acknowledgment

I'm indebted greatly to Rob Verhoeven for publicly providing, and to Dr. Andries Brouwer for contributing to, [this specification](https://github.com/rhaver/starling/blob/master/doc/starling-dbf.pdf) of the structure of Starling .dbf files — I used this specification as an almost exclusive basis for the source code of `libstarling` (although I will point out, for licensing reasons, that I neither used nor examined any of the actual code belonging to the program with which it was distributed). Windows users who have stumbled upon this repository and been disappointed by the fact that it is not compatible with their operating system of choice may instead be interested in [Verhoeven's earlier, Windows-specific implementation](https://github.com/rhaver/starling) of the same concept.

## Compilation

Add -DLIBONLY=ON to the CMake command if you want to compile only libstarling, not starling2csv. Add -DSTATICLIBS=ON if you want to statically link libstarling and starling2csv instead of building a shared library. These directions work on both macOS and Linux.

    $ git clone https://www.github.com/chickazee4/starling.git
    $ cd starling
    $ mkdir build && cd build
    $ cmake .. [-DLIBONLY=ON] [-DSTATICLIBS=ON]
    $ make
    # make install

## Usage

For instructions on individual programs/libstarling, refer to the docs/ folder — each Markdown file contains instructions for the library/program corresponding to the file's name.

### Acquiring databases

If you want to use the Starling official databases, you can download the executable(s) for the ones you want from the website: https://starling.rinet.ru/downl.php

Note that the download links on that website do not link to ready-to-use .dbf/.var files, but rather Windows executable wrappers around ZIP files. You can decompress these without Wine — simply use `unzip` (works both on macOS and Linux). Download all the database packages you want into a dedicated directory, open the terminal, and run:

    cd [path name] && unzip ./*.exe

You'll end up with a hierarchical directory tree of all of the raw databases you downloaded.

## Q&A

Q: Do `libstarling` or `starling2csv` provide an interface for *writing* Starling-compatible .dbf files?

A: **No.** This is more difficult to implement than you might expect — while the functionality of `libstarling` could quite easily be extended to create structurally valid Starling files, the problem is really that Starling databases use a deprecated, specialized character encoding that is relatively simple to convert into UTF-8, but far more difficult to convert UTF-8 into — any information that would be being written in this process would probably have been stored at some point in UTF-8, rendering it irreversible (technically it's possible, just rather hard to do in C, which is the language used by this project). This implementation's conversion of Starling databases is, for all intents and purposes, a one-way street.

Q: Is there a GUI version of `starling2csv`?

A: Yes. It can be found [here](https://github.com/chickazee4/myna).

Q: How do I ask questions not listed here?

A: If your question is not about an apparent software bug and is not a feature suggestion, then email me at <lyndsay@vivaldi.net>. If it is a feature suggestion or related to a bug (or you suspect it to be), open a GitHub issue.

### Open issues

None, currently.