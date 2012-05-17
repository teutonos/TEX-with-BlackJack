#include <windows.h>
#include <string>
#include <StdIO.h>
#include "tex.h"
//проверка коммита
// объявление функций
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);
VOID WriteImage (CHAR*, RECT);
HBITMAP MakeClientSnapshot (RECT&);
//глобализация окон
HWND hMainWnd, hInputWnd, hOutputWnd, hButtonWnd;
Formula* expression;
   int x = 600;
   int y = 300;
   int b =  4;
// функция WinMain
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

  // имя будущего класса
  LPCTSTR WinClass = "Main Window!";
  RegMyWindowClass(hInstance, WinClass);  // регистрация класса


  // вычисление координат центра экрана
  RECT screen_rect;
  GetWindowRect(GetDesktopWindow(), &screen_rect); // разрешение экрана
  int ox = (screen_rect.right - x) / 2;
  int oy = (screen_rect.bottom - y) / 2;

  // создание диалогового окна
  hMainWnd = //главное окно
  CreateWindow (WinClass, "TEX Editor v.1.61803399",
                WS_TILEDWINDOW  | WS_VISIBLE,
                ox, oy, x+6, y+40,
                NULL, NULL, hInstance, NULL);

  hInputWnd = //поле ввода
  CreateWindow ("EDIT", NULL,
                WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_WANTRETURN,
                b, (y-4*b-30)*2/3+2*b, x-2*b, (y-4*b-30)/3,
                hMainWnd, NULL,	hInstance, NULL );

   HFONT hFont =
   CreateFont (16,
               0,
               0,
               0,
               FW_DONTCARE,
               0,
               0,
               0,
               ANSI_CHARSET,
               OUT_OUTLINE_PRECIS,
               CLIP_DEFAULT_PRECIS,
               DRAFT_QUALITY,
               VARIABLE_PITCH,
               TEXT("Courier New"));
  SendMessage(hInputWnd, WM_SETFONT,(WPARAM)hFont, 0);

  hButtonWnd = //кнопка
  CreateWindow ("BUTTON",	"Нажми меня!",
                WS_VISIBLE | WS_CHILD | WS_BORDER ,
                x-b-100, y-b-30, 100, 30,
                hMainWnd, NULL, hInstance, NULL );

  SetFocus (hInputWnd); //установка фокуса курсора на окне Ввода
  tokenizer::init();
  // цикл сообщений приложения
  MSG msg = {0};    // структура сообщения
  int cond = 0;   // переменная состояния
  int i = 0;
  while ((cond = GetMessage(&msg, NULL, 0, 0 )) != 0) // цикл сообщений
  {
    i++;
    if (cond == -1) { return 1; }  // если GetMessage вернул ошибку - выход
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if (i == 15)
    {
      i = 0;
      HDC hdc = GetDC(hMainWnd); // занимает окно для приложения
      Rectangle (hdc, b, b, x-b, (y-4*b-30)*2/3+b);

      if (expression) expression->draw(hdc, x/2, (y-4*b-30)/3);

      UpdateWindow (hMainWnd);
      ReleaseDC(hMainWnd, hdc); // освобождает окно для других приложений
    }
  }

  return msg.wParam;  // возвращаем код завершения программы
}

// функция регистрации класса окон
ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR ClassName)
{
  WNDCLASS wcWindowClass = {0};
  wcWindowClass.lpfnWndProc = (WNDPROC)MainWndProc;  // адрес ф-ции обработки сообщений
  wcWindowClass.style = CS_HREDRAW|CS_VREDRAW;  // стиль окна
  wcWindowClass.hInstance = hInst;  // дискриптор экземпляра приложения
  wcWindowClass.lpszClassName = ClassName;  // название класса
  wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);  // загрузка курсора
  wcWindowClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;  // загрузка цвета окон
  return RegisterClass(&wcWindowClass); // регистрация класса
}

