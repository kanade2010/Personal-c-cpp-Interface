OBJOUT := ./out/
EXEOUT := ./out/
INCLUDE_DIR := ./logger
LIB := -lpthread
LOGGER_DIR = ./logger/

SRC := $(wildcard *.cpp)
SRC += $(wildcard $(LOGGER_DIR)*.cpp)
OBJ := $(patsubst %.cpp,%.o,$(SRC))
TARGET := HttpClient

CC := g++
CFLAGS := -I$(INCLUDE_DIR)

all: $(TARGET)
$(TARGET) : $(OBJ)
	@echo "SRC : " $(SRC) 
	@echo "OBJ : " $(OBJ) 
	@echo "TARGET : " $(TARGET) 
	#$(CC) -C $(LOGGER_DIR)
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LIB)
	#mv $^ $(OBJOUT)
	@echo "Compiling" $@ "end\n"

%.o : %.c
	@echo "Compiling" $< "..."
	$(CC) $(CFLAGS) -c $^ -o $@ $(LIB)
	@echo "Compiling" $@ "end\n"

%.o : %.cpp
	@echo "Compiling" $< "..."
	$(CC) $(CFLAGS) -c $^ -o $@ $(LIB) -std=c++11
	@echo "Compiling" $@ "end\n"

.PHONY: clean

clean:
	rm -rf $(OBJOUT)*.o $(EXEOUT)$(TARGET)
	#rm -rf $(LOGGER_DIR)*.o
	rm -rf ./*.o
