#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <libintl.h>

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
    bindtextdomain("starling", LOCALEDIR);
    textdomain("starling");
    switch(ft){
        case FT_CHARACTER:
            return gettext("character");
        case FT_CURRENCY:
            return gettext("currency");
        case FT_DATE:
            return gettext("date");
        case FT_DATETIME:
            return gettext("time");
        case FT_DOUBLE:
            return gettext("double");
        case FT_FLOAT:
            return gettext("float");
        case FT_GENERAL:
            return gettext("general");
        case FT_INTEGER:
            return gettext("integer");
        case FT_LOGICAL:
            return gettext("logical");
        case FT_MEMO:
            return gettext("memo");
        case FT_NUMERIC:
            return gettext("numeric");
        case FT_PICTURE:
            return gettext("picture");
        default:
            return NULL;
    }
}

char *
starling_tabulate_fields(Starling_db *db, char *delim, int withinf)
{
    bindtextdomain("starling", LOCALEDIR);
    textdomain("starling");
    int rlen = db->hdr_ct * 30 + strlen(delim) * (db->hdr_ct + 2) + 15;
    char *ret = malloc(rlen), *length = malloc(10);
    memset(ret, 0, rlen);
    memset(length, 0, 10);
    if(withinf)
        sprintf(ret, "%s%s%s%s%s%s%s\n", gettext("internal_name"), delim, gettext("human_name"), delim, gettext("type"), delim, gettext("length"));
    else
        sprintf(ret, "%s%s%s%s%s\n", gettext("name"), delim, gettext("type"), delim, gettext("length"));
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
