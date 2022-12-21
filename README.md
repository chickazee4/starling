# libstarling / starling2csv

libstarling is a free and open source C library for reading and decoding [Starling](https://starling.rinet.ru/index2.php?lan=en) [linguistic databases](https://starling.rinet.ru/downl.php). It is capable of transforming text encoded using the unusual Starling system into standard UTF-8, reading Starling databases into C structs, and converting the resulting structs into plain text CSV format data.

starling2csv is a free and open source C command-line program bundled with libstarling which allows human users to easily interface with the library and turn Starling databases into human-readable CSV files. It provides an interface to almost all of the key functions of the library.

libstarling and starling2csv are designed to be used on Unix-like systems and are not guaranteed to compile or work properly on Windows (although they may, with some tweaks). They are tested to compile properly on Linux and FreeBSD. The only prerequisites for compilation and installation are a libc (should be preinstalled on your system) with POSIX regex support, a C compiler, and CMake.

## Acknowledgment

I'm indebted greatly to Rob Verhoeven for publicly providing, and to Dr. Andries Brouwer for contributing to, [this specification](https://github.com/rhaver/starling/blob/master/doc/starling-dbf.pdf) of the structure of Starling .dbf files — I used this specification as an almost exclusive basis for the source code of `libstarling` (although I will point out, for licensing reasons, that I neither used nor examined any of the actual code belonging to the program with which it was distributed). Windows users who have stumbled upon this repository and been disappointed by the fact that it is not compatible with their operating system of choice may instead be interested in [Verhoeven's earlier, Windows-specific implementation](https://github.com/rhaver/starling) of the same concept.

## Compilation

Add -DLIBONLY=true to the CMake command if you want to compile only libstarling, not starling2csv. libstarling will always be compiled as a shared library regardless of whether it is compiled alone or alongside `starling2csv`.

    $ git clone https://www.github.com/chickazee4/starling.git
    $ cd starling
    $ mkdir build && cd build
    $ cmake .. [-DLIBONLY=true]
    $ make
    # make install

## Using starling2csv

Usage: starling2csv [DBF FILENAME] (OPTIONS...)

    -d              Change the delimiter of the CSV file (defaults to commas). Delimiters may be of arbitrary length and include any valid Unicode character.
    -D              Omit tabs, newlines, and commas in entries in order to avoid conflicts with commonly-used CSV delimiters.
    -l              Use field names to organize rows instead of columns (i.e. print field names in their own single column).
    -m              Print detailed database metadata without printing a table to stdout. This can be combined with -o if you still want to produce a CSV file.
    -o FILENAME     Write the generated CSV to the specified file (will overwrite any existing content!).
    -q              Silence output to stdout of database metadata (so only the CSV itself will be produced).
    -s              Make the first row of the CSV lowercase (as the field names in Starling .DBF files are typically all-caps and will be printed as such).
    -S              Clean up extraneous spaces (delete spaces before and after entry text, replace multiple spaces in a row with a single space).
    -T              Cleanse outputs of Starling formatting tags (probably a good idea unless you are doing something unusual with the CSV).
    -v FILENAME     Filename of the .VAR file associated with the .DBF that you are converting (you do not need to specify this if it is in the same directory as the .DBF and has the same basename, e.g. japet.dbf and japet.var).
    -V              Override automatic .VAR file detection if you don't specify a -v (i.e., if you want to decode japet.dbf without using japet.var *or any other .VAR*). Not necessary if you specify -v with a valid filename.

Option globbing is supported.

Suggested usage for the most likely use case (turning a standard etymology database, stored in the same folder as a .var file of the same name, into a spreadsheet for use with LibreOffice Calc, R, or something similar):

    starling2csv foo.dbf -DTS -o bar.csv

Explanation:

* `foo.dbf`: the database to be decoded
* `-D`: remove characters that conflict with standard CSV delimiting (tabs and newlines will be omitted, commas will be replaced with slashes)
* `-T`: remove Starling tags (entries often contain markup formatting tags that look like `\Iwhatever\i` — these are useless in a CSV file, so this option omits them from output)
* `-S`: remove extraneous spaces from entries (there are often leading and trailing spaces in Starling files, and sometimes multiple spaces in a row, so this gets rid of them)
* `-o bar.csv`: print the result to the file `bar.csv`

### Acquiring databases

If you want to use the Starling official databases, you can download the executable(s) for the ones you want from the website: https://starling.rinet.ru/downl.php

Note that the download links on that website do not link to ready-to-use .dbf/.var files, but rather Windows executables containing compressed versions of the databases. The executables are really just wrappers around ZIP files - you can unzip them using `unzip` on Linux. For example, if you download all the databases you want into a dedicated directory, you can simply open the terminal and run:

    unzip ./*.exe

You'll end up with a hierarchical directory tree of all of the raw databases you downloaded.

## Using libstarling

Refer to `docs/libstarling.md`.

## PAQ (Potentially Asked Questions)

Q: Do `libstarling` and/or `starling2csv` provide an interface for *writing* Starling-compatible .dbf files?

A: **No.** This is more difficult to implement than you might expect — while the functionality of `libstarling` could quite easily be extended to create structurally valid Starling files, the problem is really that Starling databases use a very non-standard character encoding that is relatively simple to convert into UTF-8, but far more difficult to convert UTF-8 into — any information that would be being written in this process would probably have been stored at some point in UTF-8, so not reversible. The conversion of Starling databases (in this implementation) is, for all intents and purposes, a one-way street.

Q: Will there be a GUI version of `starling2csv`?

A: I'd like to make the ability to read these databases accessible to all Unix users, including those who are uncomfortable with a terminal interface, so yes (hopefully). Currently, ensuring that the library and CLI implementation work perfectly is my top priority.

Q: How do I ask questions not listed here?

A: If your question is not about an apparent software bug and is not a feature suggestion, then email me at <lyndsay@vivaldi.net>. If it is a feature suggestion or related to a bug (or you suspect it to be), open a GitHub issue.

### Known issues

* Sometimes, entries will have an invalid byte or two at the end. I haven't worked out exactly what causes this (it happens to the same entries repeatedly, but there doesn't seem to be an obvious pattern in what entries it affects). This may cause some programs to give you a warning message about invalid UTF-8 characters, but otherwise, this doesn't seem to have a severe impact — the actual data is always intact.
