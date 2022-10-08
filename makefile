CC=gcc
CFLAGS= -g -I.

1712816: main.c string_functions.c socket_functions.c file_functions.c
	$(CC) -g -o 1712816 main.c string_functions.c socket_functions.c file_functions.c