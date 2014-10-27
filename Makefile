CC=gcc
CFLAGS=-g -std=gnu99  -pedantic -Wall -Wno-implicit

%.o:%.c
	$(CC) $(CFLAGS) -c $<

all: pokus

pokus: pokus.o
	$(CC) $(CFLAGS) $(LDFLAGS) -L. $^ -o $@


.PHONY : depend dep all

dep:
	$(CC) $(CFLAGS) $(CPPFLAGS) -w -E -M *.c \
	  > depend

depend:
	@touch depend

clean:
	rm -f *.o *.a pokus depend

-include depend
