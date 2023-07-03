/*
  Group Number 53
  Snehal Juneja: 2019B2A70994P
  Aryan Gautam Nadkarni: 2019B3A70406P
  Ishan Rai: 2019B3A70504P
  Shrey Bansal: 2019B3A70592P
  Karizma Khurana: 2019B4A70708P
*/
#include "lexer.h"

// global variables
char stringArr[M][MAX_LENGTH_OF_VAR] = {
  "integer",
  "real",
  "boolean",
  "of",
  "array",
  "start",
  "end",
  "declare",
  "module",
  "driver",
  "program",
  "get_value",
  "print",
  "use",
  "with",
  "parameters",
  "takes",
  "input",
  "returns",
  "for",
  "in",
  "switch",
  "case",
  "break",
  "default",
  "while"
};
struct tokenInfo lookupTable[M];
int bufferBegin = 0; //tells which buffer (0 or 1) is begin pointing to
int bufferForward = 0; //tells which buffer (0 or 1) is forward pointing to
int begin = 0; //begin pointer
int forward = 0; //forward pointer
FILE * sourceCode;
int initialRead = 0;
int commentStart = 0;
int lineNumber;
struct TwinBuffer* twinBuffer;
int openedOnce = 0;
int bufferSize;

FILE* openFile(char* fileName){
  FILE* fptr = fopen(fileName,"r");
  if(fptr==NULL){
    printf("Error in opening testcase file\n");
    printf("Exiting program. Try again\n");
    exit(0);
    return NULL;
  }
  return fptr;
}

void initializeLexer(FILE* fptr, int sizeOfBuffer){
  if(fseek(fptr,0,SEEK_SET)!=0){
    printf("ERROR\n");
  }
  twinBuffer= NULL;
  sourceCode = fptr;
  lineNumber = 1;
  bufferForward = 0;
    bufferBegin = 0;
    forward = 0;
    begin = 0;
    initialRead = 0;
    bufferSize = sizeOfBuffer;
  initializeLookup();
  twinBuffer = malloc(sizeof(struct TwinBuffer));
  twinBuffer -> flag = 0;
  getStream(sourceCode, twinBuffer);
}

void freeBuffers(){
  //printf("Freeing Buffers");
  // free(twinBuffer->buf0);
  // free(twinBuffer->buf1);
  free(twinBuffer);
  //fclose(sourceCode);
}


int getHash(char str[]) {
  return strlen(str) % M;
}

int nextProbe(int x) {
  int l = (x + 1) % M;
  return l;
}

void initializeLookup() {
  for (int i = 0; i < M; i++) {
    strcpy(lookupTable[i].lexeme, "empty");
  }

  for (int j = 0; j < M; j++) {
    char str[MAX_LENGTH_OF_VAR];
    strcpy(str, stringArr[j]);
    int len = strlen(str);
    int x = getHash(str);
    while ((strcmp(lookupTable[x].lexeme, "empty")) != 0) {
      x = nextProbe(x);
    }

    lookupTable[x].token = j;

    strcpy(lookupTable[x].lexeme, str);

  }
}

struct tokenInfo searchLookup(char str[]) {
  int x = getHash(str);
  int count = 0;

  if (strcmp(lookupTable[x].lexeme, "empty") == 0) {
    return lookupTable[x];
  }

  while (strcmp(lookupTable[x].lexeme, str) != 0) {
    x = nextProbe(x);
    count++;
    if (strcmp(lookupTable[x].lexeme, "empty") == 0) {
      return lookupTable[x];
    }
    if (count > M) {
      struct tokenInfo emp;
      strcpy(emp.lexeme, "empty");
      return emp;
    }
  }

  return lookupTable[x];

}

