REPOSITORIES_PATH = repositories
DISTRIBUTIONS = debian ubuntu
CHROOT_BASE = /var/lib/chroot
ARCHITECTURES = i386 amd64
CODES = wheezy jessie unstable precise quantal saucy
FRISO_PACKAGE = friso
FRISO_VERSION = 1.6.1
FRISO_BASE_NAME = $(FRISO_PACKAGE)-$(FRISO_VERSION)

all:

release: build sign-packages update-repository sign-repository upload

remove-existing-packages:
	for distribution in $(DISTRIBUTIONS); do		\
	  find $(REPOSITORIES_PATH)/$${distribution}/pool	\
	    -type f -delete;					\
	done

ensure-rsync-path:
	@if test -z "$(RSYNC_PATH)"; then				\
	  echo "--with-rsync-path configure option must be specified.";	\
	  false;							\
	fi

download: ensure-rsync-path
	for distribution in $(DISTRIBUTIONS); do	\
	  rsync -avz --progress --delete		\
	    $(RSYNC_PATH)/$${distribution}		\
	    ${REPOSITORIES_PATH}/;	\
	done

sign-packages:
	./sign-packages.sh '$(GPG_UID)' '$(REPOSITORIES_PATH)/' '$(CODES)'

update-repository:
	./update-repository.sh '$(PACKAGE_NAME)' '$(REPOSITORIES_PATH)/' \
	  '$(ARCHITECTURES)' '$(CODES)'

sign-repository:
	./sign-repository.sh '$(GPG_UID)' '$(REPOSITORIES_PATH)/' '$(CODES)'

upload: ensure-rsync-path
	for distribution in $(DISTRIBUTIONS); do		\
	  (cd $(REPOSITORIES_PATH)/$${distribution};		\
	   rsync -avz --progress --delete			\
	     dists pool $(RSYNC_PATH)/$${distribution});	\
	done

build: build-package-deb

build-package-deb: source
	./build-in-chroot.sh						\
	  $(PACKAGE) $(VERSION) $(srcdir)/.. $(REPOSITORIES_PATH)/	\
	  $(CHROOT_BASE) '$(ARCHITECTURES)' '$(CODES)'

build-friso-deb: friso-source
	./build-in-chroot.sh						\
	  $(FRISO_PACKAGE) $(FRISO_VERSION) $(srcdir)			\
	  $(REPOSITORIES_PATH)/	$(CHROOT_BASE)				\
	  '$(ARCHITECTURES)' '$(CODES)'

source: ../$(PACKAGE)-$(VERSION).tar.gz

friso-source: $(FRISO_PACKAGE)-$(FRISO_VERSION).tar.gz

../$(PACKAGE)-$(VERSION).tar.gz:
	ln -s $(abs_top_builddir)/$(PACKAGE)-$(VERSION).tar.gz ../

$(FRISO_PACKAGE)-$(FRISO_VERSION).tar.gz:
	wget https://friso.googlecode.com/files/$(FRISO_PACKAGE)-$(FRISO_VERSION)-src-lib-dict.zip
	unzip $(FRISO_PACKAGE)-$(FRISO_VERSION)-src-lib-dict.zip
	tar cvf $(FRISO_PACKAGE)-$(FRISO_VERSION).tar $(FRISO_PACKAGE)-$(FRISO_VERSION)
	gzip $(FRISO_PACKAGE)-$(FRISO_VERSION).tar
