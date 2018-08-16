ROOT_DIR := $(ROOT_DIRS)
MAKEFILE_ENV := $(ROOT_DIRS)/Makefile.env
include $(MAKEFILE_ENV)

SRC := $(wildcard *.cpp)
OBJ := $(patsubst %.cpp,%.o,$(SRC))
TARGET := liblog.a

INCLUDE_DIR := ./
LIB := -lpthread

CFLAGS := -I$(INCLUDE_DIR)

all: $(TARGET)
$(TARGET) : $(OBJ)
	@echo "SRC : " $(SRC) 
	@echo "OBJ : " $(OBJ) 
	@echo "TARGET : " $(TARGET) 
	$(AR) $(LIB_OUT)/$@
	#mv $^ $(OBJ_OUT)/
	@echo "Compiling" $@ "end\n"

%.o : %.c
	@echo "Compiling" $< "..."
	$(CC) $(CFLAGS) -c $^ -o $@ $(LIB)
	@echo "Compiling" $@ "end\n"

%.o : %.cpp
	@echo "Compiling" $< "..."
	$(C++) $(CFLAGS) -c $^ -o $@ $(LIB) -std=c++11
	@echo "Compiling" $@ "end\n"

.PHONY: clean

clean:
	rm -rf $(OBJ_OUT)/*.o
	rm -rf ./out/*.o
