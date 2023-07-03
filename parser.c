/*
  Group Number 53
  Snehal Juneja: 2019B2A70994P
  Aryan Gautam Nadkarni: 2019B3A70406P
  Ishan Rai: 2019B3A70504P
  Shrey Bansal: 2019B3A70592P
  Karizma Khurana: 2019B4A70708P
*/
#include "lexer.h"
#include "parser.h"

//globals
List array[NUM_RULES];
int firstSet[NUM_RULES][NUM_TERMINALS];
int followSet[NUM_NONTERMINALS][NUM_TERMINALS];
int traversedRuleFirst[NUM_RULES];
int traversedRuleNonTerminal[NUM_NONTERMINALS];
int rhsCountNT[NUM_NONTERMINALS][NUM_RULES];
int parseTable[NUM_NONTERMINALS][NUM_TERMINALS];
int synchSet[NUM_NONTERMINALS][NUM_TERMINALS];
int traversedNT[NUM_NONTERMINALS];
int first[NUM_NONTERMINALS][NUM_TERMINALS];
int initializedOnce = 0;

List createList() {
  List temp;
  temp = (List) malloc(sizeof(struct LinkedList));
  temp -> count = 0;
  temp -> head = NULL;
  return temp;
}

TREE_NODE createTreeRoot(node root) {
  TREE_NODE newTree = (TREE_NODE) malloc(sizeof(TreeNode));
  newTree -> isTerminal = root -> isTerminal;
  if (root -> isTerminal) {
    newTree -> terminal = root -> terminal;
  } else {
    newTree -> nonTerminal = root -> nonTerminal;
  }
  newTree -> value = NULL;
  return newTree;
}

int isEmpty(STACK headOfStack) {
  if (headOfStack == NULL) {
    return 1;
  } else {
    return 0;
  }
}

STACK push(STACK headOfStack, TREE_NODE valueToBeInserted) {
  STACK ptr = (STACK) malloc(sizeof(Stack));
  ptr -> treeAddress = valueToBeInserted;
  ptr -> next = NULL;

  if (headOfStack == NULL) {
    //  headOfStack = ptr;
  } else {
    ptr -> next = headOfStack;
  }

  return ptr;
}

TREE_NODE top(STACK headOfStack) {
  if (isEmpty(headOfStack) == 1) {
    printf("The stack is empty!\n");
    return NULL;
  } else {
    return headOfStack -> treeAddress;
  }
}

STACK pop(STACK headOfStack) {

  if (isEmpty(headOfStack) == 1) {
    printf("The stack is empty and so nothing can be popped\n");
    return NULL;
  } else {
    STACK freeThis = headOfStack;
    headOfStack = headOfStack -> next;
    free(freeThis);
    return headOfStack;
  }
}

Terminal getEnumT(char st[]) {
  if (strcmp(st, "INTEGER") == 0) {
    return INTEGER;
  } else if (strcmp(st, "REAL") == 0) {
    return REAL;
  } else if (strcmp(st, "BOOLEAN") == 0) {
    return BOOLEAN;
  } else if (strcmp(st, "OF") == 0) {
    return OF;
  } else if (strcmp(st, "ARRAY") == 0) {
    return ARRAY;
  } else if (strcmp(st, "START") == 0) {
    return START;
  } else if (strcmp(st, "END") == 0) {
    return END;
  } else if (strcmp(st, "DECLARE") == 0) {
    return DECLARE;
  } else if (strcmp(st, "MODULE") == 0) {
    return MODULE;
  } else if (strcmp(st, "DRIVER") == 0) {
    return DRIVER;
  } else if (strcmp(st, "PROGRAM") == 0) {
    return PROGRAM;
  } else if (strcmp(st, "GET_VALUE") == 0) {
    return GET_VALUE;
  } else if (strcmp(st, "PRINT") == 0) {
    return PRINT;
  } else if (strcmp(st, "USE") == 0) {
    return USE;
  } else if (strcmp(st, "WITH") == 0) {
    return WITH;
  } else if (strcmp(st, "PARAMETERS") == 0) {
    return PARAMETERS;
  } else if (strcmp(st, "TAKES") == 0) {
    return TAKES;
  } else if (strcmp(st, "INPUT") == 0) {
    return INPUT;
  } else if (strcmp(st, "RETURNS") == 0) {
    return RETURNS;
  } else if (strcmp(st, "FOR") == 0) {
    return FOR;
  } else if (strcmp(st, "IN") == 0) {
    return IN;
  } else if (strcmp(st, "SWITCH") == 0) {
    return SWITCH;
  } else if (strcmp(st, "CASE") == 0) {
    return CASE;
  } else if (strcmp(st, "BREAK") == 0) {
    return BREAK;
  } else if (strcmp(st, "DEFAULT") == 0) {
    return DEFAULT;
  } else if (strcmp(st, "WHILE") == 0) {
    return WHILE;
  } else if (strcmp(st, "ID") == 0) {
    return ID;
  } else if (strcmp(st, "NUM") == 0) {
    return NUM;
  } else if (strcmp(st, "RNUM") == 0) {
    return RNUM;
  } else if (strcmp(st, "AND") == 0) {
    return AND;
  } else if (strcmp(st, "OR") == 0) {
    return OR;
  } else if (strcmp(st, "TRUE") == 0) {
    return TRUE;
  } else if (strcmp(st, "FALSE") == 0) {
    return FALSE;
  } else if (strcmp(st, "PLUS") == 0) {
    return PLUS;
  } else if (strcmp(st, "MINUS") == 0) {
    return MINUS;
  } else if (strcmp(st, "MUL") == 0) {
    return MUL;
  } else if (strcmp(st, "DIV") == 0) {
    return DIV;
  } else if (strcmp(st, "LE") == 0) {
    return LE;
  } else if (strcmp(st, "LT") == 0) {
    return LT;
  } else if (strcmp(st, "GE") == 0) {
    return GE;
  } else if (strcmp(st, "GT") == 0) {
    return GT;
  } else if (strcmp(st, "EQ") == 0) {
    return EQ;
  } else if (strcmp(st, "NE") == 0) {
    return NE;
  } else if (strcmp(st, "DEF") == 0) {
    return DEF;
  } else if (strcmp(st, "ENDDEF") == 0) {
    return ENDDEF;
  } else if (strcmp(st, "DRIVERDEF") == 0) {
    return DRIVERDEF;
  } else if (strcmp(st, "DRIVERENDDEF") == 0) {
    return DRIVERENDDEF;
  } else if (strcmp(st, "COLON") == 0) {
    return COLON;
  } else if (strcmp(st, "RANGEOP") == 0) {
    return RANGEOP;
  } else if (strcmp(st, "SEMICOL") == 0) {
    return SEMICOL;
  } else if (strcmp(st, "COMMA") == 0) {
    return COMMA;
  } else if (strcmp(st, "ASSIGNOP") == 0) {
    return ASSIGNOP;
  } else if (strcmp(st, "SQBO") == 0) {
    return SQBO;
  } else if (strcmp(st, "SQBC") == 0) {
    return SQBC;
  } else if (strcmp(st, "BO") == 0) {
    return BO;
  } else if (strcmp(st, "BC") == 0) {
    return BC;
  } else if (strcmp(st, "EPSILON") == 0) {
    return EPSILON;
  } else if (strcmp(st, "DOLLAR") == 0) {
    return DOLLAR;
  } else {
    printf("%s", st);
    int i = 0;
    while (st[i] != '\0') {
      printf("%d ", st[i]);
      i++;
    }
    printf("%d\n", st[i]);
    printf("TERMINAL DOES NOT EXIST\n");
    return ERROR;
  }
}

