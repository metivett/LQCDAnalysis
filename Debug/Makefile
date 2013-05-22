CC = g++
CFLAGS = -Wall -O0 -g3 -fmessage-length=0 -std=c++0x -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"
LIB_NAME = libLQCDAnalysis.so
INCLUDES = -I/home/thibaut/workspace/LQCDAnalysis/include -I/home/thibaut/workspace/LQCDAnalysis
LIBS = -lgsl -lmgl -lMinuit2
SRC_DIR = ../src
OBJ_FILES = fit_fcn.o interpolator.o io.o io_correlator.o io_sample.o io_utils.o model_parameters.o random.o statistics.o
INSTALL_DIR = /usr/local/lib

all : $(LIB_NAME)

clean :
	rm $(LIB_NAME) $(OBJ_FILES) $(OBJ_FILES:%.o=%.d)

$(LIB_NAME) : $(OBJ_FILES)
	@echo 'Building target $@'
	@echo 'Invoking GCC C++ Linker'
	$(CC) -shared -o $(LIB_NAME) $(OBJ_FILES) $(LIBS)
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

install :
	cp $(LIB_NAME) $(INSTALL_DIR)
