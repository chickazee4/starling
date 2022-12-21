#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "starling.h"

int
clean_delims(char **out, const char *s, int len)
{
    char *ret = malloc(len);
    memset(ret, 0, len);
    int i = 0, j = 0;
    for(; i < len && j < len; i++){
        switch(s[i]){
            case '\t':
            case '\n':
            case ',':
                break;
            default:
                ret[j] = s[i];
                j++;
                break;
        }
    }
    *out = ret;
    return j;
}

int
clean_tags(char **out, const char *s, int len)
{
    char *ret = malloc(len);
    memset(ret, 0, len);
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
                        ret[j] = s[i];
                        ret[j+1] = s[i+1];
                        i++;
                        j += 2;
                        break;
                }
            }
        } else {
            ret[j] = s[i];
            j++;
        }
    }
    *out = ret;
    return j;
}

int
clean_spaces(char **out, const char *s, int len)
{
    char *ret = malloc(len);
    memset(ret, 0, len);
    int i = 0, j = 0, content_started = 0;
    // part 1 - clear leading spaces and multi-spaces
    for(; i < len && j < len;){
        if(!content_started && isspace(s[i])){
            i++;
        } else if(content_started && i < len - 1 && isspace(s[i]) && isspace(s[i + 1])){
            while(isspace(s[i])) i++;
            ret[j] = ' ';
            j++;
        } else {
            content_started = 1;
            ret[j] = s[i];
            j++;
            i++;
        }
    }
    int rlen = strlen(ret);
    // part 2 - cut off the string when trailing spaces begin, if there are any
    if(ret[rlen - 1] == ' '){
        int k = rlen - 1;
        while(isspace(ret[k])) k--;
        ret[k + 1] = '\0';
        *out = ret;
        return k + 2;
    } else {
        *out = ret;
        return rlen;
    }
}

char *
sanitize(const char *orig, int len, Sanitize_flags *flags)
{
    if(strcmp(orig, "") != 0) {
        char *result = malloc(len);
        memcpy(result, orig, len);
        if(flags != NULL){
            if(flags->clean_delims)
                clean_delims(&result, result, len);
            if(flags->clean_tags)
                clean_tags(&result, result, len);
            if(flags->clean_spaces)
                clean_spaces(&result, result, len);
        }
        return result;
    } else return "";
}