NonTerminal getEnumNT(char st[]) {
  if (strcmp(st, "program") == 0) {
    return program;
  } else if (strcmp(st, "program0") == 0) {
    return program0;
  } else if (strcmp(st, "moduleDeclarations") == 0) {
    return moduleDeclarations;
  } else if (strcmp(st, "moduleDeclaration") == 0) {
    return moduleDeclaration;
  } else if (strcmp(st, "otherModules") == 0) {
    return otherModules;
  } else if (strcmp(st, "driverModule") == 0) {
    return driverModule;
  } else if (strcmp(st, "module") == 0) {
    return module;
  } else if (strcmp(st, "ret") == 0) {
    return ret;
  } else if (strcmp(st, "input_plist") == 0) {
    return input_plist;
  } else if (strcmp(st, "input_plist2") == 0) {
    return input_plist2;
  } else if (strcmp(st, "output_plist") == 0) {
    return output_plist;
  } else if (strcmp(st, "output_plist2") == 0) {
    return output_plist2;
  } else if (strcmp(st, "dataType") == 0) {
    return dataType;
  } else if (strcmp(st, "range_arrays") == 0) {
    return range_arrays;
  } else if (strcmp(st, "type") == 0) {
    return type;
  } else if (strcmp(st, "moduleDef") == 0) {
    return moduleDef;
  } else if (strcmp(st, "statements") == 0) {
    return statements;
  } else if (strcmp(st, "statement") == 0) {
    return statement;
  } else if (strcmp(st, "ioStmt") == 0) {
    return ioStmt;
  } else if (strcmp(st, "var_print") == 0) {
    return var_print;
  } else if (strcmp(st, "boolConstt") == 0) {
    return boolConstt;
  } else if (strcmp(st, "p1") == 0) {
    return p1;
  } else if (strcmp(st, "simpleStmt") == 0) {
    return simpleStmt;
  } else if (strcmp(st, "assignmentStmt") == 0) {
    return assignmentStmt;
  } else if (strcmp(st, "whichStmt") == 0) {
    return whichStmt;
  } else if (strcmp(st, "lvalueIDStmt") == 0) {
    return lvalueIDStmt;
  } else if (strcmp(st, "lvalueARRStmt") == 0) {
    return lvalueARRStmt;
  } else if (strcmp(st, "index_arr") == 0) {
    return index_arr;
  } else if (strcmp(st, "new_index") == 0) {
    return new_index;
  } else if (strcmp(st, "moduleReuseStmt") == 0) {
    return moduleReuseStmt;
  } else if (strcmp(st, "sign") == 0) {
    return sign;
  } else if (strcmp(st, "optional") == 0) {
    return optional;
  } else if (strcmp(st, "actual_para_list") == 0) {
    return actual_para_list;
  } else if (strcmp(st, "idList") == 0) {
    return idList;
  } else if (strcmp(st, "idList2") == 0) {
    return idList2;
  } else if (strcmp(st, "expression") == 0) {
    return expression;
  } else if (strcmp(st, "u") == 0) {
    return u;
  } else if (strcmp(st, "new_NT") == 0) {
    return new_NT;
  } else if (strcmp(st, "var_id_num") == 0) {
    return var_id_num;
  } else if (strcmp(st, "unary_op") == 0) {
    return unary_op;
  } else if (strcmp(st, "arithmeticOrBooleanExpr") == 0) {
    return arithmeticOrBooleanExpr;
  } else if (strcmp(st, "arithmeticOrBooleanExpr2") == 0) {
    return arithmeticOrBooleanExpr2;
  } else if (strcmp(st, "anyTerm") == 0) {
    return anyTerm;
  } else if (strcmp(st, "anyTerm2") == 0) {
    return anyTerm2;
  } else if (strcmp(st, "arithmeticExpr") == 0) {
    return arithmeticExpr;
  } else if (strcmp(st, "arithmeticExpr2") == 0) {
    return arithmeticExpr2;
  } else if (strcmp(st, "term") == 0) {
    return term;
  } else if (strcmp(st, "term2") == 0) {
    return term2;
  } else if (strcmp(st, "factor") == 0) {
    return factor;
  } else if (strcmp(st, "n_11") == 0) {
    return n_11;
  } else if (strcmp(st, "n_10") == 0) {
    return n_10;
  } else if (strcmp(st, "element_index_with_expressions") == 0) {
    return element_index_with_expressions;
  } else if (strcmp(st, "arrExpr") == 0) {
    return arrExpr;
  } else if (strcmp(st, "arr_N4") == 0) {
    return arr_N4;
  } else if (strcmp(st, "arrTerm") == 0) {
    return arrTerm;
  } else if (strcmp(st, "arr_N5") == 0) {
    return arr_N5;
  } else if (strcmp(st, "arrFactor") == 0) {
    return arrFactor;
  } else if (strcmp(st, "op1") == 0) {
    return op1;
  } else if (strcmp(st, "op2") == 0) {
    return op2;
  } else if (strcmp(st, "logicalOp") == 0) {
    return logicalOp;
  } else if (strcmp(st, "relationalOp") == 0) {
    return relationalOp;
  } else if (strcmp(st, "declareStmt") == 0) {
    return declareStmt;
  } else if (strcmp(st, "conditionalStmt") == 0) {
    return conditionalStmt;
  } else if (strcmp(st, "caseStmts") == 0) {
    return caseStmts;
  } else if (strcmp(st, "caseStmts2") == 0) {
    return caseStmts2;
  } else if (strcmp(st, "value") == 0) {
    return value;
  } else if (strcmp(st, "default") == 0) {
    return defaultt;
  } else if (strcmp(st, "iterativeStmt") == 0) {
    return iterativeStmt;
  } else if (strcmp(st, "range_for_loop") == 0) {
    return range_for_loop;
  } else if (strcmp(st, "index_for_loop") == 0) {
    return index_for_loop;
  } else if (strcmp(st, "new_index_for_loop") == 0) {
    return new_index_for_loop;
  } else if (strcmp(st, "sign_for_loop") == 0) {
    return sign_for_loop;
  } else if (strcmp(st, "actual_para_list2") == 0) {
    return actual_para_list2;
  } else if (strcmp(st, "k") == 0) {
    return k;
  } else {
    printf("%s", st);
    int i = 0;
    while (st[i] != '\0') {
      printf("%d ", st[i]);
      i++;
    }
    printf("NON TERMINAL DOES NOT EXIST\n");
    return error;
  }
}


