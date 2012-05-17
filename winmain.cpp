#include <windows.h>
#include <string>
#include <StdIO.h>
#include "tex.h"

#define BUTTON_OK 1
#define BUTTON_SAVE 2

//проверка коммита
// объявление функций
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);
VOID WriteImage (CHAR*, RECT);
HBITMAP MakeClientSnapshot (RECT&);
char* GetFileName();
//глобализация окон
HWND hMainWnd, hInputWnd, hOutputWnd, hButtonWnd, hSaveWnd;
Formula* expression;
   int x = 600;
   int y = 300;
   int border = 4;
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
  RECT screenRect;
  GetWindowRect(GetDesktopWindow(), &screenRect); // разрешение экрана
  int globalRight = (screenRect.right - x) / 2;
  int globalTop = (screenRect.bottom - y) / 2;

  // создание диалогового окна
  hMainWnd = //главное окно
  CreateWindow (WinClass, "TEX Editor v.1.61803399",
                WS_TILEDWINDOW  | WS_VISIBLE,
                globalRight, globalTop, x+6, y+40,
                NULL, NULL, hInstance, NULL);

  hInputWnd = //поле ввода
  CreateWindow ("EDIT", NULL,
                WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_WANTRETURN,
                border,
                (y-4*border-30)*2/3+2*border,
                x-2*border,
                (y-4*border-30)/3,
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

  hButtonWnd = //кнопка подтверждения
  CreateWindow ("BUTTON",	"Нажми меня!",
                WS_VISIBLE | WS_CHILD | WS_BORDER ,
                x-border-100, y-border-30, 100, 30,
                hMainWnd, (HMENU) BUTTON_OK, hInstance, NULL );

  hSaveWnd = //кнопка сохранения картинки
  CreateWindow ("BUTTON", "Сохранить формулу",
                WS_VISIBLE | WS_CHILD | WS_BORDER ,
                border, y-border-30, 150, 30,
                hMainWnd, (HMENU) BUTTON_SAVE, hInstance, NULL );

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
      Rectangle (hdc, border, border, x-border, (y-4*border-30)*2/3+border);

      if (expression)
      {
        expression->draw(hdc, x/2, (y-4*border-30)/3);
      }

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
    {
      if (wParam == BUTTON_OK)
      {
        std::wstring inputString(L"{}");
        char str[256];
        wchar_t wstr[256];
        GetWindowText(hInputWnd, str, 256);
        mbstowcs(wstr, str, 256);
        if (wstr[0] != L'\0') { inputString = wstr; }

        if (expression) { delete expression; }
        expression = makeTreeStack(inputString);
      }
      else if (wParam == BUTTON_SAVE)
      {
        //копирование картинки
        RECT outRect;

        if (expression)
        {
          outRect.top    = expression->getBBoxTop()-3;
          outRect.left   = expression->getBBoxLeft()-3;
          outRect.bottom = expression->getBBoxBottom()-expression->getBBoxTop()+6;
          outRect.right  = expression->getBBoxRight()-expression->getBBoxLeft()+6;
        }
        else
        {
          break;
        }

        char* fname;
    	fname = GetFileName();
        WriteImage(fname, outRect);
        delete [] fname;
        //
      }
      break;
    }
    case WM_SIZE:
      {
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        SetWindowPos(hInputWnd, NULL, border, (y-4*border-30)*2/3+2*border, x-2*border, (y-4*border-30)/3, NULL);
        SetWindowPos(hButtonWnd, NULL, x-border-100, y-border-30, 100, 30, NULL);
        SetWindowPos(hSaveWnd, NULL, border, y-border-30, 150, 30, NULL);
        UpdateWindow (hMainWnd);
      }
      break;

    case WM_DESTROY:
      PostQuitMessage(0);  // реакция на сообщение
      break;

    default:
      // все сообщения не обработанные Вами обработает сама Windows
      return DefWindowProc(hMainWnd, message, wParam, lParam);
  }

  HDC hdc = GetDC(hMainWnd); // занимает окно для приложения
  Rectangle (hdc, border, border, x-border, (y-4*border-30)*2/3+border);
  if (expression) expression->draw(hdc, x/2, (y-4*border-30)/3);
  UpdateWindow (hMainWnd);
  ReleaseDC(hMainWnd, hdc); // освобождает окно для других приложений
  UpdateWindow (hMainWnd);
  return 0;
}