void removeComments(char* testCaseFile, char* cleanFile){
  FILE* inputFptr = fopen(testCaseFile,"r");
  if(inputFptr==NULL){
    printf("Error in opening testcase file\n");
    exit(0);
  }
  FILE* outputFptr = fopen(cleanFile,"w");
  if(outputFptr==NULL){
    printf("Error in opening cleaned file\n");
  }
  printf("Beginning removing\n");
  char c;
  int commentState = 1; //depening on number of *s encountered, 5 states
  while((c = fgetc(inputFptr))!=EOF){
    if(commentState==1){
      if(c!='*'){
        printf("%c",c);
        fputc(c,outputFptr);
      }else{
        commentState = 2;
      }
      continue;
    }
    if(commentState==2){
      if(c!='*'){
        printf("*");
        printf("%c",c);
        fputc('*',outputFptr);
        fputc(c,outputFptr);
        commentState = 1; //it's not a comment so go back to state 1
      }else{
        //comment started so now ignore characters
        commentState = 3;
      }
      continue;
    }
    if(commentState==3){
      //we've to keep line numbers intact so only printing \n and ignoring all others
      if(c=='\n'){
        printf("\n");
        fputc('\n',outputFptr);
      }else if(c=='*'){
        commentState = 4;
      }else{
        //do nothing - ignore
      }
      continue;
    }
    if(commentState==4){
      if(c!='*'){
        commentState = 3;
      }else{
        commentState = 5; //comment ended here
        commentState = 1; //should go back to original state
      }
    }
  }
  fclose(inputFptr);
  fclose(outputFptr);
}

int BUF_SIZE = 512;
void getStream(FILE * sourceCode, struct TwinBuffer * twinBuffer) {
  if (initialRead == 0) {
    memset(twinBuffer->buf0,'\0',BUF_SIZE);
    int i = 0;
    while (i < BUF_SIZE) {
      twinBuffer -> buf0[i] = fgetc(sourceCode);
      i++;
    }
    memset(twinBuffer->buf1,'\0',BUF_SIZE);
    i = 0;
    while (i < BUF_SIZE) {
      twinBuffer -> buf1[i] = fgetc(sourceCode);
      i++;
    }
    bufferForward = 0;
    bufferBegin = 0;
    forward = 0;
    begin = 0;
    initialRead = 1;
  } else {
    if (bufferForward == 1) {
      //fill 0th buffer
      int i = 0;
      while (i < BUF_SIZE) {
        twinBuffer -> buf0[i] = fgetc(sourceCode);
        i++;
      }
    } else {
      //fill 1st buffer
      //memset(twinBuffer->buf1,'\0',BUF_SIZE);
      int i = 0;
      while (i < BUF_SIZE) {
        twinBuffer -> buf1[i] = fgetc(sourceCode);
        i++;
      }
    }
  }
  //printf("Printing Buffers\n");
  //printf("%s\n", twinBuffer->buf0);
  //printf("%s\n", twinBuffer->buf1);
  return;
}

char readChar(struct TwinBuffer * twinBuffer) {
  char c;
  if (bufferForward == 0) {
    c = twinBuffer -> buf0[forward];
  } else {
    c = twinBuffer -> buf1[forward];
  }
  forward++;
  if (forward == BUF_SIZE) {
    forward = 0;
    bufferForward = 1 - bufferForward; //toggle buffer
    getStream(sourceCode,twinBuffer);
  }
  return c;
}

void copyLexeme(char * lexeme, struct TwinBuffer * twinBuffer) {
  char tempStr[256];
  int tempBegin = begin;
  int tempForward = forward;
  if (bufferBegin == bufferForward) {
    int i = 0;
    while (tempBegin < tempForward) {
      if (bufferBegin == 0) {
        lexeme[i] = twinBuffer -> buf0[tempBegin];
      } else {
        lexeme[i] = twinBuffer -> buf1[tempBegin];
      }
      tempBegin++;
      i++;
    }
    tempForward++; //handle forward logic here
    lexeme[i] = '\0';
  } else {
    int i = 0;
    while (tempBegin < BUF_SIZE) {
      if (bufferBegin == 0) {
        lexeme[i] = twinBuffer -> buf0[tempBegin];
      } else {
        lexeme[i] = twinBuffer -> buf1[tempBegin];
      }

      tempBegin++;
      i++;
    }
    tempBegin = 0;
    while (tempBegin < tempForward) {
      if (bufferForward == 0) {
        lexeme[i] = twinBuffer -> buf0[tempBegin];
      } else {
        lexeme[i] = twinBuffer -> buf1[tempBegin];
      }
      tempBegin++;
      i++;
    }
    tempForward++;
    lexeme[i] = '\0';
  }
  //lexeme = tempStr;
}

