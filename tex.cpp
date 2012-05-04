#include <iostream>

#include "tex.h"

void Formula::parse(std::vector<Token>* input,
                    int index,
                    TokenType last)
{
  int current = 0;
  for (int i = index; (*input)[i].type != last; i++)
  {
    switch ((*input)[i].type)
    {
      case EMPTY:
      case SYMBOL:
        content.push_back(new Lexem((*input)[i].varName));
        current++;
        break;
      case UNARY:
        {
          if ((*input)[i].pos == PREFIX)
          {
            content.push_back(new unary::Prefix((*input)[i].varName));
          }
          else
          {
            content.push_back(new unary::Postfix((*input)[i].varName));
          }
          current++;
        }
        break;
      case BINARY:
        {
          if ((*input)[i].pos == PREFIX)
          {
            content.push_back(new binary::Prefix((*input)[i].varName));
          }
          else if ((*input)[i].pos == INFIX)
          {
            content.push_back(new binary::Infix((*input)[i].varName));
          }
          else
          {
            content.push_back(new binary::Postfix((*input)[i].varName));
          }
          current++;
        }
        break;
      case OPEN_BRACKET:
        content.push_back(new Formula(input, i + 1, CLOSE_BRACKET));
        {
          int cnt = 1;
          i++;
          while(cnt > 0)
          {
            if ((*input)[i].type == OPEN_BRACKET)
            {
              cnt++;
            }
            else if ((*input)[i].type == CLOSE_BRACKET)
            {
              cnt--;
            }
            i++;
          }
          i--;
        }
        current++;
        break;
      default:
        std::cout << "Something gone wrong... ";
    }
  }
}

Node::Node(std::string str = "")
{
  subscript = supscript = NULL;
  name = str;
  used = false;
}

Binary::Binary(std::string str): Operation(str)
{
  actual = NULL;
  leftOperand = rightOperand = NULL;
}

Node::~Node()
{
  if (subscript != NULL)
    delete subscript;
  if (supscript != NULL)
    delete supscript;
}

Formula::Formula(std::vector<Token>* input,
                 int index,
                 TokenType last)
{
  parse(input, index, last);
}

Formula::~Formula()
{
  while (content.size())
  {
    delete content[0];
    content.erase(content.begin());
  }
}

void Node::setUse()
{
  used = true;
}

bool Node::isUsed()
{
  return used;
}

void Node::enTree(std::vector<Node*>* stack, int index)
{

}

void Formula::enTree(std::vector<Node*>* stack, int index)
{
  for (int i = 0; i < content.size(); i++)
  {
    content[i]->checkScript(&content, i);
  }

  for (int i = 0; i < content.size(); i++)
  {
    content[i]->enTree(&content, i);
  }

  for (int i = 0; i < content.size(); i++)
  {
    if (content[i]->isUsed())
    {
      content.erase(content.begin() + i);
      i--;
    }
  }
}

void unary::Prefix::enTree(std::vector<Node*>* stack, int index)
{
  operand = (*stack)[index + 1];
  operand->setUse();
}

void unary::Postfix::enTree(std::vector<Node*>* stack, int index)
{
  operand = (*stack)[index - 1];
  operand->setUse();
}

void binary::Prefix::enTree(std::vector<Node*>* stack, int index)
{
  leftOperand = (*stack)[index + 1];
  rightOperand = (*stack)[index + 2];
  leftOperand->setUse();
  rightOperand->setUse();
}

void Node::checkScript(std::vector<Node*>* stack, int index)
{

}

void binary::Infix::checkScript(std::vector<Node*>* stack, int index)
{
  if (name == "superscript")
  {
    (*stack)[index - 1]->setSuperScript((*stack)[index + 1]);
    delete (*stack)[index];
    stack->erase(stack->begin() + index + 1);
    stack->erase(stack->begin() + index);
    if (stack->size() > index)
    {
      (*stack)[index]->checkScript(stack, index);
    }
    return;
  }
  if (name == "subscript")
  {
    (*stack)[index - 1]->setSubScript((*stack)[index + 1]);
    delete (*stack)[index];
    stack->erase(stack->begin() + index + 1);
    stack->erase(stack->begin() + index);
    if (stack->size() > index)
    {
      (*stack)[index]->checkScript(stack, index);
    }
    return;
  }
}

void binary::Infix::enTree(std::vector<Node*>* stack, int index)
{
  leftOperand = (*stack)[index - 1];
  rightOperand = (*stack)[index + 1];
  leftOperand->setUse();
  rightOperand->setUse();
}

void binary::Postfix::enTree(std::vector<Node*>* stack, int index)
{
  leftOperand = (*stack)[index - 2];
  rightOperand = (*stack)[index - 1];
  leftOperand->setUse();
  rightOperand->setUse();
}

Unary::~Unary()
{
  if (actual != NULL)
    delete actual;
  delete operand;
}

Binary::~Binary()
{
  if (actual != NULL)
    delete actual;
  if (leftOperand != NULL)
    delete leftOperand;
  if (rightOperand != NULL)
    delete rightOperand;
}

std::string Node::putScripts()
{
  std::string tmp = "";
  if ((supscript != NULL) || (subscript != NULL))
  {
    tmp += '[';
    if (supscript != NULL)
    {
      tmp += supscript->put();
    }
    tmp += ", ";
    if (subscript != NULL)
    {
      tmp += subscript->put();
    }
    tmp += ']';
  }
  return tmp;
}

std::string Formula::put()
{
  std::string tmp = "{";
  for (int i = 0; i < content.size(); i++)
  {
    tmp += content[i]->put() + "; ";
  }
  tmp += "\b\b}" + putScripts();
  return tmp;
}

std::string Lexem::put()
{
  return  "'" +
          name +
          "'" +
          putScripts();
}

std::string Unary::put()
{
  return  getName() +
          putScripts() +
          "(" +
          getOperand()->put() +
          ")";
}

std::string Binary::put()
{
  return  getName() +
          putScripts() +
          "(" +
          getLeft()->put() +
          ", " +
          getRight()->put() +
          ")";
}

void Node::setSuperScript(Node* n)
{
  supscript = n;
}

void Node::setSubScript(Node* n)
{
  subscript = n;
}
