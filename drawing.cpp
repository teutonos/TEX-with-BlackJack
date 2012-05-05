#include "tex.h"

#define max(a,b) ((a)>(b))?(a):(b)

int Formula::getWidth(double multiplier)
{
  if (width == 0)
  {
    for (int i = 0; i < content.size(); i++)
    {
      width += content[i]->getWidth(multiplier);
    }
  }
  return width*multiplier;
}

int Formula::getHeight(double multiplier)
{
  if (height == 0)
  {
    for (int i = 0; i < content.size(); i++)
    {
      height = max(height,content[i]->getHeight(multiplier));
    }
  }
  return height*multiplier;
}

void Formula::draw(HDC hdc, int x, int y, HAlign h, VAlign v, HFONT* font)
{
  TextOut(hdc, x - 127, y - 15, "Да рисую я, рисую", 17);
}
