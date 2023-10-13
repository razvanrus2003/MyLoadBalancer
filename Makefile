CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -g
LOAD=load_balancer
SERVER=server
UTILS=utils
LIST=linked_list
HASH=hash_table

.PHONY: build clean

build: tema2

tema2: main.o $(LOAD).o $(SERVER).o $(UTILS).o $(LIST).o $(HASH).o
	$(CC) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) $^ -c

$(SERVER).o: $(SERVER).c $(SERVER).h
	$(CC) $(CFLAGS) $(SERVER).c -c

$(LOAD).o: $(LOAD).c $(LOAD).h
	$(CC) $(CFLAGS) $(LOAD).c -c

$(UTILS).o: $(UTILS).c $(UTILS).h
	$(CC) $(CFLAGS) $(UTILS).c -c

$(LIST).o: $(LIST).c $(LIST).h
	$(CC) $(CFLAGS) $(LIST).c -c

$(HASH).o: $(HASH).c $(HASH).h
	$(CC) $(CFLAGS) $(HASH).c -c

pack:
	zip -FSr 312CA_RusRazvan_Tema2.zip Makefile *.c *.h README

clean:
	rm -f *.o tema2 *.h.gch
