# CC = clang
CC = g++-4.7
LD = g++

CURRENT_DIR=$(CURDIR)

CFLAGS = -O2 -std=c++11
LDFLAGS = -fopenmp

INCLUDES = -I.
INCLUDES += -I/usr/local/include/
INCLUDES += -I$(CURRENT_DIR)/../include
INCLUDES += -I$(CURRENT_DIR)/..
INCLUDES += -I$(CURRENT_DIR)/../utils/include

LIBS = -L$(CURRENT_DIR)/.. -lLQCDAnalysis
LIBS += -L$(CURRENT_DIR)/../utils -lutils
LIBS += -lMinuit2
LIBS += -lboost_program_options
LIBS += -lgsl -lgslcblas
LIBS += -lgrace_np

SRC_DIR = .
OBJ_FILES = main.o

EXE_NAME = test

.PHONY: clean $(EXE_NAME)

all : $(EXE_NAME)

clean :
	rm -f $(EXE_NAME) $(OBJ_FILES)

$(EXE_NAME): $(OBJ_FILES)
	@echo 'Building target $@'
	@echo 'Invoking GCC C++ Linker'
	$(LD) $(LDFLAGS) -o $(EXE_NAME) $(OBJ_FILES) $(LIBS)
	@echo 'Finished building target $@'
	@echo ' '

%.o: $(SRC_DIR)/%.cpp
	@echo 'Building file $<'
	@echo 'Invoking GCC C++ Compiler'
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<
	@echo 'Finished building: $<'
	@echo ' '

%.o: $(SRC_DIR)/%.cc
	@echo 'Building file $<'
	@echo 'Invoking GCC C++ Compiler'
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<
	@echo 'Finished building: $<'
	@echo ' '

%.o: $(SRC_DIR)/%.c
	@echo 'Building file $<'
	@echo 'Invoking GCC C++ Compiler'
	gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<
	@echo 'Finished building: $<'
	@echo ' '
