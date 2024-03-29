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
	 crona::FnDeclNode *									 transFnDecl;
   crona::DeclNode *                     transDecl;
   crona::VarDeclNode *                  transVarDecl;
   crona::TypeNode *                     transType;
   crona::IDNode *                       transID;
	 crona::ExpNode*											 transExp;
	 crona::LValNode*											 transLVal;
	 crona::StrToken*										 	 transStrToken;
	 crona::AssignExpNode*								 transAssignExp;
	 std::list<crona::ExpNode*>*					 transExpList;
	 crona::CallExpNode*									 transCallExp;
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
%type <transStmtList>						stmtBody
%type <transStmtList>						stmtList
%type <transStmt>								stmt
%type <transStmtList>						fnBody
%type <transVarDecl>    				varDecl
%type <transType>       				type
%type <transID>         				id
%type <transExp>								exp
%type	<transExp>								term
%type <transLVal>								lval
%type <transAssignExp>					assignExp
%type <transCallExp>						callExp
%type <transExpList>						actualsList

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
		| fnDecl { $$ = $1; }

varDecl 	: id COLON type {
		  	size_t line = $1->line();
		  	size_t col = $1->col();
		  	$$ = new VarDeclNode(line, col, $3, $1);
		  }

type 		: INT { $$ = new IntTypeNode($1->line(), $1->col()); }
		| INT ARRAY LBRACE INTLITERAL RBRACE {
			IntTypeNode * type = new IntTypeNode($1->line(), $1->col());
			IntLitNode * size = new IntLitNode($4);
			$$ = new ArrayTypeNode($2->line(), $2->col(), type, size);
		}
		| BOOL {  $$ = new BoolTypeNode($1->line(), $1->col()); }
		| BOOL ARRAY LBRACE INTLITERAL RBRACE {
			BoolTypeNode * type = new BoolTypeNode($1->line(), $1->col());
			IntLitNode * size = new IntLitNode($4);
			$$ = new ArrayTypeNode($2->line(), $2->col(), type, size);
		}
		| BYTE { $$ = new ByteTypeNode($1->line(), $1->col()); }
		| BYTE ARRAY LBRACE INTLITERAL RBRACE {
			ByteTypeNode * type = new ByteTypeNode($1->line(), $1->col());
			IntLitNode * size = new IntLitNode($4);
			$$ = new ArrayTypeNode($2->line(), $2->col(), type, size);
		}
		| STRING { $$ = new StringTypeNode($1->line(), $1->col()); }
		| VOID { $$ = new VoidTypeNode($1->line(), $1->col()); }

fnDecl 		: id COLON type formals fnBody {
			size_t line = $1->line();
			size_t col = $1->col();
			$$ = new FnDeclNode(line, col, $3, $1, $4, $5);
		}

formals 	: LPAREN RPAREN { $$ = nullptr; }
		| LPAREN formalsList RPAREN { $$ = $2; }

formalDecl 	: id COLON type {
			size_t line = $1->line();
			size_t col = $1->col();
			$$ = new FormalDeclNode(line, col, $3, $1);
		}

formalsList	: formalDecl {
			std::list<FormalDeclNode*>* temp = new std::list<FormalDeclNode*>();
			temp->push_front($1);
			$$ = temp;
	 	}
		| formalDecl COMMA formalsList {
    	$3->push_front($1);
			$$ = $3;
		}

fnBody		: LCURLY stmtBody RCURLY { $$ = $2; }

stmtBody : /* epsilon */ { $$ = nullptr; }
				 | stmtList { $$ = $1; }

stmtList 	: stmt {
			std::list<StmtNode*>* temp = new std::list<StmtNode*>();
			temp->push_front($1);
			$$ = temp;
 		}
		| stmt stmtList {
			$2->push_front($1);
			$$ = $2;
		}

