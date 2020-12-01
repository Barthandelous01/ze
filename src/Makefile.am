bin_PROGRAMS = ze
man1_MANS = ze.1

ze_SOURCES = main.c id.c id.h error.h db.c io.c io.h conf.h conf.c db.h \
	ze.1
ze_CFLAGS = $(LMDB_CFLAGS)
ze_LDADD = $(LMDB_LIBS)
