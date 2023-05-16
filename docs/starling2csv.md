Usage: starling2csv [DBF FILENAME] (OPTIONS...)

    -c              Cleanse all entries of garbage - same as combining -D, -T, and -S flags.
    -d              Change the delimiter of the CSV file (defaults to commas). Delimiters may be of arbitrary length and include any valid Unicode character.
    -D              Omit tabs, newlines, and commas in entries in order to avoid conflicts with commonly-used CSV delimiters.
    -i FILENAME     Include in output data from an .inf file located at FILENAME.
    -I              Include in output data from an automatically detected .inf file (works if there is an .inf file with the same name in the same directory as the database file), i.e. human field names and database description.
    -l              Use field names to organize rows instead of columns (i.e. print field names in their own single column).
    -m              Print detailed database metadata without printing a table to stdout. This can be combined with -o if you still want to produce a CSV file.
    -M              Rather than printing a table of the database's content, print a table providing data about the database's fields (like -m, but CSV format). -o flag works to output the results of this to a file.
    -o FILENAME     Write the generated CSV to the specified file (will overwrite any existing content!).
    -q              Silence output to stdout of database metadata (so only the CSV itself will be produced).
    -s              Make the first row of the CSV lowercase (as the field names in Starling .DBF files are typically all-caps and will be printed as such).
    -S              Clean up extraneous spaces (delete spaces before and after entry text, replace multiple spaces in a row with a single space).
    -T              Cleanse outputs of Starling formatting tags (probably a good idea unless you are doing something unusual with the CSV).
    -v FILENAME     Filename of the .VAR file associated with the .DBF that you are converting (you do not need to specify this if it is in the same directory as the .DBF and has the same basename, e.g. japet.dbf and japet.var).
    -V              Override automatic .VAR file detection if you don't specify a -v (i.e., if you want to decode japet.dbf without using japet.var *or any other .VAR*). Not necessary if you specify -v with a valid filename.

Option globbing is supported.

Suggested usage for the most likely use case (turning a standard etymology database, stored in the same folder as a .var file of the same name, into a spreadsheet for use with LibreOffice Calc, R, or something similar):

    starling2csv foo.dbf -cIo bar.csv

Explanation:

* `foo.dbf`: the database to be decoded
* `-c`: shorthand for `-DTS`
    - `-D`: remove characters that conflict with standard CSV delimiting
    - `-T`: remove Starling tags (entries often contain markup formatting tags that look like `\Iwhatever\i` — these are useless in a CSV file, so this option omits them from output)
    - `-S`: remove extraneous spaces from entries (there are often leading and trailing spaces in Starling files, and sometimes multiple spaces in a row, so this gets rid of them)
* `-I`: use human column names from the .inf file bundled with the database and display database description on the console. 
* `-o bar.csv`: print the result to the file `bar.csv`
