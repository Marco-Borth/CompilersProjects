// A Bison parser, made by GNU Bison 3.7.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "grammar.hh"


// Unqualified %code blocks.
#line 34 "crona.yy"

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

#line 63 "parser.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 5 "crona.yy"
namespace crona {
#line 137 "parser.cc"

  /// Build a parser object.
  Parser::Parser (crona::Scanner &scanner_yyarg, crona::ProgramNode** root_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      root (root_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/

  // basic_symbol.
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t)
    : Base (t)
    , value ()
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (semantic_type) v)
    : Base (t)
    , value (YY_MOVE (v))
  {}

  template <typename Base>
  Parser::symbol_kind_type
  Parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }

  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
  }

  // by_kind.
  Parser::by_kind::by_kind ()
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  Parser::by_kind::by_kind (by_kind&& that)
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  Parser::by_kind::by_kind (const by_kind& that)
    : kind_ (that.kind_)
  {}

  Parser::by_kind::by_kind (token_kind_type t)
    : kind_ (yytranslate_ (t))
  {}

  void
  Parser::by_kind::clear ()
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  Parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  Parser::symbol_kind_type
  Parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }

  Parser::symbol_kind_type
  Parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_kind_type
  Parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value))
  {
    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.kind ());
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YYUSE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;


      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // program: globals
#line 157 "crona.yy"
                  {
		  (yylhs.value.transProgram) = new ProgramNode((yystack_[0].value.transDeclList));
		  *root = (yylhs.value.transProgram);
		  }
#line 592 "parser.cc"
    break;

  case 3: // globals: globals decl
#line 163 "crona.yy"
                  { 
	  	  (yylhs.value.transDeclList) = (yystack_[1].value.transDeclList); 
	  	  DeclNode * declNode = (yystack_[0].value.transDecl);
		  (yylhs.value.transDeclList)->push_back(declNode);
	  	  }
#line 602 "parser.cc"
    break;

  case 4: // globals: %empty
#line 169 "crona.yy"
                  {
		  (yylhs.value.transDeclList) = new std::list<DeclNode * >();
		  }
#line 610 "parser.cc"
    break;

  case 5: // decl: varDecl SEMICOLON
#line 174 "crona.yy"
                  { (yylhs.value.transDecl) = (yystack_[1].value.transVarDecl); }
#line 616 "parser.cc"
    break;

  case 6: // decl: fnDecl
#line 176 "crona.yy"
                  { (yylhs.value.transDecl) = (yystack_[0].value.transFn); }
#line 622 "parser.cc"
    break;

  case 7: // varDecl: id COLON type
#line 179 "crona.yy"
                  {
		  size_t line = (yystack_[2].value.transID)->line();
		  size_t col = (yystack_[2].value.transID)->col();
		  (yylhs.value.transVarDecl) = new VarDeclNode(line, col, (yystack_[0].value.transType), (yystack_[2].value.transID));
		  }
#line 632 "parser.cc"
    break;

  case 8: // type: INT
#line 186 "crona.yy"
                  { 
		  (yylhs.value.transType) = new IntTypeNode((yystack_[0].value.transToken)->line(), (yystack_[0].value.transToken)->col());
		  }
#line 640 "parser.cc"
    break;

  case 9: // type: INT ARRAY LBRACE INTLITERAL RBRACE
#line 190 "crona.yy"
                  { 
		  auto prim = new IntTypeNode((yystack_[4].value.transToken)->line(), (yystack_[4].value.transToken)->col());
		  (yylhs.value.transType) = new ArrayTypeNode((yystack_[4].value.transToken)->line(), (yystack_[4].value.transToken)->col(), prim, (yystack_[1].value.transIntToken)->num());
		  }
#line 649 "parser.cc"
    break;

  case 10: // type: BOOL
#line 195 "crona.yy"
                  {
		  (yylhs.value.transType) = new BoolTypeNode((yystack_[0].value.transToken)->line(), (yystack_[0].value.transToken)->col());
		  }
#line 657 "parser.cc"
    break;

  case 11: // type: BOOL ARRAY LBRACE INTLITERAL RBRACE
