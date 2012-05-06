#include "winwrap.h"

HFONT winWrap::createFont(int size)
{
  return CreateFontA(size * 1.5,
                     size,
                     0,
                     0,
                     FW_DONTCARE,
                     0,
                     0,
                     0,
                     ANSI_CHARSET,
                     OUT_OUTLINE_PRECIS,
                     CLIP_DEFAULT_PRECIS,
                     PROOF_QUALITY,
                     VARIABLE_PITCH,
                     TEXT("Courier New")
                    );
}

HFONT winWrap::createFont(int vsize, int hsize)
{
  return CreateFontA(vsize * 1.5,
                     hsize,
                     0,
                     0,
                     FW_DONTCARE,
                     0,
                     0,
                     0,
                     ANSI_CHARSET,
                     OUT_OUTLINE_PRECIS,
                     CLIP_DEFAULT_PRECIS,
                     PROOF_QUALITY,
                     VARIABLE_PITCH,
                     TEXT("Courier New")
                    );
}

void winWrap::setFont (HDC hdc, int size)
{
  HFONT hFont = createFont(size);
  SelectObject(hdc, hFont);
  SetBkMode(hdc, TRANSPARENT);
}

void winWrap::setFont (HDC hdc, int vsize, int hsize)
{
  HFONT hFont = createFont(vsize, hsize);
  SelectObject(hdc, hFont);
  SetBkMode(hdc, TRANSPARENT);
}