void
retract(struct TwinBuffer * twinBuffer) {
  forward = forward - 1;
  if (forward < 0) {
    forward = BUF_SIZE-1;
    bufferForward = 1 - bufferForward;
  }
}

struct tokenInfo getNextToken() {
  int state = 1;
  char c;

  struct tokenInfo token;
  while (1) {
    token.lineNo = lineNumber;
    c = readChar(twinBuffer);
      while(c=='\0'){
      c = readChar(twinBuffer);
    }
    switch (state) {
    case 1: {
      if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_') {
        state = 2;
      } else if (c >= '0' && c <= '9') {
        state = 4;
      } else if (c == '+') {
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        strcpy(token.lexeme, str);
        token.token = TK_PLUS;
        token.value = NULL;
        //token.lineNo = lineNumber;
        bufferBegin = bufferForward;
        begin = forward;
        free(str);
        return token;
      } else if (c == '-') {
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_MINUS;
        token.value = NULL;
        //token.lineNo = lineNumber;
        bufferBegin = bufferForward;
        begin = forward;
        free(str);
        return token;
      } else if (c == '*') {
        state = 16;
        continue;
      } else if (c == '/') {
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        strcpy(token.lexeme, str);
        token.token = TK_DIV;
        token.value = NULL;
        //token.lineNo = lineNumber;
        bufferBegin = bufferForward;
        begin = forward;
        free(str);
        return token;
      } else if (c == '<') {
        state = 22;
      } else if (c == '>') {
        state = 28;
      } else if (c == '=') {
        state = 34;
      } else if (c == '!') {
        state = 36;
      } else if (c == ':') {
        state = 38;
      } else if (c == '.') {
        state = 41;
      } else if (c == ';') {
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_SEMICOL;
        token.value = NULL;
        //token.lineNo = lineNumber;
        bufferBegin = bufferForward;
        begin = forward;
        free(str);
        return token;
      } else if (c == ',') {
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_COMMA;
        token.value = NULL;
        //token.lineNo = lineNumber;
        bufferBegin = bufferForward;
        begin = forward;
        free(str);
        return token;
      } else if (c == '[') {
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_SQBO;
        token.value = NULL;
        //token.lineNo = lineNumber;
        bufferBegin = bufferForward;
        begin = forward;
        free(str);
        return token;
      } else if (c == ']') {
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_SQBC;
        token.value = NULL;
        bufferBegin = bufferForward;
        //token.lineNo = lineNumber;
        begin = forward;
        free(str);
        return token;
      } else if (c == '(') {
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_BO;
        token.value = NULL;
        bufferBegin = bufferForward;
        //token.lineNo = lineNumber;
        begin = forward;
        free(str);
        return token;
      } else if (c == ')') {
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_BC;
        token.value = NULL;
        //token.lineNo = lineNumber;
        bufferBegin = bufferForward;
        begin = forward;
        free(str);
        return token;
      } else if (c == ' ' || c == '\t' || c == '\n' || c == 13) {
        if(c=='\n'){
          lineNumber++;
        }
        state = 49;
      } else if (c == EOF) {
        strcpy(token.lexeme, "EOF");
        printf("EOF reached\n");
        token.token = TK_ENDOFFILE;
        token.value = NULL;
        //token.lineNo = lineNumber;
        return token;
      } else {
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        if(str[0]<0){
          continue;
        }
        strcpy(token.lexeme, "error");
        printf("Lexer error unidentified symbol\n");
        token.token = TK_LEX_ERROR;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
      }
      break;
    }
    case 2: {
      if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' ||
        c >= '0' && c <= '9') {
        state = 2;
      } else {
        state = 3;
        retract(twinBuffer);
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        if (strlen(str) > 20) {
          printf("%s\n", "Length greater then 20");
          token.token = TK_LEX_ERROR;
          token.value = NULL;
          bufferBegin = bufferForward;
          begin = forward;
          token.lineNo = lineNumber;
          return token;
        }
        if(strcmp(str,"true")==0){
          token.token = TK_TRUE;
          token.value = NULL;
          strcpy(token.lexeme,str);
          bufferBegin = bufferForward;
          token.lineNo = lineNumber;
          begin = forward;
          return token;
        }else if(strcmp(str,"false")==0){
          token.token = TK_FALSE;
          token.value = NULL;
          strcpy(token.lexeme,str);
          bufferBegin = bufferForward;
          begin = forward;
          token.lineNo = lineNumber;
          return token;
        }else if(strcmp(str,"AND")==0){
          token.token = TK_AND;
          token.value = NULL;
          strcpy(token.lexeme,str);
          bufferBegin = bufferForward;
          begin = forward;
          token.lineNo = lineNumber;
          return token;
        }else if(strcmp(str,"OR")==0){
          token.token = TK_OR;
          token.value = NULL;
          strcpy(token.lexeme,str);
          bufferBegin = bufferForward;
          begin = forward;
          token.lineNo = lineNumber;
          return token;
        }
        token = searchLookup(str);
        begin = forward;
        bufferBegin = bufferForward;
        token.lineNo = lineNumber;
        if (strcmp(token.lexeme, "empty") == 0) {
          token.token = TK_ID;
          token.lineNo = lineNumber;
          strcpy(token.lexeme, str);
          free(str);
          return token;
        } else {
          free(str);
          return token;
        }
        state = 1;
      }
      break;
    }
    case 4: {
      if (c >= '0' && c <= '9') {
        state = 4;
      } else if (c != '.') {
        state = 5;
        retract(twinBuffer);
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        token.value = (int * ) malloc(sizeof(int));
        int a = atoi(str);
        token.token = TK_NUM;
        *(int * ) token.value = a;
        begin = forward;
        bufferBegin = bufferForward;
        //token.lineNo = lineNumber;
        return token;
        state = 1;
      } else {
        state = 6;
      }
      break;
    }
    case 6: {
      if (c == '.') {
        //range op
        state = 7;
        retract(twinBuffer);
        retract(twinBuffer);
        //tokenize num
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        token.value = (int * ) malloc(sizeof(int));
        int a = atoi(str);
        token.token = TK_NUM;
        *(int * ) token.value = a;
        begin = forward;
        bufferBegin = bufferForward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
        state = 1;
      } else if (c >= '0' && c <= '9') {
        state = 8;
      } else {
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        //printf("%c %d", c, state);
        printf("Lexer error after num\n");
        bufferBegin = bufferForward;
        begin = forward;
        token.token = TK_LEX_ERROR;
        //token.lineNo = lineNumber;
        free(str);
        return token;
      }
      break;
    }