void copyFirstsFromFirsts(int toRule, NonTerminal fromNT){
    for(int i = 0; i<NUM_TERMINALS; i++){
        if(first[fromNT][i] == 1)   firstSet[toRule][i] = 1;
    }
}

void copyFirsts(int copyTo, int copyFrom){
  
  for(int i = 0; i<NUM_TERMINALS; i++){

    if(first[copyFrom][i] == 1)  first[copyTo][i] = 1; 
    
  }
}
  

void createFirstforNT(NonTerminal NT){
    //printNonTerminalName(NT);
    traversedNT[NT] = 1;
    for(int i = 0; i<NUM_RULES; i++){
        if(array[i]->head->nonTerminal == NT){
            node a = array[i]->head->next;
            if(a->isTerminal){
                first[NT][a->terminal]= 1;
                //printTerminalName(a->terminal);
            }else{
                // a is a non terminal
                if(!traversedNT[a->nonTerminal]){
                    createFirstforNT(a->nonTerminal);
                }
                copyFirsts(NT, a->nonTerminal);
                node b = a->next;
                while(first[NT][EPSILON] == 1 && b!= NULL)
                {
                  //printf("yeeehaw  3");
                    first[NT][EPSILON] = 0;
                    if(b->isTerminal){
                        //printTerminalName(b->terminal);
                        first[NT][b->terminal] = 1;
                        //printf("yeeehaw  1");
                        break;
                        
                    }else
                {if(!traversedNT[b->nonTerminal]){
                    createFirstforNT(b->nonTerminal);
                }
                copyFirsts(NT, b->nonTerminal);
                b = b->next;}
                //printf("yeeehaw  2");
                }
                
                
            }
        }
    }
}
void createFirst(List temp, int k) {
    node a = temp->head->next;
  traversedRuleFirst[k] = 1;
  if (a -> isTerminal) {
    firstSet[k][a -> terminal] = 1;

  }
   else {
    copyFirstsFromFirsts(k, a->nonTerminal);
    node b = a->next;
    while(firstSet[k][EPSILON] == 1 && b!=NULL){
        firstSet[k][EPSILON] = 0;
        if(b->isTerminal){
            firstSet[k][b->terminal] = 1;
            return;
        }else{
            copyFirstsFromFirsts(k, b->nonTerminal);
        }
        b = b->next;
    }
  }

}


void createFirstSet2(){
    for(int i=0;i<NUM_NONTERMINALS;i++) {
    for(int j=0;j<NUM_TERMINALS;j++){
       first[i][j]=0;
     }
  }

  for(int i=0;i<NUM_NONTERMINALS;i++){
   //printf("\n----------------");
    
    
     if(!traversedNT[i])  {
        //printf("BEGIN -- ");
        //printNonTerminalName(i);
        createFirstforNT(i);
        }     
  }
}

void createFirstSet()
{
    createFirstSet2();
  for(int i=0;i<NUM_RULES;i++) {
    for(int j=0;j<NUM_TERMINALS;j++){
       firstSet[i][j]=0;
     }
  }
  
  for(int i=0;i<NUM_RULES;i++)
  { 
    //printf("\n%d--", i+1);
     if(!traversedRuleFirst[i])  createFirst(array[i], i);   
  }
}


void createParseTable() {
  for (int i = 0; i < NUM_NONTERMINALS; i++) {
    for (int j = 0; j < NUM_TERMINALS; j++)
      parseTable[i][j] = -1;
  }

  for (int i = 0; i < NUM_RULES; i++) {
    for (int j = 0; j < NUM_TERMINALS; j++) {
      if (firstSet[i][j] == 1) {
        int ind = array[i] -> head -> nonTerminal;
        
          //printNonTerminalName(array[i]->head->nonTerminal);
        
        parseTable[ind][j] = i;
        if (j == EPSILON) {
          //parseTable[ind][j] = -1;
          for (int k = 0; k < NUM_TERMINALS; k++) {
            if (followSet[ind][k] == 1)
              parseTable[ind][k] = i;
          }
        }
        //parseTable[ind][EPSILON] = -1;
      }
    }
  }

  for (int i = 0; i < NUM_NONTERMINALS; i++) {
    for (int j = 0; j < NUM_TERMINALS; j++) {
      if (parseTable[i][j] == -1 && followSet[i][j] == 1) {
        parseTable[i][j] = -2;
      }
    }
  }

}

node createNode(char st[]) {
  //printf("%s\n", st);
  node temp;
  temp = (node) malloc(sizeof(struct Node));
  if (st[0] == '<') {
    //NT
    int i = 1;
    char * str = malloc(sizeof(char) * 100);
    while (st[i] != '>') {
      str[i - 1] = st[i];
      i++;
    }
    str[i - 1] = '\0';
    //printf("%s\n", str);
    NonTerminal nt = getEnumNT(str);
    free(str);
    temp -> isTerminal = 0;
    temp -> nonTerminal = nt;
  } else {
    char * str = malloc(sizeof(char) * 100);
    int i = 0;
    while (st[i] != '\0') {
      str[i] = st[i];
      i++;
    }
    if (str[i - 1] == '\n') {
      str[i - 1] = '\0';
    }
    str[i] = '\0';
    Terminal t = getEnumT(str);
    free(str);
    temp -> isTerminal = 1;
    temp -> terminal = t;
  }
  temp -> next = NULL;
  return temp;
}

void insert(node temp, List l1) {
  if (l1 -> count == 0) {
    l1 -> head = temp;
    temp -> next = NULL;
    l1 -> count++;
  } else {
    node temp2 = l1 -> head;
    while (temp2 -> next != NULL) {
      temp2 = temp2 -> next;
    }
    temp -> next = NULL;
    temp2 -> next = temp;
    l1 -> count++;
  }
}

