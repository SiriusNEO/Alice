%{
    #include "../ast.hpp"

    alice::ltl::LTLFormula* root;

    void yyerror(char* msg) {
        std::cerr << "Parser error: " << msg << std::endl;
        exit(1);
    }

    extern int yylex();
%}

%union{
    int int_val;
	std::string* str_val;
    alice::ltl::LTLFormula *node_val;
};

%token <int_val> T_NEG T_CONJ T_DISJ T_IMPL T_NEXT T_ALWAYS T_EVEN T_UNTIL T_TRUE T_L_PAREN T_R_PAREN
%token <str_val> ID
%type <node_val> LTLRoot LTLFormula

%left T_CONJ T_DISJ T_IMPL
%left T_NEG T_NEXT T_ALWAYS T_EVEN T_UNTIL

%start LTLRoot

%%
LTLRoot: LTLFormula { $$ = root = $1; }
         ;

LTLFormula: T_TRUE { $$ = new alice::ltl::True(); }
            | ID { $$ = new alice::ltl::AtomicProposition($1); }
            | T_L_PAREN LTLFormula T_R_PAREN { $$ = $2; }
            | T_NEG LTLFormula { $$ = new alice::ltl::Negation($2); }
            | T_NEXT LTLFormula { $$ = new alice::ltl::Next($2); }
            | T_ALWAYS LTLFormula { $$ = new alice::ltl::Always($2); }
            | T_EVEN LTLFormula { $$ = new alice::ltl::Eventually($2); }
            | LTLFormula T_UNTIL LTLFormula { $$ = new alice::ltl::Until($1, $3); }
            | LTLFormula T_CONJ LTLFormula { $$ = new alice::ltl::Conjunction($1, $3); }
            | LTLFormula T_DISJ LTLFormula { $$ = new alice::ltl::Disjunction($1, $3); }
            | LTLFormula T_IMPL LTLFormula { $$ = new alice::ltl::Implication($1, $3); }
            ;
%%
