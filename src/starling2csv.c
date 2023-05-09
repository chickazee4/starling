#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>

#include <starling.h>

char *dbf = NULL;
char *var = NULL;
char *out = NULL;
char *delim = ",";

int dflag = 0,
    Dflag = 0,
    eflag = 0,
    fflag = 0,
    lflag = 0,
    mflag = 0,
    Mflag = 0,
    qflag = 0,
    sflag = 0,
    Sflag = 0,
    Tflag = 0,
    Vflag = 0;

Starling_db *dbp;

int
main(int argc, char **argv)
{
    setlocale(LC_ALL, "en_US.UTF-8");
    if(argc > 1){
        for(int i = 1; i < argc; i++){
            if(argv[i][0] == '-'){
                int alen = strlen(argv[i]);
                for(int j = 1; j < alen; j++){
                    switch(argv[i][j]){
                        case 'c':
                            Dflag++;
                            Tflag++;
                            Sflag++;
                            break;
                        case 'd':
                            if(i < argc + 1){
                                delim = argv[i+1];
                                i++;
                            } else {
                                fprintf(stderr, "You must specify an argument with the -d flag.\n");
                                exit(1);
                            }
                            break;
                        case 'D':
                            Dflag++;
                            break;
                        case 'e':
                            eflag++;
                            break;
                        case 'l':
                            lflag++;
                            break;
                        case 'm':
                            mflag++;
                            break;
                        case 'M':
                            Mflag++;
                            break;
                        case 'o':
                            if(i < argc + 1){
                                out = argv[i+1];
                                i++;
                            } else {
                                fprintf(stderr, "You must specify an argument with the -o flag.\n");
                                exit(1);
                            }
                            break;
                        case 'q':
                            qflag++;
                            break;
                        case 's':
                            sflag++;
                            break;
                        case 'S':
                            Sflag++;
                            break;
                        case 'T':
                            Tflag++;
                            break;
                        case 'v':
                            if(i < argc + 1){
                                var = argv[i+1];
                                i++;
                            } else {
                                fprintf(stderr, "You must specify an argument with the -v flag.\n");
                                exit(1);
                            }
                            break;
                        case 'V':
                            Vflag++;
                            break;
                    }
                }
            } else {
                dbf = argv[i];
            }
        }
    }

    if(dbf == NULL){
        fprintf(stderr, "You must specify a database!\n");
        exit(2);
    }

    if(var == NULL && !Vflag){
        int len = strlen(dbf);
        char *cvar = malloc(len);
        strcpy(cvar, dbf);
        cvar[len-3] = 'v';
        cvar[len-2] = 'a';
        cvar[len-1] = 'r';
        if(access(cvar, F_OK) != -1) {
            var = cvar;
        }
    }

    if(!qflag) printf("Accessing and parsing database...\n");
    int starling_parse_result;
    Starling_db *dbp = starling_parse_file(&starling_parse_result, dbf, var);
    if(starling_parse_result > 0) return starling_parse_result;

    if((!mflag || out) && !Mflag){
        if(!qflag) printf("Dereferencing external variables and decoding...\n");
        starling_decode_all(dbp);
    }

    if(!qflag){
        printf("Eureka!\n\n");
        printf("Database last updated %u/%u/%d.\n", dbp->month, dbp->day, (dbp->year > 22 ? dbp->year + 1900 : dbp->year + 2000));
        printf("Number of records: %d\n", dbp->rec_ct);
        printf("Number of fields: %d\n\n", dbp->hdr_ct);

        if(mflag){
            printf("Fields:\n");
            for(int i = 0; i < dbp->hdr_ct; i++){
                printf(" * %s (type: %s)\n", dbp->hdrs[i].name, starling_fieldtypetostr(dbp->hdrs[i].type));
            }
        }
    }

    if(!mflag || out){
        if (!qflag) printf("Generating table...\n");

        char *csv = NULL;

        if(Mflag){
            csv = starling_tabulate_fields(dbp, delim);
        } else {
            Starling_sanitize_flags sf = { Dflag, Tflag, Sflag };
            Starling_table_flags tf = { sflag, lflag, eflag, delim, &sf };
            csv = starling_tabulate_db(dbp, &tf);
        }

        if(csv == NULL){
            fprintf(stderr, "Error in table creation!\n");
            exit(2);
        }

        if(out != NULL){
            FILE *fp;
            if((fp = fopen(out, "w+")) != NULL){
                if(!qflag)
                    printf("Writing to file...\n");
                fprintf(fp, "%s", csv);
                fclose(fp);
            } else {
                fprintf(stderr, "Warning: specified output file is in a location that does not exist or is inaccessible. Writing to stdout instead.\n");
                printf("%s", csv);
            }
        } else printf("%s", csv);
    }

    exit(0);
}
