OBJ_HOME = $(top_builddir)/src/ze

TESTS = check_conf check_id
check_PROGRAMS = check_conf check_id

check_id_SOURCES = check_id.c unix_unit.h
check_id_CFLAGS = -DRUN_UNIT
check_id_LDADD = $(OBJ_HOME)-io.$(OBJEXT) $(OBJ_HOME)-conf.$(OBJEXT)\
	$(OBJ_HOME)-lexer.$(OBJEXT) $(OBJ_HOME)-parser.$(OBJEXT)

check_conf_SOURCES = check_conf.c unix_unit.h
check_conf_CFLAGS = -DRUN_UNIT
check_conf_LDADD = $(OBJ_HOME)-conf.$(OBJEXT) $(OBJ_HOME)-parser.$(OBJEXT)\
	$(OBJ_HOME)-lexer.$(OBJEXT) $(OBJ_HOME)-io.$(OBJEXT)