    case 8: {
      if (c >= '0' && c <= '9') {
        state = 8;
      } else if (c == 'E' || c == 'e') {
        state = 10;
      } else {
        state = 9;
        retract(twinBuffer);
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        token.value = (float * ) malloc(sizeof(float));
        float a = atof(str);
        token.token = TK_RNUM;
        *(float * ) token.value = a;
        begin = forward;
        bufferBegin = bufferForward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
        state = 1;
      }
      break;
    }

    case 10: {
      if (c == '+' || c == '-') {
        state = 11;
      } else if (c >= '0' && c <= '9') {
        state = 12;
      } else {
        printf("%c %d", c, state);
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        printf("Lexer error while parsing RNUM\n");
        token.token = TK_LEX_ERROR;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
      }

      break;
    }

    case 11: {
      if (c >= '0' && c <= '9') {
        state = 12;
      } else {
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        //printf("%c %d", c, state);
        printf("Lexer error while parsing RNUM\n");
        token.token = TK_LEX_ERROR;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
      }

      break;
    }

    case 12: {
      if (c >= '0' && c <= '9') {
        state = 12;
      } else {
        state = 13;
        retract(twinBuffer);
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        token.value = (float * ) malloc(sizeof(float));
        float a = atof(str);
        token.token = TK_RNUM;
        *(float * ) token.value = a;
        begin = forward;
        bufferBegin = bufferForward;
        //token.lineNo = lineNumber;
        free(str);
        return token;

        //tokenize RNUM
        state = 1;
      }

      break;
    }

    case 16: {
      if (c == '*') {
        //comment starts
        //printf("second *\n");
        commentStart = 1;
        state = 18;
        continue;
      } else {
        state = 17;
        retract(twinBuffer);
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_MUL;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;

        //tokenize as MUL
        state = 1;
      }

      break;
    }

