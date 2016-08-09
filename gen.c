#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>

#define MAX_NUM_FILE  9999
#define MAX_FILE_SIZE (32 * 1024)

int
main(int argc, char *argv[])
{
    FILE *fout;
    int num_file;
    long file_size, wsize;
    long line;
    char fname[30];
    int i, tmp;
    struct timeval seed;

    assert(MAX_NUM_FILE < 10000);	// outpupt format dependency..

    if (argc != 3) {
        fprintf(stderr, "usage: %s num_of_file file_size(MB)\n", argv[0]);
        return 1;
    }

    if (sscanf(argv[1], "%d", &num_file) != 1) {
usage1: fprintf(stderr, "Number of file should be >= 0, <= %d\n", MAX_NUM_FILE);
        return 1;
    }
    if ((num_file <= 0) || (num_file > MAX_NUM_FILE))
        goto usage1;

    if (sscanf(argv[2], "%ld", &file_size) != 1) {
usage2: fprintf(stderr, "File size should be >= 0, <= %d\n", MAX_FILE_SIZE);
        return 1;
    }
    if ((file_size <= 0) || (file_size > MAX_FILE_SIZE))
        goto usage2;
    printf("Generate %d files (%ldMB - ", num_file, file_size);
    file_size *= (1024 * 1024); // MB
    printf("%ld Bytes)\n", file_size);

    gettimeofday(&seed, NULL);
    srandom(seed.tv_usec);

    while (num_file) {
        // assert must be here <-- %04d : MAX_NUM_FILE
        sprintf(fname, "/tmp/file_%04d", num_file);
        if ((fout = fopen(fname, "wt")) == NULL) {
            perror(fname);
            return 1;
        }

        line = 0;
        wsize = 0;
        while (wsize < (file_size - 80)) {
            fprintf(fout, "%04d-%08lX: ", num_file, line);
            tmp = 1 + (random() % 16);
            for (i = 0; i < tmp; i++)
                fprintf(fout, "%04d", num_file);
            fputc(10, fout);
            wsize += (16 + tmp * 4); // depends on fprintf format !!!
            line++;
        }
        fprintf(fout, "%04d-%08lX: ", num_file, line);
        for (i = 0; i < (file_size - wsize - 13); i++)
            fputc('-', fout);
        fputc(10, fout);
        fclose(fout);
        num_file--;
    }
    return 0;   
}

