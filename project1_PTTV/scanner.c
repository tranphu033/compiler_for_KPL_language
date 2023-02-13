/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"


extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];

/***************************************************************/

void skipBlank() {
  // TODO
  while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_SPACE))
    readChar();
}

void skipComment() {
  // TODO
  do{
    readChar();
    if (charCodes[currentChar]==CHAR_TIMES){      
      if(charCodes[readChar()]==CHAR_RPAR)
	break;
    }
  }while (currentChar != EOF);
  if(currentChar == EOF)
      error(ERR_ENDOFCOMMENT, lineNo, colNo);
  readChar();
}

Token* readIdentKeyword(void) {
  // TODO
  Token *t;
  int a,b,i;
  char str[30];
  TokenType TK;
  a=lineNo;
  b=colNo;
  i=0;
  do{
    str[i++]=(char)currentChar;
    readChar();    
  }while(currentChar != EOF &&
	 (charCodes[currentChar]==CHAR_LETTER ||
	  charCodes[currentChar]==CHAR_DIGIT));
  str[i]='\0';
  TK=checkKeyword(str);

  if(TK==TK_NONE){
    t=makeToken(TK_IDENT,a,b);
    if(i>15) {//check length of identifier
    error(ERR_IDENTTOOLONG,a,b);
    strncpy(t->string,str,15);
    }
    else strcpy(t->string,str);
  }
  else t=makeToken(TK,a,b);
  return t;
}

Token* readNumber(void) {
  // TODO
  Token *t;
  int a,b,i;
  char str[30];
  a=lineNo;
  b=colNo;
  i=0;
  do{
    str[i++]=(char)currentChar;
    readChar();    
  }while(currentChar != EOF &&	 
	  charCodes[currentChar]==CHAR_DIGIT);
  str[i]='\0';
  t=makeToken(TK_NUMBER, a, b);
  //check value of number: 0<= number <=2147483647--> number toi da 10 chu so
  if (i>10 || (i==10 && strcmp(str,"2147483647") >0)){ 
    printf("%d-%d:Number too long!\n",a,b);
    strncpy(t->string,str,10);
  }
  else{
  strcpy(t->string,str);
  t->value=atoi(str);
  }
  return t;
}
Token* readConstChar(void) {
  // TODO
  Token *t;
  int i = 0, a, b;
  a = lineNo;
  b = colNo;
  char str[30];
  t = makeToken(TK_CHAR,a,b);
  while(currentChar != EOF && (char)readChar()!='\''){    
    if((char)currentChar == '\\'){
      readChar();
      if((char)currentChar=='\'')
	str[i++]='\'';
      else if((char)currentChar=='\\')
	str[i++]='\\';      
    }
    else str[i++] = (char)currentChar;    
  }
  readChar();
  str[i]='\0';
  if (i>1) error(ERR_INVALIDCHARCONSTANT,a,b);    
  strcpy(t->string,str);
  return t;
}

