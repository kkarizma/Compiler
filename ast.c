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

typedef enum{
AST_PROGRAM,
AST_DRIVER,
AST_MODULE,
AST_PARAMETER_LIST,
AST_ID,
AST_ARRAY,
AST_RANGE,
AST_INTEGER,
AST_REAL,
AST_BOOLEAN,
AST_STATEMENTS,
AST_GET_VALUE,
AST_PRINT,
AST_RNUM,
AST_NUM,
AST_TRUE,
AST_FALSE,
AST_ARRAY_ACCESS,
AST_ASSIGN,
AST_INDEX_ARR,
AST_PLUS,
AST_MINUS,
AST_FN_CALL,
AST_PLIST,
AST_NODE_VAL,
AST_IDENTITY,
AST_UNARYOP,
AST_DECLARE,
AST_SWITCH,
AST_CASES,
AST_CASE,
AST_DEFAULT_CASE,
AST_FOR_LOOP,
AST_WHILE_LOOP,
AST_RANGE_FOR,
AST_INDEX_FOR_LOOP

}ASTNodeName;

typedef struct ASTNode * AST_NODE;

struct ASTNode{
Terminal terminal;
NonTerminal nonTerminal;
int isTerminal;
AST_NODE parent;
AST_NODE next;
AST_NODE child;
ASTNodeName nodeName;
struct tokenInfo * value;
}ast;

ast* makeNode(ASTNodeName nodeName, ast* parent, ast* firstChild, ast* sibling, parseTree pNode) {
    ast* node = (ast*)malloc(sizeof(ast));
    node->nodeName = nodeName;
    node->parent = parent;
    node->child = firstChild;
    node->next = sibling;
    node->terminal = pNode->terminal;
    node->isTerminal = pNode->isTerminal;
    node->nonTerminal = pNode->nonTerminal;
    node->value = pNode->value;
    return node;
}

