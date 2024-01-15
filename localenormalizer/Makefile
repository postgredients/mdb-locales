CC = gcc
CFLAGS = -fPIC -Wall -Wextra -O2 -g
RM = rm -f

TARGET = mdb-locale-normalizer
SRCS = normalizer.c
OBJS = $(SRCS:.c=.o)

DESTDIR?=
ifeq ($(PREFIX),)
    PREFIX := /usr
endif

all: $(TARGET)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

clean:
	-${RM} ${TARGET} ${OBJS} $(SRCS:.c=.d)

install:
	install -d $(DESTDIR)$(PREFIX)/sbin/
	install -m 0777 ${TARGET} $(DESTDIR)$(PREFIX)/sbin/

uninstall:
	${RM} $(DESTDIR)$(PREFIX)/sbin/${TARGET}