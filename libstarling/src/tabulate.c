#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "starling.h"

char *
lowercase(const char *in)
{
    int len = strlen(in);
    char *out = malloc(len);
    for(int i = 0; i < len; i++){
        out[i] = tolower(in[i]);
    }
    return out;
}

const char *
starling_fieldtypetostr(Starling_field_type ft)
{
    switch(ft){
        case FT_CHARACTER:
            return "character";
        case FT_CURRENCY:
            return "currency";
        case FT_DATE:
            return "date";
        case FT_DATETIME:
            return "time";
        case FT_DOUBLE:
            return "double";
        case FT_FLOAT:
            return "float";
        case FT_GENERAL:
            return "general";
        case FT_INTEGER:
            return "integer";
        case FT_LOGICAL:
            return "logical";
        case FT_MEMO:
            return "memo";
        case FT_NUMERIC:
            return "numeric";
        case FT_PICTURE:
            return "picture";
        default:
            return NULL;
    }
}

char *
starling_tabulate_fields(Starling_db *db, char *delim, int withinf)
{
    int rlen = db->hdr_ct * 30 + strlen(delim) * (db->hdr_ct + 2) + 15;
    char *ret = malloc(rlen), *length = malloc(10);
    memset(ret, 0, rlen);
    memset(length, 0, 10);
    if(withinf)
        sprintf(ret, "internal_name%shuman_name%stype%slength\n", delim, delim, delim);
    else
        sprintf(ret, "name%stype%slength\n", delim, delim);
    for(int i = 0; i < db->hdr_ct; i++){
        strcat(ret, db->hdrs[i].name);
        strcat(ret, delim);
        if(withinf){
            if(db->hdrs[i].human_name)
                strcat(ret, db->hdrs[i].human_name);
            strcat(ret, delim);
        }
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
starling_tabulate_db(Starling_db *db, Starling_table_flags *flags)
{
    char *result = malloc(25600000), *sanitized = NULL;
    memset(result, 0, 25600000);
    if(flags->label_rows){
        for(int i = 0; i < db->hdr_ct; i++){
            if(flags->lowercase_labels)
                strcat(result, lowercase((flags->use_human_names ? db->hdrs[i].human_name : db->hdrs[i].name)));
            else
                strcat(result, (flags->use_human_names ? db->hdrs[i].human_name : db->hdrs[i].name));
            strcat(result, flags->delimiter);
            for(int j = 0; j < db->rec_ct; j++){
                if(!(flags->exclude_deleted && db->recs[j].is_deleted)){
                    if((sanitized = starling_sanitize(db->recs[j].entries[i].decoded_content, db->recs[i].entries[j].true_length, flags->sanitize))){
                        strcat(result, sanitized);
                        strcat(result, flags->delimiter);
                    } else {
                        return NULL; // error
                    }
                }
            }
            strcat(result, "\n");
        }
    } else {
        for(int i = 0; i < db->hdr_ct; i++){
            if(flags->lowercase_labels)
                strcat(result, lowercase((flags->use_human_names ? db->hdrs[i].human_name : db->hdrs[i].name)));
            else strcat(result, (flags->use_human_names ? db->hdrs[i].human_name : db->hdrs[i].name));
            if(i < db->hdr_ct - 1) strcat(result, flags->delimiter);
        }
        strcat(result, "\n");
        for(int i = 0; i < db->rec_ct; i++){
            if(!(flags->exclude_deleted && db->recs[i].is_deleted)){
                for(int j = 0; j < db->hdr_ct; j++){
                    if((sanitized = starling_sanitize(db->recs[i].entries[j].decoded_content, db->recs[i].entries[j].true_length, flags->sanitize))){
                        strcat(result, sanitized);
                        if(j < db->hdr_ct - 1) strcat(result, flags->delimiter);
                    } else {
                        return NULL; // error
                    }
                }
                strcat(result, "\n");
            }
        }
    }
    return result;
}
