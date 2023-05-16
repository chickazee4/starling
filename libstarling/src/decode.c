#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "starling.h"

const char *unibyte[256] = {
//  0         1         2         3         4         5                 6               7               8               9         A         B         C         D         E                 F
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,             NULL,           NULL,           NULL,           NULL,     NULL,     NULL,     NULL,     NULL,     NULL,             NULL,       // 0
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,             NULL,           NULL,           NULL,           NULL,     NULL,     NULL,     NULL,     NULL,     NULL,             NULL,       // 1
    " ",      "!",      "\"",     "#",      "$",      "%",              "&",            "\'",           "(",            ")",      "*",      "+",      ",",      "-",      ".",              "/",        // 2
    "0",      "1",      "2",      "3",      "4",      "5",              "6",            "7",            "8",            "9",      ":",      ";",      "<",      "=",      ">",              "?",        // 3
    "\u0040", "A",      "B",      "C",      "D",      "E",              "F",            "G",            "H",            "I",      "J",      "K",      "L",      "M",      "N",              "O",        // 4
    "P",      "Q",      "R",      "S",      "T",      "U",              "V",            "W",            "X",            "Y",      "Z",      "[",      "\\",     "]",      "\u0302",         "_",        // 5
    "\u0060", "a",      "b",      "c",      "d",      "e",              "f",            "g",            "h",            "i",      "j",      "k",      "l",      "m",      "n",              "o",        // 6
    "p",      "q",      "r",      "s",      "t",      "u",              "v",            "w",            "x",            "y",      "z",      "{",      "|",      "}",      "\u0303",         NULL,       // 7
    "\u0410", "\u0411", "\u0412", "\u0413", "\u0414", "\u0415",         "\u0416",       "\u0417",       "\u0418",       "\u0419", "\u041A", "\u041B", "\u041C", "\u041D", "\u041E",         "\u041F",   // 8
    "\u0420", "\u0421", "\u0422", "\u0423", "\u0424", "\u0425",         "\u0426",       "\u0427",       "\u0428",       "\u0429", "\u042A", "\u042B", "\u042C", "\u042D", "\u042E",         "\u042F",   // 9
    "\u0430", "\u0431", "\u0432", "\u0433", "\u0434", "\u0435",         "\u0436",       "\u0437",       "\u0438",       "\u0439", "\u043A", "\u043B", "\u043C", "\u043D", "\u043E",         "\u043F",   // A
    "\u0101", "\u0301", "\u00E4", "\u0328", "\u01DF", "c\u0323",        "\u010D",       "\u010D\u0323", "\u03B4",       "\u0113", NULL,     "\u0307", "\u0308", "\u025B", "\u02A1",         "\u032F",   // B
    "\u00E7", "\u0263", "\u0281", "\u0127", "\u0304", "\u012B",         "\u0268",       "\u0268\u0304", "\u030A",       "\u0325", "\u1E33", "\u028E", "\u019B", "-",      "\u019B\u0323",   "\u026B",   // C
    "\u2C62", "\u014B", "\u014D", "\u00F6", "\u022B", "\u0254",         "\u0254\u0304", "\u1E57",       "q\u0307",      "\u00DF", "~",      "\u0300", "\u0323", "\u0161", "\u1E6D",         "\u0306",   // D
    "\u0440", "\u0441", "\u0442", "\u0443", "\u0444", "\u0445",         "\u0446",       "\u0447",       "\u0448",       "\u0449", "\u044A", "\u044B", "\u044C", "\u044D", "\u044E",         "\u044F",   // E
    "\u03D1", "\u016B", "\u00FC", "\u01D6", "\u0259", "\u0259\u0304",   "\u030C",       "\u02B7",       "\u0266",       "\u03C7", "\u0292", "\u01EF", "\u017E", "\u0294", "\u0295",         "\u028C"    // F
};

const char *special_unibyte[256] = {
//  0         1         2         3         4         5         6         7         8         9         A         B         C         D         E         F
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // 0
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // 1
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // 2
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // 3
    NULL,     "\u04D4", NULL,     NULL,     "\u0111", NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     "\u026C", NULL,     NULL,     NULL,     // 4
    NULL,     NULL,     NULL,     NULL,     "\u0167", NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     "\u0311", NULL,     // 5
    NULL,     "\u00E6", "\u0180", "\u0255", "\u00F0", "\u0153", NULL,     "\u01E5", "\u1E2B", "\u0131", "\u0237", NULL,     NULL,     NULL,     NULL,     "\u00F8", // 6
    NULL,     NULL,     "\u027E", "\u0283", "\u00FE", NULL,     NULL,     NULL,     "\u1D9A", NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // 7
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // 8
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // 9
    NULL,     NULL,     NULL,     "\u0260", NULL,     "\u0467", NULL,     "\u0455", NULL,     NULL,     NULL,     "\u0459", NULL,     "\u045A", "\u046B", NULL,     // A
    "\u030B", NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     "\u032E", // B
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // C
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     "\u030F", NULL,     NULL,     NULL,     NULL,     // D
    "\u0281", NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     "\u0454", "\u017E", // E
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     "\u0195", NULL,     "\u0452", NULL,     NULL,     NULL,     NULL,     NULL,     // F
};