#line 199 "crona.yy"
                  {
		  auto prim = new BoolTypeNode((yystack_[4].value.transToken)->line(), (yystack_[4].value.transToken)->col());
		  (yylhs.value.transType) = new ArrayTypeNode((yystack_[4].value.transToken)->line(), (yystack_[4].value.transToken)->col(), prim, (yystack_[1].value.transIntToken)->num());
		  }
#line 666 "parser.cc"
    break;

  case 12: // type: BYTE
#line 204 "crona.yy"
                  {
		  (yylhs.value.transType) = new ByteTypeNode((yystack_[0].value.transToken)->line(), (yystack_[0].value.transToken)->col());
		  }
#line 674 "parser.cc"
    break;

  case 13: // type: BYTE ARRAY LBRACE INTLITERAL RBRACE
#line 208 "crona.yy"
                  {
		  auto prim = new ByteTypeNode((yystack_[4].value.transToken)->line(), (yystack_[4].value.transToken)->col());
		  (yylhs.value.transType) = new ArrayTypeNode((yystack_[4].value.transToken)->line(), (yystack_[4].value.transToken)->col(), prim, (yystack_[1].value.transIntToken)->num());
		  }
#line 683 "parser.cc"
    break;

  case 14: // type: STRING
#line 213 "crona.yy"
                  {
		  auto prim = new ByteTypeNode((yystack_[0].value.transToken)->line(), (yystack_[0].value.transToken)->col());
		  (yylhs.value.transType) = new ArrayTypeNode((yystack_[0].value.transToken)->line(), (yystack_[0].value.transToken)->col(), prim, 0);
		  }
#line 692 "parser.cc"
    break;

  case 15: // type: VOID
#line 218 "crona.yy"
                  {
		  (yylhs.value.transType) = new VoidTypeNode((yystack_[0].value.transToken)->line(), (yystack_[0].value.transToken)->col());
		  }
#line 700 "parser.cc"
    break;

  case 16: // fnDecl: id COLON type formals fnBody
#line 223 "crona.yy"
                  {
		  (yylhs.value.transFn) = new FnDeclNode((yystack_[4].value.transID)->line(), (yystack_[4].value.transID)->col(), 
		    (yystack_[4].value.transID), (yystack_[2].value.transType), (yystack_[1].value.transFormals), (yystack_[0].value.transStmts));
		  }
#line 709 "parser.cc"
    break;

  case 17: // formals: LPAREN RPAREN
#line 229 "crona.yy"
                  {
		  (yylhs.value.transFormals) = new std::list<FormalDeclNode *>();
		  }
#line 717 "parser.cc"
    break;

  case 18: // formals: LPAREN formalsList RPAREN
#line 233 "crona.yy"
                  {
		  (yylhs.value.transFormals) = (yystack_[1].value.transFormals);
		  }
#line 725 "parser.cc"
    break;

  case 19: // formalsList: formalDecl
#line 239 "crona.yy"
                  {
		  (yylhs.value.transFormals) = new std::list<FormalDeclNode *>();
		  (yylhs.value.transFormals)->push_back((yystack_[0].value.transFormal));
		  }
#line 734 "parser.cc"
    break;

  case 20: // formalsList: formalDecl COMMA formalsList
#line 244 "crona.yy"
                  {
		  (yylhs.value.transFormals) = (yystack_[0].value.transFormals);
		  (yylhs.value.transFormals)->push_front((yystack_[2].value.transFormal));
		  }
#line 743 "parser.cc"
    break;

  case 21: // formalDecl: id COLON type
#line 250 "crona.yy"
                  {
		  (yylhs.value.transFormal) = new FormalDeclNode((yystack_[2].value.transID)->line(), (yystack_[2].value.transID)->col(), 
		    (yystack_[0].value.transType), (yystack_[2].value.transID));
		  }
#line 752 "parser.cc"
    break;

  case 22: // fnBody: LCURLY stmtList RCURLY
#line 256 "crona.yy"
                  {
		  (yylhs.value.transStmts) = (yystack_[1].value.transStmts);
		  }
#line 760 "parser.cc"
    break;

  case 23: // stmtList: %empty
#line 261 "crona.yy"
                  {
		  (yylhs.value.transStmts) = new std::list<StmtNode *>();
		  //$$->push_back($1);
	   	  }
