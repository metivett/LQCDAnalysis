LEX = flex
YACC = bison -y

INC_DIR = ../include

SRC = 								\
	AsciiLexer.lpp					\
	AsciiParser.ypp					\

LEX_SRC =						\
	AsciiLexer.cpp

YACC_SRC =					\
	AsciiParser.cpp


LEX_OBJ = $(SRC:.lpp=.o)
YACC_OBJ = $(SRC:.ypp=.o)
OBJ_FILES = $(LEX_OBJ) $(YACC_OBJ) $(OBJ)


all : asciilexer asciiparser

clean :
	rm -f $(LEX_SRC) 
	rm -f $(YACC_SRC)
	rm -f $(INC_DIR)/AsciiLexer.hpp
	rm -f $(INC_DIR)/AsciiParser.hpp


asciilexer: AsciiLexer.cpp

asciiparser: AsciiParser.cpp

AsciiLexer.cpp: AsciiLexer.lpp
	@echo 'Building lexer $<'
	@echo 'Invoking FLEX'
	$(LEX) -o $*.cpp --header-file=$*.hpp $<
	mv $*.hpp $(INC_DIR)/
	@echo 'Finished building: $<'
	@echo ' '

AsciiParser.cpp: AsciiParser.ypp
	@echo 'Building parser $<'
	@echo 'Invoking BISON'
	$(YACC) -o $*.cpp --defines=$*.hpp $<
	mv $*.hpp $(INC_DIR)/
	@echo 'Finished building: $<'
	@echo ' '
