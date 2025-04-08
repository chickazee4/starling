# libstarling / starling2csv
## At a glance
**starling2csv** is free and open-source terminal software for macOS, Linux, and *BSD that converts linguistic etymology databases from [Starling](https://starling.rinet.ru/downl.php), ordinarily stored in a custom encoding in the archaic DBF format, into modern CSV-format plaintext tables which can be viewed and edited in a text editor or your preferred spreadsheet software. It aims to provide linguists, conlangers, and enthusiasts on modern Unix-like systems with a free and extensible interface to convert these very useful databases into a very useful format. (Those who like this idea, but not the idea of interacting with the terminal, may want to check out my other project, [myna](https://github.com/chickazee4/myna), which provides a more user-friendly graphical interface to view and export the same databases.)

**libstarling** is the backend library powering starling2csv. It provides a fairly flexible and speedy C interface for deconstructing DBF files and decoding text from the proprietary Starling format into the modern UTF-8 standard. libstarling is designed to be usable and compilable as a shared library so that you can develop your own software using it to interact with Starling files, whether it be for linguistic analysis, conlanging, or something else. Comprehensive documentation of the library's fields and functions is therefore available in the docs/ folder of this repository. 

## Prerequisites / system requirements

libstarling and starling2csv are designed to be used on Unix-like systems and are not guaranteed to compile or work properly on Windows (although they may, with some tweaks). They currently compile and function on Linux (using gcc) and macOS (using appleclang). BSDs and probably other more obscure Unixoids should also work, but are not actively tested. 

On a standard Unix-like system, the only non-trivial prerequisites for compilation and installation of libstarling/starling2csv are a C compiler and CMake. libintl/gettext is optional (and usually already present on Linux): if present, Russian locale support will be included.

### Acquiring databases

If your plan is to convert a database using starling2csv, you can download the packages you want from the website: https://starling.rinet.ru/downl.php

"Eek! The download links do not link to ready-to-use .dbf/.var files, which are where the actual data is stored, but rather to Windows .exe files." I hear you saying this. Do not fret. The executables are facile wrappers around ZIP-compressed data, so you can extract the files you need without even touching Wine. Simply use the `unzip` terminal command (most likely available by default on your Unix system) or an archive manager. The most efficient way I can come up with is to put all the database packages you want into a dedicated directory (to keep them from getting lost. I will use /home/eve/Documents/starling_packages as an example, but you can change it as you please, even — especially — to apply the name of your own home directory), open the terminal, and run:

    cd /home/eve/Documents/starling_packages && unzip ./*.exe

You'll end up with .dbf, .var, .inf, and .prt files, many with the same name but a different file extension, some organized into subdirectories, others left in the base folder. You can leave them alone or change the organization as you please (though I recommend not renaming the files themselves), then proceed to compiling/running starling2csv.

## Compilation/usage

Add -DLIBONLY=ON to the CMake command if you want to compile only libstarling, not starling2csv. Add -DSTATICLIBS=ON if you want to statically link libstarling and starling2csv instead of building a shared library (this option should probably stay off if you will be building myna as well). These directions work on both macOS and Linux.

    $ git clone https://www.github.com/chickazee4/starling.git
    $ cd starling
    $ mkdir build && cd build
    $ cmake .. [-DLIBONLY=ON] [-DSTATICLIBS=ON]
    $ make
    # make install

For detailed instructions on using starling2csv/libstarling, refer to the docs/ folder of this repository.

## Q&A

Q: Do `libstarling` or `starling2csv` provide an interface for *writing* Starling-compatible .dbf files?

A: No. While the functionality of `libstarling` could quite easily be extended to create structurally valid Starling files, the problem is really that Starling databases use a deprecated, specialized character encoding that is relatively simple to convert into UTF-8, but much more difficult to convert UTF-8 into. While it is theoretically possible to implement, in practice, the only reason to convert in this direction would be if you have access to a functional Star32 instance that you are using to analyze the result, in which case there is little reason for you to be using starling2csv in the first place.

Q: Is there a GUI version of `starling2csv`?

A: As mentioned above, [myna](https://github.com/chickazee4/myna) provides a graphical interface to most of the functions of starling2csv, though a few features are still only available on the terminal.

Q: How do I ask questions not listed here?

A: If your question is not about an apparent software bug and is not a feature suggestion, then email me at <lyndsay@vivaldi.net>. If it is a feature suggestion or related to a bug (or you suspect it to be), open a GitHub issue.

## Acknowledgment

I'm indebted greatly to Rob Verhoeven for publicly providing, and to Dr. Andries Brouwer for contributing to, [this specification](https://github.com/rhaver/starling/blob/master/doc/starling-dbf.pdf) of the structure of Starling .dbf files — I used this specification as an almost exclusive basis for the source code of `libstarling` (although I will note for licensing purposes that I neither used nor examined any of the code belonging to the program with which it is distributed). Windows users who have stumbled upon this repository and been disappointed by the fact that it is not compatible with their operating system of choice may instead be interested in [Verhoeven's earlier, Windows-specific implementation](https://github.com/rhaver/starling) of basically the same concept.
