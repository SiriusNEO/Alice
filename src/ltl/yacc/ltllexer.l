%{
#include <iostream>
#include "../ast.hpp"
#include "ltlparser.hpp"
%}

%option noyywrap

%%

[ \n\t\r] { continue; }

"true" { return T_TRUE; }
"("  { return T_L_PAREN; }
")"  { return T_R_PAREN; }

"!"  { return T_NEG; }
"\\/"  { return T_DISJ; }
"/\\"  { return T_CONJ; }
"->"  { return T_IMPL; }
"X"  { return T_NEXT; }
"G"  { return T_ALWAYS; }
"F"   { return T_EVEN; }
"U"   { return T_UNTIL; }

[a-zA-Z_][0-9a-zA-Z_]* {
    yylval.str_val = new std::string(yytext);
    return ID;
}

.  { std::cerr << "Lexer error: Unknown token: " << yytext << std::endl; exit(1); }
%%


void* set_input(const char* in) {
    YY_BUFFER_STATE buffer = yy_scan_string(in);
	if (buffer == nullptr) {
		std::cerr << "error on creating YY_BUFFER_STATE" << std::endl;
		return nullptr;
	}
    yy_switch_to_buffer(buffer);
    return (void*)(buffer);
}

void destory_buffer(void* buffer) {
    YY_BUFFER_STATE buffer_ = reinterpret_cast<YY_BUFFER_STATE>(buffer);
    yy_delete_buffer(buffer_);
	yylex_destroy();
}
