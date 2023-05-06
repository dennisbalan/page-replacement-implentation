all: 
	gcc -g -o simulate main.c -Wall -Werror
run: 	simulate
	./simulate
clean: 
	rm simulate
