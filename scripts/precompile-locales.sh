#!/bin/bash
#
# Precompile all locales from MDB-SUPPORTED at package build time so that
# package installation does not need to run localedef (MDB-30501).
#
# Mirrors the generation logic of debian/src/usr/sbin/mdb-locale-gen, but
# works from the source tree and produces compiled locales under
# debian/src/usr/lib/mdb-locale, which is shipped inside the mdb-locales
# package (see debian/mdb-locales.install).
#
# Optionally, a list of locales can be passed as arguments to compile only
# a subset of MDB-SUPPORTED, e.g.:
#   scripts/precompile-locales.sh "en_US.UTF-8 UTF-8" "ru_RU.UTF-8 UTF-8"
#
set -u

ROOT=$(cd "$(dirname "$0")/.." && pwd)

MDB_I18NPATH="$ROOT/debian/src/usr/share/i18n/mdb"
LOCALES="$MDB_I18NPATH/locales"
SUPPORTED="$ROOT/debian/src/usr/share/i18n/MDB-SUPPORTED"
ALIASES="$ROOT/debian/src/usr/share/locale/mdb-locale.alias"
LOCPATH="$ROOT/debian/src/usr/lib/mdb-locale"
NORMALIZER="$ROOT/localenormalizer/mdb-locale-normalizer"

if [ ! -x "$NORMALIZER" ]; then
    echo "error: $NORMALIZER not found, run 'make -C localenormalizer' first" >&2
    exit 1
fi

install -d "$LOCPATH"

if [ "$#" -gt 0 ]; then
    GENERATE=$(printf '%s\n' "$@")
else
    GENERATE=$(sed -e '/^[a-zA-Z]/!d' -e 's/ *$//g' "$SUPPORTED" | sort -u)
fi

total=0

echo "$GENERATE" | while read -r locale charset; do
    case $locale in '#'*) continue;; "") continue;; esac
    if [ -z "$locale" ] || [ -z "$charset" ]; then
        echo "error: bad entry '$locale $charset'" >&2
        continue
    fi
    total=$((total + 1))

    if [ -f "$LOCALES/$locale" ]; then
        input="$locale"
    else
        input=$(echo "$locale" | sed 's/\([^.]*\)[^@]*\(.*\)/\1\2/')
        if [ -f "$LOCALES/$input" ]; then
            :
        else
            echo "warning: no locale definition for '$locale', skipping" >&2
            continue
        fi
    fi

    normalized_locale=$("$NORMALIZER" "$locale")

    echo -n "  $locale.$charset"
    if I18NPATH="$MDB_I18NPATH" localedef --no-archive -i "$input" -c -f "$charset" \
        -A "$ALIASES" "$LOCPATH/$normalized_locale"; then
        echo ' done'
    else
        echo ' FAILED'
        continue
    fi

    aliases=$(grep -w "$normalized_locale" "$ALIASES" | awk '{ print $1 }')
    for entry in $aliases; do
        echo -n ", alias: $entry"
        if I18NPATH="$MDB_I18NPATH" localedef --no-archive -i "$input" -c -f "$charset" \
            -A "$ALIASES" "$LOCPATH/$entry"; then
            echo -n ' ok'
        else
            echo -n ' FAILED'
        fi
    done
    echo
done

echo "Precompilation finished into $LOCPATH"