#line 769 "parser.cc"
    break;

  case 24: // stmtList: stmtList stmt
#line 266 "crona.yy"
                  {
		  (yylhs.value.transStmts) = (yystack_[1].value.transStmts);
		  (yylhs.value.transStmts)->push_back((yystack_[0].value.transStmt));
	  	  }
#line 778 "parser.cc"
    break;

  case 25: // stmt: varDecl SEMICOLON
#line 272 "crona.yy"
                  {
		  (yylhs.value.transStmt) = (yystack_[1].value.transVarDecl);
		  }
#line 786 "parser.cc"
    break;

  case 26: // stmt: assignExp SEMICOLON
#line 276 "crona.yy"
                  {
		  (yylhs.value.transStmt) = new AssignStmtNode((yystack_[1].value.transAssignExp)->line(), (yystack_[1].value.transAssignExp)->col(), (yystack_[1].value.transAssignExp)); 
		  }
#line 794 "parser.cc"
    break;

  case 27: // stmt: lval DASHDASH SEMICOLON
#line 280 "crona.yy"
                  {
		  (yylhs.value.transStmt) = new PostDecStmtNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[2].value.transLVal));
		  }
#line 802 "parser.cc"
    break;

  case 28: // stmt: lval CROSSCROSS SEMICOLON
#line 284 "crona.yy"
                  {
		  (yylhs.value.transStmt) = new PostIncStmtNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[2].value.transLVal));
		  }
#line 810 "parser.cc"
    break;

  case 29: // stmt: READ lval SEMICOLON
#line 288 "crona.yy"
                  {
		  (yylhs.value.transStmt) = new ReadStmtNode((yystack_[2].value.transToken)->line(), (yystack_[2].value.transToken)->col(), (yystack_[1].value.transLVal));
		  }
#line 818 "parser.cc"
    break;

  case 30: // stmt: WRITE exp SEMICOLON
#line 292 "crona.yy"
                  {
		  (yylhs.value.transStmt) = new WriteStmtNode((yystack_[2].value.transToken)->line(), (yystack_[2].value.transToken)->col(), (yystack_[1].value.transExp));
		  }
#line 826 "parser.cc"
    break;

  case 31: // stmt: IF LPAREN exp RPAREN LCURLY stmtList RCURLY
#line 296 "crona.yy"
                  {
		  (yylhs.value.transStmt) = new IfStmtNode((yystack_[6].value.transToken)->line(), (yystack_[6].value.transToken)->col(), (yystack_[4].value.transExp), (yystack_[1].value.transStmts));
		  }
#line 834 "parser.cc"
    break;

  case 32: // stmt: IF LPAREN exp RPAREN LCURLY stmtList RCURLY ELSE LCURLY stmtList RCURLY
#line 300 "crona.yy"
                  {
		  (yylhs.value.transStmt) = new IfElseStmtNode((yystack_[10].value.transToken)->line(), (yystack_[10].value.transToken)->col(), (yystack_[8].value.transExp), 
		    (yystack_[5].value.transStmts), (yystack_[1].value.transStmts));
		  }
#line 843 "parser.cc"
    break;

  case 33: // stmt: WHILE LPAREN exp RPAREN LCURLY stmtList RCURLY
#line 305 "crona.yy"
                  {
		  (yylhs.value.transStmt) = new WhileStmtNode((yystack_[6].value.transToken)->line(), (yystack_[6].value.transToken)->col(), (yystack_[4].value.transExp), (yystack_[1].value.transStmts));
		  }
#line 851 "parser.cc"
    break;

  case 34: // stmt: RETURN exp SEMICOLON
#line 309 "crona.yy"
                  {
		  (yylhs.value.transStmt) = new ReturnStmtNode((yystack_[2].value.transToken)->line(), (yystack_[2].value.transToken)->col(), (yystack_[1].value.transExp));
		  }
#line 859 "parser.cc"
    break;

  case 35: // stmt: RETURN SEMICOLON
#line 313 "crona.yy"
                  {
		  (yylhs.value.transStmt) = new ReturnStmtNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), nullptr);
		  }
#line 867 "parser.cc"
    break;

  case 36: // stmt: callExp SEMICOLON
