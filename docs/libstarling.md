# Using libstarling

Your program must be linked to `libstarling.so`.  All of `libstarling`'s functionality can then be accessed through the single header file `starling.h`:

    #include <starling/starling.h>

This header contains the following data structures:

* `Starling_field_type` (`enum`): the 12 "field types" possible for a Starling field (character, numeric, etc.). Most of these are quite rare; character is the most common. The "field type" is immaterial to the actual type that the data is stored as by `libstarling`, which will always be a `char *`, but it could be useful for casting/parsing the data.
* `Starling_entry_type` (`enum`): either `ET_INTERNAL` or `ET_EXTERNAL`. `ET_EXTERNAL` refers to a field in the .dbf that does not contain content, but rather a pointer to somewhere in the database's .var file. `ET_INTERNAL` is used for a field that contains all of its content in the .dbf itself.
* `Starling_record_hdr` (`struct`): the struct representation of a Starling field, basically equivalent to a column of data. This struct contains the following members:
  - `name` (`char *`): the name of the field (=column heading).
  - `type` (`Starling_field_type`): the field's type.
  - `offset` (`uint32_t`): offset of the given field within a given record. According to Verhoeven's specification, this value is not always trustworthy, so take it with a grain of salt. It's not used by any of `libstarling`'s internal functions, but it is filled accurately by `starling_parse_db()`.
  - `length` (`unsigned char`): the maximal length of the field within the .dbf. Note that this value may not represent the actual length of the true content of a given entry under that field — for external entries, `Starling_record_hdr.length` will always be 6 (referring to the length of the *pointer* to the .var, not the text), but the actual length may be (and usually is) considerably longer.
  - `decimal_places` (`unsigned char`): this should store the number of decimal places belonging to a variable of a type that would have decimal places. I'm not sure how useful or reliable this is, and it's not used by any of `libstarling`'s internal functions, but it is nonetheless read and set by `starling_parse_db()` in case you need to do something with it.
  - `flags` (`unsigned char`): special flags for the field. If not 0x00 (no flags), it should be either 0x01 (system field, invisible to user), 0x02 ("nullable," according to Verhoeven — I haven't actually seen an instance of this flag, so I don't precisely know what it refers to), or 0x04 ("binary" — only for field types `FT_CHARACTER` and `FT_MEMO`).
* `Starling_entry` (`struct`): the struct representation of a Starling entry, analogous to a single cell of data. Contains the following members:
  - `hdr_index` (`int`): the index, relative to the entire list of fields in the database, of the field to which this entry belongs. You can therefore access the relevant `Starling_record_hdr` for this entry using `my_db.hdrs[my_entry.hdr_index]`.
  - `type` (`Starling_entry_type`): whether the entry is internal to the .dbf or externally defined in the .var.
  - `var_offset` (`uint32_t`): if the entry is external (`Starling_entry.type == ET_EXTERNAL`), this refers to the index in the .var buffer at which the data mainly corresponding to the entry begins. If the entry is internal, this will be set to 0 and should not be used.
  - `var_length` (`uint16_t`): see above — if external, this is the number of bytes in the .var buffer corresponding to the data for this entry. If the entry is internal, this should once again be set to 0.
  - `true_length` (`int`): the actual number of bytes occupied by the data belonging to this entry, once .var pointers have been resolved and data has been decoded, if applicable.
  - `content` (`unsigned char *`): the content of the entry, not decoded at all. For an external entry, this will be set to `NULL` until `starling_decode_external()` has been used on the entry. This should not be treated like a normal string — it will usually be contaminated by null terminators, so functions like `strlen()`, `strdup()`, etc. do not work. Use the field's length (if internal) or the `var_length` (if external) for the actual length when wrangling this variable.
  - `decoded_content` (`char *`): the decoded (and if applicable, sanitized) content of the entry. This should be free of null terminators and other invalid characters, so it can be treated like a normal string.