ast* makeAST(parseTree pNode, ast* parent){
    ast* currNode;
    if(node->child->terminal == EPSILON){
        return NULL;
    }

    //<program0> -> <program> DOLLAR
    if(pNode->nonTerminal == program0){
        currNode = makeAST(pNode->child, parent);
    }

    //program -> moduleDeclarations otherModules driverModule otherModules    
    if(pNode->nonTerminal == program){
        currNode = makeNode(AST_PROGRAM, parent, NULL, NULL, pNode);
        currNode->child = makeAST(pNode->child, currNode);
        currNode->child->next = makeAST(pNode->child->next, currNode);
        ast* temp = lastNode(currNode->child); //if otherModules => epsilon
        temp->next = makeAST(pNode->child->next->next, currNode);
        temp->next->next = makeAST(pNode->child->next->next->next, currNode);

       
    }
    
    // <moduleDeclarations> <moduleDeclaration> <moduleDeclarations>
    if(pNode->nonTerminal == moduleDeclarations){
        currNode = makeAST(pNode->child, parent);
        currNode->next = makeAST(pNode->child->next, parent);
    }
    
    //<moduleDeclaration> -> DECLARE MODULE ID SEMICOL
    if(pNode->nonTerminal == moduleDeclaration){
        currNode = makeNode(AST_ID, parent, NULL, NULL, pNode->child->next->next);
    }

    // <otherModules> <module> <otherModules> 
    if(pNode->nonTerminal == otherModules){
        currNode = makeAST(pNode->child, parent);
        currNode->next = makeAST(pNode->child->next, parent);
    }

    // <driverModule> DRIVERDEF DRIVER PROGRAM DRIVERENDDEF <moduleDef>
    if(pNode->nonTerminal == driverModule){
            currNode = makeNode(AST_DRIVER, parent, NULL, NULL, pNode->child);
            currNode->child = makeAST(pNode->child->next->next->next->next, currNode);
            
        }

    // <module> DEF MODULE ID ENDDEF TAKES INPUT SQBO <input_plist> SQBC SEMICOL <ret> <moduleDef>
    if(pNode->nonTerminal == module){
        currNode = makeNode(AST_MODULE, parent, NULL, NULL, pNode->child->next->next);  //save ID in the node
        currNode->child = makeAST(pNode->child->next->next->next->next->next->next->next, currNode);
        currNode->child->next = makeAST(pNode->child->next->next->next->next->next->next->next->next->next->next, currNode);
        
        if(currNode->child->next == NULL){
            currNode->child->next = makeAST(pNode->child->next->next->next->next->next->next->next->next->next->next->next, currNode);
        }else{
            currNode->child->next->next = makeAST(pNode->child->next->next->next->next->next->next->next->next->next->next->next, currNode);
        }
    }

    // <ret> RETURNS SQBO <output_plist> SQBC SEMICOL 
    if(pNode->nonTerminal == ret){
        currNode = makeNode(AST_PARAMETER_LIST, parent, NULL, NULL, pNode->child);
        currNode->child = makeAST(pNode->child->next->next, currNode);
    }
    
    // <input_plist> ID COLON <dataType> <input_plist2>
    if(pNode->nonTerminal == input_plist){
        /*
            ID --- ID
            |      |
            TYPE  TYPE  
        
        */
        currNode = makeNode(AST_ID, parent, NULL, NULL, pNode->child);
        currNode->child = makeAST(pNode->child->next->next, currNode);
        currNode->next = makeAST(pNode->child->next->next->next, parent);
    }
    // <input_plist2> COMMA ID COLON <dataType> <input_plist2>
    if(pNode->nonTerminal == input_plist2){
         /*
            ID --- ID
            |      |
            TYPE  TYPE  
        
        */
        currNode = makeNode(AST_ID, parent, NULL, NULL, pNode->child->next);
        currNode->child = makeAST(pNode->child->next->next->next, currNode);
        currNode->next = makeAST(pNode->child->next->next->next->next, parent);
    }

    // <output_plist> ID COLON <type> <output_plist2>
    if(pNode->nonTerminal == output_plist){
        /*
            ID --- ID
            |      |
            TYPE  TYPE  
        
        */
        currNode = makeNode(AST_ID, parent, NULL, NULL, pNode->child);
        currNode->child = makeAST(pNode->child->next->next, currNode);
        currNode->next = makeAST(pNode->child->next->next->next, parent);
    }

    // <output_plist2> COMMA ID COLON <type> <output_plist2>
    if(pNode->nonTerminal == output_plist2){
         /*
            ID --- ID
            |      |
            TYPE  TYPE  
        
        */
        currNode = makeNode(AST_ID, parent, NULL, NULL, pNode->child->next);
        currNode->child = makeAST(pNode->child->next->next->next, currNode);
        currNode->next = makeAST(pNode->child->next->next->next->next, parent);
    }

    // <dataType> ARRAY SQBO <range_arrays> SQBC OF <type> 
    if(pNode->nonTerminal == dataType && pNode->child->terminal == ARRAY){
        currNode = makeNode(AST_ARRAY, parent, NULL, NULL, AST_PARAMETER_LIST);
        currNode->child = makeAST(pNode->child->next->next, currNode);
        currNode->child->next = makeAST(pNode->child->next->next->next->next->next, currNode);
    }

    // <range_arrays> <index_arr> RANGEOP <index_arr> 
    if(pNode->nonTerminal == range_arrays){
        currNode = makeNode(AST_RANGE, parent, NULL, NULL, pNode->child->next); //pNode->child
        currNode->child = makeAST(pNode->child, currNode);
        currNode->child->next = makeAST(pNode->child->next->next, currNode);
    }

    // <dataType> INTEGER
    // <dataType> REAL
    // <dataType> BOOLEAN
    if(pNode->nonTerminal == dataType){

        if(pNode->child->terminal == INTEGER){
            currNode = makeNode(AST_INTEGER, parent, NULL, NULL, pNode->child);
        }else if(pNode->child->terminal == REAL){
            currNode = makeNode(AST_REAL, parent, NULL, NULL, pNode->child);
        }else if(pNode->child->terminal == BOOLEAN){
            currNode = makeNode(AST_BOOLEAN, parent, NULL, NULL, pNode->child);
        }

    }

    // <type> INTEGER
    // <type> REAL
    // <type> BOOLEAN
    if(pNode->nonTerminal == type){

        if(pNode->child->terminal == INTEGER){
            currNode = makeNode(AST_INTEGER, parent, NULL, NULL, pNode->child);
        }else if(pNode->child->terminal == REAL){
            currNode = makeNode(AST_REAL, parent, NULL, NULL, pNode->child);
        }else if(pNode->child->terminal == BOOLEAN){
            currNode = makeNode(AST_BOOLEAN, parent, NULL, NULL, pNode->child);
        }

    }

    // <moduleDef> START <statements> END
    if(pNode->nonTerminal == moduleDef){
        currNode = makeNode(AST_STATEMENTS, parent, NULL, NULL, pNode->child); //pNode->child->next
        currNode->child = makeAST(pNode->child->next, currNode);
    }
    
    // <statements> <statement> <statements>
    if(pNode->nonTerminal == statements){
        currNode = makeAST(pNode->child, parent);
        currNode->next = makeAST(pNode->child->next, parent);
    }

    // <statement> <ioStmt>
    // <statement> <simpleStmt>
    // <statement> <declareStmt>
    // <statement> <conditionalStmt>
    // <statement> <iterativeStmt>
    if(pNode->nonTerminal == statement){
        if(pNode->child->nonTerminal == ioStmt || pNode->child->nonTerminal == simpleStmt || pNode->child->nonTerminal == declareStmt || pNode->child->nonTerminal == iterativeStmt ||pNode->child->nonTerminal == conditionalStmt){
            currNode = makeAST(pNode->child, parent);
        }
    }
    
    // <ioStmt> GET_VALUE BO ID BC SEMICOL
    if(pNode->nonTerminal == ioStmt){
        if(pNode->child->terminal == GET_VALUE){
            currNode = makeNode(AST_GET_VALUE, parent, NULL, NULL, pNode->child->next->next);
        }else if(pNode->child->terminal == PRINT){
            // <ioStmt> PRINT BO <var_print> BC SEMICOL
            currNode = makeNode(AST_PRINT, parent, NULL, NULL, pNode->child);
            currNode->child = makeAST(pNode->child->next->next, currNode);
        }
    }
    return currNode;
}


