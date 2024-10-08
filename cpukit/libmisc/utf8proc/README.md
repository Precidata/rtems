utf8proc
========
Please read the LICENSE file, which is shipping with this software.


*** QUICK START ***

For compilation of the C library call "make c-library", for compilation of
the ruby library call "make ruby-library" and for compilation of the
PostgreSQL extension call "make pgsql-library".

For ruby you can also create a gem-file by calling "make ruby-gem".

"make all" can be used to build everything, but both ruby and PostgreSQL
installations are required in this case.


*** GENERAL INFORMATION ***

The C library is found in this directory after successful compilation and
is named "libutf8proc.a" and "libutf8proc.so". The ruby library consists of
the files "utf8proc.rb" and "utf8proc_native.so", which are found in the
subdirectory "ruby/". If you chose to create a gem-file it is placed in the
"ruby/gem" directory. The PostgreSQL extension is named "utf8proc_pgsql.so"
and resides in the "pgsql/" directory.

Both the ruby library and the PostgreSQL extension are built as stand-alone
libraries and are therefore not dependent the dynamic version of the
C library files, but this behaviour might change in future releases.

The Unicode version being supported is 5.0.0.
Note: Version 4.1.0 of Unicode Standard Annex #29 was used, as
      version 5.0.0 had not been available at the time of implementation.

For Unicode normalizations, the following options have to be used:
Normalization Form C:  STABLE, COMPOSE
Normalization Form D:  STABLE, DECOMPOSE
Normalization Form KC: STABLE, COMPOSE, COMPAT
Normalization Form KD: STABLE, DECOMPOSE, COMPAT


*** C LIBRARY ***

The documentation for the C library is found in the utf8proc.h header file.
"utf8proc_map" is most likely function you will be using for mapping UTF-8
strings, unless you want to allocate memory yourself.


*** RUBY API ***

The ruby library adds the methods "utf8map" and "utf8map!" to the String
class, and the method "utf8" to the Integer class.

The String#utf8map method does the same as the "utf8proc_map" C function.
Options for the mapping procedure are passed as symbols, i.e:
"Hello".utf8map(:casefold) => "hello"

The descriptions of all options are found in the C header file
"utf8proc.h". Please notice that the according symbols in ruby are all
lowercase.

String#utf8map! is the destructive function in the meaning that the string
is replaced by the result.

There are shortcuts for the 4 normalization forms specified by Unicode:
String#utf8nfd,  String#utf8nfd!,
String#utf8nfc,  String#utf8nfc!,
String#utf8nfkd, String#utf8nfkd!,
String#utf8nfkc, String#utf8nfkc!

The method Integer#utf8 returns a UTF-8 string, which is containing the
unicode char given by the code point.
0x000A.utf8 => "\n"
0x2028.utf8 => "\342\200\250"


*** POSTGRESQL API ***

For PostgreSQL there are two SQL functions supplied named "unifold" and
"unistrip". These functions function can be used to prepare index fields in
order to be folded in a way where string-comparisons make more sense, e.g.
where "bathtub" == "bath<soft hyphen>tub"
or "Hello World" == "hello world".

CREATE TABLE people (
  id    serial8 primary key,
  name  text,
  CHECK (unifold(name) NOTNULL)
);
CREATE INDEX name_idx ON people (unifold(name));
SELECT * FROM people WHERE unifold(name) = unifold('John Doe');

The function "unistrip" removes character marks like accents or diaeresis,
while "unifold" keeps then.

NOTICE: The outputs of the function can change between releases, as
        utf8proc does not follow a versioning stability policy. You have to
        rebuild your database indicies, if you upgrade to a newer version
        of utf8proc.


*** TODO ***

- detect stable code points and process segments independently in order to
  save memory
- do a quick check before normalizing strings to optimize speed
- support stream processing


*** CONTACT ***

If you find any bugs or experience difficulties in compiling this software,
please contact us:

Project page: http://www.public-software-group.org/utf8proc


