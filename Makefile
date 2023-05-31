# Makefile for YACC

all: alice

YACC_PREFIX = src/ltl/yacc/

YACC_GEN = ${YACC_PREFIX}ltlparser.cpp \
		  ${YACC_PREFIX}ltlparser.hpp \
		  ${YACC_PREFIX}ltllexer.cpp \

${YACC_PREFIX}ltlparser.cpp: ${YACC_PREFIX}ltlparser.y
	bison -d -o $@ $^

${YACC_PREFIX}ltlparser.hpp: ${YACC_PREFIX}ltlparser.cpp

${YACC_PREFIX}ltllexer.cpp: ${YACC_PREFIX}ltllexer.l ${YACC_PREFIX}ltlparser.hpp
	flex -o $@ $^

alice: $(YACC_GEN)

clean:
	$(RM) -rf $(YACC_GEN)
