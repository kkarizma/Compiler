/*
  Group Number 53
  Snehal Juneja: 2019B2A70994P
  Aryan Gautam Nadkarni: 2019B3A70406P
  Ishan Rai: 2019B3A70504P
  Shrey Bansal: 2019B3A70592P
  Karizma Khurana: 2019B4A70708P
*/
#ifndef _PARSER_
#define _PARSER_
#include "parserDef.h"
#include "lexer.h"

List createList();
TREE_NODE createTreeRoot(node root);
int isEmpty(STACK headOfStack);
STACK push(STACK headOfStack, TREE_NODE valueToBeInserted);
TREE_NODE top(STACK headOfStack);
STACK pop(STACK headOfStack);
Terminal getEnumT(char st[]);
NonTerminal getEnumNT(char st[]);
void copyFirstsFromFirsts(int toRule, NonTerminal fromNT);
void copyFirsts(int copyTo, int copyFrom);
void createFirstforNT(NonTerminal NT);
void createFirst(List temp, int k);
void createFirstSet2();
void createFirstSet();
void createParseTable();
node createNode(char st[]);
void insert(node temp, List l1);
void addRules();
void copyFollowsfromFirsts(NonTerminal copyTo, NonTerminal copyFrom);
void copyFollowsfromFollows(NonTerminal copyTo, NonTerminal copyFrom);
void createFollowSet();
STACK createReverseList(STACK head);
int checkTokenInFirstSet(NonTerminal nt, TOKEN token);
void copyTokenInfoData(struct tokenInfo copyFrom, struct tokenInfo * copyTo);
TREE_NODE parseSourceCode();
void printGrammar();
void printParseTree(TREE_NODE node, FILE* fp);
char* returnNonTerminalName(NonTerminal nt);
char* returnTerminalName(Terminal t);
void printTerminalName(Terminal t);
void printNonTerminalName(NonTerminal nt);
void createFollow(NonTerminal NT);
TREE_NODE initializeParser(FILE* fptr, int sizeOfBuffer);
void freeParser();

#endif