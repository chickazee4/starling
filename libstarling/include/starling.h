#ifndef STARLING_DATABASE_H
#define STARLING_DATABASE_H
#include <stdint.h>
#ifndef NULL
#define NULL (void *)0x00
#endif
#ifdef __cplusplus
extern "C" {
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
} Starling_field_type;

typedef enum {
    et_external,
    et_internal
} Starling_entry_type;

typedef struct {
    char *name;
    Starling_field_type type;
    uint32_t offset; // do not trust per Verhoeven
    unsigned char length;
    unsigned char decimal_places;
    unsigned char flags;
} Starling_record_hdr;

typedef struct {
    int hdr_index;
    Starling_entry_type type;
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
} Starling_sanitize_flags;

typedef struct {
    int lowercase_labels;
    int label_rows;
    int exclude_deleted;
    char *delimiter;
    Starling_sanitize_flags *sanitize; // can be null
} Starling_table_flags;

extern const char * unibyte[256];
extern const char * special_unibyte[256];
extern const char * grk_dualbyte[256];
extern const char * cyr_dualbyte[256];

// dissect.c
int starling_parse_records(Starling_db *, unsigned char *, int);
int starling_parse_header(Starling_db *, unsigned char *, int);
int starling_parse_db(Starling_db *, unsigned char *, int);
Starling_db * starling_parse_file(int *, char *, char *);

// decode.c
int    starling_decode_text(char **, unsigned char *, int);
void   starling_decode_external(Starling_entry *, unsigned char **);
void   starling_decode_all_text(Starling_db *);
void   starling_decode_all_external(Starling_db *);
void   starling_decode_all(Starling_db *);

// sanitize.c
int    starling_clean_delims(char **, const char *, int);
int    starling_clean_tags(char **, const char *, int);
int    starling_clean_spaces(char **, const char *, int);
char * starling_sanitize(const char *, int, Starling_sanitize_flags *);

// tabulate.c
const char * starling_fieldtypetostr(Starling_field_type ft);
char       * starling_tabulate_fields(Starling_db *, char *);
char       * starling_tabulate_db(Starling_db *, Starling_table_flags *);
#ifdef __cplusplus
}
#endif
#endif