* `Starling_record` (`struct`): the struct representation of a Starling record — a row, in other words.
  - `is_deleted` (`int`): either 0 (not deleted) or 1 (is deleted). Whether you want to use records that have been "deleted" for your purposes is up to you — they are not automatically removed by any functions provided by `libstarling`, including `starling_tabulate_db()`.
  - `entries` (`Starling_entry *`): the list of entries within this record — the total should be the same as the number of fields (`Starling_db.hdr_ct`).
* `Starling_db` (`struct`): the struct representation of the entire structure of a Starling database, corresponding to the content of both the .dbf and .var file, if applicable.
  - `year` (`unsigned char`): byte 0x01 in the .dbf file, indicating the year that the database was last updated. Frustratingly, this is at some times offset from 1900, but at others offset from 2000. One way to determine which of these is the case is to see whether offsetting from 2000 would put the update year in the future and if so, assume it is offsetting from 1900 instead — this is what `starling2csv` does.
  - `month` (`unsigned char`): byte 0x02 in the .dbf file, the month at which the database was last updated. Unlike `year`, this does not have any special considerations and should not act strangely.
  - `day` (`unsigned char`): byte 0x03 in the .dbf file, the day of the month (0x01 to 0x1F/1 to 31) at which the database was last updated.
  - `rec_ct` (`uint32_t`): the number of records (rows) in the database.
  - `hdr_size` (`uint16_t`): the size, in bytes, of the database header (the segment of the .dbf file that does not contain records). Equivalent to the offset in the .dbf file of the first record.
  - `rec_size` (`uint16_t`): the number of bytes occupied by each record in the .dbf file. Note that this is not equivalent to the true size of the content of each record, which may be smaller or larger depending on whether the data has been padded and whether it references an external .var entry.
  - `hdrs` (`Starling_record_hdr *`): pointer referencing the fields belonging to this database.
  - `hdr_ct` (`uint32_t`): the number of fields in the database (i.e. the number of `Starling_record_hdr`s that `hdrs` points to).
  - `recs` (`Starling_record *`): pointer referencing all the records belonging to the database.
  - `ext_entries` (`unsigned char *`): the entire content of the .var file associated with this database. Assume this to be contaminated with null terminators - you can't use functions like `strlen()` or `strcpy()`.
  - `ext_len` (`uint32_t`): the number of bytes in the .var file, i.e., the size of `ext_entries`.
  - `db_description` (`char *`): the description of the database from any/all DBINFO blocks in the corresponding .inf file for the database, if one exists
  - `is_altai`/`is_stibet` (`int`): indicate whether the database is altai.dbf or stibet.dbf. Both appear to have specific corrupted records in the versions distributed by the Starling website and will crash the program unless this is set. This will omit the responsible records in output: in altet.dbf, only the 5th field of the 1728th record is impacted, while in stibet.dbf, the entire 2785th record is skipped due to multiple corrupted entries. 
* `Starling_sanitize_flags` (`struct`): a set of flags pertaining to text operations that will be performed on entries, for use with the `starling_tabulate_db()` and `starling_sanitize()` functions.
  - `starling_clean_delims` (`int`): if set to 1, those functions will call `starling_clean_delims()` on the input entry/entries, which deletes tabs, newlines, and commas.
  - `starling_clean_tags` (`int`): if set to 1, those functions will call `starling_clean_tags()` for each input entry/entries, deleting Starling's custom formatting tags from entry text.
  - `starling_clean_spaces` (`int`): if set to 1, those functions will call `starling_clean_spaces()` for each input entry/entries, deleting leading, trailing, and duplicated spaces.
* `Starling_table_flags` (`struct`): a set of flags that define the behavior of `starling_tabulate_db()` when called.
  - `lowercase_labels` (`int`): if set to 1, the names of fields will be transformed to lowercase when producing a CSV table (since they are usually, possibly always, entered in all-caps in original Starling databases).
  - `label_rows` (`int`): if set to 1, records will be printed as columns and fields will be printed as rows (default behavior is the other way around). This typically produces a vertically shorter but horizontally longer table.
  - `delimiter` (`char *`): the delimiter that will be used to separate cells in the table. This does not have a default in `libstarling`, so must be set (although `starling2csv` sets its own default — a comma).
  - `sanitize` (`Starling_sanitize_flags *`): a pointer to a `Starling_sanitize_flags` struct defining the behavior of the `starling_sanitize()` function when called by `starling_tabulate_db()`. If you do not wish to perform any of the three sanitization functions, this may be set either to NULL or { 0, 0, 0 } — `starling_sanitize()` will still technically be called, but will do nothing.