void addRules() {
  FILE * ptr = NULL;
  char ch;
  ptr = fopen("grammar.txt", "r");
  if (NULL == ptr) {
    printf("Error in opening file\n");
    return;
  }

  char temp[512];
  int i = 0;
  memset(temp, '\0', 512);
  array[i] = createList();
  while (fgets(temp, 512, ptr) != NULL) {
    int j = 0;
    char currString[100];
    int index = 0;
    while (temp[j] != '\0') {
      if (temp[j] == ' ') {
        //printf("%s\n",currString);
        if (currString[0] == '<' || (currString[0] >= 'A' && currString[0] <= 'Z')) {
          node b = createNode(currString);
          insert(b, array[i]);
          if (!b -> isTerminal && (array[i] -> count) > 1) {
            //printf("ack %d %d", b->nonTerminal, i);
            rhsCountNT[b -> nonTerminal][i] += 1;
          }

        }

        memset(currString, '\0', 100);
        index = 0;
        j++;
      } else {
        currString[index] = temp[j];
        index++;
        j++;
      }
    }
    //printf("%s\n",currString);
    if (currString[0] == '<' || (currString[0] >= 'A' && currString[0] <= 'Z')) {
      node b = createNode(currString);
      insert(b, array[i]);

      if (!b -> isTerminal && (array[i] -> count) > 1) {
        // printf("ack %d %d", b->nonTerminal, i);
        rhsCountNT[b -> nonTerminal][i] += 1;
      }

    }
    memset(currString, '\0', 100);
    index = 0;
    j++;
    memset(temp, '\0', 512);
    i++;
    array[i] = createList();
  }
  //fclose(ptr);

}


void copyFollowsfromFirsts(NonTerminal copyTo, NonTerminal copyFrom) {
  for (int i = 0; i < NUM_RULES; i++) {
    if (array[i] -> head -> nonTerminal == copyFrom) {
      for (int j = 0; j < NUM_TERMINALS; j++) {
        if (firstSet[i][j] == 1) followSet[copyTo][j] = 1;
      }
    }
  }
}


void copyFollowsfromFollows(NonTerminal copyTo, NonTerminal copyFrom) {

  for (int j = 0; j < NUM_TERMINALS; j++) {
    if (followSet[copyFrom][j] == 1) followSet[copyTo][j] = 1;
  }

}

void createFollowSet() {


  for (int i = 0; i < NUM_NONTERMINALS; i++) {
    //printf("\n%d--", i+1);
    //printNonTerminalName(i);
    if (!traversedRuleNonTerminal[i]){
      createFollow(i);
      
    } 
        
  }

}


STACK createReverseList(STACK head) {
  STACK prev = NULL;
  STACK currNext;
  while (head != NULL) {
    currNext = head -> next;
    head -> next = prev;
    prev = head;
    head = currNext;
  }
  return prev;
}

int checkTokenInFirstSet(NonTerminal nt, TOKEN token) {
  for (int i = 0; i < NUM_RULES; i++) {
    if (array[i] -> head -> nonTerminal == nt) {
      if (firstSet[i][token] == 1) {
        return 1;
      }
    }
  }
  return 0;
}

void copyTokenInfoData(struct tokenInfo copyFrom, struct tokenInfo * copyTo) {
  copyTo -> token = copyFrom.token;
  int i = 0;
  while (i < MAX_LENGTH_OF_VAR) {
    copyTo -> lexeme[i] = copyFrom.lexeme[i];
    i++;
  }
  copyTo -> lineNo = copyFrom.lineNo;
  copyTo -> value = copyFrom.value;
  return;
}

