if GIT
SCRIPT = ze-git
MAN = ze-git.1
else
SCRIPT =
MAN =
endif

dist_bin_SCRIPTS = $(SCRIPT)
dist_man1_MANS = $(MAN)

ze-git: ze-git.in
	sed -e 's/ZETTEL_HOME/~\/.ze\/zettels/' < $? > $@
	chmod +x $@
