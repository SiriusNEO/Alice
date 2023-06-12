# LTL Parser

### Lexer

Valid Tokens

```
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
```

The rule of identifiers (atom propositions): `[a-z]+` (i.e all lower case letters)

Note: Escape character`\` should be written as `\\`

### BNF

```
LTLFormula: T_TRUE
            | ID
            | T_L_PAREN LTLFormula T_R_PAREN
            | T_NEG LTLFormula
            | T_NEXT LTLFormula
            | T_ALWAYS LTLFormula
            | T_EVEN LTLFormula
            | LTLFormula T_UNTIL LTLFormula
            | LTLFormula T_CONJ LTLFormula
            | LTLFormula T_DISJ LTLFormula
            | LTLFormula T_IMPL LTLFormula
            ;
```

### Reduction

Some operators are reduced to basic operators. They are:

- $\lozenge \varphi:= \mathbf{true}\ \mathsf{U}\ \varphi$ 
- $\square \varphi = \neg(\mathbf{true}\ \mathsf{U}\ \neg \varphi)$
- $\varphi_1 \vee \varphi_2 = \neg (\neg \varphi_1 \wedge \neg \varphi_2)$
- $\varphi_1 \rightarrow \varphi_2 = \neg (\varphi_1 \wedge \neg \varphi_2)$