TREE_NODE parseSourceCode() {
  STACK headOfStack = NULL;
  TREE_NODE treeRoot = createTreeRoot(array[0] -> head);
  STACK st = push(headOfStack, treeRoot); //pushed dollar on stack
  TREE_NODE treeTemp = treeRoot;
  struct tokenInfo token = getNextToken();
  int errorFlag = 0;
  while (token.token == TK_IGNORE) {
    token = getNextToken();
  }
  int lexErrorNumber = 0;
  while (token.token != TK_ENDOFFILE && !isEmpty(st)) {
    //printf("%d\n", lexErrorNumber);
    if(token.token==TK_ENDOFFILE){
      return treeRoot;
    }
    if(lexErrorNumber>=9){
      printf("Too many lexical errors to be handled. Exiting program!\n");
      return treeRoot;
    }
    if(token.token==TK_LEX_ERROR){
      printf("Line %d: Lexical error\n",token.lineNo);
      //printf("Lexical error in Line %d\n",token.lineNo);
      token = getNextToken();
      lexErrorNumber++;
      errorFlag = 1;
      continue;
    }
    // printf("\n");
    // if (token.token != TK_IGNORE && token.token != TK_NUM && token.token != TK_RNUM) {
    //   printf("Parsing %s %d ", token.lexeme, token.lineNo);
    //   printTokenName(token.token);
    //   printf("\n");
    // } else if (token.token == TK_NUM) {
    //   printf("Parsing %d %d ", *(int * ) token.value, token.lineNo);
    //   printTokenName(token.token);
    //   printf("\n");
    // } else if (token.token == TK_RNUM) {
    //   printf("Parsing %f %d ", *(float * ) token.value, token.lineNo);
    //   printTokenName(token.token);
    //   printf("\n");
    // }
    TREE_NODE tp = top(st);
    if (tp -> isTerminal == 1) {
      // printf("Terminal ");
      // printTerminalName(tp -> terminal);
      // printf("\n");
      if (tp -> terminal == token.token) {
        //create leaf node
        treeTemp -> value = malloc(sizeof(struct tokenInfo));
        copyTokenInfoData(token, treeTemp -> value);
        // printf("Successfully matched token!!!!!!\n");
        // if (token.token != TK_IGNORE && token.token != TK_NUM && token.token != TK_RNUM) {
        //   printf("Done %s %d %d\n", token.lexeme, token.lineNo, token.token);
        // } else if (token.token == TK_NUM) {
        //   printf("Done %d %d %d\n", *(int * ) token.value, token.lineNo, token.token);
        // } else if (token.token == TK_RNUM) {
        //   printf("Done %f %d %d\n", *(float * ) token.value, token.lineNo, token.token);
        // }
        st = pop(st); //pop matching terminal

        token = getNextToken();
        while (token.token == TK_IGNORE) {
          token = getNextToken();
        }
      } else {

        if (tp -> terminal == EPSILON) {
          //do nothing just pop and get next rule
        } else {
          //error 1
          printf("Line %d: Terminal doesn't match. Expected terminal is ", token.lineNo);
          printTerminalName(tp->terminal);
          printf("\n");
          //should return from here?
          errorFlag = 1;
        }
        st = pop(st);
      }
    } else {
      //top is non terminal
      // printf("Non Terminal ");
      // printNonTerminalName(tp -> nonTerminal);
      // printf("\n");
      if (parseTable[tp -> nonTerminal][token.token] != -1 && parseTable[tp -> nonTerminal][token.token] != -2) {
        //entry present
        //create non leaf node of tree
        st = pop(st);
        int ruleNum = parseTable[tp -> nonTerminal][token.token];
        //printf("Rule to push %d\n", ruleNum);
        node lhs = array[ruleNum] -> head;
        node rhs = lhs -> next;
        node tempHead = rhs;
        // if (lhs -> isTerminal) {
        //   printf("LHS T-%d ", lhs -> terminal);
        // } else {
        //   printf("LHS NT-%d ", lhs -> nonTerminal);
        // }
        // if (rhs -> isTerminal) {
        //   printf("RHS T-%d ", rhs -> terminal);
        // } else {
        //   printf("RHS NT-%d ", rhs -> nonTerminal);
        // }

        STACK stNode = malloc(sizeof(Stack));

        TREE_NODE treeNode = createTreeRoot(rhs);
        tp -> child = treeNode;
        // if (treeNode -> isTerminal) {
        //   printf("Treenode data %d\n", treeNode -> terminal);
        // } else {
        //   printf("Treenode data %d\n", treeNode -> nonTerminal);
        // }

        stNode -> treeAddress = treeNode;
        STACK dummyHead = stNode;
        rhs = rhs -> next;
        //printf("Creating list\n");
        while (rhs != NULL) {
          // if (rhs -> isTerminal) {
          //   printf("Terminal ");
          //   printTerminalName(rhs -> terminal);
          //   printf("\n");
          // } else {
          //   printf("Non Terminal ");
          //   printNonTerminalName(rhs -> nonTerminal);
          //   //printf("\n");
          // }
          TREE_NODE loopTreeNode = createTreeRoot(rhs);
          STACK loopStackNode = malloc(sizeof(Stack));
          loopStackNode -> treeAddress = loopTreeNode;
          stNode -> next = loopStackNode;
          stNode = stNode -> next;
          treeNode -> next = loopTreeNode;
          treeNode = treeNode -> next;
          rhs = rhs -> next;
        }
        //printf("Printing stack list\n");
        stNode = dummyHead;
        // while (stNode != NULL) {
        //   if (stNode -> treeAddress -> isTerminal) {
        //     printf("Terminal ");
        //     printTerminalName(stNode -> treeAddress -> terminal);
        //     printf("\n");
        //   } else {
        //     printf("Non Terminal ");
        //     printNonTerminalName(stNode -> treeAddress -> nonTerminal);
        //     //printf("\n");
        //     //printf("Non Terminal %d\n", stNode -> treeAddress -> nonTerminal);
        //   }
        //   stNode = stNode -> next;
        // }

        stNode = dummyHead;
        stNode = createReverseList(stNode);
        dummyHead = stNode; //copy head of reversed list now
        //printf("Printing reverse stack list\n");
        //stNode = dummyHead;
        // while (stNode != NULL) {
        //   if (stNode -> treeAddress -> isTerminal) {
        //     printf("Terminal ");
        //     printTerminalName(stNode -> treeAddress -> terminal);
        //     printf("\n");
        //   } else {
        //     printf("Non Terminal ");
        //     printNonTerminalName(stNode -> treeAddress -> nonTerminal);
        //     //printf("\n");
        //   }
        //   stNode = stNode -> next;
        // }

        stNode = dummyHead;
        while (stNode != NULL) {
          st = push(st, stNode -> treeAddress);
          stNode = stNode -> next;
        }

        //freeing nodes
        // stNode = dummyHead;
        // while (stNode != NULL) {
        //   STACK temp = stNode;
        //   stNode = stNode -> next;
        //   free(temp);
        // }

        //break;
      } else if (parseTable[tp -> nonTerminal][token.token] == -2) {
        //synch set exists
        printf("Line %d: Non Terminal not matching. Using Synch set\n", token.lineNo);
        st = pop(st);
        errorFlag = 1;
      } else {
        //error e2
        printf("Line %d: No entry in parse table for token ", token.lineNo);
        printTokenName(token.token);
        printf("\n");
        errorFlag = 1;
        token = getNextToken();
        while (token.token == TK_IGNORE) {
          token = getNextToken();
        }
      }
    }
  }

  if (isEmpty(st) == 1) {
    //error 4
    printf("Line %d: Error in parsing. Stack empty\n",token.lineNo);
    // printf("Error in parsing. Stack empty\n");
    // printf("Error in line %d\n", token.lineNo);
    return treeRoot;
  }

  TREE_NODE tp = top(st);
  if (tp -> isTerminal == 1 && tp -> terminal == DOLLAR && token.token == TK_ENDOFFILE && errorFlag==0) {
    //printf("%d\n",errorFlag);
    printf("Input sourcecode is syntactically correct\n");
    return treeRoot;
  }

  if (token.token == TK_ENDOFFILE) {
    //reached end of file
    //check stack contents
    if (tp -> isTerminal == 0) {
      for (int i = 0; i < NUM_RULES; i++) {
        if (array[i] -> head -> nonTerminal == tp -> nonTerminal) {
          if (firstSet[i][EPSILON] == 1) {
            st = pop(st);
          }
        }
      }
      while ((tp = top(st)) && !(tp -> isTerminal && tp -> terminal == DOLLAR)) {
        for (int i = 0; i < NUM_RULES; i++) {
          if (array[i] -> head -> nonTerminal == tp -> nonTerminal) {
            if (firstSet[i][EPSILON] == 1) {
              st = pop(st);
            }
          }
        }
      }
      tp = top(st);
      if (tp -> isTerminal && tp -> terminal == DOLLAR && errorFlag==0) {
        printf("Input sourcecode is syntactically correct\n");
        return treeRoot;
      } else {
        printf("Line %d: Error in parsing. Stack not empty\n",token.lineNo);
        // printf("Error in parsing. Stack not empty\n");
        // printf("Error in line %d\n", token.lineNo);
        return treeRoot;
      }
    }
  }

  if (isEmpty(st)) {
    printf("Line %d: Error in parsing. Stack empty but input not completely parsed\n",token.lineNo);
    // printf("Error in paring. Stack empty but input not completely parsed\n");
    // printf("Error in line %d\n", token.lineNo);
    return treeRoot;
  }

  return treeRoot;
}

