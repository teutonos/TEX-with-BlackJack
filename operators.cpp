#include "operators.h"
#include "winwrap.h"

int Integral::getWidth(double multiplier)
{
  return (width = FONT_WIDTH + operand->getWidth()) * multiplier;
}

int Integral::getHeight(double multiplier)
{
  return height = operand->getHeight(multiplier);
}

void Integral::draw(HDC hdc, int x, int y, HAlign h, VAlign v, double multiplier)
{
  switch(h)
  {
    case HA_LEFT:
      break;
    case HA_CENTER:
      x -= getWidth(multiplier) / 2;
      break;
    case HA_RIGHT:
      x -= getWidth(multiplier);
      break;
  }
  switch(v)
  {
    case VA_TOP:
      break;
    case VA_MIDDLE:
      y -= getHeight(multiplier) / 2;
      break;
    case VA_BOTTOM:
      y -= getHeight(multiplier);
      break;
  }

  winWrap::setFont(hdc,
                   operand->getHeight(multiplier) * INTEGRAL_KOEF + FONT_SIZE,
                   FONT_SIZE * multiplier
                  );
  TextOutW(hdc,
           x,
           y - FONT_SIZE * 0.5,
           name.data(),
           name.length()
          );

  x += FONT_SIZE * multiplier;
  if (supscript != NULL)
  {
    y -= FONT_SIZE * multiplier * 0.2;
    supscript->draw(hdc, x, y, HA_CENTER, VA_BOTTOM, multiplier * 0.5);
    y += FONT_SIZE * multiplier * 0.2;
  }

  if (subscript != NULL)
  {
    y += getHeight(multiplier);
    x -= FONT_SIZE * multiplier / 4;
    subscript->draw(hdc, x, y, HA_CENTER, VA_TOP, multiplier * 0.5);
    y -= getHeight(multiplier);
    x += FONT_SIZE * multiplier / 4;
  }

  y += getHeight(multiplier) * INTEGRAL_KOEF + FONT_SIZE / 2;
  operand->draw(hdc, x, y, HA_LEFT, VA_MIDDLE, multiplier);
}

Integral::~Integral()
{
  delete operand;
}

int Over::getWidth(double multiplier)
{
  return
    (width = max(leftOperand->getWidth(FRACTION_KOEF),
                 rightOperand->getWidth(FRACTION_KOEF))
    ) * multiplier;
}

int Over::getHeight(double multiplier)
{
  return
    (height = (leftOperand->getHeight(FRACTION_KOEF) +
               rightOperand->getHeight(FRACTION_KOEF)) +
               FONT_SIZE / 2
    ) * multiplier;
}

void Over::draw(HDC hdc, int x, int y, HAlign h, VAlign v, double multiplier)
{
  switch(h)
  {
    case HA_LEFT:
      break;
    case HA_CENTER:
      x -= getWidth(multiplier) / 2;
      break;
    case HA_RIGHT:
      x -= getWidth(multiplier);
      break;
  }
  switch(v)
  {
    case VA_TOP:
      break;
    case VA_MIDDLE:
      y -= getHeight(multiplier) / 2;
      break;
    case VA_BOTTOM:
      y -= getHeight(multiplier);
      break;
  }

  y += getHeight(multiplier) / 2;

  Rectangle(hdc, x, y, x + getWidth(multiplier), y+1);

  x += getWidth(multiplier) / 2;

  {
    y -= FONT_SIZE / 4;
    leftOperand->draw(hdc, x, y, HA_CENTER, VA_BOTTOM, 0.7);
    y += FONT_SIZE / 4;
  }

  {
    y += FONT_SIZE / 4;
    rightOperand->draw(hdc, x, y, HA_CENTER, VA_TOP, 0.7);
    y -= FONT_SIZE / 4;
  }

}

Over::~Over()
