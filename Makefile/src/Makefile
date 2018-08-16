ROOT_DIR := $(ROOT_DIRS)
MAKEFILE_ENV := $(ROOT_DIRS)/Makefile.env
EX_INCLUDE := config

include $(MAKEFILE_ENV)

EXECUTE				:= sw-server

INCLUDE_DIR 		+= $(PROTOCOL_DIR)
INCLUDE_DIR 		+= $(TOOLS_DIR)
INCLUDE_DIR 		+= $(4g_DIR)
INCLUDE_DIR			+= $(COM_DIR)
INCLUDE_DIR 		+= $(808_DIR)
INCLUDE_DIR 		+= $(55AA_DIR)
INCLUDE_DIR			+= $(LIB_DIR)

USELIB				:= $(LIB)/libeasySQLITE.a

SRC 				:= $(wildcard $(PROTOCOL_DIR)/*.cpp)
SRC 				+= $(wildcard $(TOOLS_DIR)/*.cpp)
SRC 				+= $(wildcard $(4g_DIR)/*.cpp)
SRC 				+= $(wildcard $(COM_DIR)/*.cpp)
SRC 				+= $(wildcard $(808_DIR)/*.cpp)
SRC 				+= $(wildcard $(55AA_DIR)/*.cpp)
SRC 				+= $(wildcard $(55AA_DIR)/Command/*.cpp)
SRC 				+= $(wildcard $(808_DIR)/Command/*.cpp)

OBJ 				:= $(patsubst %.cpp,%.o,$(SRC))

all : $(SUBDIRS) $(EXECUTE)
$(SUBDIRS) : ECHO
	$(MAKE) -C $@

ECHO : 
	@echo "Compiling " $(SUBDIRS) "..."

$(EXECUTE) :
	@echo "OBJ : " $(OBJ) 
	@echo "TARGET : " $(EXECUTE)
	$(C++) $(CFLAGS) $(addprefix -I,$(INCLUDE_DIR)) main.cpp -o $(EXE_OUT)/$(EXECUTE) $(OBJ) $(addprefix -L,$(LIBDIR)) $(LDFLAGS) -l$(USELIB)

.PHONY : clean
clean :
	@echo "Clean " $(SUBDIRS) "..."
	@for dir in $(SUBDIRS); 		\
	do                      		\
		make clean -C $$dir;	\
	done
	rm -rf $(OBJ_OUT)/*.o