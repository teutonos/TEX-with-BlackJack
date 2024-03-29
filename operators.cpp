#include "operators.h"
#include "winwrap.h"

void Integral::calc(HDC hdc, double multiplier)
{
  if (width*height == 0)
  {
    operand->calc(hdc, multiplier);
    width = FONT_WIDTH * multiplier + operand->getWidth();
    height = operand->getHeight() +
             FONT_HEIGHT * multiplier +
             getSubScriptHeight() +
             getSuperScriptHeight();
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

  bbTop = y;
  bbBottom = y + getHeight();

  HFONT hf = winWrap::setFont(hdc,
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
  DeleteObject(hf);

  x += FONT_WIDTH * multiplier;

  y += getHeight() / 2;
  operand->draw(hdc, x, y, HA_LEFT, VA_MIDDLE, multiplier);

  if (supscript != NULL)
  {
    y -= getHeight() / 2;
    bbTop = y - supscript->getHeight();
    supscript->draw(hdc, x, y, HA_RIGHT, VA_BOTTOM, multiplier * SCRIPT_SIZE);
    y += getHeight() / 2;
  }

  if (subscript != NULL)
  {
    y += getHeight() / 2;
    bbBottom = y + subscript->getHeight();
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
  height = max(leftOperand->getHeight(), rightOperand->getHeight()) * 2;
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
      y += leftOperand->getHeight();
      break;
    case VA_MIDDLE:
      break;
    case VA_BOTTOM:
      y -= rightOperand->getHeight();
      break;
  }

  bbTop = y - leftOperand->getHeight();
  bbBottom = y + rightOperand->getHeight();

  Rectangle(hdc, x, y, x + getWidth(), y + 1);

  x += getWidth() / 2;

  {
    if (leftOperand->getSubScriptHeight())
      y -= leftOperand->getSubScriptHeight() - leftOperand->getHeight() / 2;
    leftOperand->draw(hdc, x, y, HA_CENTER, VA_BOTTOM, FRACTION_KOEF * multiplier);
    if (leftOperand->getSubScriptHeight())
      y += leftOperand->getSubScriptHeight() - leftOperand->getHeight() / 2;
  }

  {
    if (rightOperand->getSuperScriptHeight())
      y += rightOperand->getSuperScriptHeight() - rightOperand->getHeight() / 2;
    rightOperand->draw(hdc, x, y, HA_CENTER, VA_TOP, FRACTION_KOEF * multiplier);
    if (rightOperand->getSuperScriptHeight())
      y -= rightOperand->getSuperScriptHeight() - rightOperand->getHeight() / 2;
  }
}

Over::~Over()
{
//  delete leftOperand;
//  delete rightOperand;
}
