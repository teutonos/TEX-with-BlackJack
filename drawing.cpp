#include "tex.h"
#include "winwrap.h"

int Node::getWidth()
{
  return width;
}

int Node::getHeight()
{
  return height;
}

void Formula::calc(HDC hdc, double multiplier)
{
  if (width == 0)
  {
    for (unsigned int i = 0; i < content.size(); i++)
    {
      content[i]->calc(hdc, multiplier);
      width += content[i]->getWidth();
    }
  }
  if (height == 0)
  {
    for (unsigned int i = 0; i < content.size(); i++)
    {
      content[i]->calc(hdc, multiplier);
      height = max(height, content[i]->getHeight());
    }
  }
}

void Formula::draw(HDC hdc,
                   int x,
                   int y,
                   HAlign h,
                   VAlign v,
                   double multiplier
                  )
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

  for (unsigned int i = 0; i < content.size(); i++)
  {
    content[i]->draw(hdc, x, y, HA_LEFT, VA_MIDDLE, multiplier);
    x += content[i]->getWidth();
  }

  if (supscript != NULL)
  {
    y -= getHeight() / 2;
    supscript->draw(hdc, x, y, HA_CENTER, VA_BOTTOM, multiplier * SCRIPT_SIZE);
    y += getHeight() / 2;
  }

  if (subscript != NULL)
  {
    y += getHeight() / 2;
    subscript->draw(hdc, x, y, HA_CENTER, VA_TOP, multiplier * SCRIPT_SIZE);
    y -= getHeight() / 2;
  }
}

void Lexem::calc(HDC hdc, double multiplier)
{
  if (width * height == 0)
  {
    winWrap::setFont(hdc, FONT_SIZE * multiplier);
    RECT r;
    DrawTextW(hdc, name.data(), name.length(), &r, DT_CALCRECT);
    width = r.right - r.left;
    height = r.bottom - r.top;
  }
}

void Lexem::draw(HDC hdc,
                 int x,
                 int y,
                 HAlign h,
                 VAlign v,
                 double multiplier
                )
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

  winWrap::setFont(hdc, FONT_SIZE * multiplier);

  TextOutW(hdc, x, y, name.data(), name.length());

  x += getWidth();

  if (supscript != NULL)
  {
    supscript->draw(hdc, x, y, HA_LEFT, VA_MIDDLE, multiplier * SCRIPT_SIZE);
  }

  if (subscript != NULL)
  {
    y += getHeight();
    subscript->draw(hdc, x, y, HA_LEFT, VA_MIDDLE, multiplier * SCRIPT_SIZE);
    y -= getHeight();
  }
}

void Unary::calc(HDC hdc, double multiplier)
{
  actual->calc(hdc, multiplier);
}

int Unary::getWidth()
{
  if (width == 0)
  {
    width = actual->getWidth();
  }
  return width;
}

int Unary::getHeight()
{
  if (height == 0)
  {
    height = actual->getHeight();
  }
  return height;
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

void Binary::calc(HDC hdc, double multiplier)
{
  actual->calc(hdc, multiplier);
}

int Binary::getWidth()
{
  if (width == 0)
  {
    width = actual->getWidth();
  }
  return width;
}

int Binary::getHeight()
{
  if (height == 0)
  {
    height = actual->getHeight();
  }
  return height;
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
