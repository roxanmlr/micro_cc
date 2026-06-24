#ifndef MICRO_CC_H
#define MICRO_CC_H
#include "lexer/lexer.h"

typedef unsigned char byte;

struct micro_cc{
	byte stop_lexer;
	byte stop_parser;
	byte stop_codegen;
};

// -------------------------------------

#endif