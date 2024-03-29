#include "tex.h"
#include "operators.h"

void Formula::parse(std::vector<Token>* input,
                    int index,
                    TokType last
                   )
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

Node::Node(std::wstring str = L"")
{
  subscript = supscript = NULL;
  name = str;
  used = false;
  width =
  height =
  bbLeft =
  bbTop = 0;
}

Unary::Unary(std::wstring str): Operation(str)
{
  if (str == L"integral")
  {
    actual = new Integral(L"∫");
  }
  else
  {
    actual = NULL;
  }
  operand = NULL;
}

Binary::Binary(std::wstring str): Operation(str)
{
  if (str == L"fraction")
  {
    actual = new Over(L"");
  }
  else
  {
    actual = NULL;
  }
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
                 TokType last
                ): Node()
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
  for (unsigned int i = 0; i < content.size(); i++)
  {
    content[i]->enTree(&content, i);
  }

  for (unsigned int i = 0; i < content.size(); i++)
  {
    if (content[i]->isUsed())
    {
      content.erase(content.begin() + i);
      i--;
    }
  }
}

void Formula::checkScript(std::vector<Node*>* stack, unsigned int index)
{
  for (unsigned int i = 0; i < content.size(); i++)
  {
    content[i]->checkScript(&content, i);
  }
}

void unary::Prefix::enTree(std::vector<Node*>* stack, int index)
{
  operand = (*stack)[index + 1];
  if (actual != NULL)
  {
    actual->setOperand(operand);
    actual->setSubScript(subscript);
    actual->setSuperScript(supscript);
    subscript = supscript = NULL;
  }
  operand->setUse();
}

void unary::Postfix::enTree(std::vector<Node*>* stack, int index)
{
  operand = (*stack)[index - 1];
  if (actual != NULL)
  {
    actual->setOperand(operand);
    actual->setSubScript(subscript);
    actual->setSuperScript(supscript);
    subscript = supscript = NULL;
  }
  operand->setUse();
}

void binary::Prefix::enTree(std::vector<Node*>* stack, int index)
{
  leftOperand = (*stack)[index + 1];
  rightOperand = (*stack)[index + 2];
  if (actual != NULL)
  {
    actual->setLeft(leftOperand);
    actual->setRight(rightOperand);
    actual->setSubScript(subscript);
    actual->setSuperScript(supscript);
    subscript = supscript = NULL;
  }
  leftOperand->setUse();
  rightOperand->setUse();
}

void Node::checkScript(std::vector<Node*>* stack, unsigned int index)
{

}

void binary::Infix::checkScript(std::vector<Node*>* stack, unsigned int index)
{
  if (name == L"superscript")
  {
    (*stack)[index - 1]->setSuperScript((*stack)[index + 1]);
    (*stack)[index + 1]->checkScript(stack, index + 1);
    (*stack)[index + 1]->enTree(stack, index + 1);
    delete (*stack)[index];
    stack->erase(stack->begin() + index + 1);
    stack->erase(stack->begin() + index);
    if (stack->size() > index)
    {
      (*stack)[index]->checkScript(stack, index);
    }
    return;
  }
  if (name == L"subscript")
  {
    (*stack)[index - 1]->setSubScript((*stack)[index + 1]);
    (*stack)[index + 1]->checkScript(stack, index + 1);
    (*stack)[index + 1]->enTree(stack, index + 1);
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
  if (actual != NULL)
  {
    actual->setLeft(leftOperand);
    actual->setRight(rightOperand);
    actual->setSubScript(subscript);
    actual->setSuperScript(supscript);
    subscript = supscript = NULL;
  }
  leftOperand->setUse();
  rightOperand->setUse();
}

void binary::Postfix::enTree(std::vector<Node*>* stack, int index)
{
  leftOperand = (*stack)[index - 2];
  rightOperand = (*stack)[index - 1];
  if (actual != NULL)
  {
    actual->setLeft(leftOperand);
    actual->setRight(rightOperand);
    actual->setSubScript(subscript);
    actual->setSuperScript(supscript);
    subscript = supscript = NULL;
  }
  leftOperand->setUse();
  rightOperand->setUse();
}

Unary::~Unary()
{
  if (actual != NULL)
  {
    delete actual;
  }
  else
  {
    delete operand;
  }
}

Binary::~Binary()
{
  if (actual != NULL)
  {
    delete actual;
  }
  else
  {
    delete leftOperand;
    delete rightOperand;
  }
}

std::wstring Node::putScripts()
{
  std::wstring tmp = L"";
  if ((supscript != NULL) || (subscript != NULL))
  {
    tmp += '[';
    if (supscript != NULL)
    {
      tmp += supscript->put();
    }
    tmp += L", ";
    if (subscript != NULL)
    {
      tmp += subscript->put();
    }
    tmp += ']';
  }
  return tmp;
}

std::wstring Formula::put()
{
  std::wstring tmp = L"{";
  for (unsigned int i = 0; i < content.size(); i++)
  {
    tmp += content[i]->put() + L"; ";
  }
  tmp = tmp.substr(0, tmp.length() - 2);
  tmp += L"}" + putScripts();
  return tmp;
}

std::wstring Lexem::put()
{
  return  L"'" +
          name +
          L"'" +
          putScripts();
}

std::wstring Unary::put()
{
  return  getName() +
          putScripts() +
          L"(" +
          getOperand()->put() +
          L")";
}

std::wstring Binary::put()
{
  return  getName() +
          putScripts() +
          L"(" +
          getLeft()->put() +
          L", " +
          getRight()->put() +
          L")";
}

void Node::setSuperScript(Node* n)
{
  supscript = n;
}

void Node::setSubScript(Node* n)
{
  subscript = n;
}