* Four `char *` arrays of size 256. Each of these is a lookup table indexed by byte values in the Starling encoding system, returning the appropriate Unicode character sequence corresponding to the byte. If no character sequence should correspond to a given byte, or if the sequence is independent of the currently-selected table, then its value in the current lookup table will correspond to a `NULL`. Note that these tables are only *declared* in the header; they are actually *defined* in `decode.c`.
  - `unibyte` (`(char *)[256]`): the default table corresponding to the vast majority of characters in a given Starling database. This contains most Latin-script characters that Starling encodes. Unless a different encoding has been set, you should look up input characters in this table.
  - `special_unibyte` (`(char *)[256]`): table corresponding to Starling's "special" characters, which are of the format 0x1D 0xXX (the 0xXX portion to be used as an index).
  - `grk_dualbyte` (`(char *)[256]`): corresponds to Greek characters, encoded by Starling in the format 0x83 0xXX (again, index only with 0xXX).
  - `cyr_dualbyte` (`(char *)[256]`): corresponds to Cyrillic characters, encoded by Starling in the format 0x87 0xXX (index with 0xXX).

In addition, this header contains declarations of all of the functions offered by `libstarling`. Functions prefixed with `starling_parse_`, except for `starling_parse_inf()` (in `inf.c`), are implemented in `dissect.c` and constitute the first stage in decoding a Starling database:

* `int starling_parse_records(Starling_db *out, unsigned char *raw, int len)`: populate a `Starling_db` with records (with filled-out entries), given the data from which to construct these records. Entries will not, at this point, be decoded.
  - argument `out` (`Starling_db *`): the database to populate. This should already contain a structured version of the data in the database's header.
  - argument `raw` (`unsigned char *`): the raw data stream containing the records. This should be offset so that `data[0]` is the first byte of record data — that is, you must increment the .dbf buffer by the length of its header when passing it to this function.
  - argument `len` (`int`): the length, in bytes, of the raw data.
  - returns: a return code 0, 3, or 4. 0 means OK, 3 means the `size` argument is inaccurate, and 4 means the database is corrupted or the reading frame is off.
* `int starling_parse_db(Starling_db *out, unsigned char *raw, int len)`: given a pointer to an *existing* `Starling_db` and the raw source data, fill the `Starling_db`'s metadata and records.
  - argument `out` (`Starling_db *`): this should be a pointer to a mostly empty `Starling_db`, except with `ext_entries` and `ext_len` set, if applicable.
  - argument `raw` (`unsigned char *`): the raw data stream to convert.
  - argument `len` (`int`): the length, in bytes, of the raw data.
  - returns: a return code 0, 2, 3, or 4. 2 indicates a corrupt or misaligned header, other return codes have the same meaning as in `starling_parse_records()`.
* `Starling_db *starling_parse_file(int *result, char *dbf_filename, char *var_filename)`: parse a Starling database file.
  - argument `result` (`int *`): where to store the return code.
  - argument `dbf_filename` (`char *`): self-explanatory.
  - argument `var_filename` (`char *`): filename of the .var file associated with the database. If one does not exist or you do not wish to use one, this can be set to `NULL`.
  - returns: a pointer to a `Starling_db` containing filled-out header metadata and records. External records will not yet contain content, but will contain the correct pointers to the .var. Internal records will have content, but will still be encoded in Starling's encoding rather than UTF-8.

Functions prefixed with `starling_decode_` are implemented in `decode.c` and help with dereferencing external entries and transcoding text from Starling's encoding into UTF-8.

