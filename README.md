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
# --with-mdblocales is optional (enabled by default)
./configure --enable-cassert --enable-debug --with-mdblocales
make
make install
```

4. Run demo cluster:
```
mkdir -p pg_data_dir
initdb -D pg_data_dir
pg_ctl -D pg_data_dir -l logfile start
```

5. Test that the collation is glibc-independent:
```
~ ldd --version
ldd (Debian GLIBC 2.31-13+deb11u5) 2.31
Copyright (C) 2020 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
Written by Roland McGrath and Ulrich Drepper.

# On vanilla PostgreSQL:
postgres=# select * from pg_collation where collprovider='c';
  oid  |  collname  | collnamespace | collowner | collprovider | collisdeterministic | collencoding | collcollate | collctype  | collversion
-------+------------+---------------+-----------+--------------+---------------------+--------------+-------------+------------+-------------
   950 | C          |            11 |        10 | c            | t                   |           -1 | C           | C          |
   951 | POSIX      |            11 |        10 | c            | t                   |           -1 | POSIX       | POSIX      |
 12545 | ucs_basic  |            11 |        10 | c            | t                   |            6 | C           | C          |
 12546 | C.UTF-8    |            11 |        10 | c            | t                   |            6 | C.UTF-8     | C.UTF-8    |
 12547 | en_US.utf8 |            11 |        10 | c            | t                   |            6 | en_US.utf8  | en_US.utf8 | 2.31
 12548 | en_US      |            11 |        10 | c            | t                   |            6 | en_US.utf8  | en_US.utf8 | 2.31
(6 rows)

postgres=# SELECT '1-1' < '11' COLLATE "en_US.utf8";
 ?column?
----------
 t
(1 row)

# On PostgreSQL with mdb-locales patch:
postgres=# select * from pg_collation where collprovider='c';
  oid  |  collname  | collnamespace | collowner | collprovider | collisdeterministic | collencoding | collcollate | collctype  | collversion
-------+------------+---------------+-----------+--------------+---------------------+--------------+-------------+------------+-------------
   950 | C          |            11 |        10 | c            | t                   |           -1 | C           | C          |
   951 | POSIX      |            11 |        10 | c            | t                   |           -1 | POSIX       | POSIX      |
 12545 | ucs_basic  |            11 |        10 | c            | t                   |            6 | C           | C          |
 12546 | C.UTF-8    |            11 |        10 | c            | t                   |            6 | C.UTF-8     | C.UTF-8    |
 12547 | en_US.utf8 |            11 |        10 | c            | t                   |            6 | en_US.utf8  | en_US.utf8 | 2.27
 12548 | en_US      |            11 |        10 | c            | t                   |            6 | en_US.utf8  | en_US.utf8 | 2.27
(6 rows)


postgres=# SELECT '1-1' < '11' COLLATE "en_US.utf8";
 ?column?
----------
 f
(1 row)
```


License
=======

Copyright (c) 2024, YANDEX LLC
