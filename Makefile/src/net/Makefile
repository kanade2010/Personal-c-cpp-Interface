ROOT_DIR := $(ROOT_DIRS)
MAKEFILE_ENV := $(ROOT_DIRS)/Makefile.env
EX_INCLUDE := 4g sms wifi

include $(MAKEFILE_ENV)

#EXECUTE				:= sw-server

INCLUDE_DIR 		:= .

all: $(SUBDIRS) $(OBJ)
$(SUBDIRS) : ECHO
	$(MAKE) -C $@
ECHO : 
	@echo "Compiling " $(SUBDIRS) "..."

.PHONY : clean
clean :
	@echo "Clean " $(SUBDIRS) "..."
	@for dir in $(SUBDIRS); 		\
	do                      		\
		make clean -C $$dir;	\
	done