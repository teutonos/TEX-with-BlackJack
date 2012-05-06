#include <windows.h>
#include <cstring>
// объ€вление функций
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);
//глобализаци€ окон
HWND hMainWnd, hInputWnd, hOutputWnd, hButtonWnd;
char inputString[80];

// функци€ WinMain
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

  // им€ будущего класса
  LPCTSTR WinClass = "My Window Class!";
  RegMyWindowClass(hInstance, WinClass);  // регистраци€ класса

  // вычисление координат центра экрана
  RECT screen_rect;
  GetWindowRect(GetDesktopWindow(), &screen_rect); // разрешение экрана
  int x = screen_rect.right / 2 - 300;
  int y = screen_rect.bottom / 2 - 200;

  // создание диалогового окна
  hMainWnd = //главное окно
  CreateWindow (WinClass, "Dialog Window",
                WS_SYSMENU  | WS_VISIBLE    ,
                x, y, 600, 400,
                NULL, NULL, hInstance, NULL);
  
  hInputWnd = //поле ввода
  CreateWindow ("EDIT", NULL,
                WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_WANTRETURN,	
                10, 10, 570, 120,	
                hMainWnd, NULL,	hInstance, NULL );
 /*
    hOutputWnd = //поле вывода
    CreateWindow ("EDIT",	NULL, 
                  WS_VISIBLE | WS_CHILD | ES_READONLY ,
                  10,	180, 570,	180,
                  hMainWnd, NULL, hInstance, NULL );
*/


  hButtonWnd = //кнопка
  CreateWindow ("BUTTON",	"OK",
                WS_VISIBLE | WS_CHILD | WS_BORDER ,
                480, 140, 100, 30,
                hMainWnd, NULL, hInstance, NULL );

  SetFocus (hInputWnd); //установка фокуса курсора на окне ¬вода

  // цикл сообщений приложени€
  MSG msg = {0};    // структура сообщени€
  int cond = 0;   // переменна€ состо€ни€
  while ((cond = GetMessage(&msg, NULL, 0, 0 )) != 0) // цикл сообщений
  {
    if (cond == -1) return 3;  // если GetMessage вернул ошибку - выход
    TranslateMessage(&msg);    
    DispatchMessage(&msg);

    RECT rect;    
    HDC hdc = GetDC(hMainWnd); // занимает окно дл€ приложени€
    GetClientRect(hMainWnd, &rect); 
    int length = strlen(inputString);
    Rectangle (hdc, 10, 180, 580, 360);
    TextOut ( hdc, 300 - length*3.4, 260, inputString, length );
    UpdateWindow (hMainWnd);
    ReleaseDC(hMainWnd, hdc); // освобождает окно дл€ других приложений
    
  }

  return msg.wParam;  // возвращаем код завершени€ программы
}

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

// функци€ обработки сообщений дл€ главного окна
LRESULT CALLBACK MainWndProc(
  HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_COMMAND:
    switch (wParam)
    {
    case BN_CLICKED:
      GetWindowText(hInputWnd, inputString, 80);
    }
    break;

  case WM_DESTROY:
    PostQuitMessage(0);  // реакци€ на сообщение
    break;

  default:  
    // все сообщени€ не обработанные ¬ами обработает сама Windows
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}