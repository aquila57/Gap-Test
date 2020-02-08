OBJ=lfsrgap.o \
	calcchi.o

CC=gcc

CFLAGS=-c -Wall -O2

LDFLAGS=-L. -lgap -lgsl -lgslcblas -lm

lfsrgap:			$(OBJ)
		$(CC) -Wall -O2 $(OBJ) -o lfsrgap $(LDFLAGS)

lfsrgap.o:			lfsrgap.c
		$(CC) $(CFLAGS) lfsrgap.c

calcchi.o:			calcchi.c
		$(CC) $(CFLAGS) calcchi.c

clean:
		rm -f $(OBJ) lfsrgap
