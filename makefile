mp3tag : driver.o utility.o message.o argument.o id3.o id3v2.o *.h
	gcc -Wall driver.o utility.o message.o argument.o id3.o id3v2.o -o mp3tag -lm

driver.o : driver.c
	gcc -Wall -c driver.c   -o driver.o

utility.o : utility.c
	gcc -Wall -c utility.c  -o utility.o

message.o : message.c
	gcc -Wall -c message.c  -o message.o

argument.o : argument.c
	gcc -Wall -c argument.c -o argument.o

id3.o : id3.c
	gcc -Wall -c id3.c      -o id3.o

id3v2.o : id3v2.c
	gcc -Wall -c id3v2.c    -o id3v2.o

stage : 
	git add driver.c utility.c utility.h message.c message.h argument.c argument.h id3.c id3.h id3v2.c id3v2.h types.h makefile
	git status

wordcount : 
	wc -l driver.* utility.* message.* argument.* id3.* id3v2.* types.h

reset_backup : 
	cp ./backup/* ./

clean :
	rm *.o mp3tag
	clear
