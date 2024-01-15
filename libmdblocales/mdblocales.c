#include "mdblocales.h"
#include <stdio.h>

static char    *
get_localename(const char *locale, const char *prefix)
{
	if (prefix == NULL || prefix[0] == '\0') {
		return strdup(locale);
	} else {
		size_t		prefix_len = strlen(prefix);
		char           *res = malloc(strlen(locale) + prefix_len + 1);
		char           *copy = strdup(locale);
		char           *copy_suffix = strstr(copy, ".");

		/*
		 * Check for names like C.UTF-8 by chopping off the encoding
		 * suffix
		 */
		if (copy_suffix)
			*copy_suffix = '\0';

		if (strcasecmp("c", copy) == 0 || strcasecmp("posix", copy) == 0) {
			free(copy);
			strcpy(res, locale);
			return res;
		}
		free(copy);

		if (strncmp(locale, prefix, prefix_len) == 0) {
			strcpy(res, locale);
			return res;
		}
		strcpy(res, prefix);
		strcpy(res + prefix_len, locale);

		return res;
	}
}

static char    *
strip_prefix(char *localename)
{
	if (localename == NULL || localename[0] == '\0') {
		return localename;
	}
	if (strncmp(DEF_MDB_LOCALES_PREFIX_STR, localename, strlen(DEF_MDB_LOCALES_PREFIX_STR)) == 0) {
		return localename + strlen(DEF_MDB_LOCALES_PREFIX_STR);
	}
	return localename;
}

char           *
mdb_setlocale(int category, const char *locale)
{
	setenv("LOCPATH", DEF_MDB_LOCALES_LOCPATH_STR, 1);

	if (locale == NULL || locale[0] == '\0') {
		return setlocale(category, locale);
	} else {
		char           *localename = get_localename(locale, DEF_MDB_LOCALES_PREFIX_STR);
		char           *res = setlocale(category, localename);
		free(localename);

		return strip_prefix(res);
	}
}

locale_t
mdb_newlocale(int category, const char *locale, locale_t base)
{
	setenv("LOCPATH", DEF_MDB_LOCALES_LOCPATH_STR, 1);

	if (locale == NULL || locale[0] == '\0') {
		return newlocale(category, locale, base);
	} else {
		char           *localename = get_localename(locale, DEF_MDB_LOCALES_PREFIX_STR);
		locale_t	res = newlocale(category, localename, base);

		free(localename);
		return res;
	}
}

char           *
mdb_localesversion()
{
	return DEF_MDB_LOCALES_VERSION_STR;
}
