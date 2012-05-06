#include "tex.h"
#include "winwrap.h"

int Formula::getWidth(double multiplier)
{
  if (width == 0)
  {
    for (unsigned int i = 0; i < content.size(); i++)
    {
      width += content[i]->getWidth(multiplier);
    }
  }
  return width * multiplier;
}

int Formula::getHeight(double multiplier)
{
  if (height == 0)
  {
    for (unsigned int i = 0; i < content.size(); i++)
    {
      height = max(height, content[i]->getHeight(multiplier));
    }
  }
  return height * multiplier;
}

void Formula::draw(HDC hdc,
                   int x,
                   int y,
                   HAlign h,
                   VAlign v,
                   double multiplier
                  )
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

  for (unsigned int i = 0; i < content.size(); i++)
  {
  	content[i]->draw(hdc, x, y, h, v, multiplier);
  	x += content[i]->getWidth(multiplier);
  }

  if (supscript != NULL)
  {
    supscript->draw(hdc, x, y, HA_CENTER, VA_BOTTOM, multiplier * 0.5);
  }

  if (subscript != NULL)
  {
    y += getHeight(multiplier);
    subscript->draw(hdc, x, y, HA_CENTER, VA_TOP, multiplier * 0.5);
    y -= getHeight(multiplier);
  }
}

int Lexem::getWidth(double multiplier)
{
  return (width = FONT_WIDTH * name.length()) * multiplier;
}

int Lexem::getHeight(double multiplier)
{
  return (height = FONT_HEIGHT) * multiplier;
}

void Lexem::draw(HDC hdc,
                 int x,
                 int y,
                 HAlign h,
                 VAlign v,
                 double multiplier
                )
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

  winWrap::setFont(hdc, FONT_SIZE * multiplier);

  TextOutW(hdc, x, y, name.data(), name.length());

  x += getWidth(multiplier);

  if (supscript != NULL)
  {
    supscript->draw(hdc, x, y, HA_CENTER, VA_BOTTOM, multiplier * 0.5);
  }

  if (subscript != NULL)
  {
    y += getHeight(multiplier);
    subscript->draw(hdc, x, y, HA_CENTER, VA_TOP, multiplier * 0.5);
    y -= getHeight(multiplier);
  }
}

int Unary::getWidth(double multiplier)
{
  if (width == 0)
  {
    width = actual->getWidth();
  }
  return width * multiplier;
}

int Unary::getHeight(double multiplier)
{
  if (height == 0)
  {
    height = actual->getHeight();
  }
  return height * multiplier;
}

void Unary::draw(HDC hdc,
                 int x,
                 int y,
                 HAlign h,
                 VAlign v,
                 double multiplier
                )
{
  actual->draw(hdc, x, y, h, v, multiplier);
}

int Binary::getWidth(double multiplier)
{
  if (width == 0)
  {
    width = actual->getWidth();
  }
  return width * multiplier;
}

int Binary::getHeight(double multiplier)
{
  if (height == 0)
  {
    height = actual->getHeight();
  }
  return height * multiplier;
}

void Binary::draw(HDC hdc,
                  int x,
                  int y,
                  HAlign h,
                  VAlign v,
                  double multiplier)
{
  actual->draw(hdc, x, y, h, v, multiplier);
}