    case 18: {
      if (c == '*') {
        state = 19;
        continue;
      } else if(c!=EOF){
        state = 18;
        continue;
      } else{
      	strcpy(token.lexeme, "EOF");
        printf("EXIT");
        token.token = TK_ENDOFFILE;
        token.value = NULL;
        //token.lineNo = lineNumber;
        return token;
      }

      break;
    }
    case 19: {
      if (c == '*') {
        state = 20;
        char * str = malloc(sizeof(char) * 1024);
        copyLexeme(str, twinBuffer);
        //comment over
        strcpy(token.lexeme, "empty");
        token.token = TK_IGNORE;
        commentStart = 0;
        begin = forward;
        //token.lineNo = lineNumber;
        bufferBegin = bufferForward;
        free(str);
        return token; //CHECK THIS NOTHING SHOULD BE RETURNED TECHNICALLY
        state = 1;
      } else {
        state = 18;
      }
      break;
    }

    

    case 22: {
      if (c == '=') {
        state = 24;
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_LE;
        token.value = NULL;
        bufferBegin = bufferForward;
        //token.lineNo = lineNumber;
        begin = forward;
        free(str);
        return token;
      } else if (c == '<') {
        state = 25;

      } else {
        state = 23;
        retract(twinBuffer);
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_LT;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
      }

      break;
    }

    case 25: {
      if (c == '<') {
        //printf("%d %c\n", state, c);
        state = 27;
        //printf("%d %c\n", state, c);
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_DRIVERDEF;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
      } else {
        state = 26;
        retract(twinBuffer);
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_DEF;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
        state = 1;
      }

      break;
    }

    case 28: {
      if (c == '=') {
        state = 30;
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_GE;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
      } else if (c == '>') {
        state = 31;

      } else {
        state = 29;
        retract(twinBuffer);
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_GT;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
        state = 1;
      }
      break;
    }

    case 31: {
      if (c == '>') {
        state = 33;
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_DRIVERENDDEF;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
      } else {
        state = 32;
        retract(twinBuffer);
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_ENDDEF;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
        state = 1;
      }

      break;
    }

    case 34: {
      if (c == '=') {
        state = 35;
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_EQ;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
      } else {
        //printf("%c %d", c, state);
        printf("Lexer error after equals\n");
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_LEX_ERROR;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
      }

      break;
    }

    case 36: {
      if (c == '=') {
        state = 37;
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_NE;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
      } else {
        //printf("%c %d", c, state);
        printf("Lexer error after not equals\n");
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_LEX_ERROR;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
      }

      break;

    }

    case 38: {
      if (c == '=') {
        state = 40;
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_ASSIGNOP;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
      } else {
        state = 39;
        retract(twinBuffer);
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_COLON;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;

        //tokenize as COLON 
        state = 1;
      }
      break;
    }

    case 41: {
      if (c == '.') {
        state = 42;
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_RANGEOP;
        token.value = NULL;
        bufferBegin = bufferForward;
        begin = forward;
        //token.lineNo = lineNumber;
        free(str);
        return token;
      } else {
        //printf("%c %d", c, state);
        printf("Lexer error after rangeop\n");
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        token.token = TK_LEX_ERROR;
        token.value = NULL;
        //token.lineNo = lineNumber;
        bufferBegin = bufferForward;
        begin = forward;
        free(str);
        return token;
      }

      break;
    }

    case 49: {
      if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
        if(c=='\n')        lineNumber++;
        state = 49;
      } else {
        state = 50;
        retract(twinBuffer);
        bufferBegin = bufferForward;
        begin = forward;
        char * str = malloc(sizeof(char) * 256);
        copyLexeme(str, twinBuffer);
        //printf("%s\n", str);
        //printf("%ld\n", strlen(str));
        strcpy(token.lexeme, str);
        //delimiter 
        strcpy(token.lexeme, "empty");
        token.token = TK_IGNORE;
        //token.lineNo = lineNumber;
        free(str);
        return token; //CHECK THIS NOTHING SHOULD BE RETURNED TECHNICALLY	
        state = 1;
      }
      break;
    }

    default: {
      //printf("%d defualt\n", c);
      printf("Lexer error default\n");
      char * str = malloc(sizeof(char) * 256);
      copyLexeme(str, twinBuffer);
      //printf("%s\n", str);
      //printf("%ld\n", strlen(str));
      strcpy(token.lexeme, str);
      token.token = TK_LEX_ERROR;
      token.value = NULL;
      bufferBegin = bufferForward;
      begin = forward;
      //token.lineNo = lineNumber;
      free(str);
      return token;
    }

  }
}
}

