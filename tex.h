#ifndef TEX_H
#define TEX_H

#include <vector>
#include <string>
#include <windows.h>

#include "tokenizer.h"

enum HAlign
{
  HA_LEFT, HA_CENTER, HA_RIGHT
};

enum VAlign
{
  VA_TOP, VA_MIDDLE, VA_BOTTOM
};

class Node
{
  protected:
    bool used;
    int scriptOffset,
        fontSize,
        width,
        height;
    Node  *subscript,
          *supscript;
    std::wstring name;
  public:
    virtual int getWidth(double multiplier = 1.) {}
    virtual int getHeight(double multiplier = 1.) {}
    virtual void draw(HDC hdc,
                      int x,
                      int y,
                      HAlign h = HA_CENTER,
                      VAlign v = VA_MIDDLE,
                      HFONT* font = NULL
                     ) {}
    virtual ~Node();
    Node(std::wstring str);
    virtual void enTree(std::vector<Node*>* stack, int index);
    virtual void checkScript(std::vector<Node*>* stack, int index);
    void setUse();
    bool isUsed();
    std::wstring getName() {return name;}
    virtual std::wstring put() {return L"";}
    std::wstring putScripts();
    void setSuperScript(Node*);
    void setSubScript(Node*);
};

class Formula: public Node
{
  protected:
    std::vector<Node*> content;
  public:
    virtual int getWidth(double multiplier = 1.);
    virtual int getHeight(double multiplier = 1.);
    virtual void draw(HDC hdc,
                      int x,
                      int y,
                      HAlign h = HA_CENTER,
                      VAlign v = VA_MIDDLE,
                      HFONT* font = NULL);
    Formula(std::vector<Token>*, int = 0, TokType = ENDLINE);
    void parse(std::vector<Token>*, int = 0, TokType = ENDLINE);
    virtual void enTree(std::vector<Node*>* stack, int index);
    virtual ~Formula();
    std::vector<Node*>* getContent() {return &content;}
    virtual std::wstring put();
};

class Lexem: public Node
{
  protected:
  public:
    Lexem(std::wstring str = L""): Node(str) {}
    virtual std::wstring put();
};

class Operation: public Node
{
  public:
    Operation(std::wstring str): Node(str) {}
};

class Unary: public Operation
{
  protected:
    Unary* actual;
    Node *operand;
  public:
    Unary(std::wstring str);
    virtual ~Unary();
    Node* getOperand() {return operand;}
    virtual std::wstring put();
};

class Binary: public Operation
{
  protected:
    Binary* actual;
    Node  *leftOperand,
          *rightOperand;
  public:
    Binary(std::wstring str);
    virtual ~Binary();
    Node* getLeft() {return leftOperand;}
    Node* getRight() {return rightOperand;}
    virtual std::wstring put();
};

namespace unary
{
  class Prefix: public Unary
  {
    public:
      virtual void enTree(std::vector<Node*>* stack, int index);
      Prefix(std::wstring str): Unary(str) {}
  };

  class Postfix: public Unary
  {
    public:
      virtual void enTree(std::vector<Node*>* stack, int index);
      Postfix(std::wstring str): Unary(str) {}
  };
};

namespace binary
{
  class Prefix: public Binary
  {
    public:
      virtual void enTree(std::vector<Node*>* stack, int index);
      Prefix(std::wstring str): Binary(str) {}
  };

  class Infix: public Binary
  {
    public:
      virtual void enTree(std::vector<Node*>* stack, int index);
      Infix(std::wstring str): Binary(str) {}
      virtual void checkScript(std::vector<Node*>* stack, unsigned int index);
  };

  class Postfix: public Binary
  {
    public:
      virtual void enTree(std::vector<Node*>* stack, int index);
      Postfix(std::wstring str): Binary(str) {}
  };
};

#endif