void printGrammar() {
  for (int i = 0; i < NUM_RULES; i++) {
    printf("Rule-%d\t", i);
    List temp = array[i];
    node temp2 = temp -> head;
    while (temp2 != NULL) {
      if (temp2 -> isTerminal) {
        printf("TERMINAL-");
        printf("%d ", temp2 -> terminal);
      } else {
        printf("NON TERMINAL-");
        printf("%d ", temp2 -> nonTerminal);
      }
      temp2 = temp2 -> next;

    }
    printf("\n");
  }
}

char* returnTerminalName(Terminal t) {
  switch (t) {
  case INTEGER:
    return "INTEGER";
    break;
  case REAL:
    return "REAL";
    break;
  case BOOLEAN:
    return "BOOLEAN";
    break;
  case OF:
    return "OF";
    break;
  case ARRAY:
    return "ARRAY";
    break;
  case START:
    return "START";
    break;
  case END:
    return "END";
    break;
  case DECLARE:
    return "DECLARE";
    break;
  case MODULE:
    return "MODULE";
    break;
  case DRIVER:
    return "DRIVER";
    break;
  case PROGRAM:
    return "PROGRAM";
    break;
  case GET_VALUE:
    return "GET_VALUE";
    break;
  case PRINT:
    return "PRINT";
    break;
  case USE:
    return "USE";
    break;
  case WITH:
    return "WITH";
    break;
  case PARAMETERS:
    return "PARAMETERS";
    break;
  case TAKES:
    return "TAKES";
    break;
  case INPUT:
    return "INPUT";
    break;
  case RETURNS:
    return "RETURNS";
    break;
  case FOR:
    return "FOR";
    break;
  case IN:
    return "IN";
    break;
  case SWITCH:
    return "SWITCH";
    break;
  case CASE:
    return "CASE";
    break;
  case BREAK:
    return "BREAK";
    break;
  case DEFAULT:
    return "DEFAULT";
    break;
  case WHILE:
    return "WHILE";
    break;
  case ID:
    return "ID";
    break;
  case NUM:
    return "NUM";
    break;
  case RNUM:
    return "RNUM";
    break;
  case AND:
    return "AND";
    break;
  case OR:
    return "OR";
    break;
  case TRUE:
    return "TRUE";
    break;
  case FALSE:
    return "FALSE";
    break;
  case PLUS:
    return "PLUS";
    break;
  case MINUS:
    return "MINUS";
    break;
  case MUL:
    return "MUL";
    break;
  case DIV:
    return "DIV";
    break;
  case LT:
    return "LT";
    break;
  case LE:
    return "LE";
    break;
  case GE:
    return "GE";
    break;
  case GT:
    return "GT";
    break;
  case EQ:
    return "EQ";
    break;
  case NE:
    return "NE";
    break;
  case DEF:
    return "DEF";
    break;
  case ENDDEF:
    return "ENDDEF";
    break;
  case DRIVERDEF:
    return "DRIVERDEF";
    break;
  case DRIVERENDDEF:
    return "DRIVERENDDEF";
    break;
  case COLON:
    return "COLON";
    break;
  case RANGEOP:
    return "RANGEOP";
    break;
  case SEMICOL:
    return "SEMICOL";
    break;
  case COMMA:
    return "COMMA";
    break;
  case ASSIGNOP:
    return "ASSIGNOP";
    break;
  case SQBO:
    return "SQBO";
    break;
  case SQBC:
    return "SQBC";
    break;
  case BO:
    return "BO";
    break;
  case BC:
    return "BC";
    break;
  case EPSILON:
    return "EPSILON";
    break;
  case DOLLAR:
    return "DOLLAR";
    break;
  case ERROR:
    return "ERROR";
    break;
  }
}

char* returnNonTerminalName(NonTerminal nt){
  switch (nt) {
    case program0:
      return "program0";
      break;
    case program:
      return"program";
      break;
    case moduleDeclarations:
      return "moduleDeclarations";
      break;
    case moduleDeclaration:
      return "moduleDeclaration";
      break;
    case otherModules:
      return "otherModules";
      break;
    case driverModule:
      return "driverModule";
      break;
    case module:
      return "module";
      break;
    case ret:
      return "ret";
      break;
    case input_plist:
      return "input_plist";
      break;
    case input_plist2:
      return "input_plist2";
      break;
    case output_plist:
      return "output_plist";
      break;
    case output_plist2:
      return "output_plist2";
      break;
    case dataType:
      return "dataType";
      break;
    case range_arrays:
      return "range_arrays";
      break;
    case type:
      return "type";
      break;
    case moduleDef:
      return "moduleDef";
      break;
    case statements:
      return "statements";
      break;
    case statement:
      return "statement";
      break;
    case ioStmt:
      return "ioStmt";
      break;
    case var_print:
      return "var_print";
      break;
    case boolConstt:
      return "boolConstt";
      break;
    case p1:
      return "p1";
      break;
    case simpleStmt:
      return "simpleStmt";
      break;
    case assignmentStmt:
      return "assignmentStmt";
      break;
    case whichStmt:
      return "whichStmt";
      break;
    case lvalueIDStmt:
      return "lvalueIDStmt";
      break;
    case lvalueARRStmt:
      return "lvalueARRStmt";
      break;
    case index_arr:
      return "index_arr";
      break;
    case new_index:
      return "new_index";
      break;
    case sign:
      return "sign";
      break;
    case moduleReuseStmt:
      return "moduleReuseStmt";
      break;
    case optional:
      return "optional";
      break;
    case actual_para_list:
       return "actual_para_list";
       break;
    case actual_para_list2:
       return "actual_para_list2";
       break;
    case k:
       return "k";
       break;
    case idList:
       return "idList";
       break;
    case idList2:
       return "idList2";
       break;
    case expression:
       return "expression";
       break;
    case u: 
       return "u";
       break;
    case new_NT: 
       return "new_NT";
       break;
    case var_id_num:
       return "var_id_num";
       break;
    case unary_op:  
       return "unary_op";
       break;
    case arithmeticOrBooleanExpr:
       return "arithmeticOrBooleanExpr";
       break;
    case arithmeticOrBooleanExpr2:
       return "arithmeticOrBooleanExpr2";
       break;
    case anyTerm:  
       return "anyTerm";
       break;
    case anyTerm2:  
       return "anyTerm2";
       break;
    case arithmeticExpr:
       return "arithmeticExpr";
       break;
    case arithmeticExpr2:  
       return "arithmeticExpr2";
       break;
    case term:  
       return "term";
       break;
    case term2:  
       return "term2";
       break;
    case factor:  
       return "factor";
       break;
    case n_11:
      return "n_11";
      break;
    case n_10:
      return "n_10";
      break;
    case element_index_with_expressions:
      return "element_index_with_expressions";
      break;
    case arrExpr:  
      return "arrExpr";
      break;
    case arr_N4:  
      return "arr_N4";
      break;
    case arrTerm:  
      return "arrTerm";
      break;
    case arr_N5:  
      return "arr_N5";  
      break;
    case arrFactor:  
      return "arrFactor";
      break;
    case op1:  
      return "op1";
      break;
    case op2:  
      return "op2";
      break;  
    case logicalOp:  
      return "logicalOp";
      break;
    case relationalOp:  
      return "relationalOp";
      break;
    case declareStmt:  
      return "declareStmt";
      break;
    case conditionalStmt:  
      return "conditionalStmt";
      break;
    case caseStmts:  
      return "caseStmts";
      break;
    case caseStmts2:  
      return "caseStmts2";
      break;
    case value:  
      return "value";
      break;
    case defaultt:  
      return "defaultt";
      break;
    case iterativeStmt:  
      return "iterativeStmt";
      break;
    case range_for_loop:  
      return "range_for_loop";
      break;
    case index_for_loop:  
      return "new_index_for_loop";
      break;
    case sign_for_loop:  
      return "sign_for_loop";
      break;
    case error:
      return "error";
      break;
  }
}

