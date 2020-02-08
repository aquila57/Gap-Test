OBJ=sinegap.o \
	calcchi.o

CC=gcc

CFLAGS=-c -Wall -O2

LDFLAGS=-L. -lgap -lgsl -lgslcblas -lm

sinegap:			$(OBJ)
		$(CC) -Wall -O2 $(OBJ) -o sinegap $(LDFLAGS)

sinegap.o:			sinegap.c
		$(CC) $(CFLAGS) sinegap.c

calcchi.o:			calcchi.c
		$(CC) $(CFLAGS) calcchi.c

clean:
		rm -f $(OBJ) sinegap
