all: shell server

shell: myshell.a
myshell.a: shell.o func.o
	ar -rcs myshell.a shell.o func.o

shell: shell.o 

server: server.o

shell.o: shell.c lib.h
	gcc -Wall -g -c shell.c 
	 

server.o: server.c lib.h
	gcc -Wall -g -c server.c

func.o: func.c lib.h
	gcc -Wall -g -c func.c

.PHONY: clean all

clean:
	rm -f *.o *.a shell server