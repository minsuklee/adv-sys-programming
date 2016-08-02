
all: gen merge
#sbig fbig mbig

gen : gen.c
	gcc -g -ogen gen.c

merge : merge.c
	gcc -g -omerge merge.c

clean:
	rm -rf gen merge *.o a.out xxx *.dSYM file_*
