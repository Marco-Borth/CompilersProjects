%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace{crona}
%define api.parser.class {Parser}
%define parse.error verbose
%output "parser.cc"
%token-table

%code requires{
	#include <list>
	#include "tokens.hpp"
	#include "ast.hpp"
	namespace crona {
		class Scanner;
	}

//The following definition is required when
// we don't have the %locations directive
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
%parse-param { crona::ProgramNode** root }
%code{
   // C std code for utility functions
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   // Our code for interoperation between scanner/parser
   #include "scanner.hpp"
   #include "ast.hpp"
   #include "tokens.hpp"

  //Request tokens from our scanner member, not
  // from a global function
  #undef yylex
  #define yylex scanner.yylex
}

%union {
   crona::Token*                         transToken;
   crona::IntLitToken*                   transIntToken;
   crona::IDToken*                       transIDToken;
   crona::ProgramNode*                   transProgram;
	 std::list<crona::StmtNode*>*					 transStmtList;
	 crona::StmtNode*											 transStmt;
   std::list<crona::DeclNode *> *        transDeclList;
	 std::list<crona::FormalDeclNode*>*		 transFormalDeclList;
	 crona::FormalDeclNode*								 transFormalDecl;
	 std::list<crona::StmtNode*>*					 transStmtList;
	 crona::StmtNode*											 transStmt;
	 crona::FnDeclNode *									 transFnDecl;
   crona::DeclNode *                     transDecl;
   crona::VarDeclNode *                  transVarDecl;
	 std::list<crona::FormalDeclNode*>*		 transFormalDeclList;
   crona::FormalDeclNode *               transFormalDecl;
   crona::FnDeclNode *                   transFnDecl;
   crona::TypeNode *                     transType;
   crona::IDNode *                       transID;
}

%define parse.assert

/* Terminals
 *  No need to touch these, but do note the translation type
 *  of each node. Most are just "transToken", which is defined in
 *  the %union above to mean that the token translation is an instance
 *  of crona::Token *, and thus has no fields (other than line and column).
 *  Some terminals, like ID, are "transIDToken", meaning the translation
 *  also has a name field.
*/
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
%token	<transToken>     READ
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


/* Nonterminals
*  TODO: You will need to add more nonterminals
*  to this list as you add productions to the grammar
*  below (along with indicating the appropriate translation
*  attribute type). Note that the specifier in brackets
*  indicates the type of the translation attribute using
*  the names defined in the %union directive above
*/
/*    (attribute type)    (nonterminal)    */
%type <transProgram>    				program
%type <transDeclList>   				globals
%type <transDecl>       				decl
%type <transFnDecl>							fnDecl
%type <transFormalDecl>					formalDecl
%type <transFormalDeclList>			formals
%type <transFormalDeclList>			formalsList
%type <transStmtList>						stmtList
%type <transStmt>								stmt
%type <transStmtList>						fnBody
%type <transVarDecl>    				varDecl
%type <transType>       				type
%type <transID>         				id


%right ASSIGN
%left OR
%left AND
%nonassoc LESS GREATER LESSEQ GREATEREQ EQUALS NOTEQUALS
%left DASH CROSS
%left STAR SLASH
%left NOT

%%

program 	: globals {
		  $$ = new ProgramNode($1);
		  *root = $$;
		  }

globals 	: globals decl {
	  	  $$ = $1;
	  	  DeclNode * declNode = $2;
		  	$$->push_back(declNode);
	  	  }
		| /* epsilon */ {$$ = new std::list<DeclNode * >(); }

decl 		: varDecl SEMICOLON {
		  //TODO: Make sure to fill out this rule
		  // (as well as any other empty rule!)
		  // with the appropriate SDD to create an AST
		  }
		| fnDecl { $$ = $1;}
;
varDecl 	: id COLON type {
		  size_t line = $1->line();
		  size_t col = $1->col();
		  $$ = new VarDeclNode(line, col, $3, $1);
		  }


type 		: INT { $$ = new IntTypeNode($1->line(), $1->col()); }
		| INT ARRAY LBRACE INTLITERAL RBRACE {
			IntTypeNode * intType = new IntTypeNode($1->line(), $1->col());
			IntLitNode * size = new IntLitNode($4);
			$$ = new ArrayTypeNode($2->line(), $2->col(), intType, size);
		}
		| BOOL {  $$ = new BoolTypeNode($1->line(), $1->col()); }
		| BOOL ARRAY LBRACE INTLITERAL RBRACE {
			BoolTypeNode * intType = new BoolTypeNode($1->line(), $1->col());
			IntLitNode * size = new IntLitNode($4);
			$$ = new ArrayTypeNode($2->line(), $2->col(), intType, size);
		}
		| BYTE { $$ = new ByteTypeNode($1->line(), $1->col()); }
		| BYTE ARRAY LBRACE INTLITERAL RBRACE {
			ByteTypeNode * intType = new ByteTypeNode($1->line(), $1->col());
			IntLitNode * size = new IntLitNode($4);
			$$ = new ArrayTypeNode($2->line(), $2->col(), intType, size);
		}
		| STRING { $$ = new StringTypeNode($1->line(), $1->col()); }
		| VOID { $$ = new VoidTypeNode($1->line(), $1->col()); }

fnDecl 		: id COLON type formals fnBody {
		//size_t line = $1->line();
		//size_t col = $1->col();
		//$$ = new FnDeclNode(line, col, $3, $1, $4, $5);
		}

formals 	: LPAREN RPAREN { }
		| LPAREN formalsList RPAREN {
		//$$ = $2; 
		}

formalDecl 	: id COLON type {
		//size_t line = $1->line();
		//size_t col = $1->col();
		//$$ = new FormalDeclNode(line, col, $3, $1);
		}

formalsList	: formalDecl {
		//std::list<FormalDeclNode*>* temp = new std::list<FormalDeclNode*>();
		//temp->push_front($1);
		//$$ = temp;
	 	}
		| formalDecl COMMA formalsList {
    //$3->push_front($1);
		//$$ = $3;
		}

fnBody		: LCURLY stmtList RCURLY { }

stmtList 	: /* epsilon */ { }
		| stmtList stmt { }

stmt		: varDecl SEMICOLON { }
		| assignExp SEMICOLON { }
		| lval DASHDASH SEMICOLON { }
		| lval CROSSCROSS SEMICOLON { }
		| READ lval SEMICOLON { }
		| WRITE exp SEMICOLON { }
		| IF LPAREN exp RPAREN LCURLY stmtList RCURLY { }
		| IF LPAREN exp RPAREN LCURLY stmtList RCURLY ELSE LCURLY stmtList RCURLY { }
		| WHILE LPAREN exp RPAREN LCURLY stmtList RCURLY { }
		| RETURN exp SEMICOLON { }//$$ = new ReturnStmtNode(size_t lineIn, size_t colIn, ExpNode * exp); }
		| RETURN SEMICOLON { }
		| callExp SEMICOLON { }

exp		: assignExp { }
		| exp DASH exp { }
		| exp CROSS exp { }
		| exp STAR exp { }
		| exp SLASH exp { }
		| exp AND exp { }
		| exp OR exp { }
		| exp EQUALS exp { }
		| exp NOTEQUALS exp { }
		| exp GREATER exp { }
		| exp GREATEREQ exp { }
		| exp LESS exp { }
		| exp LESSEQ exp { }
		| NOT exp { }
		| DASH term { }
		| term { }

assignExp	: lval ASSIGN exp { }

callExp		: id LPAREN RPAREN { }
		| id LPAREN actualsList RPAREN { }

actualsList	: exp { }
		| actualsList COMMA exp { }

term 		: lval { }
		| INTLITERAL { }
		| STRLITERAL { }
		| TRUE { }
		| FALSE { }
		| HAVOC { }
		| LPAREN exp RPAREN { }
		| callExp { }

lval		: id { }
		| id LBRACE exp RBRACE { }

id		: ID { $$ = new IDNode($1); }

%%

void crona::Parser::error(const std::string& msg){
	std::cout << msg << std::endl;
	std::cerr << "syntax error" << std::endl;
}
