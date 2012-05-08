#include <windows.h>
#include <string>
#include "tex.h"
// ���������� �������
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);
//������������ ����
HWND hMainWnd, hInputWnd, hOutputWnd, hButtonWnd;
Formula* expression;
   int x = 600;
   int y = 300;
   int b = 5;
// ������� WinMain
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

  // ��� �������� ������
  LPCTSTR WinClass = "Main Window!";
  RegMyWindowClass(hInstance, WinClass);  // ����������� ������


  // ���������� ��������� ������ ������
  RECT screen_rect;
  GetWindowRect(GetDesktopWindow(), &screen_rect); // ���������� ������
  int ox = (screen_rect.right - x) / 2;
  int oy = (screen_rect.bottom - y) / 2;

  // �������� ����������� ����
  hMainWnd = //������� ����
  CreateWindow (WinClass, "TEX Editor v.1.61803399",
                WS_SYSMENU  | WS_VISIBLE | WS_SIZEBOX  ,
                ox, oy, x+6, y+40,
                NULL, NULL, hInstance, NULL);

  hInputWnd = //���� �����
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

  hButtonWnd = //������
  CreateWindow ("BUTTON",	"�������!",
                WS_VISIBLE | WS_CHILD | WS_BORDER ,
                x-b-100, y-b-30, 100, 30,
                hMainWnd, NULL, hInstance, NULL );

  SetFocus (hInputWnd); //��������� ������ ������� �� ���� �����
  tokenizer::init();
  // ���� ��������� ����������
  MSG msg = {0};    // ��������� ���������
  int cond = 0;   // ���������� ���������
  int i = 0;
  while ((cond = GetMessage(&msg, NULL, 0, 0 )) != 0) // ���� ���������
  {
    i++;
    if (cond == -1) { return 1; }  // ���� GetMessage ������ ������ - �����
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if (i == 15)
    {
      i = 0;
      HDC hdc = GetDC(hMainWnd); // �������� ���� ��� ����������
      Rectangle (hdc, b, b, x-b, (y-4*b-30)*2/3+b);
      if (expression) expression->draw(hdc, x/2, (y-4*b-30)/3);
      UpdateWindow (hMainWnd);
      ReleaseDC(hMainWnd, hdc); // ����������� ���� ��� ������ ����������
    }
  }

  return msg.wParam;  // ���������� ��� ���������� ���������
}

// ������� ����������� ������ ����
ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR ClassName)
{
  WNDCLASS wcWindowClass = {0};
  wcWindowClass.lpfnWndProc = (WNDPROC)MainWndProc;  // ����� �-��� ��������� ���������
  wcWindowClass.style = CS_HREDRAW|CS_VREDRAW;  // ����� ����
  wcWindowClass.hInstance = hInst;  // ���������� ���������� ����������
  wcWindowClass.lpszClassName = ClassName;  // �������� ������
  wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);  // �������� �������
  wcWindowClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;  // �������� ����� ����
  return RegisterClass(&wcWindowClass); // ����������� ������
}

// ������� ��������� ��������� ��� �������� ����
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
      char str[100];
      wchar_t wstr[100];
      GetWindowText(hInputWnd, str, 100);
      mbstowcs(wstr, str, 100);
      if (wstr[0] != L'\0') { inputString = wstr; }
      if (expression) { delete expression; }
      expression = makeTreeStack(inputString);
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
    PostQuitMessage(0);  // ������� �� ���������
    break;

  default:
    // ��� ��������� �� ������������ ���� ���������� ���� Windows
    return DefWindowProc(hMainWnd, message, wParam, lParam);
  }
  HDC hdc = GetDC(hMainWnd); // �������� ���� ��� ����������
  Rectangle (hdc, b, b, x-b, (y-4*b-30)*2/3+b);
  if (expression) expression->draw(hdc, x/2, (y-4*b-30)/3);
  UpdateWindow (hMainWnd);
  ReleaseDC(hMainWnd, hdc); // ����������� ���� ��� ������ ����������
  UpdateWindow (hMainWnd);
  return 0;
}
