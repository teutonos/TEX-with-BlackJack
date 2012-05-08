#ifndef OPERATORS_H
#define OPERATORS_H

#include "tex.h"

#define INTEGRAL_KOEF (3./2)
#define INT_TOP_OFF (3./15)
#define INT_BOTT_OFF (2./15)
#define FRACTION_KOEF 0.95

class Integral: public unary::Prefix
{
  public:
    virtual void calc(HDC hdc, double multiplier);
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
    virtual void calc(HDC hdc, double multiplier);
    virtual void draw(HDC hdc,
                      int x,
                      int y,
                      HAlign h = HA_CENTER,
                      VAlign v = VA_MIDDLE,
                      double multiplier = 1.
                     );
    Over(std::wstring str) : binary::Infix(str) {}
    ~Over();
};


#endif
