#!/usr/bin/make -f

CC=				gcc
CFLAGS_OPT?=    -O3 -g -pipe
CFLAGS_STD?=    -std=gnu11
CFLAGS_WARN?=   -Wall -W -Wextra -Wpedantic

CFLAGS?=    	${CFLAGS_OPT}
CFLAGS+=    	${CFLAGS_STD} ${CFLAGS_WARN}

CFLAGS+=   	    -Werror
CFLAGS+=		-Wbad-function-cast -Wcast-align -Wcast-qual -Wchar-subscripts \
				-Winline -Wnested-externs -Wpointer-arith \
				-Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings

RM?=        	rm -f

SRCS=			main.c
OBJS=			$(subst .c,.o,$(SRCS))
BINS=			$(subst .c,,$(SRCS))

all: 			$(BINS)

clean:
				${RM} -- ${OBJS} ${BINS}

.PHONY: 		all clean
