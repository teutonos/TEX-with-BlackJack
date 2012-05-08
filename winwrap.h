#ifndef WINWRAP_H
#define WINWRAP_H

#include <windows.h>

namespace winWrap
{
  HFONT createFont(int size);
  HFONT createFont(int vsize, int hsize);
  HFONT setFont (HDC hdc, int size);
  HFONT setFont (HDC hdc, int vsize, int hsize);
}

#endif // WINWRAP_H

