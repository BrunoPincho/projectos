CC=gcc -Wall -g 
CFLAGS= -pthread
OBJ=cli.o psiskv_lib.o
HEAD=psiskv.h

All: cli outros

cli: $(OBJ)
	$(CC) $(OBJ) -o $@ $(CFLAGS)

cli.o: cli.c $(HEAD)
	$(CC) -c cli.c $(CFLAGS)

psiskv_lib.o: psiskv_lib.c psiskv.h 
	$(CC) -c psiskv_lib.c $(CFLAGS)



outros:
	rm *.o

clean:
	rm cli
