ROOT_DIR := $(ROOT_DIRS)
MAKEFILE_ENV := $(ROOT_DIRS)/Makefile.env
EX_INCLUDE := asio easySQLite log sqlite3 xml

include $(MAKEFILE_ENV)

all : $(SUBDIRS)
$(SUBDIRS) : ECHO1
	$(MAKE) -C $@

ECHO1 : 
	@echo "Compiling " $(SUBDIRS) "..."

.PHONY : clean
clean :
	@echo "Clean " $(SUBDIRS) "..."
	$(MAKE) clean -C $(SUBDIRS)