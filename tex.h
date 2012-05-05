#ifndef TEX_H
#define TEX_H

#include <vector>
#include <string>

#include "tokenizer.h"

//testiiiiiing

class Node
{
  protected:
    bool used;
    int scriptOffset;
    Node  *subscript,
          *supscript;
    std::string name;
  public:
//    virtual double getWidth();
//    virtual double getHeight();
//    virtual void draw();
    virtual ~Node();
    Node(std::string str);
    virtual void enTree(std::vector<Node*>* stack, int index);
    virtual void checkScript(std::vector<Node*>* stack, int index);
    void setUse();
    bool isUsed();
    std::string getName() {return name;}
    virtual std::string put() {}
    std::string putScripts();
    void setSuperScript(Node*);
    void setSubScript(Node*);
};

class Formula: public Node
{
  protected:
    std::vector<Node*> content;
  public:
    Formula(std::vector<Token>*, int = 0, TokenType = ENDLINE);
    void parse(std::vector<Token>*, int = 0, TokenType = ENDLINE);
    virtual void enTree(std::vector<Node*>* stack, int index);
    virtual ~Formula();
    std::vector<Node*>* getContent() {return &content;}
    virtual std::string put();
};

class Lexem: public Node
{
  protected:
  public:
    Lexem(std::string str = ""): Node(str) {}
    virtual std::string put();
};

class Operation: public Node
{
  public:
    Operation(std::string str): Node(str) {}
};

class Unary: public Operation
{
  protected:
    Unary* actual;
    Node *operand;
  public:
    Unary(std::string str): Operation(str) {actual = NULL;}
    virtual ~Unary();
    Node* getOperand() {return operand;}
    virtual std::string put();
};

class Binary: public Operation
{
  protected:
    Binary* actual;
    Node  *leftOperand,
          *rightOperand;
  public:
    Binary(std::string str);
    virtual ~Binary();
    Node* getLeft() {return leftOperand;}
    Node* getRight() {return rightOperand;}
    virtual std::string put();
};

namespace unary
{
  class Prefix: public Unary
  {
    public:
      virtual void enTree(std::vector<Node*>* stack, int index);
      Prefix(std::string str): Unary(str) {}
  };

  class Postfix: public Unary
  {
    public:
      virtual void enTree(std::vector<Node*>* stack, int index);
      Postfix(std::string str): Unary(str) {}
  };
};

namespace binary
{
  class Prefix: public Binary
  {
    public:
      virtual void enTree(std::vector<Node*>* stack, int index);
      Prefix(std::string str): Binary(str) {}
  };

  class Infix: public Binary
  {
    public:
      virtual void enTree(std::vector<Node*>* stack, int index);
      Infix(std::string str): Binary(str) {}
      virtual void checkScript(std::vector<Node*>* stack, int index);
  };

  class Postfix: public Binary
  {
    public:
      virtual void enTree(std::vector<Node*>* stack, int index);
      Postfix(std::string str): Binary(str) {}
  };
};

#endif
