ACLOCAL_AMFLAGS = -I m4

SUBDIRS =					\
	tokenizers				\
	test

check:
	(cd test; ./run-test.sh)

echo-groonga:
	@echo $(GROONGA)