// функция обработки сообщений для главного окна
LRESULT CALLBACK MainWndProc(
  HWND hMainWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_COMMAND:
    switch (wParam)
    {
    case BN_CLICKED:
      std::wstring inputString(L"{}");
      char str[256];
      wchar_t wstr[256];
      GetWindowText(hInputWnd, str, 256);
      mbstowcs(wstr, str, 256);
      if (wstr[0] != L'\0') { inputString = wstr; }
      if (expression) { delete expression; }
      expression = makeTreeStack(inputString);

      //копирование картинки
      HDC hdcScreen  = GetDC(hMainWnd);
      HDC hdcCompatible = CreateCompatibleDC(hdcScreen);

      HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, x, y);

      SelectObject(hdcCompatible, hbmScreen);

      BitBlt(hdcCompatible,
               0,0,
               500, 500,
               hdcScreen,
               0,0,
               SRCCOPY);

      RECT outRect;

      outRect.top    = expression->getBBoxTop();
      outRect.bottom = expression->getBBoxBottom();
      outRect.left   = expression->getBBoxLeft();
      outRect.right  = expression->getBBoxRight();

      outRect.top    = 0;
      outRect.bottom = 200;
      outRect.left   = 0;
      outRect.right  = 400;
      WriteImage("image.bmp", outRect);
      //

      break;
    }
    break;

  case WM_SIZE:
    x = LOWORD(lParam);
    y = HIWORD(lParam);
    SetWindowPos(hInputWnd, NULL, b, (y-4*b-30)*2/3+2*b, x-2*b, (y-4*b-30)/3, NULL);
    SetWindowPos(hButtonWnd, NULL, x-b-100, y-b-30, 100, 30, NULL);
    UpdateWindow (hMainWnd);
    break;

  case WM_DESTROY:
    PostQuitMessage(0);  // реакция на сообщение
    break;

  default:
    // все сообщения не обработанные Вами обработает сама Windows
    return DefWindowProc(hMainWnd, message, wParam, lParam);
  }
  HDC hdc = GetDC(hMainWnd); // занимает окно для приложения
  Rectangle (hdc, b, b, x-b, (y-4*b-30)*2/3+b);
  if (expression) expression->draw(hdc, x/2, (y-4*b-30)/3);
  UpdateWindow (hMainWnd);
  ReleaseDC(hMainWnd, hdc); // освобождает окно для других приложений
  UpdateWindow (hMainWnd);
  return 0;
}

VOID WriteImage (CHAR* pstrPath, RECT outRect)
{
  HBITMAP hSnapshot;


  if ((hSnapshot = MakeClientSnapshot (outRect)) != NULL) {
  	UINT uiBytesPerRow = 3 * (outRect.right - outRect.left); // RGB takes 24 bits
  	UINT uiRemainderForPadding;

  	if ((uiRemainderForPadding = uiBytesPerRow % sizeof (DWORD)) > 0) {
  		uiBytesPerRow += (sizeof (DWORD) - uiRemainderForPadding);
  		}

  	UINT uiBytesPerAllRows = uiBytesPerRow * (outRect.bottom - outRect.top);
  	PBYTE pDataBits;

  	if ((pDataBits = new BYTE [uiBytesPerAllRows]) != NULL) {
  		BITMAPINFOHEADER bmi = {0};
  		BITMAPFILEHEADER bmf = {0};
  		HDC hDC = GetDC (hMainWnd);

  		// Prepare to get the data out of HBITMAP:
  		bmi.biSize = sizeof (bmi);
  		bmi.biPlanes = 1;
  		bmi.biBitCount = 24;
  		bmi.biHeight = outRect.bottom - outRect.top;
  		bmi.biWidth = outRect.right - outRect.left;

  		// Get it:
  		GetDIBits (hDC, hSnapshot, outRect.top, outRect.bottom - outRect.top,
  			pDataBits, (BITMAPINFO*) &bmi, DIB_RGB_COLORS);

  		ReleaseDC (hMainWnd, hDC);

  		// Fill the file header:
  		bmf.bfOffBits = sizeof (bmf) + sizeof (bmi);
  		bmf.bfSize = bmf.bfOffBits + uiBytesPerAllRows;
  		bmf.bfType = 0x4D42;

  		// Writing:
  		FILE* pFile;

  		if ((pFile = fopen (pstrPath, "wb")) != NULL) {
  			fwrite (&bmf, sizeof (bmf), 1, pFile);
  			fwrite (&bmi, sizeof (bmi), 1, pFile);
  			fwrite (pDataBits, sizeof (BYTE), uiBytesPerAllRows, pFile);
  			fclose (pFile);
  			}
  		delete [] pDataBits;
  		}
  	DeleteObject (hSnapshot);
  	}
}

HBITMAP MakeClientSnapshot (RECT& rcClient) {
      HDC hDC = GetDC (hMainWnd);
      //RECT rcClient;
      BOOL bOk = FALSE;
      HBITMAP hImage = NULL;

      //GetClientRect (hMainWnd, &rcClient);
      //client = rcClient;
      if ((hImage = CreateCompatibleBitmap (hDC,
          rcClient.right - rcClient.left,
          rcClient.bottom - rcClient.top)) != NULL) {

        HDC hMemDC;
        HBITMAP hDCBmp;

        if ((hMemDC = CreateCompatibleDC (hDC)) != NULL) {
          hDCBmp = (HBITMAP) SelectObject (hMemDC, hImage);

          BitBlt (hMemDC, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
            hDC, 0, 0, SRCCOPY);

          SelectObject (hMemDC, hDCBmp);
          DeleteDC (hMemDC);
          bOk = TRUE;
          }
        }
      ReleaseDC (hMainWnd, hDC);
      if (! bOk) {
        if (hImage) {
          DeleteObject (hImage);
          hImage = NULL;
          }
        }
      return hImage;
      }
