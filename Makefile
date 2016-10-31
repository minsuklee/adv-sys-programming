
all: gen merge chat

gen : gen.c
	gcc -g -ogen gen.c

merge : merge.c
	gcc -g -omerge merge.c

chat : chat.c
	gcc -g -ochat chat.c

clean:
	rm -rf gen merge chat *.o a.out xxx *.dSYM file_*
