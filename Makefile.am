ACLOCAL_AMFLAGS = -I m4

SUBDIRS =					\
	packages				\
	tokenizers				\
	test

check:
	(cd test; ./run-test.sh)

echo-groonga:
	@echo $(GROONGA)
