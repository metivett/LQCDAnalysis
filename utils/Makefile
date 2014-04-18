CC = g++

CFLAGS = -Wall -O3 -g3 -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

INCLUDES = -I/home/thibaut/workspace/LQCDAnalysis/include -I/home/thibaut/workspace/LQCDAnalysis/utils/include -I/home/thibaut/workspace/LQCDAnalysis
LIBS = -lgsl -lmgl

LIB_NAME = libutils.so

SRC_DIR = src
INC_DIR = include
INSTALL_DIR = /usr/local/lib

SRC = 								\
	Exceptions.cpp

HEADERS = 							\
	Exceptions.hpp					\
	Factory.hpp						\
	FactoryError.hpp				\
	IO.hpp							\
	Policies.hpp					\
	SingletonHolder.hpp				\
	Utilities.hpp					\


OBJ = $(SRC:.cpp=.o)
OBJ_FILES = $(OBJ)

all: $(LIB_NAME)

clean:
	rm -f $(LIB_NAME) 
	rm -f $(OBJ_FILES) $(OBJ_FILES:%.o=%.d)

$(LIB_NAME): $(OBJ_FILES)
	@echo 'Building target $@'
	@echo 'Invoking GCC C++ Linker'
	$(CC) -shared -o $(LIB_NAME) $(OBJ_FILES) $(LIBS)
	@echo 'Finished building target $@'
	@echo ' '

-include $(OBJ_FILES:.o=.d)

%.o: $(SRC_DIR)/%.cpp
	@echo 'Building file $<'
	@echo 'Invoking GCC C++ Compiler'
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<
	@echo 'Finished building: $<'
	@echo ' '