* `int starling_decode_text(char **out, unsigned char *in, int len)`: translate Starling-encoded text into UTF-8.
  - argument `out` (`char **`): a pointer to the pointer in which the decoded UTF-8 text should be placed. The pointer to which `out` points does not need to be allocated, and in fact probably shouldn't, since the function allocates it itself (i.e., you can do `char *p = NULL; starling_decode_text(&p, data, length)`). 
  - argument `in` (`unsigned char *`): pointer to the first character of the raw Starling-encoded text.
  - argument `len` (`int`): the true length of the Starling-encoded text.
  - returns: the integer length of the decoded content. At this stage, you should not use `strlen()` to get the length of the decoded string. Rely on the value that this function returns instead; otherwise, you may end up with garbage.
* `void starling_decode_external(Starling_entry *entr, unsigned char **var)`: dereference an external entry, populating its `content` field.
  - argument `entr` (`Starling_entry *`): pointer to the entry you want to dereference.
  - argument `var` (`unsigned char **`): pointer to the buffer containing the entire contents of the .var. Don't offset this.
  - returns: void (the result will be stored in the original `Starling_entry` passed to argument `entr`)
* `void starling_decode_all_text(Starling_db *db)`: decode the `content` of every entry in a given `Starling_db`, populating each entry's `decoded_content` field.
  - argument `db` (`Starling_db *`): pointer to the database that should be decoded. It should already have entries with `content` fields populated, i.e., `starling_parse_records()` should be run prior to using this.
  - returns: void (result will be stored in the original database)
* `void starling_decode_all_external(Starling_db *db)`: populate the `content` field of every `ET_EXTERNAL` entry in a given `Starling_db` by obtaining the associated data from the .var buffer.
  - argument `db` (`Starling_db *`): pointer to the database that this should be performed on.
  - returns: void (result will be stored in the original database)
* `void starling_decode_all(Starling_db *db)`: dereference all .var pointers in `*db`, then transcode the text of every entry into UTF-8.
  - argument `db` (`Starling_db *`): pointer to a database with valid metadata, populated with records.
  - returns: void (result will be stored in the original database)

Functions prefixed with `starling_inf_` are implemented in `inf.c`, along with one other function.

* `char *starling_inf_clean_spaces(const char *s, int len)`: Cleans extraneous spaces from DBINFO blocks, which are often rife with them in a way that does not translate well to console output. Also converts Windows line endings to Unix line endings, as .inf files usually use the former. 
  - argument `s` (`const char *`): the string to be cleaned up
  - argument `len` (`int`): the number of characters in the string that should be included for cleaning
  - returns: a `char *` containing the cleaned version of the string with one trailing newline
