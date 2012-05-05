#include "tokenizer.h"
#include "tex.h"

namespace tokenizer
{
  std::vector<std::wstring> keywords;
  std::vector<Token> keyTokens;
};

void tokenizer::init()
{
  keywords.push_back(L"\\int");
  keywords.push_back(L"\\over");
  keywords.push_back(L"_");
  keywords.push_back(L"^");

  keyTokens.push_back(Token(UNARY, PREFIX, INTEGRAL, L"integral"));
  keyTokens.push_back(Token(BINARY, INFIX, FRACTION, L"fraction"));
  keyTokens.push_back(Token(BINARY, INFIX, SUBSCRIPT, L"subscript"));
  keyTokens.push_back(Token(BINARY, INFIX, SUPERSCRIPT, L"superscript"));
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

std::vector<std::wstring>* parseString(std::wstring str)
{
  std::vector<std::wstring>* result = new std::vector<std::wstring>;
  str += L" ";

  for(int i = 0; i < str.size(); i++)
  {
    switch(str[i])
    {
      case '\\':
        {
          std::wstring name(L"\\");
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

          if(name != L"")
          {
            result->push_back(name);
          }
        }
        break;

        CASENUM
        CASECHAR
        CASECAPCHAR
        {
          std::wstring name(L"");
          while( (isAlpha(str[i])) || (isNum(str[i])) ) //переменная/число
          {
            name += str[i++];
          }
          i--;

          if(name != L"")
          {
            result->push_back(name);
          }
        }
        break;

//      CASESYM
//          result->push_back(std::wstring(1, str[i]));
//          break;

      default:
        if (!genSpace(str[i]))
        {
          result->push_back(std::wstring(1, str[i]));
        }
        break;
    }
  }

  for(int i = 0; i < result->size(); i++)
  {
    if (((*result)[i] == L"{")   && ((*result)[i+1] == L"}"))
    {
      result->erase(result->begin() + i + 1);
      (*result)[i] = L"{}";
    }
  }
  return result;
}

std::vector<Token>* tokenize(std::wstring str)
{
  std::vector<Token>* result = new std::vector<Token>;
  std::vector<std::wstring>* subStrs = parseString(str);

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

    if ((*subStrs)[i] == L"{}")
    {
      t.type = EMPTY;
      t.varName = L"\\empty";
    }
    else if ((*subStrs)[i] == L"{")
    {
      t.type = OPEN_BRACKET;
    }
    else if ((*subStrs)[i] == L"}")
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

Formula* makeTreeStack(std::wstring str)
{
  std::vector<Token>* pTokenVector = tokenize(str);
  Formula* result = new Formula(pTokenVector);

  result->enTree(result->getContent(), 0);

  delete pTokenVector;
  return result;
}

Token::Token(TokType t, Place p, OpKind k, std::wstring name)
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
  varName = L"";
}
