CC = g++
LEX = flex
YACC = bison -y

CFLAGS = -Wall -O3 -g3 -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"
LIB_NAME = libLQCDAnalysis.so

INCLUDES = -I/home/thibaut/workspace/LQCDAnalysis/include -I/home/thibaut/workspace/LQCDAnalysis/utils/include -I/home/thibaut/workspace/LQCDAnalysis
LIBS = -lgsl -lmgl -lMinuit2

SRC_DIR = src
INC_DIR = include
UTILS_DIR = utils
INSTALL_DIR = /usr/local/lib

LIBS += -L$(UTILS_DIR) -lutils

SRC = 								\
	DataFile.cpp					\
	FitInterface.cpp				\
	GracePlotRenderer.cpp			\
	Graph.cpp						\
	Plot.cpp						\
	Random.cpp

LEX_SRC =							\
	AsciiLexer.lpp

YACC_SRC =							\
	AsciiParser.ypp

HEADERS = 							\
	CostFunction.hpp				\
	DataFile.hpp					\
	DataReader.hpp					\
	DataSet.hpp						\
	DataSetIterator.hpp				\
	Fit.hpp							\
	FitInterface.hpp				\
	FitOptions.hpp					\
	FitResult.hpp					\
	Function.hpp					\
	FunctionInterpolator.hpp		\
	GSLRootFinder.hpp				\
	Globals.hpp						\
	GracePlot.hpp					\
	Graph.hpp						\
	IOObject.hpp					\
	LinalgUtils.hpp					\
	MatrixSample.hpp				\
	MetaProgUtils.hpp				\
	Minimize.hpp					\
	Minimizer.hpp					\
	Minuit2Minimizer.hpp			\
	ParametrizedFunction.hpp		\
	ParserState.hpp					\
	Plot.hpp						\
	PlotObject.hpp					\
	PlotRenderer.hpp				\
	Random.hpp						\
	Reduction.hpp					\
	RootFinder.hpp					\
	Sample.hpp						\
	StaticFunction.hpp				\
	StaticParametrizedFunction.hpp	\
	TypeTraits.hpp					\
	XYData.hpp						\
	XYDataInterface.hpp				\
	XYDataSample.hpp


OBJ = $(SRC:.cpp=.o)
LEX_OBJ = $(LEX_SRC:.lpp=.o)
YACC_OBJ = $(YACC_SRC:.ypp=.o)
OBJ_FILES = $(LEX_OBJ) $(YACC_OBJ) $(OBJ)


all: $(LIB_NAME)

clean:
	$(MAKE) -C $(SRC_DIR) clean
	rm -f $(LIB_NAME) 
	rm -f $(OBJ_FILES) $(OBJ_FILES:%.o=%.d)

$(LIB_NAME): libutils lexer parser $(OBJ_FILES)
	@echo 'Building target $@'
	@echo 'Invoking GCC C++ Linker'
	$(CC) -shared -o $(LIB_NAME) $(OBJ_FILES) $(LIBS)
	@echo 'Finished building target $@'
	@echo ' '

-include $(OBJ_FILES:.o=.d)

libutils:
	$(MAKE) -C $(UTILS_DIR) all

lexer:
	$(MAKE) -C $(SRC_DIR) asciilexer

parser:
	$(MAKE) -C $(SRC_DIR) asciiparser

%.o: $(SRC_DIR)/%.cpp
	@echo 'Building file $<'
	@echo 'Invoking GCC C++ Compiler'
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<
	@echo 'Finished building: $<'
	@echo ' '

install :
	cp $(LIB_NAME) $(INSTALL_DIR)