const char *grk_dualbyte[256] = { // first byte 0x83
//  0                   1                 2               3         4         5         6         7         8         9         A                 B                   C               D               E         F
    NULL,               NULL,             NULL,           NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,             NULL,               NULL,           NULL,           NULL,     NULL,     // 0
    NULL,               NULL,             NULL,           NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,             NULL,               NULL,           NULL,           NULL,     NULL,     // 1
    NULL,               NULL,             NULL,           NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,             NULL,               NULL,           NULL,           NULL,     NULL,     // 2
    NULL,               NULL,             NULL,           NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,             NULL,               NULL,           NULL,           NULL,     NULL,     // 3
    NULL,               NULL,             NULL,           NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,             NULL,               NULL,           NULL,           NULL,     NULL,     // 4
    NULL,               NULL,             NULL,           NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,             NULL,               NULL,           NULL,           NULL,     NULL,     // 5
    NULL,               NULL,             NULL,           NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,             NULL,               NULL,           NULL,           NULL,     NULL,     // 6
    NULL,               NULL,             NULL,           NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,             NULL,               NULL,           NULL,           NULL,     NULL,     // 7
    NULL,               NULL,             NULL,           NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,             NULL,               NULL,           NULL,           NULL,     NULL,     // 8
    "\u0344",           "\u0314\u0301",   "\u0313\u0301", "\u0301", NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     "\u0308\u0300",   "\u0314\u0300",     "\u0313\u0300", "\u0314\u0342", "\u0387", NULL,     // 9
    "\u0314",           "\u0313",         NULL,           "\u0308", "\u0391", "\u0392", "\u03A7", "\u0394", "\u0395", "\u03A6", "\u0393",         "\u0397",           "\u0399",       "\u1FF3",       "\u039A", "\u039B", // A
    "\u039C",           "\u039D",         "\u039F",       "\u03A0", "\u0398", "\u03A1", "\u03A3", "\u03A4", "\u03A5", "\u1FC3", "\u03A9",         "\u039E",           "\u03A8",       "\u0396",       NULL,     NULL,     // B
    "\u0313\u0342",     "\u0300",         "\u03B1",       "\u03B2", "\u03C7", "\u03B4", "\u03B5", "\u03C6", "\u03B3", "\u03B7", "\u03B9",         "\u03C2",           "\u03BA",       "\u03BB",       "\u03BC", "\u03BD", // C
    "\u03BF",           "\u03C0",         "\u03B8",       "\u03C1", "\u03C3", "\u03C4", "\u03C5", "\u1FB3", "\u03C9", "\u03BE", "\u03C8",         "\u03B6",           "\u0342",       NULL,           NULL,     NULL,     // D
    NULL,               NULL,             NULL,           NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,             NULL,               NULL,           NULL,           NULL,     NULL,     // E
    NULL,               NULL,             NULL,           NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,             NULL,               NULL,           NULL,           NULL,     NULL,     // F
};

const char *cyr_dualbyte[256] = { // first byte 0x87
//  0         1         2         3                 4         5         6         7         8         9         A         B         C         D         E         F
    NULL,     NULL,     NULL,     NULL,             NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // 0
    NULL,     NULL,     NULL,     NULL,             NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // 1
    NULL,     NULL,     NULL,     NULL,             NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // 2
    NULL,     NULL,     NULL,     NULL,             NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // 3
    NULL,     NULL,     NULL,     NULL,             NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // 4
    NULL,     NULL,     NULL,     NULL,             NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // 5
    NULL,     NULL,     NULL,     NULL,             NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // 6
    NULL,     NULL,     NULL,     NULL,             NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // 7
    NULL,     NULL,     NULL,     "\u0433\u0311",   "\u0431", NULL,     "\u044E", NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // 8
    NULL,     NULL,     NULL,     "\u0436",         NULL,     NULL,     "\u042E", NULL,     NULL,     NULL,     "\u0418", "\u0421", NULL,     NULL,     "\u0410", "\u041F", // 9
    "\u0420", NULL,     "\u041E", "\u041B",         "\u0414", NULL,     NULL,     NULL,     "\u0417", NULL,     "\u041A", NULL,     "\u0415", "\u0413", "\u041C", NULL,     // A
    NULL,     "\u041D", NULL,     "\u0445",         NULL,     NULL,     NULL,     NULL,     "\u044A", "\u0444", "\u0438", "\u0441", "\u0432", "\u0443", "\u0430", "\u043F", // B
    "\u0440", "\u0448", "\u043E", "\u043B",         "\u0434", "\u044C", "\u0442", NULL,     "\u0437", NULL,     "\u043A", "\u044B", "\u0435", "\u0433", "\u043C", "\u0446", // C
    "\u0447", "\u043D", "\u044F", "\u0425",         NULL,     "\u0463", NULL,     NULL,     "\uA657", NULL,     "\u0467", NULL,     NULL,     NULL,     NULL,     NULL,     // D
    NULL,     NULL,     NULL,     NULL,             NULL,     NULL,     NULL,     "\u046B", "\u0415", "e",      NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // E
    NULL,     NULL,     "\u0461", "\u046C",         "\u046D", "\u0464", "\u0465", NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     // F
};

