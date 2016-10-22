#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

typedef struct LinkedNode{
    char data;
    struct LinkedNode *pLink;
}LinkedNode;

typedef struct LinkedStack{
    int currentCount;
    LinkedNode *pTop;
}LinkedStack;

LinkedStack *createStack(){
    LinkedStack *pReturn = NULL;
    pReturn = (LinkedStack *)malloc(sizeof(LinkedStack));
    memset(pReturn, 0 , sizeof(LinkedStack));
    return pReturn;
}

int Push(LinkedStack *pStack, char data){
    int ret = 0;
    LinkedNode *pNode = NULL;

    if(pStack != NULL){
        pNode = (LinkedNode *)malloc(sizeof(LinkedNode));
        if(pNode != NULL){
            pNode->data = data;
            pNode->pLink = pStack->pTop;
            pStack->pTop = pNode;

            pStack->currentCount++;
            ret = 1;
        }
        else printf("error, Push()\n");
        return ret;
    }
}

LinkedNode *Pop(LinkedStack *pStack){
    LinkedNode *pReturn = NULL;
    if(pStack != NULL){
        if(pStack->currentCount != 0){
            pReturn = pStack->pTop;
            pStack->pTop = pReturn->pLink;
            pReturn->pLink = NULL;

            pStack->currentCount--;
        }
    }
    return pReturn;
}
int readaline_and_out(FILE *fin, FILE *fout);

int
main(int argc, char *argv[])
{
    FILE *file1, *file2, *fout;
    int eof1 = 0, eof2 = 0;
    long line1 = 0, line2 = 0, lineout = 0;
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
    
    gettimeofday(&before, NULL);
    do {
        if (!eof1) {
            if (!readaline_and_out(file1, fout)) {
                line1++; lineout++;
            } else
                eof1 = 1;
        }
        if (!eof2) {
            if (!readaline_and_out(file2, fout)) {
                line2++; lineout++;
            } else
                eof2 = 1;
        }
    } while (!eof1 || !eof2);
    gettimeofday(&after, NULL);
    
    duration = (after.tv_sec - before.tv_sec) * 1000000 + (after.tv_usec - before.tv_usec);
    printf("Processing time = %d.%06d sec\n", duration / 1000000, duration % 1000000);
    printf("File1 = %ld, File2= %ld, Total = %ld Lines\n", line1, line2, lineout);
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

/* Read a line from fin and write it to fout */
/* return 1 if fin meets end of file */
int
readaline_and_out(FILE *fin, FILE *fout)
{    
    int ch, count = 0;
    LinkedStack *pStack = NULL;
    LinkedNode *pNode = NULL;

    pStack = createStack();
    do {
        if ((ch = fgetc(fin)) == EOF) {
            if (!count)
                return 1;
            else {
                fputc(0x0a, fout);
                break;
            }
        }
        Push(pStack,ch);
        count++;
    } while (ch != 0x0a);

    while(count!= 0){ 
        pNode = Pop(pStack);
        fputc(pNode->data, fout);
	free(pNode);
        count--;
    }
    return 0;
}
