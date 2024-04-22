Movable DataBase Locales for PostgreSQL
==============

MDB locales provides static glibc2.27 locales that are independent from the actual glibc version. It allows to avoid index corruptions when moving the existing database between Linux distributions that have major glibc collation changes (such as Ubuntu 18.04 to 22.04 migrations).

Please consult this [PostgreSQL wiki page](https://wiki.postgresql.org/wiki/Locale_data_changes) for more details on the collation change problem. 

MDB locales consist of three parts:
1. PostgreSQL patch that replaces all glibc locale-related calls with a calls to an external libary ([pg_patch](https://github.com/postgredients/mdb-locales/tree/main/pg_patch))
2. External library that loads locales from custom path ([libmdblocales](https://github.com/postgredients/mdb-locales/tree/main/libmdblocales))
3. Deb package that install mdb-locales based on locales package from glibc2.27 ([mdb-locales](https://github.com/postgredients/mdb-locales/tree/main/debian)). 


Usage
=======

1. (Optional) Build the libmblocales and mdb-locales packages:
```
dpkg-buildpackage -us -uc
```

2. Install the deb packages:
```
dpkg -i libmdblocales1_0.0.1-1_amd64.deb mdb-locales_0.0.1-1_amd64.deb libmdblocales-dev_0.0.1-1_amd64.deb
```

3. Build PostgreSQL w/ mdb-locales patch applied:
```
git clone https://git.postgresql.org/git/postgresql.git
cd postgresql
# uploaded patch has been tested on REL_16_0
git checkout REL_16_0
git apply --whitespace=warn ../pg_patch/0001-Implement-mdb-locales.patch
# flags other than --with-mdblocales are provided only for reference and are not required for this patch
./configure --enable-cassert --enable-debug  CFLAGS="-ggdb -Og -g3 -fno-omit-frame-pointer" --with-mdblocales --without-icu --without-readline --without-zlib
make
make install
```

License
=======

Copyright (c) 2024, YANDEX LLC
