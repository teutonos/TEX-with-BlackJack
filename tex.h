#ifndef TEX_H
#define TEX_H

#include <vector>
#include <string>
#include <windows.h>
#include <iostream>

#include "tokenizer.h"

#define max(a,b) (((a)>(b))?(a):(b))
#define min(a,b) (((a)<(b))?(a):(b))
#define FONT_SIZE 20
#define FONT_WIDTH FONT_SIZE
#define FONT_HEIGHT FONT_SIZE
#define SCRIPT_SIZE 0.78

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
        width,
        height,
        bbTop,
        bbLeft,
        bbBottom,
        bbRight;
    Node  *subscript,
          *supscript;
    std::wstring name;
  public:
    virtual void calc(HDC hdc, double multiplier) = 0;
    virtual int getWidth();
    virtual int getHeight();
    virtual void draw(HDC hdc,
                      int x,
                      int y,
                      HAlign h = HA_CENTER,
                      VAlign v = VA_MIDDLE,
                      double multiplier = 1.
                     ) = 0;
    virtual ~Node();
    Node(std::wstring str);
    virtual void enTree(std::vector<Node*>* stack, int index);
    virtual void checkScript(std::vector<Node*>* stack, unsigned int index);
    void setUse();
    bool isUsed();
    std::wstring getName()
    {
      return name;
    }
    virtual std::wstring put() = 0;
    std::wstring putScripts();
    void setSuperScript(Node*);
    void setSubScript(Node*);
    Node* getSuperScript() {return supscript;}
    Node* getSubScript() {return subscript;}
    virtual int getSuperScriptHeight();
    virtual int getSubScriptHeight();
    virtual int getBBoxTop();
    virtual int getBBoxLeft();
    virtual int getBBoxBottom();
    virtual int getBBoxRight();
};

class Formula: public Node
{
  protected:
    std::vector<Node*> content;
  public:
    virtual void calc(HDC hdc, double multiplier);
    virtual void draw(HDC hdc,
                      int x,
                      int y,
                      HAlign h = HA_CENTER,
                      VAlign v = VA_MIDDLE,
                      double multiplier = 1.
                     );
    Formula(std::vector<Token>*, int = 0, TokType = ENDLINE);
    void parse(std::vector<Token>*, int = 0, TokType = ENDLINE);
    virtual void enTree(std::vector<Node*>* stack, int index);
    virtual void checkScript(std::vector<Node*>* stack, unsigned int index);
    virtual ~Formula();
    std::vector<Node*>* getContent()
    {
      return &content;
    }
    virtual std::wstring put();
    virtual int getSuperScriptHeight();
    virtual int getSubScriptHeight();
};

class Lexem: public Node
{
  protected:
  public:
    virtual void calc(HDC hdc, double multiplier);
    virtual void draw(HDC hdc,
                      int x,
                      int y,
                      HAlign h = HA_CENTER,
                      VAlign v = VA_MIDDLE,
                      double multiplier = 1.
                     );
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
    virtual void calc(HDC hdc, double multiplier);
    virtual int getWidth();
    virtual int getHeight();
    virtual void draw(HDC hdc,
                      int x,
                      int y,
                      HAlign h = HA_CENTER,
                      VAlign v = VA_MIDDLE,
                      double multiplier = 1.
                     );
    Unary(std::wstring str);
    virtual ~Unary();
    Node* getOperand()
    {
      return operand;
    }
    void setOperand(Node* n)
    {
      operand = n;
    }
    virtual std::wstring put();
};

class Binary: public Operation
{
  protected:
    Binary* actual;
    Node  *leftOperand,
    *rightOperand;
  public:
    virtual void calc(HDC hdc, double multiplier);
    virtual int getWidth();
    virtual int getHeight();
    virtual void draw(HDC hdc,
                      int x,
                      int y,
                      HAlign h = HA_CENTER,
                      VAlign v = VA_MIDDLE,
                      double multiplier = 1.
                     );
    Binary(std::wstring str);
    virtual ~Binary();
    Node* getLeft()
    {
      return leftOperand;
    }
    Node* getRight()
    {
      return rightOperand;
    }
    void setLeft(Node* n)
    {
      leftOperand = n;
    }
    void setRight(Node* n)
    {
      rightOperand = n;
    }
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
