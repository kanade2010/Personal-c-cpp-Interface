ROOT_DIR := $(ROOT_DIRS)
MAKEFILE_ENV := $(ROOT_DIRS)/Makefile.env
EX_INCLUDE :=

include $(MAKEFILE_ENV)

SRC 				:= $(wildcard *.cpp)
OBJ 				:= $(patsubst %.cpp,%.o,$(SRC))
#EXECUTE				:= sw-server

INCLUDE_DIR 		:= .
INCLUDE_DIR 		+= $(PROTOCOL_DIR)
INCLUDE_DIR 		+= $(55AA_DIR)
INCLUDE_DIR 		+= ./Command
INCLUDE_DIR 		+= $(TOOLS_DIR)
INCLUDE_DIR 		+= $(LIB_DIR)

all: $(SUBDIRS) $(OBJ)
$(SUBDIRS) : ECHO
	$(MAKE) -C $@
ECHO : 
	@echo "Compiling " $(SUBDIRS) "..."

#$(OBJ) :
	@echo "SRC : " $(SRC) 
	@echo "OBJ : " $(OBJ) 
	@echo "TARGET : " $(OBJ) 
	#mv $^ $(OBJ_OUT)/
	@echo "Compiling" $@ "end\n"

%.o : %.c
	@echo "Compiling" $< "..."
	$(CC) $(CFLAGS) $(addprefix -I,$(INCLUDE_DIR)) -c $^ -o $@
	@echo "Compiling" $@ "end\n"

%.o : %.cpp
	@echo "Compiling" $< "..."
	$(C++) $(CFLAGS) $(addprefix -I,$(INCLUDE_DIR)) -c $^ -o $@
	@echo "Compiling" $@ "end\n"

.PHONY : clean
clean :
	@echo "Clean " $(SUBDIRS) "..."
	@for dir in $(SUBDIRS); 		\
	do                      		\
		make clean -C $$dir;	\
	done
	rm -rf $(OBJ_OUT)/*.o
