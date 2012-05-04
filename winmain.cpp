#include <windows.h>

// объ€вление функций
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);
HWND hMainWnd, hInputWnd, hOutputWnd, hButtonWnd;
////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////// 
// функци€ вхождений программы WinMain
int APIENTRY WinMain(HINSTANCE hInstance,
  HINSTANCE         hPrevInstance,
  LPSTR             lpCmdLine,
  int               nCmdShow)
{

  // им€ будущего класса
  LPCTSTR WinClass = TEXT("My Window Class!");
  RegMyWindowClass(hInstance, WinClass);  // регистраци€ класса


  // вычисление координат центра экрана
  RECT screen_rect;
  GetWindowRect(GetDesktopWindow(), &screen_rect); // разрешение экрана
  int x = screen_rect.right / 2 - 300;
  int y = screen_rect.bottom / 2 - 200;

  // создание диалогового окна

  /*
  HWND CreateWindow
    (
    LPCTSTR lpClassName,		// указатель на зарегистрированное им€ класса
    LPCTSTR lpWindowName,		// указатель на им€ окна
    DWORD dwStyle,			// стиль окна
    int x,				// горизонтальна€ позици€ окна
    int y,				// вертикальна€ позици€ окна 
    int nWidth,			// ширина окна
    int nHeight,			// высота окна
    HWND hWndParent,			// дескриптор родительского или окна владельца
    HMENU hMenu,			// дескриптор меню или идентификатор дочернего окна
    HANDLE hInstance,			// дескриптор экземпл€ра приложени€
    LPVOID lpParam 			// указатель на данные создани€ окна
    );
   */

  //главное окно
  hMainWnd = 
  CreateWindow (WinClass, "Dialog Window",
                WS_SYSMENU  | WS_VISIBLE    ,
                x, y, 1000, 700,
                NULL, NULL, hInstance, NULL);

  //поле ввода
  hInputWnd = 
  CreateWindow ("EDIT", NULL,
                WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_WANTRETURN,	
                10, 10, 570, 100,	
                hMainWnd, NULL,	hInstance, NULL );
  
  //поле вывода
  hOutputWnd = 
  CreateWindow ("EDIT",	NULL, 
                WS_VISIBLE | WS_CHILD | ES_READONLY ,
                10,	120, 570,	150,
                hMainWnd, NULL, hInstance, NULL );

  //кнопка
  hButtonWnd = 
  CreateWindow ("BUTTON",	"OK",
                WS_VISIBLE | WS_CHILD | WS_BORDER ,
                230, 300, 120, 30,
                hMainWnd, NULL, hInstance, NULL );

 
  // цикл сообщений приложени€
  MSG msg = {0};    // структура сообщени€
  int iGetOk = 0;   // переменна€ состо€ни€
  while ((iGetOk = GetMessage(&msg, NULL, 0, 0 )) != 0) // цикл сообщений
  {
    if (iGetOk == -1) return 3;  // если GetMessage вернул ошибку - выход
    TranslateMessage(&msg);    
    DispatchMessage(&msg);
  }

  return msg.wParam;  // возвращаем код завершени€ программы
}

////////////////////////////////////////////////////////////////////////// 
// функци€ регистрации класса окон
ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR ClassName)
{
  WNDCLASS wcWindowClass = {0};
  wcWindowClass.lpfnWndProc = (WNDPROC)MainWndProc;  // адрес ф-ции обработки сообщений
  wcWindowClass.style = CS_HREDRAW|CS_VREDRAW;  // стиль окна
  wcWindowClass.hInstance = hInst;  // дискриптор экземпл€ра приложени€
  wcWindowClass.lpszClassName = ClassName;  // название класса
  wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);  // загрузка курсора
  wcWindowClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;  // загрузка цвета окон
  return RegisterClass(&wcWindowClass); // регистраци€ класса
}

////////////////////////////////////////////////////////////////////////// 
// функци€ обработки сообщений дл€ главного окна
LRESULT CALLBACK MainWndProc(
  HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  // выборка и обработка сообщений
  switch (message)
  {
  case WM_LBUTTONUP:
    // реакци€ на сообщение
    UpdateWindow (hWnd);
    break;

  case WM_DESTROY:
    PostQuitMessage(0);  // реакци€ на сообщение
    break;

  case WM_COMMAND:
    switch (wParam)
    {
    case BN_CLICKED:
      
      //InvalidateRect(hMainWnd,NULL,true);
      //UpdateWindow(hMainWnd);

      HDC hdc;
      RECT rect;    
      PAINTSTRUCT ps;  // ќбъ€вл€ем структуру дл€ рисовани€.
      hdc = GetDC(hMainWnd);
      GetClientRect(hMainWnd, &rect); 
      Rectangle (hdc, 500, 500, 800, 600);
      ReleaseDC(hMainWnd, hdc);

      //CDC::TextOut(500, 500, "play");

      UpdateWindow(hMainWnd);

      for(int i = 0; i < 100; ++i) // роза должна лежать в диапазоне 0;2pi за условием.
      {
       // SetPixel(hOutputWnd, i, i, RGB(0, 0, 0)); 
      }
      char str[80];
      GetWindowText(hInputWnd, str, 80);
      SetWindowText(hOutputWnd, str);
      UpdateWindow (hMainWnd);
      //MessageBox(hWnd, TEXT("кнопка!"), TEXT("событие"), 0);
    }
    break;

  default:  
    // все сообщени€ не обработанные ¬ами обработает сама Windows
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}