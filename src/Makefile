CC = clang
INCLUDE = .
CFLAGS = -Wall -std=c11

hw1: hw1.o termctrl.o stringutil.o
	$(CC) -o hw1 hw1.o termctrl.o stringutil.o
hw1.o: hw1.c termctrl.h stringutil.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c hw1.c
termctrl.o: termctrl.c termctrl.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c termctrl.c
stringutil.o: stringutil.c stringutil.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c stringutil.c
clean:
	rm -f hw1.o termctrl.o stringutil.o hw1