Token* getToken(void) {
  Token *token;
  int ln, cn;

  if (currentChar == EOF) 
    return makeToken(TK_EOF, lineNo, colNo);

  switch (charCodes[currentChar]) {
  case CHAR_SPACE: skipBlank(); return getToken();
  case CHAR_LETTER: return readIdentKeyword();
  case CHAR_DIGIT: return readNumber();
  case CHAR_PLUS: 
    token = makeToken(SB_PLUS, lineNo, colNo);
    readChar(); 
    return token;

  case CHAR_MINUS:
    token = makeToken(SB_MINUS, lineNo, colNo);
    readChar(); 
    return token;
    
  case CHAR_TIMES:
    token = makeToken(SB_TIMES, lineNo, colNo);
    readChar(); 
    return token;
    
  case CHAR_SLASH:
    token = makeToken(SB_SLASH, lineNo, colNo);
    readChar(); 
    return token;
    
  case CHAR_LT:
    ln=lineNo;
    cn=colNo;
    token = makeToken(SB_LT, ln,cn);
    readChar();
    if(charCodes[currentChar]==CHAR_EQ) {
      free(token);
      token = makeToken(SB_LE, ln,cn);
      readChar();
    }      
    return token;
    
  case CHAR_GT:
    ln=lineNo;
    cn=colNo;    
    token = makeToken(SB_GT, ln,cn);
    readChar();
    if(charCodes[currentChar]==CHAR_EQ) {
      free(token);
      token = makeToken(SB_GE, ln,cn);
      readChar();
    }
    return token;
    
  case CHAR_EXCLAIMATION:
    ln=lineNo;
    cn=colNo;
    readChar();
    if(charCodes[currentChar]==CHAR_EQ) {
      token = makeToken(SB_NEQ, ln,cn);
      readChar();
    }
    return token;
        
  case CHAR_EQ:
    token = makeToken(SB_EQ, lineNo, colNo);
    readChar(); 
    return token;
    
  case CHAR_COMMA:
    token = makeToken(SB_COMMA, lineNo, colNo);
    readChar(); 
    return token;
    
  case CHAR_PERIOD:
    ln=lineNo;
    cn=colNo;
    token = makeToken(SB_PERIOD, lineNo, colNo);
    readChar();
    if(charCodes[currentChar]==CHAR_RPAR) {
      free(token);
      token = makeToken(SB_RSEL, ln,cn);
      readChar();
    }
    return token;
    
  case CHAR_COLON:
    ln=lineNo;
    cn=colNo;
    token = makeToken(SB_COLON, lineNo, colNo);
    readChar();
    if(charCodes[currentChar]==CHAR_EQ) {
      free(token);
      token = makeToken(SB_ASSIGN, ln,cn);
      readChar();
    }
    return token;
    
  case CHAR_SEMICOLON:
    token = makeToken(SB_SEMICOLON, lineNo, colNo);
    readChar(); 
    return token;
    
  case CHAR_SINGLEQUOTE: return readConstChar();
    
  case CHAR_LPAR:
    ln=lineNo;
    cn=colNo;
    token = makeToken(SB_LPAR, lineNo, colNo);
    readChar();
    if(charCodes[currentChar]==CHAR_TIMES) {
      free(token);
      skipComment();      
      return getToken();
    }
    else if(charCodes[currentChar]==CHAR_PERIOD){
      free(token);
      token = makeToken(SB_LSEL, ln,cn);
      readChar();
      return token;
    }
    else return token;
    
  case CHAR_RPAR:
    token = makeToken(SB_RPAR, lineNo, colNo);
    readChar(); 
    return token;
    
  default:
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALIDSYMBOL, lineNo, colNo);
    readChar(); 
    return token;
  }
}


/******************************************************************/

void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_EOF: printf("TK_EOF\n"); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_OF: printf("KW_OF\n"); break;
  case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
  case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
  case KW_BEGIN: printf("KW_BEGIN\n"); break;
  case KW_END: printf("KW_END\n"); break;
  case KW_CALL: printf("KW_CALL\n"); break;
  case KW_IF: printf("KW_IF\n"); break;
  case KW_THEN: printf("KW_THEN\n"); break;
  case KW_ELSE: printf("KW_ELSE\n"); break;
  case KW_WHILE: printf("KW_WHILE\n"); break;
  case KW_DO: printf("KW_DO\n"); break;
  case KW_FOR: printf("KW_FOR\n"); break;
  case KW_TO: printf("KW_TO\n"); break;

  case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
  case SB_COLON: printf("SB_COLON\n"); break;
  case SB_PERIOD: printf("SB_PERIOD\n"); break;
  case SB_COMMA: printf("SB_COMMA\n"); break;
  case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
  case SB_EQ: printf("SB_EQ\n"); break;
  case SB_NEQ: printf("SB_NEQ\n"); break;
  case SB_LT: printf("SB_LT\n"); break;
  case SB_LE: printf("SB_LE\n"); break;
  case SB_GT: printf("SB_GT\n"); break;
  case SB_GE: printf("SB_GE\n"); break;
  case SB_PLUS: printf("SB_PLUS\n"); break;
  case SB_MINUS: printf("SB_MINUS\n"); break;
  case SB_TIMES: printf("SB_TIMES\n"); break;
  case SB_SLASH: printf("SB_SLASH\n"); break;
  case SB_LPAR: printf("SB_LPAR\n"); break;
  case SB_RPAR: printf("SB_RPAR\n"); break;
  case SB_LSEL: printf("SB_LSEL\n"); break;
  case SB_RSEL: printf("SB_RSEL\n"); break;
  }
}

int scan(char *fileName) {
  Token *token;

  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  token = getToken();
  while (token->tokenType != TK_EOF) {
    printToken(token);
    free(token);
    token = getToken();
  }

  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

/******************************************************************/

/*int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("scanner: no input file.\n");
    return -1;
  }

  if (scan(argv[1]) == IO_ERROR) {
    printf("Can\'t read input file!\n");
    return -1;
  }

  return 0;
  }*/

int main()
{

if (scan("example0.kpl") == IO_ERROR) {
   printf("Can\'t read input file!\n");
      }
return 0;

}