void printTokenName(TOKEN token){
      switch(token) {
        case TK_INTEGER:
            printf("INTEGER");
            break;
        case TK_REAL:
            printf("REAL");
            break;
        case TK_BOOLEAN:
            printf("BOOLEAN");
            break;
        case TK_OF:
            printf("OF");
            break;
        case TK_ARRAY:
            printf("ARRAY");
            break;
        case TK_START:
            printf("START");
            break;
        case TK_END:
            printf("END");
            break;
        case TK_DECLARE:
            printf("DECLARE");
            break;
        case TK_MODULE:
            printf("MODULE");
            break;
        case TK_DRIVER:
            printf("DRIVER");
            break;
        case TK_PROGRAM:
            printf("PROGRAM");
            break;
        case TK_GET_VALUE:
            printf("GET_VALUE");
            break;
        case TK_PRINT:
            printf("PRINT");
            break;
        case TK_USE:
            printf("USE");
            break;
        case TK_WITH:
            printf("WITH");
            break;
        case TK_PARAMETERS:
            printf("PARAMETERS");
            break;
        case TK_TAKES:
            printf("TAKES");
            break;
        case TK_INPUT:
            printf("INPUT");
            break;
        case TK_RETURNS:
            printf("RETURNS");
            break;
        case TK_FOR:
            printf("FOR");
            break;
        case TK_IN:
            printf("IN");
            break;
        case TK_SWITCH:
            printf("SWITCH");
            break;
        case TK_CASE:
            printf("CASE");
            break;
        case TK_BREAK:
            printf("BREAK");
            break;
        case TK_DEFAULT:
            printf("DEFAULT");
            break;
        case TK_WHILE:
            printf("WHILE");
            break;
        case TK_ID:
            printf("ID");
            break;
        case TK_NUM:
            printf("NUM");
            break;
        case TK_RNUM:
            printf("RNUM");
            break;
        case TK_AND:
            printf("AND");
            break;
        case TK_OR:
            printf("OR");
            break;
        case TK_TRUE:
            printf("TRUE");
            break;
        case TK_FALSE:
            printf("FALSE");
            break;
        case TK_PLUS:
            printf("PLUS");
            break;
        case TK_MINUS:
            printf("MINUS");
            break;
        case TK_MUL:
            printf("MUL");
            break;
        case TK_DIV:
            printf("DIV");
            break;
        case TK_LT:
            printf("LT");
            break;
        case TK_LE:
            printf("LE");
            break;
        case TK_GE:
            printf("GE");
            break;
        case TK_GT:
            printf("GT");
            break;
        case TK_EQ:
            printf("EQ");
            break;
        case TK_NE:
            printf("NE");
            break;
        case TK_DEF:
            printf("DEF");
            break;
        case TK_ENDDEF:
            printf("ENDDEF");
            break;
        case TK_DRIVERDEF:
            printf("DRIVERDEF");
            break;
        case TK_DRIVERENDDEF:
            printf("DRIVERENDDEF");
            break;
        case TK_COLON:
            printf("COLON");
            break;
        case TK_RANGEOP:
            printf("RANGEOP");
            break;
        case TK_SEMICOL:
            printf("SEMICOL");
            break;
        case TK_COMMA:
            printf("COMMA");
            break;
        case TK_ASSIGNOP:
            printf("ASSIGNOP");
            break;
        case TK_SQBO:
            printf("SQBO");
            break;
        case TK_SQBC:
            printf("SQBC");
            break;
        case TK_BO:
            printf("BO");
            break;
        case TK_BC:
          printf("BC");
          break;
        case TK_ENDOFFILE:
          printf("ENDOFFILE");
          break;
        case TK_LEX_ERROR:
          printf("LEX_ERROR");
          break;
        case TK_IGNORE:
          printf("IGNORE");
          break;
  }
}