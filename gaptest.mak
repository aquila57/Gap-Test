OBJ=gaptest.o \
	calcchi.o

CC=gcc

CFLAGS=-c -Wall -O2

LDFLAGS=-L. -lgap -lgsl -lgslcblas -lm

gaptest:			$(OBJ)
		$(CC) -Wall -O2 $(OBJ) -o gaptest $(LDFLAGS)

gaptest.o:			gaptest.c
		$(CC) $(CFLAGS) gaptest.c

calcchi.o:			calcchi.c
		$(CC) $(CFLAGS) calcchi.c

clean:
		rm -f $(OBJ) gaptest
