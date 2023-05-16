#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <libintl.h>

#include <starling.h>

char *dbf = NULL;
char *var = NULL;
char *inf = NULL;
char *out = NULL;
char *delim = ",";

int dflag = 0,
    Dflag = 0,
    eflag = 0,
    fflag = 0,
    Iflag = 0,
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
    setlocale(LC_ALL, "");
    bindtextdomain("starling", LOCALEDIR);
    textdomain("starling");
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
                                fprintf(stderr, "%s\n", gettext("You must specify an argument with the -d flag."));
                                exit(STARLING_BAD_FLAG);
                            }
                            break;
                        case 'D':
                            Dflag++;
                            break;
                        case 'e':
                            eflag++;
                            break;
                        case 'i':
                            if(i < argc + 1){
                                Iflag++;
                                inf = argv[i+1];
                                i++;
                            } else {
                                fprintf(stderr, "%s\n", gettext("You must specify an argument with the -i flag."));
                                exit(STARLING_BAD_FLAG);
                            }
                            break;
                        case 'I':
                            Iflag++;
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
                                fprintf(stderr, "%s\n", gettext("You must specify an argument with the -o flag."));
                                exit(STARLING_BAD_FLAG);
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
                                fprintf(stderr, "%s\n", gettext("You must specify an argument with the -v flag."));
                                exit(STARLING_BAD_FLAG);
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

    if(!dbf){
        fprintf(stderr, "%s\n", gettext("You must specify a database!"));
        exit(STARLING_BAD_DBF_FILE);
    }

    if(!var && !Vflag){
        int len = strlen(dbf);
        var = malloc(len + 1);
        strcpy(var, dbf);
        var[len-3] = 'v';
        var[len-2] = 'a';
        var[len-1] = 'r';
        if(access(var, F_OK) == -1)
            var = NULL;
    }

    if(!qflag)
        printf("%s\n", gettext("Accessing and parsing database..."));
    int starling_parse_result;
    Starling_db *dbp = starling_parse_file(&starling_parse_result, dbf, var);
    free(var);
    if(starling_parse_result != STARLING_OK){
        fprintf(stderr, "%s %i", gettext("Error"), starling_parse_result);
        switch(starling_parse_result){
            case STARLING_BAD_DB_LEN:
                fprintf(stderr, ": %s\n", gettext("Parser tried to exceed length of database"));
                break;
            case STARLING_BAD_DELETE_FLAG:
                fprintf(stderr, ": %s\n", gettext("Invalid record (delete flag not detected; check for database corruption)"));
                break;
            case STARLING_BAD_HDR:
                fprintf(stderr, ": %s\n", gettext("Invalid header (check for database corruption)"));
                break;
            case STARLING_BAD_VAR_FILE:
                fprintf(stderr, ": %s\n", gettext("Invalid var file"));
                break;
            default:               
                fprintf(stderr, "\n");
                break;
        }
        exit(starling_parse_result);
    }

    if(Iflag && !inf){
        int len = strlen(dbf);
        inf = malloc(len + 1);
        strcpy(inf, dbf);
        inf[len-3] = 'i';
        inf[len-2] = 'n';
        inf[len-1] = 'f';
        if(access(inf, F_OK) == -1){
            fprintf(stderr, "%s\n", gettext("Error: Specified -I flag, but no .inf file exists and none was provided."));
            exit(STARLING_BAD_INF_FILE);
        }
    }

    if(inf){
        int iresult;
        if((iresult = starling_parse_inf(dbp, inf)) != STARLING_OK)
            exit(iresult);
    }

    if((!mflag || out) && !Mflag){
        if(!qflag) printf("%s\n", gettext("Dereferencing external variables and decoding..."));
        starling_decode_all(dbp);
    }

    if(!qflag){
        printf("%s\n\n", gettext("Eureka!"));
        printf("%s %u/%u/%d.\n", gettext("Database last updated"), dbp->month, dbp->day, (dbp->year > 22 ? dbp->year + 1900 : dbp->year + 2000));
        printf("%s: %d\n", gettext("Number of records"), dbp->rec_ct);
        printf("%s: %d\n\n", gettext("Number of fields"), dbp->hdr_ct);

        if(Iflag){
            printf("%s:\n%s\n", gettext("Database summary"), dbp->db_description);
        }

        if(mflag){
            printf("%s:\n", gettext("Fields"));
            for(int i = 0; i < dbp->hdr_ct; i++){
                if(inf)
                    printf(" * %s (%s: %s; %s: %s)\n", dbp->hdrs[i].name, gettext("type"), starling_fieldtypetostr(dbp->hdrs[i].type), gettext("human name"), (dbp->hdrs[i].human_name ? dbp->hdrs[i].human_name : "none"));
                else
                    printf(" * %s (%s: %s)\n", dbp->hdrs[i].name, gettext("type"), starling_fieldtypetostr(dbp->hdrs[i].type));
            }
        }
    }

    if(!mflag || out){
        if (!qflag) printf("%s\n", gettext("Generating table..."));

        char *csv = NULL;

        if(Mflag){
            csv = starling_tabulate_fields(dbp, delim, (inf ? 1 : 0));
        } else {
            Starling_sanitize_flags sf = { Dflag, Tflag, Sflag };
            Starling_table_flags tf = { sflag, lflag, eflag, delim, &sf, Iflag };
            csv = starling_tabulate_db(dbp, &tf);
        }

        if(!csv){
            fprintf(stderr, "%s\n", gettext("Error in table creation!"));
            exit(STARLING_TABULATE_FAILED);
        }

        if(out){
            FILE *fp;
            if((fp = fopen(out, "w+")) != NULL){
                if(!qflag)
                    printf("%s\n", gettext("Writing to file..."));
                fprintf(fp, "%s", csv);
                fclose(fp);
            } else {
                fprintf(stderr, "%s\n", gettext("Warning: specified output file is in a location that does not exist or is inaccessible. Writing to stdout instead."));
                printf("%s", csv);
            }
        } else printf("%s", csv);
    }
    exit(0);
}
