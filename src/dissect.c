#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "starling.h"

int
parse_records(Starling_db *out, unsigned char *raw, int len)
{
    int db_index = 0;
    out->recs = malloc(out->rec_ct * sizeof(Starling_record));
    for(int i = 0; i < out->rec_ct; i++){
        if(db_index > len){
            return 3;
        }
        Starling_record cur = {};
        out->recs[i] = cur;
        if(raw[db_index] == 0x20)
            out->recs[i].is_deleted = 0;
        else if(raw[db_index] == 0x2A)
            out->recs[i].is_deleted = 1;
        else
            return 4; // corrupted record or database - missing delete flag
        db_index++;
        out->recs[i].entries = calloc(out->hdr_ct, sizeof(Starling_entry));
        for(int k = 0; k < out->hdr_ct; k++){
            out->recs[i].entries[k].hdr_index = k;
            if(out->hdrs[k].length == 6 && out->hdrs[k].type == ft_character){
                out->recs[i].entries[k].type = et_external;
                out->recs[i].entries[k].content = NULL;
                out->recs[i].entries[k].var_offset = (uint32_t)0;
                memcpy(&(out->recs[i].entries[k].var_offset), raw + db_index, 4);
                db_index += 4;
                out->recs[i].entries[k].var_length = (uint16_t)0;
                memcpy(&(out->recs[i].entries[k].var_length), raw + db_index, 2);
                db_index += 2;
            } else {
                out->recs[i].entries[k].type = et_internal;
                out->recs[i].entries[k].content = malloc(out->hdrs[k].length);
                out->recs[i].entries[k].var_offset = (uint32_t)0;
                out->recs[i].entries[k].var_length = (uint16_t)0;
                memcpy(out->recs[i].entries[k].content, raw + db_index, out->hdrs[k].length);
                db_index += out->hdrs[k].length;
            }
            out->recs[i].entries[k].decoded_content = NULL;
        }
    }
    return 0;
}

int
parse_header(Starling_db *out, unsigned char *raw, int len)
{
    out->year = raw[1];
    out->month = raw[2];
    out->day = raw[3];
    out->rec_ct = 0;
    memcpy(&(out->rec_ct), raw + 4, 4);
    out->hdr_size = (uint16_t)0;
    memcpy(&(out->hdr_size), raw + 8, 2);
    out->rec_size = (uint16_t)0;
    memcpy(&(out->rec_size), raw + 10, 2);
    out->hdr_ct = 0;
    for(int i = 32; i < out->hdr_size; i += 32){
        if(i > out->hdr_size - 32) break;
        Starling_record_hdr hdr = {};
        hdr.name = malloc(10);
        memcpy(hdr.name, raw + i, 10);
        switch(raw[i + 11]){
            case 'C':
                hdr.type = ft_character;
                break;
            case 'Y':
                hdr.type = ft_currency;
                break;
            case 'N':
                hdr.type = ft_numeric;
                break;
            case 'F':
                hdr.type = ft_float;
                break;
            case 'D':
                hdr.type = ft_date;
                break;
            case 'T':
                hdr.type = ft_datetime;
                break;
            case 'B':
                hdr.type = ft_double;
                break;
            case 'I':
                hdr.type = ft_integer;
                break;
            case 'L':
                hdr.type = ft_logical;
                break;
            case 'M':
                hdr.type = ft_memo;
                break;
            case 'G':
                hdr.type = ft_general;
                break;
            case 'P':
                hdr.type = ft_picture;
                break;
            default:
                return 2; // bad header
        }
        hdr.offset = 0;
        memcpy(&(hdr.offset), raw + i + 12, 4);
        hdr.length = raw[i + 16];
        hdr.decimal_places = raw[i + 17];
        hdr.flags = raw[i + 18];
        out->hdr_ct++;
        out->hdrs = realloc(out->hdrs, out->hdr_ct * sizeof(Starling_record_hdr));
        out->hdrs[out->hdr_ct - 1] = hdr;
    }
    return 0;
}

int
parse_db(Starling_db *out, unsigned char *raw, int len)
{
    int r = 0;
    if ((r = parse_header(out, raw, len)) > 0) return r;
    else return (parse_records(out, raw + out->hdr_size, len - out->hdr_size));
}

Starling_db *
parse_file(int *result, char *dbf_filename, char *var_filename) // var_filename may be set to NULL if you *KNOW* there will not be any external-referencing entries
{
    if(!result){
        int i = 0;
        result = &i;
    }
    Starling_db *out = malloc(sizeof(Starling_db));

    FILE *fp;
    unsigned char *buf;
    long len;
    if ((fp = fopen(dbf_filename, "rb")) != NULL){
        fseek(fp, 0L, SEEK_END);
        len = ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        buf = (unsigned char *)calloc(len, sizeof(unsigned char));
        fread(buf, sizeof(unsigned char), len, fp);
        fclose(fp);

        if((*result = parse_db(out, buf, len)) > 0){
            return out;
        } else {
            if(var_filename != NULL){
                FILE *vfp;
                unsigned char *vbuf;
                long vlen;
                if((vfp = fopen(var_filename, "rb")) != NULL){
                    fseek(vfp, 0L, SEEK_END);
                    vlen = ftell(vfp);
                    fseek(vfp, 0L, SEEK_SET);
                    vbuf = malloc(vlen);
                    fread(vbuf, sizeof(unsigned char), vlen, vfp);
                    fclose(vfp);
                    out->ext_entries = vbuf;
                    out->ext_len = vlen;
                } else {
                    *result = 5; // invalid var file or inaccessible
                    return out;
                }
            } else {
                out->ext_entries = NULL;
                out->ext_len = 0;
            }
        }
        free(buf);
    }
    return out;
}
