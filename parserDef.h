/*
  Group Number 53
  Snehal Juneja: 2019B2A70994P
  Aryan Gautam Nadkarni: 2019B3A70406P
  Ishan Rai: 2019B3A70504P
  Shrey Bansal: 2019B3A70592P
  Karizma Khurana: 2019B4A70708P
*/
#ifndef _PARSERDEF_
#define _PARSERDEF_

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#define NUM_TERMINALS 58
#define NUM_NONTERMINALS 74
#define NUM_RULES 144

typedef struct Stack Stack;
typedef struct Stack * STACK;
typedef struct TreeNode TreeNode;
typedef struct TreeNode * TREE_NODE;
typedef struct LinkedList * List;
typedef struct Node * node;

typedef enum TERMINAL {
  INTEGER,
  REAL,
  BOOLEAN,
  OF,
  ARRAY,
  START,
  END,
  DECLARE,
  MODULE,
  DRIVER,
  PROGRAM,
  GET_VALUE,
  PRINT,
  USE,
  WITH,
  PARAMETERS,
  TAKES,
  INPUT,
  RETURNS,
  FOR,
  IN,
  SWITCH,
  CASE,
  BREAK,
  DEFAULT,
  WHILE,
  ID,
  NUM,
  RNUM,
  AND,
  OR,
  TRUE,
  FALSE,
  PLUS,
  MINUS,
  MUL,
  DIV,
  LT,
  LE,
  GE,
  GT,
  EQ,
  NE,
  DEF,
  ENDDEF,
  DRIVERDEF,
  DRIVERENDDEF,
  COLON,
  RANGEOP,
  SEMICOL,
  COMMA,
  ASSIGNOP,
  SQBO,
  SQBC,
  BO,
  BC,
  EPSILON,
  DOLLAR,
  ERROR
}
Terminal;

typedef enum NonTerminal {
  program0,
  program,
  moduleDeclarations,
  moduleDeclaration,
  otherModules,
  driverModule,
  module,
  ret,
  input_plist,
  input_plist2,
  output_plist,
  output_plist2,
  dataType,
  range_arrays,
  type,
  moduleDef,
  statements,
  statement,
  ioStmt,
  var_print,
  boolConstt,
  p1,
  simpleStmt,
  assignmentStmt,
  whichStmt,
  lvalueIDStmt,
  lvalueARRStmt,
  index_arr,
  new_index,
  sign,
  moduleReuseStmt,
  optional,
  actual_para_list,
  actual_para_list2,
  k,
  idList,
  idList2,
  expression,
  u,
  new_NT,
  var_id_num,
  unary_op,
  arithmeticOrBooleanExpr,
  arithmeticOrBooleanExpr2,
  anyTerm,
  anyTerm2,
  arithmeticExpr,
  arithmeticExpr2,
  term,
  term2,
  factor,
  n_11,
  n_10,
  element_index_with_expressions,
  arrExpr,
  arr_N4,
  arrTerm,
  arr_N5,
  arrFactor,
  op1,
  op2,
  logicalOp,
  relationalOp,
  declareStmt,
  conditionalStmt,
  caseStmts,
  caseStmts2,
  value,
  defaultt,
  iterativeStmt,
  range_for_loop,
  index_for_loop,
  new_index_for_loop,
  sign_for_loop,
  error
}
NonTerminal;


struct Stack {
  TREE_NODE treeAddress;
  STACK next;
};

struct Node {
  Terminal terminal;
  NonTerminal nonTerminal;
  int isTerminal;
  struct Node * next;
};

struct LinkedList {
  int count;
  struct Node * head;
};

struct TreeNode {
  Terminal terminal;
  NonTerminal nonTerminal;
  TREE_NODE next;
  TREE_NODE child;
  struct tokenInfo * value; // if the value is NULL then its non-leaf, otherwise leaf 
  int isTerminal; // 1 if it is terminal 
};

#endif