void printParseTree(TREE_NODE node, FILE* fptr) {
  if (node == NULL)
    return;
  if (node -> child)
    printParseTree(node -> child,fptr);
  if (node -> isTerminal)
    // printf("%s\n", printTerminalName(node -> terminal));
    {
      char* str1 = returnTerminalName(node->terminal);
      fprintf(fptr, "%s\n", str1);
    }else{
      char* str1 = returnNonTerminalName(node->nonTerminal);
      fprintf(fptr, "%s\n",str1);
    }
    

  printParseTree(node -> next,fptr);
}

void printTerminalName(Terminal t) {
  switch (t) {
  case INTEGER:
    printf("INTEGER");
    break;
  case REAL:
    printf("REAL");
    break;
  case BOOLEAN:
    printf("BOOLEAN");
    break;
  case OF:
    printf("OF");
    break;
  case ARRAY:
    printf("ARRAY");
    break;
  case START:
    printf("START");
    break;
  case END:
    printf("END");
    break;
  case DECLARE:
    printf("DECLARE");
    break;
  case MODULE:
    printf("MODULE");
    break;
  case DRIVER:
    printf("DRIVER");
    break;
  case PROGRAM:
    printf("PROGRAM");
    break;
  case GET_VALUE:
    printf("GET_VALUE");
    break;
  case PRINT:
    printf("PRINT");
    break;
  case USE:
    printf("USE");
    break;
  case WITH:
    printf("WITH");
    break;
  case PARAMETERS:
    printf("PARAMETERS");
    break;
  case TAKES:
    printf("TAKES");
    break;
  case INPUT:
    printf("INPUT");
    break;
  case RETURNS:
    printf("RETURNS");
    break;
  case FOR:
    printf("FOR");
    break;
  case IN:
    printf("IN");
    break;
  case SWITCH:
    printf("SWITCH");
    break;
  case CASE:
    printf("CASE");
    break;
  case BREAK:
    printf("BREAK");
    break;
  case DEFAULT:
    printf("DEFAULT");
    break;
  case WHILE:
    printf("WHILE");
    break;
  case ID:
    printf("ID");
    break;
  case NUM:
    printf("NUM");
    break;
  case RNUM:
    printf("RNUM");
    break;
  case AND:
    printf("AND");
    break;
  case OR:
    printf("OR");
    break;
  case TRUE:
    printf("TRUE");
    break;
  case FALSE:
    printf("FALSE");
    break;
  case PLUS:
    printf("PLUS");
    break;
  case MINUS:
    printf("MINUS");
    break;
  case MUL:
    printf("MUL");
    break;
  case DIV:
    printf("DIV");
    break;
  case LT:
    printf("LT");
    break;
  case LE:
    printf("LE");
    break;
  case GE:
    printf("GE");
    break;
  case GT:
    printf("GT");
    break;
  case EQ:
    printf("EQ");
    break;
  case NE:
    printf("NE");
    break;
  case DEF:
    printf("DEF");
    break;
  case ENDDEF:
    printf("ENDDEF");
    break;
  case DRIVERDEF:
    printf("DRIVERDEF");
    break;
  case DRIVERENDDEF:
    printf("DRIVERENDDEF");
    break;
  case COLON:
    printf("COLON");
    break;
  case RANGEOP:
    printf("RANGEOP");
    break;
  case SEMICOL:
    printf("SEMICOL");
    break;
  case COMMA:
    printf("COMMA");
    break;
  case ASSIGNOP:
    printf("ASSIGNOP");
    break;
  case SQBO:
    printf("SQBO");
    break;
  case SQBC:
    printf("SQBC");
    break;
  case BO:
    printf("BO");
    break;
  case BC:
    printf("BC");
    break;
  case EPSILON:
    printf("EPSILON");
    break;
  case DOLLAR:
    printf("DOLLAR");
    break;
  case ERROR:
    printf("ERROR");
    break;
  }
}

