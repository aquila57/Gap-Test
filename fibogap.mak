OBJ=fibogap.o \
	calcchi.o

CC=gcc

CFLAGS=-c -Wall -O2

LDFLAGS=-L. -lgap -lgsl -lgslcblas -lm

fibogap:			$(OBJ)
		$(CC) -Wall -O2 $(OBJ) -o fibogap $(LDFLAGS)

fibogap.o:			fibogap.c
		$(CC) $(CFLAGS) fibogap.c

calcchi.o:			calcchi.c
		$(CC) $(CFLAGS) calcchi.c

clean:
		rm -f $(OBJ) fibogap
