
CC = gcc
CFLAGS = -std=gnu99 -Wall -pedantic -I../

src  = $(wildcard *.c)
src += $(wildcard ../*.c)
obj  = $(src:.c=.o)
dep  = $(obj:.o=.d) 

.PHONY: all clean

build: csv2xml

all: clean build

clean:
	rm -rf $(dep) $(obj) csv2xml *.xml
	
install: csv2xml
	cp ./csv2xml /usr/bin/csv2xml
	
csv2xml: $(obj)
	gcc $(CFLAGS) -o $@ $^

-include $(dep)

%.d: %.c
	$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@