void printNonTerminalName(NonTerminal nt){
  switch (nt) {
    case program0:
      printf("program0\n");
      break;
    case program:
      printf("program\n");
      break;
    case moduleDeclarations:
      printf("moduleDeclarations\n");
      break;
    case moduleDeclaration:
      printf("moduleDeclaration\n");
      break;
    case otherModules:
      printf("otherModules\n");
      break;
    case driverModule:
      printf("driverModule\n");
      break;
    case module:
      printf("module\n");
      break;
    case ret:
      printf("ret\n");
      break;
    case input_plist:
      printf("input_plist\n");
      break;
    case input_plist2:
      printf("input_plist2\n");
      break;
    case output_plist:
      printf("output_plist\n");
      break;
    case output_plist2:
      printf("output_plist2\n");
      break;
    case dataType:
      printf("dataType\n");
      break;
    case range_arrays:
      printf("range_arrays\n");
      break;
    case type:
      printf("type\n");
      break;
    case moduleDef:
      printf("moduleDef\n");
      break;
    case statements:
      printf("statements\n");
      break;
    case statement:
      printf("statement\n");
      break;
    case ioStmt:
      printf("ioStmt\n");
      break;
    case var_print:
      printf("var_print\n");
      break;
    case boolConstt:
      printf("boolConstt\n");
      break;
    case p1:
      printf("p1\n");
      break;
    case simpleStmt:
      printf("simpleStmt\n");
      break;
    case assignmentStmt:
      printf("assignmentStmt\n");
      break;
    case whichStmt:
      printf("whichStmt\n");
      break;
    case lvalueIDStmt:
      printf("lvalueIDStmt\n");
      break;
    case lvalueARRStmt:
      printf("lvalueARRStmt\n");
      break;
    case index_arr:
      printf("index_arr\n");
      break;
    case new_index:
      printf("new_index\n");
      break;
    case sign:
      printf("sign\n");
      break;
    case moduleReuseStmt:
      printf("moduleReuseStmt\n");
      break;
    case optional:
      printf("optional\n");
      break;
    case actual_para_list:
       printf("actual_para_list\n");
       break;
    case actual_para_list2:
       printf("actual_para_list2\n");
       break;
    case k:
       printf("k\n");
       break;
    case idList:
       printf("idList\n");
       break;
    case idList2:
       printf("idList2\n");
       break;
    case expression:
       printf("expression\n");
       break;
    case u: 
       printf("u\n");
       break;
    case new_NT: 
       printf("new_NT\n");
       break;
    case var_id_num:
       printf("var_id_num\n");
       break;
    case unary_op:  
       printf("unary_op\n");
       break;
    case arithmeticOrBooleanExpr:
       printf("arithmeticOrBooleanExpr\n");
       break;
    case arithmeticOrBooleanExpr2:
       printf("arithmeticOrBooleanExpr2\n");
       break;
    case anyTerm:  
       printf("anyTerm\n");
       break;
    case anyTerm2:  
       printf("anyTerm2\n");
       break;
    case arithmeticExpr:
       printf("arithmeticExpr\n");
       break;
    case arithmeticExpr2:  
       printf("arithmeticExpr2\n");
       break;
    case term:  
       printf("term\n");
       break;
    case term2:  
       printf("term2\n");
       break;
    case factor:  
       printf("factor\n");
       break;
    case n_11:
      printf("n_11\n");
      break;
    case n_10:
      printf("n_10\n");
      break;
    case element_index_with_expressions:
      printf("element_index_with_expressions\n");
      break;
    case arrExpr:  
      printf("arrExpr\n");
      break;
    case arr_N4:  
      printf("arr_N4\n");
      break;
    case arrTerm:  
      printf("arrTerm\n");
      break;
    case arr_N5:  
      printf("arr_N5\n");  
      break;
    case arrFactor:  
      printf("arrFactor\n");
      break;
    case op1:  
      printf("op1\n");
      break;
    case op2:  
      printf("op2\n");
      break;  
    case logicalOp:  
      printf("logicalOp\n");
      break;
    case relationalOp:  
      printf("relationalOp\n");
      break;
    case declareStmt:  
      printf("declareStmt\n");
      break;
    case conditionalStmt:  
      printf("conditionalStmt\n");
      break;
    case caseStmts:  
      printf("caseStmts\n");
      break;
    case caseStmts2:  
      printf("caseStmts2\n");
      break;
    case value:  
      printf("value\n");
      break;
    case defaultt:  
      printf("defaultt\n");
      break;
    case iterativeStmt:  
      printf("iterativeStmt\n");
      break;
    case range_for_loop:  
      printf("range_for_loop\n");
      break;
    case index_for_loop:  
      printf("new_index_for_loop\n");
      break;
    case sign_for_loop:  
      printf("sign_for_loop\n");
      break;
    case error:
      printf("error\n");
      break;
  }
}


void createFollow(NonTerminal NT){
    traversedRuleNonTerminal[NT] = 1;
    for(int rule = 0; rule<NUM_RULES; rule++){
        node a = array[rule]->head->next;

        while(rhsCountNT[NT][rule] > 0){
            while(a->isTerminal || a->nonTerminal != NT)  a = a->next;
            node b = a->next;
            while(b != NULL){
                if(b->isTerminal){
                    if(b->terminal != EPSILON){ //the only possible case
                        followSet[NT][b->terminal] = 1;
                        break;
                    }
                }else{
                    //b is a non terminal
                    copyFollowsfromFirsts(NT, b->nonTerminal);
                    if(followSet[NT][EPSILON] == 1){
                        b = b->next;
                        followSet[NT][EPSILON] = 0;
                    }else{
                        break;
                    }
                }

                
            }

            if(b == NULL){
                //follow(NT) = follow(head)
                NonTerminal head = array[rule]->head->nonTerminal;
                if(!traversedRuleNonTerminal[head])   createFollow(head);
                copyFollowsfromFollows(NT,head);
            }
            a = a->next;
            rhsCountNT[NT][rule]--;

        }
    }

}

TREE_NODE initializeParser(FILE* fptr, int sizeOfBuffer){
  //fseek(fptr,0,SEEK_SET);

  if(initializedOnce==0){
    for (int i = 0; i < NUM_NONTERMINALS; i++) {
    for (int j = 0; j < NUM_RULES; j++)
      rhsCountNT[i][j] = 0;
    
  }
  addRules();
  //fclose(ptr);
  //printf("Rules added\n");
  createFirstSet();
  //printf("First created\n");
  createFollowSet();
  //printf("Follow created\n");
  //createFollowSet2();
  createParseTable();
  initializedOnce =1;
  }
  initializeLexer(fptr, sizeOfBuffer);
    
  //FILE* ptr = fopen("final_grammar.txt","r");
  
  //printf("parse table created\n");
  //createSynchSet();
  //printf("Rules added\n");
  //printGrammar();
  TREE_NODE tree = parseSourceCode();
  // printf("Printing Parse Tree\n");
  // printParseTree(tree);
  //printf("Yes");
  //freeBuffers();
  //freeParser();
  return tree;
}

void freeParser(){
  for(int i=0;i<NUM_RULES;i++)
  {
    node temp = array[i]->head;
    node a = NULL;
    while(temp)
    {
      a = temp;
      temp = temp->next;
      free(a);
    }
    free(a);
  }
}

// int main(int argc, char *argv[]) {
//   char* fileName = argv[1];
//   FILE* fptr = fopen(fileName,"r");
//   int sizeOfBuffer = 512;
//   initializeLexer(fptr, sizeOfBuffer);
//   for (int i = 0; i < NUM_NONTERMINALS; i++) {
//     for (int j = 0; j < NUM_RULES; j++)
//       rhsCountNT[i][j] = 0;
//   }
//   addRules();
//   createFirstSet();
//   createFollowSet();
//   //createFollowSet2();
//   createParseTable();
//   //createSynchSet();
//   //printf("Rules added\n");
//   //printGrammar();
//   TREE_NODE tree = parseSourceCode();
//   // printf("Printing Parse Tree\n");
//   // printParseTree(tree);
//   // printf("Yes");
//   freeBuffers();
// }