VOID WriteImage (char* pstrPath, RECT outRect)
{
  HBITMAP hSnapshot;


  if ((hSnapshot = MakeClientSnapshot (outRect)) != NULL)
  {
  	UINT uiBytesPerRow = 3 * (outRect.right); // RGB takes 24 bits
  	UINT uiRemainderForPadding;

  	if ((uiRemainderForPadding = uiBytesPerRow % sizeof (DWORD)) > 0)
    {
  		uiBytesPerRow += (sizeof (DWORD) - uiRemainderForPadding);
  		}

  	UINT uiBytesPerAllRows = uiBytesPerRow * (outRect.bottom);
  	PBYTE pDataBits;

  	if ((pDataBits = new BYTE [uiBytesPerAllRows]) != NULL)
    {
  		BITMAPINFOHEADER bmi = {0};
  		BITMAPFILEHEADER bmf = {0};
  		HDC hDC = GetDC (hMainWnd);

  		// Prepare to get the data out of HBITMAP:
  		bmi.biSize = sizeof (bmi);
  		bmi.biPlanes = 1;
  		bmi.biBitCount = 24;
  		bmi.biHeight = outRect.bottom;
  		bmi.biWidth = outRect.right;

  		// Get it:
  		GetDIBits (hDC, hSnapshot, 0, outRect.bottom,
  			pDataBits, (BITMAPINFO*) &bmi, DIB_RGB_COLORS);

  		ReleaseDC (hMainWnd, hDC);

  		// Fill the file header:
  		bmf.bfOffBits = sizeof (bmf) + sizeof (bmi);
  		bmf.bfSize = bmf.bfOffBits + uiBytesPerAllRows;
  		bmf.bfType = 0x4D42;

  		// Writing:
  		FILE* pFile;

  		if ((pFile = fopen (pstrPath, "wb")) != NULL)
		{
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

HBITMAP MakeClientSnapshot (RECT& rcClient)
{
  HDC hDC = GetDC (hMainWnd);
  //RECT rcClient;
  BOOL bOk = FALSE;
  HBITMAP hImage = NULL;

  //GetClientRect (hMainWnd, &rcClient);
  //client = rcClient;
  if ((hImage = CreateCompatibleBitmap (hDC,
      rcClient.right,
      rcClient.bottom)) != NULL) {

    HDC hMemDC;
    HBITMAP hDCBmp;

    if ((hMemDC = CreateCompatibleDC (hDC)) != NULL)
    {
      hDCBmp = (HBITMAP) SelectObject (hMemDC, hImage);

      BitBlt (hMemDC, 0, 0, rcClient.right, rcClient.bottom,
        hDC, rcClient.left, rcClient.top, SRCCOPY);

      SelectObject (hMemDC, hDCBmp);
      DeleteDC (hMemDC);
      bOk = TRUE;
    }
  }
  ReleaseDC (hMainWnd, hDC);
  if (! bOk)
  {
    if (hImage)
    {
      DeleteObject (hImage);
      hImage = NULL;
    }
  }
  return hImage;
}

char* GetFileName()
{
  SYSTEMTIME time;
  char* timestamp = new char
  [
    4+ // year
    1+ // '-'
    2+ // month
    1+ // '-'
    2+ // day
    1+ // '_'
    2+ // hours
    1+ // '-'
    2+ // minutes
    1+ // '-'
    2+ // seconds
    4  // '.bmp'
  ];
  GetLocalTime(&time);
  sprintf(timestamp,
          "%04d-%02d-%02d_%02d-%02d-%02d.bmp",
          time.wYear,
          time.wMonth,
          time.wDay,
          time.wHour,
          time.wMinute,
          time.wSecond);
  return timestamp;
}
