//
// Simple locale normalzier based on glibc 2.27 to mimic the localedef locale-archive naming
//
#define _GNU_SOURCE
#include <stdio.h>
#include <locale.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/*
 * Normalize codeset name.  There is no standard for the codeset names.
 * Normalization allows the user to use any of the common names.  The return
 * value is dynamically allocated and has to be freed by the caller.
 * Src: intl/l10nflist.c
 */
const char     *
nl_normalize_codeset(const char *codeset, size_t name_len)
{
	size_t		len = 0;
	int		only_digit = 1;
	char           *retval;
	char           *wp;
	size_t		cnt;
	locale_t	locale = newlocale(0, "C", NULL);

	for (cnt = 0; cnt < name_len; ++cnt)
		if (__isalnum_l((unsigned char)codeset[cnt], locale)) {
			++len;

			if (!__isdigit_l((unsigned char)codeset[cnt], locale))
				only_digit = 0;
		}
	retval = (char *)malloc((only_digit ? 3 : 0) + len + 1);

	if (retval != NULL) {
		if (only_digit)
			wp = stpcpy(retval, "iso");
		else
			wp = retval;

		for (cnt = 0; cnt < name_len; ++cnt)
			if (__isalpha_l((unsigned char)codeset[cnt], locale))
				*wp++ = __tolower_l((unsigned char)codeset[cnt], locale);
			else if (__isdigit_l((unsigned char)codeset[cnt], locale))
				*wp++ = codeset[cnt];

		*wp = '\0';
	}
	return (const char *)retval;
}

int
main(int argc, char **argv)
{
	if (argc != 2) {
		return -1;
	}
	char           *name = argv[1];
	/*
	 * If the name contains a codeset, then we normalize the name before
	 * doing the lookup.
	 */
	const char     *p = strchr(name, '.');
	if (p != NULL && p[1] != '@' && p[1] != '\0') {
		const char     *rest = strchrnul(++p, '@');
		const char     *normalized_codeset = nl_normalize_codeset(p, rest - p);
		if (normalized_codeset == NULL)	/* malloc failure */
			return -1;
		if (strncmp(normalized_codeset, p, rest - p) != 0
		    || normalized_codeset[rest - p] != '\0') {
			/*
			 * There is a normalized codeset name that is
			 * different from what was specified; reconstruct a
			 * new locale name using it.
			 */
			size_t		normlen = strlen(normalized_codeset);
			size_t		restlen = strlen(rest) + 1;
			char           *newname = alloca(p - name + normlen + restlen);
			memcpy(__mempcpy(__mempcpy(newname, name, p - name),
					 normalized_codeset, normlen),
			       rest, restlen);
			name = newname;
		}
		free((char *)normalized_codeset);
	}
	printf("%s", name);
	return 0;
}
