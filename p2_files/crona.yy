%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {crona}
 /*
 If your bison install has trouble with the
 line %define api.parser.class {Parser} try
 using the older %define parser_class_name {Parser}
 instead
 */
%define api.parser.class {Parser}
%define parse.assert
%define parse.error verbose
%output "parser.cc"
%token-table

%code requires{
	#include <list>
	#include "tokens.hpp"
  using namespace std;

	namespace crona {
		class Scanner;
	}

//The following definition is required when
// we don't use the %locations directive (which we won't)
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

//End "requires" code
}

%parse-param { crona::Scanner &scanner }

%code{
   // C std code for utility functions
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   // Our code for interoperation between scanner/parser
   #include "scanner.hpp"
   //#include "tokens.hpp"

  //Request tokens from our scanner member, not
  // from a global function
  #undef yylex
  #define yylex scanner.yylex
}

/*
The %union directive is a way to specify the
set of possible types that might be used as
translation attributes that a symbol might take.
For this project, only terminals have types (we'll
have translation attributes for non-terminals in the next
project)
*/
%union {
   crona::Token*                         lexeme;
   crona::Token*                         transToken;
}

%token                   END	   0 "end file"
%token	<transToken>     AND
%token	<transToken>     ARRAY
%token	<transToken>     ASSIGN
%token	<transToken>     BOOL
%token	<transToken>     BYTE
%token	<transToken>     COLON
%token	<transToken>     COMMA
%token	<transToken>     CROSS
%token	<transToken>     CROSSCROSS
%token	<transToken>     DASH
%token	<transToken>     DASHDASH
%token	<transToken>     ELSE
%token	<transToken>     EQUALS
%token	<transToken>     FALSE
%token	<transToken>     READ
%token	<transToken>     HAVOC
%token	<transIDToken>   ID
%token	<transToken>     IF
%token	<transToken>     INT
%token	<transIntToken>  INTLITERAL
%token	<transToken>     GREATER
%token	<transToken>     GREATEREQ
%token	<transToken>     LBRACE
%token	<transToken>     LCURLY
%token	<transToken>     LESS
%token	<transToken>     LESSEQ
%token	<transToken>     LPAREN
%token	<transToken>     NOT
%token	<transToken>     NOTEQUALS
%token	<transToken>     OR
%token	<transToken>     RBRACE
%token	<transToken>     RCURLY
%token	<transToken>     RETURN
%token	<transToken>     RPAREN
%token	<transToken>     SEMICOLON
%token	<transToken>     SLASH
%token	<transToken>     STRING
%token	<transToken>     STAR
%token	<transStrToken>  STRLITERAL
%token	<transToken>     TRUE
%token	<transToken>     VOID
%token	<transToken>     WHILE
%token	<transToken>     WRITE

/* NOTE: Make sure to add precedence and associativity
 * declarations
*/

%%

/* TODO: add productions for the other nonterminals in the
   grammar and make sure that all of the productions of the
   given nonterminals are complete
*/
program : globals { }

globals : globals decl 	{ }
        | /* epsilon */ 	{ }

decl : varDecl{ cout << "Var decl matched" << endl; }
     | fnDecl { cout << "Fn decl matched" << endl; }

varDecl : id COMMA varDecl	{ }
        | id COLON type	SEMICOLON { }

type : INT		{ cout << "INT "; }
     | INT ARRAY LBRACE INTLITERAL RBRACE   { cout << "INT ARRAY "; }
     | BOOL		{ cout << "BOOL "; }
     | BOOL ARRAY LBRACE INTLITERAL RBRACE 	{ cout << "BOOL ARRAY "; }
     | BYTE		{ cout << "BYTE "; }
     | BYTE ARRAY LBRACE INTLITERAL RBRACE 	{ cout << "BYTE ARRAY "; }
     | STRING		{ cout << "STRING "; }
     | VOID { cout << "VOID "; }
     /* TODO: add the rest of the types */

fnDecl : id COLON type formals fnBody { }

formals : LPAREN RPAREN { }
        | LPAREN formalsList RPAREN { }

formalsList : formalDecl { cout << "Formal Decl matched -> "; }
            | formalDecl COMMA formalsList { cout << "Formal Decl matched -> "; }

<<<<<<< HEAD
varDecl : id COMMA varDecl	{ }
        | id COLON type	{ }

type : INT		{ }
     | ARRAY INT	{ }
     | BOOL		{ }
     | ARRAY BOOL	{ }
     | BYTE		{ }
     | ARRAY BYTE	{ }
     | STRING		{ }
     
     /* TODO: The productions for exp, term and the rest in crona.grammar are
     ambiguous. We need to add precedence and associativity. */
exp : assignExp	 { }
    | exp DASH exp	 { }
    | exp CROSS exp	 { }
    | exp STAR exp	 { }
    | exp SLASH exp	 { }
    | exp AND exp	 { }
    | exp OR exp	 { }
    | exp EQUALS exp	 { }
    | exp NOTEQUALS exp { }
    | exp GREATER exp	 { }
    | exp GREATEREQ exp { }
    | exp LESS exp	 { }
    | exp LESSEQ exp	 { }
    | NOT exp		 { }
    | DASH term	 { }
    | term		 { }

term : val			{ }
     | INTLITERAL		{ }
     | STRLITERAL		{ }
     | TRUE			{ }
     | FALSE			{ }
     | HAVOC			{ }
     | LPAREN exp RPAREN	{ }
     | fncall			{ }

formalDecl  : id COLON type { }

fnBody  : LCURLY stmtList RCURLY { }

stmtList : stmtList stmt { }
         | /* epsilon */ { }

stmt  : varDecl { cout << "Var decl matched in Fn, "; }
      | lval DASHDASH SEMICOLON { cout << "increment matched in Fn, "; }
      | lval CROSSCROSS SEMICOLON { cout << "decrement matched in Fn, "; }
      | READ lval SEMICOLON { cout << "READ matched in Fn, "; }
      | RETURN SEMICOLON { cout << "RETURN matched in Fn, "; }
      | fncall SEMICOLON { cout << "fn call matched in Fn, "; }

fncall          :  id LPAREN RPAREN   // fn call with no args {  }

lval  : id { }


id : ID { cout << " ... "; }

 /* TODO: add productions for the entire grammar of the language */

%%

void crona::Parser::error(const std::string& err_message){
   /* For project grading, only report "syntax error"
      if a program has bad syntax. However, you will
      probably want better output for debugging. Thus,
      this error function prints a verbose message to
      stdout, but only prints "syntax error" to stderr
   */
	cout << err_message << std::endl;
	cerr << "syntax error" << std::endl;
}
