#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "starling.h"

char *
lowercase(const char *in)
{
    int len = strlen(in);
    char *out = malloc(len + 1);
    memset(out, 0, len + 1);
    for(int i = 0; i < len; i++){
        out[i] = tolower(in[i]);
    }
    return out;
}

const char *
starling_fieldtypetostr(Starling_field_type ft)
{
#ifdef HAS_LIBINTL_H
    bindtextdomain("starling", LOCALEDIR);
    textdomain("starling");
#endif
    switch(ft){
        case FT_CHARACTER:
            return _("character");
        case FT_CURRENCY:
            return _("currency");
        case FT_DATE:
            return _("date");
        case FT_DATETIME:
            return _("time");
        case FT_DOUBLE:
            return _("double");
        case FT_FLOAT:
            return _("float");
        case FT_GENERAL:
            return _("general");
        case FT_INTEGER:
            return _("integer");
        case FT_LOGICAL:
            return _("logical");
        case FT_MEMO:
            return _("memo");
        case FT_NUMERIC:
            return _("numeric");
        case FT_PICTURE:
            return _("picture");
        default:
            return NULL;
    }
}

char *
starling_tabulate_fields(Starling_db *db, char *delim, int withinf)
{
#ifdef HAS_LIBINTL_H
    bindtextdomain("starling", LOCALEDIR);
    textdomain("starling");
#endif
    char *ret = NULL, *length = malloc(10);
    int clen = 0, dlen = strlen(delim);
    memset(length, 0, 10);
    for(int i = 0; i < db->hdr_ct; i++){
        clen += strlen(db->hdrs[i].name) + dlen + 2;
        ret = realloc(ret, clen);
        if(i == 0) memset(ret, 0, clen);
        strcat(ret, db->hdrs[i].name);
        strcat(ret, delim);
        if(withinf){
            if(db->hdrs[i].human_name){
                clen += strlen(db->hdrs[i].human_name) + 1;
                ret = realloc(ret, clen);
                strcat(ret, db->hdrs[i].human_name);
            }
            clen += dlen + 1;
            ret = realloc(ret, dlen);
            strcat(ret, delim);
        }
        clen += strlen(starling_fieldtypetostr(db->hdrs[i].type)) + dlen + 13;
        ret = realloc(ret, clen);
        strcat(ret, starling_fieldtypetostr(db->hdrs[i].type));
        strcat(ret, delim);
        snprintf(length, 10, "%i", (int)db->hdrs[i].length);
        strcat(ret, length);
        strcat(ret, "\n");
    }
    free(length);
    return ret;
}

char *
starling_tabulate_db_tall(Starling_db *db, Starling_table_flags *flags)
{
    char *result = NULL, *sanitized = NULL, *header = NULL;
    int clen = 0, dlen = strlen(flags->delimiter);
    for(int i = 0; i < db->hdr_ct; i++){
        if(flags->lowercase_labels){
            header = lowercase((flags->use_human_names ? db->hdrs[i].human_name : db->hdrs[i].name));
            clen += strlen(header) + 1;
            result = realloc(result, clen);
            strcat(result, header);
            free(header);
        } else {
            header = (flags->use_human_names ? db->hdrs[i].human_name : db->hdrs[i].name);
            clen += strlen(header) + 1;
            result = realloc(result, clen);
            strcat(result, header);
        }
        if(i < db->hdr_ct - 1){
            clen += dlen + 1;
            result = realloc(result, clen);
            strcat(result, flags->delimiter);
        }
    }
    clen += 2;
    result = realloc(result, clen);
    strcat(result, "\n");
    for(int i = 0; i < db->rec_ct; i++){
        if(!(flags->exclude_deleted && db->recs[i].is_deleted)){
            for(int j = 0; j < db->hdr_ct; j++){
                if(((starling_sanitize(&sanitized, db->recs[i].entries[j].decoded_content, db->recs[i].entries[j].true_length, flags->sanitize)) != STARLING_PASSED_EMPTY_TABLE_FLAGS) && sanitized){
                    clen += strlen(sanitized) + 1;
                    result = realloc(result, clen);
                    strcat(result, sanitized);
                    free(sanitized);
                }
                if(j < db->hdr_ct - 1){
                    clen += 2;
                    result = realloc(result, clen);
                    strcat(result, flags->delimiter);
                }
            }
            clen += 2;
            result = realloc(result, clen);
            strcat(result, "\n");
        }
    }
    return result;
}

char *
starling_tabulate_db_wide(Starling_db *db, Starling_table_flags *flags)
{
    char *result = NULL, *sanitized = NULL, *header = NULL;
    int clen = 0, dlen = strlen(flags->delimiter);
    for(int i = 0; i < db->hdr_ct; i++){
        if(flags->lowercase_labels){
            header = lowercase((flags->use_human_names ? db->hdrs[i].human_name : db->hdrs[i].name));
            clen += strlen(header) + 1;
            result = realloc(result, clen);
            strcat(result, header);
            free(header);
        } else {
            header = (flags->use_human_names ? db->hdrs[i].human_name : db->hdrs[i].name);
            clen += strlen(header) + 1;
            result = realloc(result, clen);
            strcat(result, header);
        }
        clen += dlen + 1;
        result = realloc(result, clen);
        strcat(result, flags->delimiter);
        for(int j = 0; j < db->rec_ct; j++){
            if(!(flags->exclude_deleted && db->recs[j].is_deleted)){
                if(((starling_sanitize(&sanitized, db->recs[j].entries[i].decoded_content, db->recs[j].entries[i].true_length, flags->sanitize)) != STARLING_PASSED_EMPTY_TABLE_FLAGS) && sanitized){
                    clen += strlen(sanitized) + dlen + 1;
                    result = realloc(result, clen);
                    strcat(result, sanitized);
                    free(sanitized);
                    strcat(result, flags->delimiter);
                }
            }
        }
        clen += 2;
        result = realloc(result, clen);
        strcat(result, "\n");
    }
    return result;
}

char *
starling_tabulate_db(Starling_db *db, Starling_table_flags *flags)
{
    if(flags->label_rows)
        return starling_tabulate_db_wide(db, flags);
    else
        return starling_tabulate_db_tall(db, flags);
}
