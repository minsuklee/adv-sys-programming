#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

long lineout = 0;
int readaline_and_out(FILE *fin, FILE *fout);

int
main(int argc, char *argv[])
{
    FILE *file1, *file2, *fout;
    int eof1 = 0, eof2 = 0;
    long line1 = 0, line2 = 0;
    struct timeval before, after;
    int duration;
    int ret = 1;

    if (argc != 4) {
        fprintf(stderr, "usage: %s file1 file2 fout\n", argv[0]);
        return 1;
    }
    if ((file1 = fopen(argv[1], "rt")) == NULL) {
        perror(argv[1]);
        return 1;
    }
    if ((file2 = fopen(argv[2], "rt")) == NULL) {
        perror(argv[2]);
        goto leave1;
    }
    if ((fout = fopen(argv[3], "wt")) == NULL) {
        perror(argv[3]);
        goto leave2;
    }
    gettimeofday(&before, NULL);
    do {
        if (!eof1) {
            if (!readaline_and_out(file1, fout))
                line1++;
            else
                eof1 = 1;
        }
        if (!eof2) {
            if (!readaline_and_out(file2, fout))
                line2++;
            else
                eof2 = 1;
        }
    } while (!eof1 || !eof2);
    gettimeofday(&after, NULL);
    duration = (after.tv_sec - before.tv_sec) * 1000000 + (after.tv_usec - before.tv_usec);
    printf("Processing time = %dusec\n", duration);
    printf("File1 = %ld, File2= %ld, Total = %ld Lines\n", line1, line2, lineout);
leave3:
    fclose(fout);
leave2:
    fclose(file2);
leave1:
    fclose(file1);
    return ret; 
}

/* Read a line from fin and write it to fout */
/* return 1 if fin meets end of file */
int
readaline_and_out(FILE *fin, FILE *fout)
{    
    int ch, count = 0;

    do {
        if ((ch = fgetc(fin)) == EOF) {
            if (!count)
                return 1;
            else {
                fputc(0x0a, fout);
                break;
            }
        }
        //printf("%c(0x%02X)", ch, ch);
        fputc(ch, fout);
        count++;
    } while (ch != 0x0a);
    lineout++;
    return 0;
}

