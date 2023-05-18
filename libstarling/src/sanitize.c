#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "starling.h"

int
starling_clean_delims(char **out, const char *s, int len)
{
    memset(*out, 0, len);
    int i = 0, j = 0;
    for(; i < len && j < len; i++){
        switch(s[i]){
            case ',':
            case '\t':
            case '\n':
                (*out)[j] = ' ';
                j++;
                break;
            default:
                (*out)[j] = s[i];
                j++;
                break;
        }
    }
    return j;
}

int
starling_clean_tags(char **out, const char *s, int len)
{
    memset(*out, 0, len);
    int i = 0, j = 0;
    for(; i < len && j < len; i++){
        if(s[i] == '\\'){
            if(i < len - 1){
                switch(s[i+1]) {
                    case 'B':
                    case 'b':
                    case 'I':
                    case 'i':
                    case 'U':
                    case 'u':
                    case 'H':
                    case 'h':
                    case 'L':
                    case 'l':
                    case 'C':
                    case 'c':
                    case 'x':
                        i++;
                        break;
                    case 'X':
                        if(i < len - 2 && isdigit(s[i+2])){
                            i+=2;
                            break;
                        } else if (i < len - 2 && s[i+2] == '<'){
                            i += 3;
                            while(s[i] != '>') i++;
                            break;
                        } else break;
                    default:
                        (*out)[j] = s[i];
                        (*out)[j+1] = s[i+1];
                        i++;
                        j += 2;
                        break;
                }
            }
        } else {
            (*out)[j] = s[i];
            j++;
        }
    }
    return j;
}

int
starling_clean_spaces(char **out, const char *s, int len)
{
    memset(*out, 0, len);
    int i = 0, j = 0, content_started = 0;
    // part 1 - clear leading spaces and multi-spaces
    for(; i < len && j < len;){
        if(!content_started && isspace(s[i])){
            i++;
        } else if(content_started && i < len - 1 && isspace(s[i]) && isspace(s[i + 1])){
            while(isspace(s[i])) i++;
            i--;
        } else {
            content_started = 1;
            (*out)[j] = s[i];
            j++;
            i++;
        }
    }
    int rlen = strlen(*out);
    // part 2 - cut off the string when trailing spaces begin, if there are any
    if((*out)[rlen - 1] == ' '){
        int k = rlen - 1;
        while(isspace((*out)[k])) k--;
        (*out)[k + 1] = '\0';
        return k + 2;
    } else {
        return rlen;
    }
}

int
starling_sanitize(char **out, const char *orig, int olen, Starling_sanitize_flags *flags)
{
    int len = olen;
    *out = NULL;
    if(orig && (strcmp(orig, "") != 0)) {
        *out = malloc(len);
        memset(*out, 0, len);
        strncpy(*out, orig, len);
        if(flags){
            if(flags->clean_delims){
                char *dres = malloc(len);
                len = starling_clean_delims(&dres, *out, len);
                memset(*out, 0, olen);
                strncpy(*out, dres, len);
                free(dres);
            }
            if(flags->clean_tags){
                char *tres = malloc(len);
                len = starling_clean_tags(&tres, *out, len);
                memset(*out, 0, olen);
                strncpy(*out, tres, len);
                free(tres);
            }
            if(flags->clean_spaces){
                char *sres = malloc(len);
                len = starling_clean_spaces(&sres, *out, len);
                memset(*out, 0, olen);
                strncpy(*out, sres, len);
                free(sres);
            }
        } else return STARLING_PASSED_EMPTY_TABLE_FLAGS;
        return STARLING_OK;
    } else {
        return STARLING_OK & STARLING_EMPTY_CONTENTS;
    }
}