stmt		: varDecl SEMICOLON { $$ = $1; }
		| assignExp SEMICOLON { $$ = new AssignStmtNode($1);}
		| lval DASHDASH SEMICOLON {
			size_t line = $1->line();
			size_t col = $1->col();
			$$ = new PostDecStmtNode(line, col, $1);
		}
		| lval CROSSCROSS SEMICOLON {
			size_t line = $1->line();
			size_t col = $1->col();
			$$ = new PostIncStmtNode(line, col, $1);
		}
		| READ lval SEMICOLON {
			size_t line = $1->line();
			size_t col = $1->col();
			$$ = new ReadStmtNode(line, col, $2);
		}
		| WRITE exp SEMICOLON {
			size_t line = $1->line();
			size_t col = $1->col();
			$$ = new WriteStmtNode(line, col, $2);
		}
		| IF LPAREN exp RPAREN LCURLY stmtBody RCURLY {
			size_t line = $1->line();
			size_t col = $1->col();
			$$ = new IfStmtNode(line, col, $3, $6);
		}
		| IF LPAREN exp RPAREN LCURLY stmtBody RCURLY ELSE LCURLY stmtBody RCURLY {
			size_t line = $1->line();
			size_t col = $1->col();
			$$ = new IfElseStmtNode(line, col, $3, $6, $10);
		}
		| WHILE LPAREN exp RPAREN LCURLY stmtBody RCURLY {
			size_t line = $1->line();
			size_t col = $1->col();
			$$ = new WhileStmtNode(line, col, $3, $6);
		}
		| RETURN exp SEMICOLON {
			size_t line = $1->line();
			size_t col = $1->col();
			$$ = new ReturnStmtNode(line, col, $2);
		}
		| RETURN SEMICOLON {
			size_t line = $1->line();
			size_t col = $1->col();
			$$ = new ReturnStmtNode(line, col, nullptr);
		}
		| callExp SEMICOLON {$$= new CallStmtNode($1);}

exp		: assignExp { $$ = $1; }
		| exp DASH exp { $$ = new MinusExpNode($1->line(), $1->col(), $1, $3);}
		| exp CROSS exp {$$ = new PlusExpNode($1->line(), $1->col(), $1, $3); }
		| exp STAR exp { $$ = new MultExpNode($1->line(), $1->col(), $1, $3); }
		| exp SLASH exp { $$ = new DivExpNode($1->line(), $1->col(), $1, $3); }
		| exp AND exp { $$ = new AndExpNode($1->line(), $1->col(), $1, $3);}
		| exp OR exp { $$ = new OrExpNode($1->line(), $1->col(), $1, $3); }
		| exp EQUALS exp { $$ = new EqualsExpNode($1->line(), $1->col(), $1, $3); }
		| exp NOTEQUALS exp { $$ = new NotEqualsExpNode($1->line(), $1->col(), $1, $3);}
		| exp GREATER exp { $$ = new GreaterExpNode($1->line(), $1->col(), $1, $3);}
		| exp GREATEREQ exp { $$ = new GreaterEqExpNode($1->line(), $1->col(), $1, $3);}
		| exp LESS exp { $$ = new LessExpNode($1->line(), $1->col(), $1, $3);}
		| exp LESSEQ exp { $$ = new LessEqExpNode($1->line(), $1->col(), $1, $3);}
		| NOT exp { $$ = new NotNode($1->line(), $1->col(), $2);}
		| DASH term { $$ = new NegNode($1->line(), $1->col(), $2);}
		| term { $$ = $1; }

assignExp	: lval ASSIGN exp { $$ = new AssignExpNode($1->line(), $1->col(), $1, $3);}

callExp		: id LPAREN RPAREN { $$ = new CallExpNode($1->line(), $1->col(), $1, nullptr);}
		| id LPAREN actualsList RPAREN { $$ = new CallExpNode($1->line(), $1->col(), $1, $3);}

actualsList	: exp { std::list<ExpNode*>* temp = new std::list<ExpNode*>;
										temp->push_front($1);
										$$ = temp;
									}
		| exp COMMA actualsList {
															$3->push_front($1);
															$$ = $3;
														}

term 		: lval { $$ = $1;}
		| INTLITERAL { $$ = new IntLitNode($1);}
		| STRLITERAL { $$ = new StrLitNode($1);}
		| TRUE { $$ = new TrueNode($1->line(), $1->col());}
		| FALSE { $$ = new FalseNode($1->line(), $1->col());}
		| HAVOC { $$ = new HavocNode($1->line(), $1->col());}
		| LPAREN exp RPAREN { $$ = $2; }
		| callExp { $$ = $1;}

lval		: id { $$ = $1; }
		| id LBRACE exp RBRACE { $$ = new IndexNode($1, $3); }

id		: ID { $$ = new IDNode($1); }

%%

void crona::Parser::error(const std::string& msg){
	std::cout << msg << std::endl;
	std::cerr << "syntax error" << std::endl;
}
