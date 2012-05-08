#include "operators.h"
#include "winwrap.h"

void Integral::calc(HDC hdc, double multiplier)
{
  if (width*height == 0)
  {
    operand->calc(hdc, multiplier);
    width = FONT_WIDTH + operand->getWidth();
    height = operand->getHeight() + FONT_HEIGHT;
  }
}

void Integral::draw(HDC hdc, int x, int y, HAlign h, VAlign v, double multiplier)
{
  calc(hdc, multiplier);
  switch(h)
  {
    case HA_LEFT:
      break;
    case HA_CENTER:
      x -= getWidth() / 2;
      break;
    case HA_RIGHT:
      x -= getWidth();
      break;
  }
  switch(v)
  {
    case VA_TOP:
      break;
    case VA_MIDDLE:
      y -= getHeight() / 2;
      break;
    case VA_BOTTOM:
      y -= getHeight();
      break;
  }

  winWrap::setFont(hdc,
                   getHeight() * INTEGRAL_KOEF,
                   FONT_WIDTH * multiplier
                  );
  y -= getHeight() * (INT_TOP_OFF) * INTEGRAL_KOEF;
  TextOutW(hdc,
           x,
           y,
           name.data(),
           name.length()
          );
  y += getHeight() * (INT_TOP_OFF) * INTEGRAL_KOEF;

  x += FONT_WIDTH * multiplier;

  y += getHeight() / 2;
  operand->draw(hdc, x, y, HA_LEFT, VA_MIDDLE, multiplier);

  if (supscript != NULL)
  {
    y -= getHeight() / 2;
    supscript->draw(hdc, x, y, HA_RIGHT, VA_BOTTOM, multiplier * SCRIPT_SIZE);
    y += getHeight() / 2;
  }

  if (subscript != NULL)
  {
    y += getHeight() / 2;
    subscript->draw(hdc, x, y, HA_RIGHT, VA_TOP, multiplier * SCRIPT_SIZE);
    y -= getHeight() / 2;
  }
}

Integral::~Integral()
{
  //delete operand;
}

void Over::calc(HDC hdc, double multiplier)
{
  leftOperand->calc(hdc, multiplier * FRACTION_KOEF);
  rightOperand->calc(hdc, multiplier * FRACTION_KOEF);
  width = max(leftOperand->getWidth(), rightOperand->getWidth());
  height = leftOperand->getHeight() + rightOperand->getHeight();
}

void Over::draw(HDC hdc, int x, int y, HAlign h, VAlign v, double multiplier)
{
  calc(hdc, multiplier);
  switch(h)
  {
    case HA_LEFT:
      break;
    case HA_CENTER:
      x -= getWidth() / 2;
      break;
    case HA_RIGHT:
      x -= getWidth();
      break;
  }
  switch(v)
  {
    case VA_TOP:
      y += getHeight() / 2;
      break;
    case VA_MIDDLE:
      break;
    case VA_BOTTOM:
      y -= getHeight() / 2;
      break;
  }

  Rectangle(hdc, x, y, x + getWidth(), y + 1);

  x += getWidth() / 2;

  {
    if (leftOperand->getSubScript() != NULL)
      y -= leftOperand->getSubScript()->getHeight() - leftOperand->getHeight() / 2;
    leftOperand->draw(hdc, x, y, HA_CENTER, VA_BOTTOM, FRACTION_KOEF * multiplier);
    if (leftOperand->getSubScript() != NULL)
      y += leftOperand->getSubScript()->getHeight() - leftOperand->getHeight() / 2;
  }

  {
    if (rightOperand->getSuperScript() != NULL)
      y += rightOperand->getSuperScript()->getHeight() - rightOperand->getHeight() / 2;
    rightOperand->draw(hdc, x, y, HA_CENTER, VA_TOP, FRACTION_KOEF * multiplier);
    if (rightOperand->getSuperScript() != NULL)
      y -= rightOperand->getSuperScript()->getHeight() - rightOperand->getHeight() / 2;
  }
}

Over::~Over()
{
//  delete leftOperand;
//  delete rightOperand;
}
