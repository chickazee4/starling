#ifndef STARLING_DATABASE_H
#define STARLING_DATABASE_H
#include <stdint.h>
#ifndef NULL
#define NULL (void *)0x00
#endif
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    STARLING_OK,
    STARLING_BAD_HDR,
    STARLING_BAD_DELETE_FLAG,
    STARLING_BAD_DB_LEN,
    STARLING_BAD_DBF_FILE,
    STARLING_BAD_VAR_FILE,
    STARLING_BAD_INF_FILE,
    STARLING_TABULATE_FAILED,
    STARLING_BAD_FLAG,
    STARLING_MEM_ERR,
    STARLING_PASSED_EMPTY_DB,
    STARLING_PASSED_EMPTY_TABLE_FLAGS,
    STARLING_EMPTY_CONTENTS // not necessarily an error
} Starling_return_code;

typedef enum { // record 0x11
    FT_CHARACTER, // C
    FT_CURRENCY, // Y
    FT_NUMERIC, // N
    FT_FLOAT, // F
    FT_DATE, // D
    FT_DATETIME, // T
    FT_DOUBLE, // B
    FT_INTEGER, // I
    FT_LOGICAL, // L
    FT_MEMO, // M
    FT_GENERAL, // G
    FT_PICTURE // P
} Starling_field_type;

typedef enum {
    ET_EXTERNAL,
    ET_INTERNAL
} Starling_entry_type;

typedef struct {
    char *name;
    Starling_field_type type;
    uint32_t offset; // do not trust per Verhoeven
    unsigned char length;
    unsigned char decimal_places;
    unsigned char flags;
    char *human_name;
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
    char *db_description; // human-readable description from .inf
    int is_altai; // altaic needs an adjustment due to internal corruption in the version distributed
    int is_stibet; // likewise
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
    int use_human_names;
} Starling_table_flags;

extern const char * unibyte[256];
extern const char * special_unibyte[256];
extern const char * grk_dualbyte[256];
extern const char * cyr_dualbyte[256];

// dissect.c
int           starling_parse_records(Starling_db *, unsigned char *, int);
int           starling_parse_header(Starling_db *, unsigned char *, int);
int           starling_parse_db(Starling_db *, unsigned char *, int);
Starling_db * starling_parse_file(int *, char *, char *);

// decode.c
int    starling_decode_text(char **, unsigned char *, int);
void   starling_decode_external(Starling_entry *, unsigned char **);
void   starling_decode_all_text(Starling_db *);
void   starling_decode_all_external(Starling_db *);
void   starling_decode_all(Starling_db *);

// inf.c
char * starling_inf_clean_spaces(const char *, int);
char * starling_inf_clean_alias(const char *);
int    starling_inf_is_info_end(const char *);
int    starling_inf_is_info_start(const char *);
int    starling_inf_is_alias(const char *);
int    starling_parse_inf(Starling_db *, const char *);

// sanitize.c
int starling_clean_delims(char **, const char *, int);
int starling_clean_tags(char **, const char *, int);
int starling_clean_spaces(char **, const char *, int);
int starling_sanitize(char **, const char *, int, Starling_sanitize_flags *);

// tabulate.c
const char * starling_fieldtypetostr(Starling_field_type);
char       * starling_tabulate_fields(Starling_db *, char *, int);
char       * starling_tabulate_db_wide(Starling_db *, Starling_table_flags *);
char       * starling_tabulate_db_tall(Starling_db *, Starling_table_flags *);
char       * starling_tabulate_db(Starling_db *, Starling_table_flags *);
#ifdef __cplusplus
}
#endif
#endif
