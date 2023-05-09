## Unstable branch changes since v0.1
* Added a `starling_` prefix to every function and field that could be ambiguous in order to avoid conflicts with other libraries.
    - Removed `lowercase()` from `database.h` (formerly `starling.h`) entirely, as it is unlikely to be useful for most users and may conflict with similarly named functions.
    - Changed name of `stringify_ft()` to `starling_fieldtypetostr`.
* Added a new function in `tabulate.c`, `starling_tabulate_fields()`, that creates a CSV table of just the structure of a database's fields.
    - Added a new -M flag to starling2csv which allows the user to do this in place of the standard table.
    - Renamed `[starling_]tabulate()` to `starling_tabulate_db()` in order to clarify the distinction.
* Changed `[starling_]cleanse_delims()` to insert spaces in place of tabs and newlines rather than simply omitting them, as the latter solution was causing words to occasionally be erroneously concatenated when separated by a newline. This may lead to extra spaces in the output, but this can be corrected with the use of `starling_cleanse_spaces()`.
* Fixed a bug causing garbage characters to sometimes be appended to entries during the `sanitize` phase of table generation.
* General code cleanup and minor improvements for efficiency and readability.

Note that docs/libstarling.md has not been updated since v0.1 except to fix the function names — some of what is in there is not perfectly accurate for the state of the unstable branch.

## v0.1 - 12/20/22
* Initial release
