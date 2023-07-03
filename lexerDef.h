/*
  Group Number 53
  Snehal Juneja: 2019B2A70994P
  Aryan Gautam Nadkarni: 2019B3A70406P
  Ishan Rai: 2019B3A70504P
  Shrey Bansal: 2019B3A70592P
  Karizma Khurana: 2019B4A70708P
*/
#ifndef _LEXERDEF_
#define _LEXERDEF_

// libraries
#include <stdio.h>

#include <stdlib.h>

#include <string.h>

// constants
#define M 26
#define MAX_LENGTH_OF_VAR 20


typedef enum TOKEN {
    TK_INTEGER,
    TK_REAL,
    TK_BOOLEAN,
    TK_OF,
    TK_ARRAY,
    TK_START,
    TK_END,
    TK_DECLARE,
    TK_MODULE,
    TK_DRIVER,
    TK_PROGRAM,
    TK_GET_VALUE,
    TK_PRINT,
    TK_USE,
    TK_WITH,
    TK_PARAMETERS,
    TK_TAKES,
    TK_INPUT,
    TK_RETURNS,
    TK_FOR,
    TK_IN,
    TK_SWITCH,
    TK_CASE,
    TK_BREAK,
    TK_DEFAULT,
    TK_WHILE,
    TK_ID,
    TK_NUM,
    TK_RNUM,
    TK_AND,
    TK_OR,
    TK_TRUE,
    TK_FALSE,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_LT,
    TK_LE,
    TK_GE,
    TK_GT,
    TK_EQ,
    TK_NE,
    TK_DEF,
    TK_ENDDEF,
    TK_DRIVERDEF,
    TK_DRIVERENDDEF,
    TK_COLON,
    TK_RANGEOP,
    TK_SEMICOL,
    TK_COMMA,
    TK_ASSIGNOP,
    TK_SQBO,
    TK_SQBC,
    TK_BO,
    TK_BC,
    TK_ENDOFFILE,
    TK_LEX_ERROR,
    TK_IGNORE
  }
  TOKEN;

// structures
struct TwinBuffer {
  char buf0[512];
  char buf1[512];
  int flag;
};


struct tokenInfo {
  TOKEN token;
  char lexeme[MAX_LENGTH_OF_VAR];
  unsigned int lineNo;
  void * value;
};


#endif