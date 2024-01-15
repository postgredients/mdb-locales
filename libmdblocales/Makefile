MDB_LOCALES_PREFIX_STR?=
MDB_LOCALES_VERSION_STR?=2.27
MDB_LOCALES_LOCPATH_STR?=/usr/lib/mdb-locale

CC = gcc
CFLAGS = -fPIC -Wall -Wextra -O2 -g -D'DEF_MDB_LOCALES_PREFIX_STR="${MDB_LOCALES_PREFIX_STR}"' -D'DEF_MDB_LOCALES_VERSION_STR="${MDB_LOCALES_VERSION_STR}"' -D'DEF_MDB_LOCALES_LOCPATH_STR="${MDB_LOCALES_LOCPATH_STR}"'
LDFLAGS = -shared
RM = rm -f
TARGET_LIB = libmdblocales.so.1.0.1
TARGET_HEADER = mdblocales.h

SRCS = mdblocales.c
OBJS = $(SRCS:.c=.o)

DESTDIR?=
ifeq ($(PREFIX),)
    PREFIX := /usr
endif


.PHONY: all clean install
all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -Wl,-soname,libmdblocales.so.1 -o $@ $^ -lc

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)

install:
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 ${TARGET_LIB} $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/
	install -m 644 ${TARGET_HEADER} $(DESTDIR)$(PREFIX)/include/
	ln -fs ${TARGET_LIB} $(DESTDIR)$(PREFIX)/lib/libmdblocales.so.1
	ln -fs ${TARGET_LIB} $(DESTDIR)$(PREFIX)/lib/libmdblocales.so

uninstall:
	${RM} $(DESTDIR)$(PREFIX)/lib/libmdblocales.so*
