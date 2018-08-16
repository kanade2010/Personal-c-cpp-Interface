ROOT_DIR := $(ROOT_DIRS)
MAKEFILE_ENV := $(ROOT_DIRS)/Makefile.env
EX_INCLUDE := arch lib include documents samples build cross-tool

include $(MAKEFILE_ENV)

all : $(SUBDIRS)
$(SUBDIRS) : ECHO1
	$(MAKE) -C $@

ECHO1 : 
	@echo "Compiling " $(SUBDIRS) "..."

.PHONY : clean
clean :
	@echo "Clean " $(SUBDIRS) "..."
	@for dir in $(SUBDIRS); 		\
	do                      		\
		make clean -C $$dir;	\
	done
	rm -rf $(EXE_OUT)/*