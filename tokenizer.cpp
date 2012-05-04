#include "tokenizer.h"
#include "tex.h"

namespace tokenizer
{
  std::vector<std::string> keywords;
  std::vector<Token> keyTokens;
};

void tokenizer::init()
{
  keywords.push_back("\\int");
  keywords.push_back("\\over");
  keywords.push_back("_");
  keywords.push_back("^");

  keyTokens.push_back(Token(UNARY, PREFIX, INTEGRAL, "integral"));
  keyTokens.push_back(Token(BINARY, INFIX, FRACTION, "fraction"));
  keyTokens.push_back(Token(BINARY, INFIX, SUBSCRIPT, "subscript"));
  keyTokens.push_back(Token(BINARY, INFIX, SUPERSCRIPT, "superscript"));
}

bool isTerminating(char c)
{
  return
    (
      (c == '^')  ||
      (c == '$')  ||
      (c == '{')  ||
      (c == '}')  ||
      (c == '&')  ||
      (c == '~')  ||
      (c == '\\') ||
      (c == '#')  ||
      (c == '_')  ||
      (c == '%')
    );
}

bool isAlpha(char c)
{
  return
    (
      ((c >= 'A')  && (c <= 'Z'))  ||
      ((c >= 'a')  && (c <= 'z'))
    );
}

bool isNum(char c)
{
  return ((c >= '0')  && (c <= '9'));
}

bool genSpace(char c)
{
  return ((c == ' ') || (c == '\n') || (c == '\r') || (c == '\t'));
}

std::vector<std::string>* parseString(std::string str)
{
  std::vector<std::string>* result = new std::vector<std::string>;
  str += " ";

  for(int i = 0; i < str.size(); i++)
  {
    switch(str[i])
    {
      case '\\':
        {
          std::string name("\\");
          i++;
          if (isTerminating(str[i])) //экранируемый символ
          {
            name += str[i++];
          }
          else
          {
            while(isAlpha(str[i])) //служебное слово
            {
              name += str[i++];
            }
          }
          i--;

          if(name != "")
          {
            result->push_back(name);
          }
        }
        break;

        CASENUM
        CASECHAR
        CASECAPCHAR
        {
          std::string name("");
          while( (isAlpha(str[i])) || (isNum(str[i])) ) //переменная/число
          {
            name += str[i++];
          }
          i--;

          if(name != "")
          {
            result->push_back(name);
          }
        }
        break;

//      CASESYM
//          result->push_back(std::string(1, str[i]));
//          break;

      default:
        if (!genSpace(str[i]))
        {
          result->push_back(std::string(1, str[i]));
        }
        break;
    }
  }

  for(int i = 0; i < result->size(); i++)
  {
    if (((*result)[i] == "{")   && ((*result)[i+1] == "}"))
    {
      result->erase(result->begin() + i + 1);
      (*result)[i] = "{}";
    }
  }
  return result;
}

std::vector<Token>* tokenize(std::string str)
{
  std::vector<Token>* result = new std::vector<Token>;
  std::vector<std::string>* subStrs = parseString(str);

  for (int i = 0; i < subStrs->size(); i++)
  {
    bool key = false;
    for (int j = 0; j < LAST; j++)
    {
      if ((*subStrs)[i].compare(tokenizer::keywords[j]) == 0)
      {
        result->push_back(tokenizer::keyTokens[j]);
        key = true;
        break;
      }
    }
    if (key)
      continue;
    Token t;

    if ((*subStrs)[i] == "{}")
    {
      t.type = EMPTY;
      t.varName = "\\empty";
    }
    else if ((*subStrs)[i] == "{")
    {
      t.type = OPEN_BRACKET;
    }
    else if ((*subStrs)[i] == "}")
    {
      t.type = CLOSE_BRACKET;
    }
    else
    {
      t.type = SYMBOL;
      t.varName = (*subStrs)[i];
      if (t.varName[0] == '\\')
        t.varName.erase(t.varName.begin());
    }
    result->push_back(t);
  }

  Token t;
  t.type = ENDLINE;
  result->push_back(t);

  for (int i = 0; i < result->size() - 2; i++)
  {
  	if (((*result)[i].type == OPEN_BRACKET) &&
        ((*result)[i+2].type == CLOSE_BRACKET))
    {
      result->erase(result->begin() + i + 2);
      result->erase(result->begin() + i);
    }
  }

  delete subStrs;
  return result;
}

Formula* makeTreeStack(std::string str)
{
  std::vector<Token>* pTokenVector = tokenize(str);
  Formula* result = new Formula(pTokenVector);

  result->enTree(result->getContent(), 0);

  delete pTokenVector;
  return result;
}

Token::Token(TokenType t, Place p, OpKind k, std::string name)
{
  type = t;
  pos = p;
  kind = k;
  varName = name;
}

Token::Token()
{
  type    = UNKNOWN;
  pos     = NONE;
  kind    = NOT_OPERATOR;
  varName = "";
}
