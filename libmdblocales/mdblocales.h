#ifndef MDB_LOCALES_H
#define MDB_LOCALES_H

#include <stdlib.h>
#include <string.h>
#include <locale.h>

char           *mdb_setlocale(int, const char *);
locale_t	mdb_newlocale(int, const char *, locale_t);
char           *mdb_localesversion();
#endif	/* //MDB_LOCALES_H */
