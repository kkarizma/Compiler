/*
  Group Number 53
  Snehal Juneja: 2019B2A70994P
  Aryan Gautam Nadkarni: 2019B3A70406P
  Ishan Rai: 2019B3A70504P
  Shrey Bansal: 2019B3A70592P
  Karizma Khurana: 2019B4A70708P
*/
#ifndef _LEXER_
#define _LEXER_

#include "lexerDef.h"

// function prototypes
void initializeLexer(FILE* fptr, int sizeOfBuffer);
void freeBuffers();
void printTokenName(TOKEN token);
void getStream(FILE * codeFile, struct TwinBuffer * twinBuffer);
// this prototype is slightly changed from the one given by Vanadana Ma'am size of the buffer is kept the same as a block of disk in most systems
int getHash(char str[]);
int nextProbe(int x);
void initializeLookup();
struct tokenInfo searchLookup(char str[]);
void removeComments(char * testCaseFile, char * cleanFile);
char readChar(struct TwinBuffer * twinBuffer);
void copyLexeme(char * lexeme, struct TwinBuffer * twinBuffer);
void retract(struct TwinBuffer * twinBuffer);
struct tokenInfo getNextToken();

#endif