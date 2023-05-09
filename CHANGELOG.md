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
