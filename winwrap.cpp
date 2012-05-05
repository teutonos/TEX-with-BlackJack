#include "winwrap.h"

HFONT winWrap::createFont(int size)
{
  return CreateFontA(size*2,
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
                     ANTIALIASED_QUALITY,
                     VARIABLE_PITCH,
                     TEXT("Lucida Console")
                     );
}
