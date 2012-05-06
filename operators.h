#ifndef OPERATORS_H
#define OPERATORS_H

#include "tex.h"

#define INTEGRAL_KOEF (5/3)

class Integral: public unary::Prefix
{
  public:
    virtual int getWidth(double multiplier = 1.);
    virtual int getHeight(double multiplier = 1.);
    virtual void draw(HDC hdc,
                      int x,
                      int y,
                      HAlign h = HA_CENTER,
                      VAlign v = VA_MIDDLE,
                      double multiplier = 1.
                      );
    Integral(std::wstring str) : unary::Prefix(str) {}
    ~Integral();
};

class Over: public binary::Infix
{
  public:
    virtual int getWidth(double multiplier = 1.);
    virtual int getHeight(double multiplier = 1.);
    virtual void draw(HDC hdc,
                      int x,
                      int y,
                      HAlign h = HA_CENTER,
                      VAlign v = VA_MIDDLE,
                      double multiplier = 1.
                      );
    Over(std::wstring str) : binary::Infix(str) {}
};


#endif
