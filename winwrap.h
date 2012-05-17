#ifndef WINWRAP_H
#define WINWRAP_H

#include <windows.h>

namespace winWrap
{
  HFONT createFont(int vsize, int hsize = 0);
  HFONT setFont (HDC hdc, int vsize, int hsize = 0);
}

#endif // WINWRAP_H

