#ifndef STARLING_H
#define STARLING_H
#include <stdint.h>
#ifndef NULL
#define NULL (void *)0x00
#endif

typedef enum { // record 0x11
    ft_character, // C
    ft_currency, // Y
    ft_numeric, // N
    ft_float, // F
    ft_date, // D
    ft_datetime, // T
    ft_double, // B
    ft_integer, // I
    ft_logical, // L
    ft_memo, // M
    ft_general, // G
    ft_picture // P
} Field_type;

typedef enum {
    et_external,
    et_internal
} Entry_type;

typedef struct {
    char *name;
    Field_type type;
    uint32_t offset; // do not trust per Verhoeven
    unsigned char length;
    unsigned char decimal_places;
    unsigned char flags;
} Starling_record_hdr;

typedef struct {
    int hdr_index;
    Entry_type type;
    uint32_t var_offset; // not set if internally defined in .dbf
    uint16_t var_length; // see above
    int true_length; // length of decoded version
    unsigned char *content; // this is NOT sanitized / decoded!
    char *decoded_content; // for use when decoded
} Starling_entry;

typedef struct {
    int is_deleted;
    Starling_entry *entries;
} Starling_record;

typedef struct {
    unsigned char year;
    unsigned char month;
    unsigned char day;
    uint32_t rec_ct;
    uint16_t hdr_size;
    uint16_t rec_size;
    Starling_record_hdr *hdrs;
    uint32_t hdr_ct;
    Starling_record *recs;
    unsigned char *ext_entries; // i.e. contents of var file
    uint32_t ext_len; // length of ext buffer, not number of entries
} Starling_db;

typedef struct {
    int clean_delims;
    int clean_tags;
    int clean_spaces;
} Sanitize_flags;

typedef struct {
    int lowercase_labels;
    int label_rows;
    char *delimiter;
    Sanitize_flags *sanitize; // can be null
} Table_flags;

#define DELIMS  "[\t\n,]"
#define TAGS    "\\\\[BbIiUuHhLlCcXx]"
#define PADDING "^\\s*(.*)\\s*$"

extern const char * unibyte[256];
extern const char * special_unibyte[256];
extern const char * grk_dualbyte[256];
extern const char * cyr_dualbyte[256];

// dissect.c
int parse_records(Starling_db *, unsigned char *, int);
int parse_header(Starling_db *, unsigned char *, int);
int parse_db(Starling_db *, unsigned char *, int);
Starling_db * parse_file(int *, char *, char *);

// decode.c
char * decode_text(unsigned char *, int);
void   decode_external(Starling_entry *, unsigned char **);
void   decode_all_text(Starling_db *);
void   decode_all_external(Starling_db *);
void   decode_all(Starling_db *);

// sanitize.c
int    clean_delims(char **, const char *, int);
int    clean_tags(char **, const char *, int);
int    clean_spaces(char **, const char *, int);
char * sanitize(const char *, int, Sanitize_flags *);

// tabulate.c
char       * lowercase(const char *);
const char * stringify_ft(Field_type ft);
char       * tabulate(Starling_db *, Table_flags *);
#endif
