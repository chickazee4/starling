# libstarling / starling2csv

libstarling is a free and open source C library for reading and decoding [Starling](https://starling.rinet.ru/index2.php?lan=en) [linguistic databases](https://starling.rinet.ru/downl.php). It is capable of transcoding Starling-encoded text into standard UTF-8, reading Starling databases into C structs, and converting the resulting structs into plain text CSV format data.

starling2csv is a free and open source C command-line program bundled with libstarling which allows human users to easily interface with the library and turn Starling databases into human-readable CSV files.

libstarling and starling2csv are designed to be used on Unix-like systems and are not guaranteed to compile or work properly on Windows (although they may, with some tweaks). They are tested to compile properly on Gentoo Linux and should work out-of-the-box on any up-to-date Linux distribution with the software prerequisites. Pull requests and issues are welcome to improve compatibility with macOS and \*BSD, since they are not actively tested. The only prerequisites for compilation and installation of libstarling/starling2csv are a libc (should be preinstalled on your system), a C compiler, and CMake.

## Acknowledgment

I'm indebted greatly to Rob Verhoeven for publicly providing, and to Dr. Andries Brouwer for contributing to, [this specification](https://github.com/rhaver/starling/blob/master/doc/starling-dbf.pdf) of the structure of Starling .dbf files — I used this specification as an almost exclusive basis for the source code of `libstarling` (although I will point out, for licensing reasons, that I neither used nor examined any of the actual code belonging to the program with which it was distributed). Windows users who have stumbled upon this repository and been disappointed by the fact that it is not compatible with their operating system of choice may instead be interested in [Verhoeven's earlier, Windows-specific implementation](https://github.com/rhaver/starling) of the same concept.

## Compilation

Add -DLIBONLY=ON to the CMake command if you want to compile only libstarling, not starling2csv or myna. Add -DSTATICLIBS=ON if you want to statically link libstarling and starling2csv instead of building a shared library. 

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

Note that the download links on that website do not link to ready-to-use .dbf/.var files, but rather Windows executables containing compressed versions of the databases. The executables are really just wrappers around ZIP files - you do not need Wine or anything and you can simply unzip them using `unzip` on Linux/BSD(/maybe macOS?). For example, if you download all the database packages you want into a dedicated directory, you can simply open the terminal and run:

    cd [path name] && unzip ./*.exe

You'll end up with a hierarchical directory tree of all of the raw databases you downloaded.

## Q&A

Q: Do `libstarling` or `starling2csv` provide an interface for *writing* Starling-compatible .dbf files?

A: **No.** This is more difficult to implement than you might expect — while the functionality of `libstarling` could quite easily be extended to create structurally valid Starling files, the problem is really that Starling databases use a very non-standard character encoding that is relatively simple to convert into UTF-8, but far more difficult to convert UTF-8 into — any information that would be being written in this process would probably have been stored at some point in UTF-8, which is not (easily) reversible. The conversion of Starling databases (in this implementation) is, for all intents and purposes, a one-way street.

Q: Will there be a GUI version of `starling2csv`?

A: There is a GUI program, `myna`, currently in development. Check back soon.

Q: How do I ask questions not listed here?

A: If your question is not about an apparent software bug and is not a feature suggestion, then email me at <lyndsay@vivaldi.net>. If it is a feature suggestion or related to a bug (or you suspect it to be), open a GitHub issue.

### Open issues

* altet.dbf, stibet.dbf, and yenet.dbf cause crashes — the first two appear to be the result of the same underlying issue, but Yeniseian appears to be a separate problem. See issues [#1](https://github.com/chickazee4/starling/issues/1) and [#2](https://github.com/chickazee4/starling/issues/2) respectively.