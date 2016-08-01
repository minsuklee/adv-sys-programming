
all: gen
#sbig fbig mbig

gen : gen.c
	gcc -g -ogen gen.c

#mbig : mbig.c
#	gcc -g -ombig mbig.c

clean:
	rm -rf *.o a.out gen *.dSYM file_*
