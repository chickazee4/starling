#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "starling.h"

size_t
strlen_newline(const char *s)
{
    int i = 0;
    while(s[i] != '\r' && s[i] != '\n' && s[i] != '\0') i++;
    return i;
}

char *
starling_inf_clean_spaces(const char *s, int len)
{
    char *out = malloc(len);
    memset(out, 0, len);
    int j = 0;
    for(int i = 0; i < len; i++){
        switch(s[i]){
            case '\r':
                break;
            case '\n':
                out[j] = '\n';
                j++;
                while(i < len && isspace(s[i]))
                    i++;
                i--;
                break;
            // fallthrough here is intentional
            case '\t':
                if(i == 0)
                    break; // newline case handles others outside of first char
            case ' ':
                if(i == 0){
                    while(i < len && isspace(s[i]))
                        i++;
                    i--;
                    break;
                }
            default:
                out[j] = s[i];
                j++;
                break;
        }
    }
    return out;
}

char *
starling_inf_clean_alias(const char *s)
{
    int len = strlen_newline(s);
    char *out1 = malloc(3 * len);
    memset(out1, 0, 3 * len);

    int si;
    for(si = 0; si < len; si++){
        if(s[si] == '=') break;
    }
    si += 2;

    int dresult;
    if((dresult = starling_decode_text(&out1, s + si, len - si)) <= 0)
        return NULL;
    
    int j = 0;
    char *out2 = malloc(dresult);
    memset(out2, 0, dresult);

    for(int i = 0; i < dresult; i++){
        switch(out1[i]){
            case '\\':
                i++;
                break;
            case ':':
                if(j == 0)
                    break;
                else {
                    out2[j] = '/';
                    j++;
                }
                break;
            case '\n':
                break;
            default:
                out2[j] = out1[i];
                j++;
                break;
        }
    }
    free(out1);
    return out2;
}

int
starling_inf_is_info_end(const char *line)
{
    int len = strlen_newline(line);
    if(len < 10)
        return 0;   
    else
        return (strncmp(line, "DBINFO END", 10) == 0);
}

int
starling_inf_is_info_start(const char *line)
{
    int len = strlen_newline(line);
    if(len < 12)
        return 0;
    else
        return (strncmp(line, "DBINFO START", 12) == 0);
}

int
starling_inf_is_alias(const char *line)
{
    int len = strlen_newline(line);
    if(len < 11)
        return 0;
    else
        return (strncmp(line, "field_alias", 11) == 0);
}

int
starling_parse_inf(Starling_db *db, const char *inff)
{
    if(!db) return STARLING_PASSED_EMPTY_DB;
    FILE *fp;
    if ((fp = fopen(inff, "r")) != NULL){
        char *line = NULL;
        size_t size;
        int len, index = 0, ninfo = 0, *infostart_p, *infoend_p;
        unsigned char *inf_conts = malloc(256000);
        memset(inf_conts, 0, 256000);

        while((len = getline(&line, &size, fp)) != -1){
            strcat(inf_conts, line);
            index += strlen_newline(line) + 2; // works if using Windows line endings, which infs do. need to add a check to accommodate unix style lines
            if(starling_inf_is_alias(line)){
                unsigned char *key = malloc(11);
                memset(key, 0, 11);
                strncpy(key, line + 12, 10);
                for(int i = 0; i < 10; i++)
                    if(key[i]==']') key[i] = '\0';
                for(int i = 0; i < db->hdr_ct; i++){
                    if(strcmp(key, db->hdrs[i].name) == 0)
                        db->hdrs[i].human_name = starling_inf_clean_alias(line);
                }
                free(key);
            } else if(starling_inf_is_info_start(line)){
                ninfo++;
                infostart_p = realloc(infostart_p, ninfo * sizeof(int));
                infostart_p[ninfo - 1] = index;
            } else if(starling_inf_is_info_end(line)){
                if(ninfo < 1) return STARLING_BAD_INF_FILE;
                infoend_p = realloc(infoend_p, ninfo * sizeof(int));
                infoend_p[ninfo - 1] = index - strlen_newline(line) - 2;
            }
        }

        char *infos = malloc(256000);
        memset(infos, 0, 256000);
        int dlen;

        for(int i = 0; i < ninfo; i++){
            dlen = infoend_p[i] - infostart_p[i] + 1;
            char *decoded = malloc(dlen), *cleaned = NULL;
            memset(decoded, 0, dlen);
            if((dlen = starling_decode_text(&decoded, inf_conts + infostart_p[i], infoend_p[i] - infostart_p[i])) <= 0)
                return STARLING_BAD_INF_FILE;
            // db descriptions tend to include tags and spaces that can be cleaned out
            starling_clean_tags(&cleaned, decoded, dlen);
            strcat(infos, starling_inf_clean_spaces(cleaned, strlen(cleaned)));
            // avoid taking up unnecessary mem
            free(decoded);
            free(cleaned);
        }
        free(inf_conts);
        db->db_description = infos;
        return STARLING_OK;
    } else return STARLING_BAD_INF_FILE;
}