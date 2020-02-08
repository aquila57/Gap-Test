OBJ=rugap.o \
	calcchi.o

CC=gcc

CFLAGS=-c -Wall -O2

LDFLAGS=-L. -lgap -lgsl -lgslcblas -lm

rugap:				$(OBJ)
		$(CC) -Wall -O2 $(OBJ) -o rugap $(LDFLAGS)

rugap.o:			rugap.c
		$(CC) $(CFLAGS) rugap.c

calcchi.o:			calcchi.c
		$(CC) $(CFLAGS) calcchi.c

clean:
		rm -f $(OBJ) rugap
