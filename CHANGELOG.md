## v0.3.0 - 05/21/23
* Improvements to memory handling in tabulate/sanitize functions
    - Tabulate previously allocated a finite amount of memory; a hypothetical very large database would have overflown the buffer, although none of Starostin's are actually big enough. It now flexibly allocates memory depending on the database, which reduces memory consumption in most cases and makes the library more extensible. Slightly laggier operation and increased CPU usage when tabulating may be noticed, but these appear to be relatively minimal and should be outweighed by the benefits. 
    - All pointers allocated by sanitize functions *should* now be freeable, reducing the incidence of memory leaks in starling2csv and any other software derivative of libstarling.
* Changes to the invocation of the aforementioned functions
    - Unfortunately, the changes to sanitize memory handling have required breaking `starling_sanitize()` invocation: it now takes a `char **` argument, pointing to the desired output, and returns a `Starling_return_code` (casted to integer).
    - Invocations of other individual sanitize functions must now provide pointers with preallocated memory the exact size of the passed `len`. Uninitialized pointers will lead to segfaults.
* Two new tabulate functions (`starling_tabulate_db_[wide/tall]`) have been added, which do not provide new functionality, but may improve code flow and clarity. These simply delineate the two table formats supported by libstarling (rows vs. columns based on database headers, respectively).
    - `starling_tabulate_db()` will remain present and no changes are required to existing references to it. It now serves simply as a wrapper to these two functions.
* Made libintl support optional
    - CMake checks for presence and adds compile definitions
    - Added some preprocessor macros to prevent invalid references
    - Program will default to English messages without internationalization support
* Changed references to gettext() to _() to make typing easier if messages are expanded and to reduce disruption of code flow
* Note: Tested macOS build for the first time — it works without changes

## v0.2 - 05/16/23
* Made adjustments to stabilize the three databases that were previously broken (issues [#1](https://github.com/chickazee4/starling/issues/1) and [#2](https://github.com/chickazee4/starling/issues/2))
    - The Altaic and Sino-Tibetan databases appear to have had corrupt entries (a problem with the distributed versions, not my copies alone). libstarling now checks whether a database loaded from a file is either of these and skips over those entries if so.
    - Yeniseian seems to have fixed itself in the course of some other change. It remains unclear what exactly was causing the problem. I will be keeping an active eye on it to make sure the issue is not reintroduced.
    - The -z flag has been removed from starling2csv, since these databases are not problems anymore.
* Minor adjustments to memory handling across the board
    - Especially changed `starling_sanitize()` to stop abandoning alloc'd pointers

## v0.1.3 - 05/15/23
* Localization suport
    - Made all relevant strings in libstarling and starling2csv localizable via gettext (parent program must handle locale when linking against libstarling)
    - Added a Russian localization for libstarling and starling2csv
    - System gettext support is now mandatory for build pending more flexible infrastructure
* Minor QOL adjustments

## v0.1.2 - 05/12/23
* Added a Russian translation for starling2csv instructions.
* Capitalized all enum values to conform with C convention
* Somewhat improved error handling infrastructure
    - Replaced nonspecific integer-based error handling with more descriptive and consistent error enums across both libstarling and starling2csv
    - Added more checkpoints and error types to improve debugging and error handling opportunities for developers
    - Added some human-readable error descriptions to starling2csv
* Added handling for .inf metadata files, found in `inf.c`.
    - Also added new fields to database structs allowing the incorporation of .inf data. 
    - Modified certain function arguments to accommodate new inf settings.
    - Added functionality to starling2csv for dealing with these files.
* Added a temporary safeguard in starling2csv to prevent processing databases that currently cause crashes (altet, stibet, yenet). This can be bypassed with the -z flag.
* New flags for starling2csv:
    - -i [FILENAME]: load a custom .inf file from FILENAME. Enables -I also.
    - -I: use header names and output description (unless -q flag is on) from the database's .inf file (defaults to the database filename with extension changed, but may be customized using -i)
    - -z: debug mode (bypass safeguards against using specific databases that cause breakage)
* Updated docs to reflect these changes
* Other minor improvements for readability/consistency/efficiency

## v0.1.1 - 05/09/23
* Added a `starling_` prefix to every function and field that could be ambiguous in order to avoid conflicts with other libraries.
    - Removed `lowercase()` from `starling.h` entirely, as it is unlikely to be useful for most users and may conflict with similarly named functions (it is retained internally).
    - Changed name of `stringify_ft()` to `starling_fieldtypetostr()`.
* Added a new function in `tabulate.c`, `starling_tabulate_fields()`, that creates a CSV table of just the structure of a database's fields.
    - Added a new -M flag to starling2csv which allows the user to do this in place of the standard table.
    - Renamed `[starling_]tabulate()` to `starling_tabulate_db()` in order to clarify the distinction.
* Changed `[starling_]cleanse_delims()` to insert spaces in place of tabs, commas, and newlines rather than simply omitting them, as the latter solution was causing words to occasionally be erroneously concatenated when separated by a newline. This may lead to extra spaces in the output, but this can be corrected with the use of `starling_cleanse_spaces()`.
* Fixed a bug causing garbage characters to sometimes be appended to entries during the `sanitize` phase of table generation.
* Fixed CMakeLists not installing libstarling headers.
* General code cleanup and minor improvements for efficiency and readability.

## v0.1 - 12/20/22
* Initial release
