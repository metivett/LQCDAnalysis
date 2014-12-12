CC = g++-4.7
# CC = g++-4.9
# CC = clang
LEX = flex
YACC = bison -y

CURRENT_DIR=$(CURDIR)

CFLAGS = -Wall -O3 -g3 -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"
LIB_NAME = libLQCDAnalysis.so

INCLUDES = -I$(CURRENT_DIR)/include -I$(CURRENT_DIR)/utils/include -I$(CURRENT_DIR)
LIBS = -lgsl -lgslcblas -lmgl -lMinuit2

SRC_DIR = src
INC_DIR = include
UTILS_DIR = utils
INSTALL_DIR = /usr/local/lib

LIBS += -L$(UTILS_DIR) -lutils

SRC = 								\
	DataFile.cpp					\
	FitInterface.cpp				\
	Random.cpp						\
	# GracePlotRenderer.cpp			\
	# Graph.cpp						\
	# Plot.cpp						\
	# PlotModifier.cpp

LEX_SRC =							\
	AsciiLexer.lpp

YACC_SRC =							\
	AsciiParser.ypp

UTILS_SRC =							\
	Exceptions.cpp

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
	PlotModifier.hpp				\
	PlotObject.hpp					\
	PlotOptions.hpp					\
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
UTILS_OBJ = $(addprefix $(UTILS_DIR), $(UTILS_SRC:.cpp=.o))
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
