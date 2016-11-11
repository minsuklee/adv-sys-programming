#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024
#define DELIM "\n"

int readaline_and_out(FILE *fin1, FILE *fin2, FILE *fout, long *line);
char *reverse_str(char *str);

int
main(int argc, char *argv[]){
    FILE *file1, *file2, *fout;
    long line[2] = {0, 0}, lineout = 0;
    struct timeval before, after;
    int duration;
    int ret = 1;

    if (argc != 4) {
        fprintf(stderr, "usage: %s file1 file2 fout\n", argv[0]);
        goto leave0;
    }
    if ((file1 = fopen(argv[1], "rt")) == NULL) {
        perror(argv[1]);
        goto leave0;
    }
    if ((file2 = fopen(argv[2], "rt")) == NULL) {
        perror(argv[2]);
        goto leave1;
    }
    if ((fout = fopen(argv[3], "wt")) == NULL) {
        perror(argv[3]);
        goto leave2;
    }
    
    //stream buffer 크기 제어
    setvbuf(file1, NULL, _IOFBF, BUFFER_SIZE);
    setvbuf(file2, NULL, _IOFBF, BUFFER_SIZE);
    setvbuf(fout, NULL, _IOFBF, BUFFER_SIZE);

    gettimeofday(&before, NULL);

    readaline_and_out(file1, file2, fout, line);
    lineout = line[0] + line[1];

    gettimeofday(&after, NULL);
    
    duration = (after.tv_sec - before.tv_sec) * 1000000 + (after.tv_usec - before.tv_usec);
    printf("Processing time = %d.%06d sec\n", duration / 1000000, duration % 1000000);
    printf("File1 = %ld, File2= %ld, Total = %ld Lines\n", line[0], line[1], lineout);
    ret = 0;
    
    leave3:
        fclose(fout);
    leave2:
        fclose(file2);
    leave1:
        fclose(file1);
    leave0:
        return ret; 
}

// Read a line from fin and write it to fout 
// return 1 if fin meets end of file 
int
readaline_and_out(FILE *fin1, FILE *fin2, FILE *fout, long *line){    
    int tmp;
    long f_size1, f_size2;
    char *buff1, *buff2;
    char *token1, *token2;
    char *ptr[2];
    size_t tmp1, tmp2;

    //파일의 크기를 각각 f_size1, f_size2에 저장한다.
    fseek(fin1, 0, SEEK_END); 
    f_size1 = ftell(fin1);
    rewind(fin1);

    fseek(fin2, 0, SEEK_END);
    f_size2 = ftell(fin2);
    rewind(fin2);

    //f_size를 이용해서 buff의 메모리를 할당한다.
    buff1 = (char *)malloc(sizeof(char)*f_size1);
    if (buff1 == NULL){
        fputs ("error, buff1_malloc\n", stderr);
        free(buff1);
        return 1;
    }
    buff2 = (char *)malloc(sizeof(char)*f_size2);
    if (buff2 == NULL){
        fputs ("error, buff2_malloc\n", stderr);
        free(buff1);
        free(buff2);
        return 1;
    }

    //파일을 fread를 이용해 읽어온다.
    tmp1 = fread(buff1, f_size1, 1, fin1);
    if(tmp1 != f_size1){
        fputs("error, fread_fin1\n", stderr);
        return 1;
    }
    tmp2 = fread(buff2, f_size2, 1, fin2);
    if(tmp2 != f_size2){
        fputs("error, fread_fin2\n", stderr);
        return 1;
    }

    //파일을 strtok_r을 이용해 파싱한다.
    token1 = strtok_r(buff1, DELIM, &ptr[0]);
    token2 = strtok_r(buff2, DELIM, &ptr[1]);

    //파일을 fwrite로 fout에 쓰기. 
    while(token1 != NULL || token2 != NULL){
        if(token1 != NULL){
            fwrite(reverse_str(token1), strlen(token1), 1, fout);
            fputc(0x0a, fout);
            token1 = strtok_r(NULL, DELIM, &ptr[0]);
            line[0]++;
        }
        if(token2 != NULL){
            fwrite(reverse_str(token2), strlen(token2), 1, fout);
            fputc(0x0a, fout);
            token2 = strtok_r(NULL, DELIM, &ptr[1]);
            line[1]++;
        }
    }

    free(buff1);
    free(buff2);

    return 0;
}

char* 
reverse_str(char *str){
    int i = 0, tmp, length = strlen(str);

    for (i = 0; i < length / 2; i++) {
    tmp = str[i];
    str[i] = str[(length - 1) - i];
    str[(length - 1) - i] = tmp;
  }
  return str;
} 
