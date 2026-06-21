# Minimal Lisp

## Grammar

```bnf
<sexp>       ::= <atom> | <list>

<atom>       ::= <symbol> | <number> | <string> | <boolean> | <nil>

<list>       ::= '(' <sexp-list> ')'

<sexp-list>  ::= <sexp> <sexp-list> | <empty>

<symbol>     ::= <letter> | <symbol> <letter> | <symbol> <digit> | <symbol> <special>
               ;; Symbols usually cannot start with a digit and contain alphanumeric + ! $ % & * / : < = > ? @ ^ _ ~

<letter>     ::= 'a'...'z' | 'A'...'Z'
<digit>      ::= '0'...'9'
<special>    ::= '!' | '$' | '%' | '&' | '*' | '/' | ':' | '<' | '=' | '>' | '?' | '@' | '^' | '_' | '~'

<number>     ::= <integer> | <float> | <hex>
<integer>    ::= '-'? <digit>+
<float>      ::= <integer> '.' <digit>+
<nil>        ::= 'nil' | '()'
<boolean>    ::= '#t' | '#f'
```