int
starling_decode_text(char **out, unsigned char *in, int len)
{
    if(in == NULL || len == 8224){
        char *ret = "";
        *out = ret;
        return STARLING_OK;
    }
    int bytemode = 1, cpos = 0, clen = 0;
    const char *(*current)[256] = &unibyte;
    char *ret = malloc(len * 4);
    memset(ret, 0, len * 4);
    for(int i = 0; i < len; i++){
        switch(in[i]){
            case 0x00:
                break;
            case 0x01:
                bytemode = 2;
                break;
            case 0x09: // tab
                ret[cpos] = '\t';
                cpos++;
                break;
            case 0x0A: // newline
                ret[cpos] = '\n';
                cpos++;
                if(i < len - 1 && in[i+1] == 0x0D) // "occasionally 0x0D follows 0x0A, together forming one newline"
                    i++;
                break;
            case 0x15: // "new paragraph"
                ret[cpos] = '\n';
                cpos++;
                break;
            case 0x1D:
                current = &special_unibyte;
                break;
            case 0x7F:
                bytemode = 1;
                break;
            case 0x83:
                current = &grk_dualbyte;
                break;
            case 0x85:
                if(bytemode == 2 && i < len - 1 && in[i+1] == 0xAF){
                    clen = strlen("\u03DD");
                    i++;
                    strcat(ret, "\u03DD");
                    cpos += clen;
                }
                break;
            case 0x87:
                if(bytemode == 2)
                    current = &cyr_dualbyte;
                break;
            case 0x88:
                if(bytemode == 2 && i < len - 1){
                    switch(in[i+1]){
                        case 0x81:
                            clen = strlen("\u0475");
                            strcat(ret, "\u0475");
                            cpos += clen;
                            break;
                        case 0x83:
                            clen = strlen("\u0467");
                            strcat(ret, "\u0467");
                            cpos += clen;
                            break;
                        default:
                            // this should not happen
                            break;
                    }
                }
                i++;
                break;
            default:
                if(bytemode == 2 && in[i] < 0x7F) {
                    current = &unibyte;
                    bytemode = 1;
                }
                const char *add = (*current)[in[i]];
                if(add != NULL){
                    clen = strlen(add);
                    strcat(ret, add);
                    cpos += clen;
                }
                if(*current == special_unibyte)
                    current = &unibyte;
                break;
        }
    }
    *out = ret;
    return cpos + 1;
}

void
starling_decode_external(Starling_entry *entr, unsigned char **var)
{
    if(entr->var_offset == 538976288 && entr->var_length == 8224){ // 20202020 and 2020 = empty
        entr->content = NULL;
    } else {
        entr->content = malloc(entr->var_length);
        memcpy(entr->content, *var + entr->var_offset, entr->var_length);
    }
}

void
starling_decode_all_text(Starling_db *db)
{
    int elen = 0;
    for(int i = 0; i < db->rec_ct; i++){
        for(int j = 0; j < db->hdr_ct; j++){
            if(db->recs[i].entries[j].decoded_content == NULL){
                if(db->recs[i].entries[j].type == ET_EXTERNAL)
                    elen = db->recs[i].entries[j].var_length;
                else
                    elen = db->hdrs[db->recs[i].entries[j].hdr_index].length;
                db->recs[i].entries[j].decoded_content = malloc(1);
                db->recs[i].entries[j].true_length = starling_decode_text(&(db->recs[i].entries[j].decoded_content), db->recs[i].entries[j].content, elen);
            }
        }
    }
}

void
starling_decode_all_external(Starling_db *db)
{
    for(int i = 0; i < db->rec_ct; i++){
        for(int j = 0; j < db->hdr_ct; j++){
            if(db->recs[i].entries[j].type == ET_EXTERNAL){
                if ((!db->is_altai || i != 1727 || j != 5) && (!db->is_stibet || i != 2784))
                    starling_decode_external(&(db->recs[i].entries[j]), &(db->ext_entries));
                else db->recs[i].entries[j].content = NULL;
            }
        }
    }
}

void
starling_decode_all(Starling_db *db){
    if(db->ext_entries != NULL)
        starling_decode_all_external(db);
    starling_decode_all_text(db);
}
