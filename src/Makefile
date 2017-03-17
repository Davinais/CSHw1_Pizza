CC = clang
INCLUDE = .
CFLAGS = -Wall -std=c99

hw1: hw1.o termctrl.o
	$(CC) -o hw1 hw1.o termctrl.o
hw1.o: hw1.c termctrl.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c hw1.c
termctrl.o: termctrl.c termctrl.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c termctrl.c

clean:
	rm -f *.o hw1
