OBJ_HOME = $(top_builddir)/src/ze

TESTS = check_id check_conf
check_PROGRAMS = check_id check_conf

check_id_SOURCES = check_id.c $(top_builddir)/src/id.h
check_id_CFLAGS = @CHECK_CFLAGS@
check_id_LDADD = @CHECK_LIBS@ $(OBJ_HOME)-io.o $(OBJ_HOME)-conf.o \
	$(OBJ_HOME)-id.o

check_conf_SOURCES = check_conf.c $(top_builddir)/src/conf.h
check_conf_CFLAGS = @CHECK_CFLAGS@
check_conf_LDADD = @CHECK_LIBS@ $(OBJ_HOME)-conf.o $(OBJ_HOME)-io.o