* `char * starling_inf_clean_alias(const char *s)`: .inf files provide "human-readable" aliases for the often truncated and/or linguistically inconsistent internal field names, but they are normally provided in the same archaic encoding as the databases and multilingual names are delimited in an inconvenient manner (i.e. \1:*English name*\2:*Russian name in Starling encoding*). This function makes the "human-readable" aliases *actually* human-readable by converting them to UTF-8 and delimiting multilingual names with a simple slash (i.e. *English name*/*Russian name in UTF-8*). 
  - argument `s` (`const char *`): a pointer to the **entire line** from which to draw the alias (do not try to extract the alias yourself!).
  - returns: a `char *` containing the human-readable version of the alias alone (does not include the rest of the line that was passed to the function)
* `int starling_inf_is_info_end(const char *line)`: Checks if a string is a DBINFO END code.
  - argument `line`: the string to check
  - returns: 1 if true, 0 if false
* `int starling_inf_is_info_start(const char *line)`: Checks if a string is a DBINFO START code. Behaves the same as above.
* `int starling_inf_is_alias(const char *line)`: Checks if a string contains an alias for a field. Behaves the same as above.
* `int starling_parse_inf(Starling_db *db, const char *inff)`: Populates an existing, **already parsed** (does not have to be decoded) Starling_db with metadata from a .inf file. 
  - argument `db`: a pointer to the Starling_db to populate. The main caveat is that it must include header data, which can be accomplished with `starling_parse_db()`.
  - argument `inff`: the filename of the .inf file. This function will read the .inf file from the provided filename on its own; you should not pass the actual contents of the file in memory. 
  - returns: an integer corresponding to a `Starling_return_code` (success = `STARLING_OK`)

Functions prefixed with `starling_clean_` are implemented in `sanitize.c`, along with one other function.

* `int starling_clean_delims(char **out, const char *s, int len)`: remove tabs, newlines, and commas from a string (mainly useful for generating clean CSVs).
  - argument `out` (`char **`): a pointer to a string that will contain the result.
  - argument `s` (`const char *`): a pointer to the string that will be cleaned.
  - argument `len` (`int`) the length of the string that will be cleaned.
  - returns: the length, in number of bytes, of `out`.
* `int starling_clean_tags(char **out, const char *s, int len)`: remove Starling formatting tags (which look like \Ithis, for example\i) from a string. Arguments and the return value are the same as for `starling_clean_delims()`.
* `int starling_clean_spaces(char **out, const char *s, int len)`: remove leading, trailing, and repeated spaces from a string. Arguments and return value are the same as for the other two `starling_clean_` functions.
* `char *starling_sanitize(const char *orig, int len, Starling_sanitize_flags *flags)`: wrapper around the other three functions, which calls them conditionally based on the values in `flags`. Doesn't do anything else unique.
  - argument `orig` (`const char *`): the string to sanitize.
  - argument `len` (`int`): the length of the string to sanitize.
  - argument `flags` (`Starling_sanitize_flags *`): the flags for `starling_sanitize()` to use (=which of the above three functions should be performed). This may be set to `NULL` if no operations are to be performed. See also the explanation of `Starling_sanitize_flags` towards the start of this document.
  - returns: the sanitized string.

Finally, the following functions are defined in `tabulate.c`:
* `const char *starling_fieldtypetostr(Field_type ft)`: provide a human-readable single-word description of a given `Field_type`.
  - argument `ft` (`Field_type`): the `Field_type` to summarize.
  - returns: a plain-text description of `ft`.
* `char *starling_tabulate_fields(Starling_db *db, char *delim, int withinf)`: produce a plain text CSV table delineating the structure (field types) of the `Starling_db`
  - argument `db` (`Starling_db *`): a pointer to a `Starling_db` that has already been parsed.
  - argument `delim` (`char *`): a `char *` containing the desired CSV delimiter.
  - argument `withinf` (`int`): set to 1 if the table should include human field names from an .inf file, 0 otherwise. **Don't** enable this unless you have already performed `starling_parse_inf()` on the database. 
  - returns: a `char *` containing the resulting CSV.
* `char *starling_tabulate_db(Starling_db *db, Starling_table_flags *flags)`: produce a plain text CSV table from a complete `Starling_db` struct.
  - argument `db` (`Starling_db *`): a pointer to a `Starling_db` that has already been parsed and that has already had its entries fully decoded. Entries do not need to be sanitized; `starling_tabulate_db()` can do this for you based on `flags->sanitize`.
  - argument `flags` (`Starling_table_flags *`): a pointer to a `Starling_table_flags` struct, defining the options for `starling_tabulate_db()`. See discussion of this struct towards the start of the document.
  - returns: a `char *` containing the resulting CSV.

## Minimal example steps

If you want to convert, for example, `chuket.dbf` (with an associated .var file `chuket.var`) into a CSV using the default options, the following steps are necessary at minimum:

1. `Starling_db *mydb = starling_parse_file(NULL, "chuket.dbf", "chuket.var");` — produce a `Starling_db` with metadata and records, not yet decoded.
2. `starling_decode_all(mydb);` — turn external-referencing variables and Starling encoded strings into valid UTF-8 data
3. `Starling_table_flags myflags = { 0, 0, ",", NULL };` — set the flags for the table
4. `char *mycsv = starling_tabulate_db(mydb, &myflags);` — produce a CSV, which will be stored in `mycsv`

For more complex applications, you can consult the source code for `starling2csv`, which has a more developed workflow in order to account for different configuration options and user choices.
