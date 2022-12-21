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
stringify_ft(Field_type ft)
{
    switch(ft){
        case ft_character:
            return "character";
        case ft_currency:
            return "currency";
        case ft_date:
            return "date";
        case ft_datetime:
            return "time";
        case ft_double:
            return "double";
        case ft_float:
            return "float";
        case ft_general:
            return "general";
        case ft_integer:
            return "integer";
        case ft_logical:
            return "logical";
        case ft_memo:
            return "memo";
        case ft_numeric:
            return "numeric";
        case ft_picture:
            return "picture";
        default:
            return NULL;
    }
}

char *
tabulate(Starling_db *db, Table_flags *flags)
{
    char *result = malloc(25600000), *sanitized = NULL;
    if(flags->label_rows){
        for(int i = 0; i < db->hdr_ct; i++){
            if(flags->lowercase_labels)
                strcat(result, lowercase(db->hdrs[i].name));
            else
                strcat(result, db->hdrs[i].name);
            strcat(result, flags->delimiter);
            for(int j = 0; j < db->rec_ct; j++){
                if((sanitized = sanitize(db->recs[j].entries[i].decoded_content, db->recs[j].entries[i].true_length, flags->sanitize))){
                    strcat(result, sanitized);
                    strcat(result, flags->delimiter);
                } else {
                    return NULL; // error
                }
            }
            strcat(result, "\n");
        }
    } else {
        for(int i = 0; i < db->hdr_ct; i++){
            if(flags->lowercase_labels)
                strcat(result, lowercase(db->hdrs[i].name));
            else strcat(result, db->hdrs[i].name);
            if(i < db->hdr_ct - 1) strcat(result, flags->delimiter);
        }
        strcat(result, "\n");
        for(int i = 0; i < db->rec_ct; i++){
            for(int j = 0; j < db->hdr_ct; j++){
                if((sanitized = sanitize(db->recs[i].entries[j].decoded_content, db->recs[i].entries[j].true_length, flags->sanitize))){
                    strcat(result, sanitized);
                    if(j < db->hdr_ct - 1) strcat(result, flags->delimiter);
                } else {
                    return NULL; // error
                }
            }
            strcat(result, "\n");
        }
    }
    return result;
}
