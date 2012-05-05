#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <string>

#define C(a) case a:
#define CASENUM     C('0') C('1') C('2') C('3') C('4') C('5') C('6') C('7') C('8') C('9')

#define CASECHAR    C('a') C('b') C('c') C('d') C('e') C('f') C('g') C('h') C('i') C('j') C('k') C('l') C('m') \
                    C('n') C('o') C('p') C('q') C('r') C('s') C('t') C('u') C('v') C('w') C('x') C('y') C('z')

#define CASECAPCHAR C('A') C('B') C('C') C('D') C('E') C('F') C('G') C('H') C('I') C('J') C('K') C('L') C('M') \
                    C('N') C('O') C('P') C('Q') C('R') C('S') C('T') C('U') C('V') C('W') C('X') C('Y') C('Z')

#define CASESYM     C('#') C('$') C('%') C('^') C('&') C('_') C('{') C('}') C('~')

struct Token;
class Formula;

//blah

namespace tokenizer
{
  extern std::vector<std::string> keywords;
  extern std::vector<Token> keyTokens;
  void init();
};

enum TokenType
{
  UNKNOWN, EMPTY, SYMBOL, UNARY, BINARY,
  OPEN_BRACKET, CLOSE_BRACKET, ENDLINE
};

enum Place
{
  NONE, PREFIX, INFIX, POSTFIX
};

enum OpKind
{
  INTEGRAL,
  FRACTION,
  SUPERSCRIPT, SUBSCRIPT,

  LAST,
  NOT_OPERATOR
};

struct Token
{
  TokenType type;
  Place pos;
  int number;
  OpKind kind;
  std::string varName;

  Token(TokenType t, Place p, OpKind k, std::string name);
  Token();
};

std::vector<std::string>* parseString(std::string str);
std::vector<Token>* tokenize(std::string str);
Formula* makeTreeStack(std::string str);

bool isTerminating(char c);
bool isAlpha(char c);
bool isNum(char c);

#endif