#line 317 "crona.yy"
                  { (yylhs.value.transStmt) = new CallStmtNode((yystack_[1].value.transCallExp)->line(), (yystack_[1].value.transCallExp)->col(), (yystack_[1].value.transCallExp)); }
#line 873 "parser.cc"
    break;

  case 37: // exp: assignExp
#line 320 "crona.yy"
                  { (yylhs.value.transExp) = (yystack_[0].value.transAssignExp); }
#line 879 "parser.cc"
    break;

  case 38: // exp: exp DASH exp
#line 322 "crona.yy"
                  {
		  (yylhs.value.transExp) = new MinusNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 887 "parser.cc"
    break;

  case 39: // exp: exp CROSS exp
#line 326 "crona.yy"
                  {
		  (yylhs.value.transExp) = new PlusNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 895 "parser.cc"
    break;

  case 40: // exp: exp STAR exp
#line 330 "crona.yy"
                  {
		  (yylhs.value.transExp) = new TimesNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 903 "parser.cc"
    break;

  case 41: // exp: exp SLASH exp
#line 334 "crona.yy"
                  {
		  (yylhs.value.transExp) = new DivideNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 911 "parser.cc"
    break;

  case 42: // exp: exp AND exp
#line 338 "crona.yy"
                  {
		  (yylhs.value.transExp) = new AndNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 919 "parser.cc"
    break;

  case 43: // exp: exp OR exp
#line 342 "crona.yy"
                  {
		  (yylhs.value.transExp) = new OrNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 927 "parser.cc"
    break;

  case 44: // exp: exp EQUALS exp
#line 346 "crona.yy"
                  {
		  (yylhs.value.transExp) = new EqualsNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 935 "parser.cc"
    break;

  case 45: // exp: exp NOTEQUALS exp
#line 350 "crona.yy"
                  {
		  (yylhs.value.transExp) = new NotEqualsNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 943 "parser.cc"
    break;

  case 46: // exp: exp GREATER exp
#line 354 "crona.yy"
                  {
		  (yylhs.value.transExp) = new GreaterNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 951 "parser.cc"
    break;

  case 47: // exp: exp GREATEREQ exp
#line 358 "crona.yy"
                  {
		  (yylhs.value.transExp) = new GreaterEqNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 959 "parser.cc"
    break;

  case 48: // exp: exp LESS exp
#line 362 "crona.yy"
                  {
		  (yylhs.value.transExp) = new LessNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 967 "parser.cc"
    break;

  case 49: // exp: exp LESSEQ exp
#line 366 "crona.yy"
                  {
		  (yylhs.value.transExp) = new LessEqNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 975 "parser.cc"
    break;

  case 50: // exp: NOT exp
#line 370 "crona.yy"
                  {
		  (yylhs.value.transExp) = new NotNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[0].value.transExp));
		  }
#line 983 "parser.cc"
    break;

  case 51: // exp: DASH term
#line 374 "crona.yy"
                  {
		  (yylhs.value.transExp) = new NegNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[0].value.transExp));
		  }
#line 991 "parser.cc"
    break;

  case 52: // exp: term
#line 378 "crona.yy"
                  { (yylhs.value.transExp) = (yystack_[0].value.transExp); }
#line 997 "parser.cc"
    break;

  case 53: // assignExp: lval ASSIGN exp
#line 381 "crona.yy"
                  {
		  (yylhs.value.transAssignExp) = new AssignExpNode((yystack_[1].value.transToken)->line(), (yystack_[1].value.transToken)->col(), (yystack_[2].value.transLVal), (yystack_[0].value.transExp));
		  }
#line 1005 "parser.cc"
    break;

  case 54: // callExp: id LPAREN RPAREN
#line 386 "crona.yy"
                  {
		  std::list<ExpNode *> * noargs =
		    new std::list<ExpNode *>();
		  (yylhs.value.transCallExp) = new CallExpNode((yystack_[2].value.transID)->line(), (yystack_[2].value.transID)->col(), (yystack_[2].value.transID), noargs);
		  }
#line 1015 "parser.cc"
    break;

  case 55: // callExp: id LPAREN actualsList RPAREN
#line 392 "crona.yy"
                  {
		  (yylhs.value.transCallExp) = new CallExpNode((yystack_[3].value.transID)->line(), (yystack_[3].value.transID)->col(), (yystack_[3].value.transID), (yystack_[1].value.transActuals));
		  }
#line 1023 "parser.cc"
    break;

  case 56: // actualsList: exp
#line 397 "crona.yy"
                  {
		  std::list<ExpNode *> * list =
		    new std::list<ExpNode *>();
		  list->push_back((yystack_[0].value.transExp));
		  (yylhs.value.transActuals) = list;
		  }
#line 1034 "parser.cc"
    break;

  case 57: // actualsList: actualsList COMMA exp
#line 404 "crona.yy"
                  {
		  (yylhs.value.transActuals) = (yystack_[2].value.transActuals);
		  (yylhs.value.transActuals)->push_back((yystack_[0].value.transExp));
		  }
#line 1043 "parser.cc"
    break;

  case 58: // term: lval
#line 410 "crona.yy"
                  { (yylhs.value.transExp) = (yystack_[0].value.transLVal); }
#line 1049 "parser.cc"
    break;

  case 59: // term: INTLITERAL
#line 412 "crona.yy"
                  { (yylhs.value.transExp) = new IntLitNode((yystack_[0].value.transIntToken)->line(), (yystack_[0].value.transIntToken)->col(), (yystack_[0].value.transIntToken)->num()); }
#line 1055 "parser.cc"
    break;

  case 60: // term: STRLITERAL
#line 414 "crona.yy"
                  { (yylhs.value.transExp) = new StrLitNode((yystack_[0].value.transStrToken)->line(), (yystack_[0].value.transStrToken)->col(), (yystack_[0].value.transStrToken)->str()); }
#line 1061 "parser.cc"
    break;

  case 61: // term: TRUE
#line 416 "crona.yy"
                  { (yylhs.value.transExp) = new TrueNode((yystack_[0].value.transToken)->line(), (yystack_[0].value.transToken)->col()); }
#line 1067 "parser.cc"
    break;

  case 62: // term: FALSE
#line 418 "crona.yy"
                  { (yylhs.value.transExp) = new FalseNode((yystack_[0].value.transToken)->line(), (yystack_[0].value.transToken)->col()); }
#line 1073 "parser.cc"
    break;

  case 63: // term: HAVOC
#line 420 "crona.yy"
                  { (yylhs.value.transExp) = new HavocNode((yystack_[0].value.transToken)->line(), (yystack_[0].value.transToken)->col()); }
#line 1079 "parser.cc"
    break;

  case 64: // term: LPAREN exp RPAREN
#line 422 "crona.yy"
                  { (yylhs.value.transExp) = (yystack_[1].value.transExp); }
#line 1085 "parser.cc"
    break;

  case 65: // term: callExp
#line 424 "crona.yy"
                  {
		  (yylhs.value.transExp) = (yystack_[0].value.transCallExp);
		  }
#line 1093 "parser.cc"
    break;

  case 66: // lval: id
#line 429 "crona.yy"
                  {
		  (yylhs.value.transLVal) = (yystack_[0].value.transID);
		  }
#line 1101 "parser.cc"
    break;

  case 67: // lval: id LBRACE exp RBRACE
#line 433 "crona.yy"
                  {
		  (yylhs.value.transLVal) = new IndexNode((yystack_[3].value.transID)->line(), (yystack_[3].value.transID)->col(), (yystack_[3].value.transID), (yystack_[1].value.transExp));
		  }
#line 1109 "parser.cc"
    break;

  case 68: // id: ID
#line 438 "crona.yy"
                  {
		  (yylhs.value.transID) = new IDNode((yystack_[0].value.transIDToken)->line(), (yystack_[0].value.transIDToken)->col(), (yystack_[0].value.transIDToken)->value()); 
		  }
#line 1117 "parser.cc"
    break;


#line 1121 "parser.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  Parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // Parser::context.
  Parser::context::context (const Parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  Parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        int yychecklim = yylast_ - yyn + 1;
        int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }



  int
  Parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char Parser::yypact_ninf_ = -69;

  const signed char Parser::yytable_ninf_ = -1;

  const short
  Parser::yypact_[] =
  {
     -69,     4,    -4,   -69,   -69,   -69,   -13,   -69,     9,   -69,
     265,    25,    30,    35,   -69,   -69,    16,    21,    23,    44,
     -11,    47,    48,    52,    53,   -69,    39,    67,    71,   -69,
     -69,    51,    56,    57,   -69,    -4,   265,    -7,   -69,   -69,
     -69,   -69,   -69,    62,   -69,    -4,   277,    63,   348,    43,
     -69,    68,    72,    -3,    38,   348,    73,    60,   375,   -69,
     -69,   -69,   348,   348,   -69,   -69,   -69,    55,   -69,   -69,
     -69,    89,    -5,   348,   104,   -69,   -69,   -69,   348,    80,
      81,   265,   348,   295,   133,   -69,   -69,   -69,   155,   -69,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     -69,   348,   348,   177,   -69,   230,   -69,   -69,   -69,   199,
     -69,   230,     6,    90,   -69,   317,    -8,    -8,   323,   323,
     323,   323,   323,   323,   252,   -69,   -69,    95,   -69,   348,
     -69,   -69,   -69,   230,    78,   340,   107,   -69,    99,   -69,
     353,   -69
  };

  const signed char
  Parser::yydefact_[] =
  {
       4,     0,     2,     1,    68,     3,     0,     6,     0,     5,
       0,    10,    12,     8,    14,    15,     7,     0,     0,     0,
       0,     0,     0,     0,     0,    17,     0,    19,     0,    23,
      16,     0,     0,     0,    18,     0,     0,     0,    11,    13,
       9,    20,    21,     0,    22,     0,     0,     0,     0,     0,
      24,     0,     0,     0,    66,     0,     0,    66,     0,    62,
      63,    59,     0,     0,    35,    60,    61,     0,    37,    65,
      52,    58,    66,     0,     0,    25,    26,    36,     0,     0,
       0,     0,     0,     0,     0,    29,    51,    58,     0,    50,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,     0,     0,    30,    53,    28,    27,     7,     0,
      54,    56,     0,     0,    64,    42,    39,    38,    44,    46,
      47,    48,    49,    45,    43,    41,    40,     0,    67,     0,
      55,    23,    23,    57,     0,     0,    31,    33,     0,    23,
       0,    32
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -69,   -69,   -69,   -69,   123,   -20,   -69,   -69,    93,   -69,
     -69,   -68,   -69,   -42,   -34,   -32,   -69,    79,   -36,    -2
  };

  const signed char
  Parser::yydefgoto_[] =
  {
      -1,     1,     2,     5,    49,    16,     7,    21,    26,    27,
      30,    37,    50,    67,    68,    69,   112,    70,    71,    72
  };

  const short
  Parser::yytable_[] =
  {
       8,    53,    78,    51,     3,    52,    74,     4,    79,    56,
      80,     4,    43,    84,     4,   129,    42,    10,    28,    82,
      88,    89,    87,    83,     9,    25,    44,    45,    46,    17,
     101,   103,   102,    28,    18,    54,   105,    47,    48,    19,
     109,   111,   130,    57,    20,    22,    81,    23,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    90,   125,
     126,   108,    82,   134,   135,    91,    83,    92,    24,    31,
      93,   140,    29,    32,    33,    34,    35,    94,    95,    36,
      75,    96,    97,    38,    82,    98,    99,   133,    39,    40,
      55,    73,   100,   101,    78,   102,     4,    43,    53,    53,
      51,    51,    52,    52,    53,    76,    51,    90,    52,    77,
      85,   136,    45,    46,    91,   131,    92,   106,   107,    93,
     132,   138,    47,    48,   139,     6,    94,    95,    41,     0,
      96,    97,    54,    54,    98,    99,    90,    86,    54,     0,
       0,   104,   101,    91,   102,    92,     0,     0,    93,     0,
       0,     0,     0,     0,     0,    94,    95,     0,    90,    96,
      97,     0,     0,    98,    99,    91,     0,    92,     0,   113,
      93,   101,     0,   102,     0,     0,     0,    94,    95,     0,
      90,    96,    97,     0,     0,    98,    99,    91,     0,    92,
       0,   114,    93,   101,     0,   102,     0,     0,     0,    94,
      95,     0,    90,    96,    97,     0,     0,    98,    99,    91,
       0,    92,     0,   127,    93,   101,     0,   102,     0,     0,
       0,    94,    95,     0,     0,    96,    97,     0,     0,    98,
      99,   128,     0,    90,     0,     0,     0,   101,     0,   102,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,    94,    95,     0,    90,    96,    97,     0,     0,
      98,    99,    91,     0,    92,     0,     0,    93,   101,     0,
     102,    11,    12,     0,    94,    95,     0,     0,    96,    97,
       0,     0,    98,     0,     0,    13,     0,     0,     0,    58,
     101,     0,   102,    59,    60,     4,     0,     0,    61,     0,
       0,     0,     0,     0,    14,    62,    63,    58,    15,     0,
       0,    59,    60,     4,    64,     0,    61,     0,    65,    66,
       0,     0,     0,    62,    63,     0,     0,    91,     0,    92,
       0,   110,    93,    91,     0,    92,    65,    66,    -1,    94,
      95,     0,     0,    96,    97,    -1,    -1,    98,     0,    -1,
      -1,     0,     0,    -1,     0,   101,     0,   102,     4,    43,
      58,   101,     0,   102,    59,    60,     4,     0,     0,    61,
       0,     4,    43,   137,    45,    46,    62,    63,     0,     0,
       0,     0,     0,     0,    47,    48,   141,    45,    46,    65,
      66,    59,    60,     4,     0,     0,    61,    47,    48,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66
  };

  const short
  Parser::yycheck_[] =
  {
       2,    37,     5,    37,     0,    37,    48,    18,    11,    45,
      13,    18,    19,    55,    18,     9,    36,     8,    20,    24,
      62,    63,    58,    28,    37,    36,    33,    34,    35,     4,
      38,    73,    40,    35,     4,    37,    78,    44,    45,     4,
      82,    83,    36,    45,    28,    24,     8,    24,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,     3,   101,
     102,    81,    24,   131,   132,    10,    28,    12,    24,    21,
      15,   139,    25,    21,    21,    36,     9,    22,    23,     8,
      37,    26,    27,    32,    24,    30,    31,   129,    32,    32,
      28,    28,    37,    38,     5,    40,    18,    19,   134,   135,
     134,   135,   134,   135,   140,    37,   140,     3,   140,    37,
      37,    33,    34,    35,    10,    25,    12,    37,    37,    15,
      25,    14,    44,    45,    25,     2,    22,    23,    35,    -1,
      26,    27,   134,   135,    30,    31,     3,    58,   140,    -1,
      -1,    37,    38,    10,    40,    12,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    23,    -1,     3,    26,
      27,    -1,    -1,    30,    31,    10,    -1,    12,    -1,    36,
      15,    38,    -1,    40,    -1,    -1,    -1,    22,    23,    -1,
       3,    26,    27,    -1,    -1,    30,    31,    10,    -1,    12,
      -1,    36,    15,    38,    -1,    40,    -1,    -1,    -1,    22,
      23,    -1,     3,    26,    27,    -1,    -1,    30,    31,    10,
      -1,    12,    -1,    36,    15,    38,    -1,    40,    -1,    -1,
      -1,    22,    23,    -1,    -1,    26,    27,    -1,    -1,    30,
      31,    32,    -1,     3,    -1,    -1,    -1,    38,    -1,    40,
      10,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    23,    -1,     3,    26,    27,    -1,    -1,
      30,    31,    10,    -1,    12,    -1,    -1,    15,    38,    -1,
      40,     6,     7,    -1,    22,    23,    -1,    -1,    26,    27,
      -1,    -1,    30,    -1,    -1,    20,    -1,    -1,    -1,    12,
      38,    -1,    40,    16,    17,    18,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    39,    28,    29,    12,    43,    -1,
      -1,    16,    17,    18,    37,    -1,    21,    -1,    41,    42,
      -1,    -1,    -1,    28,    29,    -1,    -1,    10,    -1,    12,
      -1,    36,    15,    10,    -1,    12,    41,    42,    15,    22,
      23,    -1,    -1,    26,    27,    22,    23,    30,    -1,    26,
      27,    -1,    -1,    30,    -1,    38,    -1,    40,    18,    19,
      12,    38,    -1,    40,    16,    17,    18,    -1,    -1,    21,
      -1,    18,    19,    33,    34,    35,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    45,    33,    34,    35,    41,
      42,    16,    17,    18,    -1,    -1,    21,    44,    45,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    42
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    47,    48,     0,    18,    49,    50,    52,    65,    37,
       8,     6,     7,    20,    39,    43,    51,     4,     4,     4,
      28,    53,    24,    24,    24,    36,    54,    55,    65,    25,
      56,    21,    21,    21,    36,     9,     8,    57,    32,    32,
      32,    54,    51,    19,    33,    34,    35,    44,    45,    50,
      58,    60,    61,    64,    65,    28,    64,    65,    12,    16,
      17,    21,    28,    29,    37,    41,    42,    59,    60,    61,
      63,    64,    65,    28,    59,    37,    37,    37,     5,    11,
      13,     8,    24,    28,    59,    37,    63,    64,    59,    59,
       3,    10,    12,    15,    22,    23,    26,    27,    30,    31,
      37,    38,    40,    59,    37,    59,    37,    37,    51,    59,
      36,    59,    62,    36,    36,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    36,    32,     9,
      36,    25,    25,    59,    57,    57,    33,    33,    14,    25,
      57,    33
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    46,    47,    48,    48,    49,    49,    50,    51,    51,
      51,    51,    51,    51,    51,    51,    52,    53,    53,    54,
      54,    55,    56,    57,    57,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    60,    61,    61,    62,    62,    63,    63,
      63,    63,    63,    63,    63,    63,    64,    64,    65
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     1,     2,     0,     2,     1,     3,     1,     5,
       1,     5,     1,     5,     1,     1,     5,     2,     3,     1,
       3,     3,     3,     0,     2,     2,     2,     3,     3,     3,
       3,     7,    11,     7,     3,     2,     2,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     1,     3,     3,     4,     1,     3,     1,     1,
       1,     1,     1,     1,     3,     1,     1,     4,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end file\"", "error", "\"invalid token\"", "AND", "ARRAY", "ASSIGN",
  "BOOL", "BYTE", "COLON", "COMMA", "CROSS", "CROSSCROSS", "DASH",
  "DASHDASH", "ELSE", "EQUALS", "FALSE", "HAVOC", "ID", "IF", "INT",
  "INTLITERAL", "GREATER", "GREATEREQ", "LBRACE", "LCURLY", "LESS",
  "LESSEQ", "LPAREN", "NOT", "NOTEQUALS", "OR", "RBRACE", "RCURLY", "READ",
  "RETURN", "RPAREN", "SEMICOLON", "SLASH", "STRING", "STAR", "STRLITERAL",
  "TRUE", "VOID", "WHILE", "WRITE", "$accept", "program", "globals",
  "decl", "varDecl", "type", "fnDecl", "formals", "formalsList",
  "formalDecl", "fnBody", "stmtList", "stmt", "exp", "assignExp",
  "callExp", "actualsList", "term", "lval", "id", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,   156,   156,   162,   169,   173,   175,   178,   185,   189,
     194,   198,   203,   207,   212,   217,   222,   228,   232,   238,
     243,   249,   255,   261,   265,   271,   275,   279,   283,   287,
     291,   295,   299,   304,   308,   312,   316,   319,   321,   325,
     329,   333,   337,   341,   345,   349,   353,   357,   361,   365,
     369,   373,   377,   380,   385,   391,   396,   403,   409,   411,
     413,   415,   417,   419,   421,   423,   428,   432,   437
  };

  void
  Parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  Parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  Parser::symbol_kind_type
  Parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45
    };
    // Last valid token kind.
    const int code_max = 300;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return YY_CAST (symbol_kind_type, translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 5 "crona.yy"
} // crona
#line 1777 "parser.cc"

#line 442 "crona.yy"


void crona::Parser::error(const std::string& msg){
	std::cout << msg << std::endl;
	std::cerr << "syntax error" << std::endl;
}
