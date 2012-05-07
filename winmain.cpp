#include <windows.h>
#include <string>
#include "tex.h"
// ���������� �������
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);
//������������ ����
HWND hMainWnd, hInputWnd, hOutputWnd, hButtonWnd;
Formula* f;
// ������� WinMain
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

  // ��� �������� ������
  LPCTSTR WinClass = "My Window Class!";
  RegMyWindowClass(hInstance, WinClass);  // ����������� ������

  // ���������� ��������� ������ ������
  RECT screen_rect;
  GetWindowRect(GetDesktopWindow(), &screen_rect); // ���������� ������
  int x = screen_rect.right / 2 - 300;
  int y = screen_rect.bottom / 2 - 200;

  // �������� ����������� ����
  hMainWnd = //������� ����
  CreateWindow (WinClass, "Dialog Window",
                WS_SYSMENU  | WS_VISIBLE    ,
                x, y, 600, 400,
                NULL, NULL, hInstance, NULL);

  hInputWnd = //���� �����
  CreateWindow ("EDIT", NULL,
                WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_WANTRETURN,
                10, 200, 570, 120,
                hMainWnd, NULL,	hInstance, NULL );

  hButtonWnd = //������
  CreateWindow ("BUTTON",	"OK",
                WS_VISIBLE | WS_CHILD | WS_BORDER ,
                480, 330, 100, 30,
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
    if (cond == -1) return 3;  // ���� GetMessage ������ ������ - �����
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if (i == 20) {
      i=0;
      RECT rect;
      HDC hdc = GetDC(hMainWnd); // �������� ���� ��� ����������
      GetClientRect(hMainWnd, &rect);
      Rectangle (hdc, 10, 10, 580, 190);
      if (f)
        f->draw(hdc, 300, 120);
//        TextOutW ( hdc, 300, 120, f->put().data(), f->put().length());
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
  HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
      if (wstr[0] != L'\0') {
        inputString = wstr;
      }
      if (f) delete f;
      f = makeTreeStack(inputString);
      break;
    }
    break;

  case WM_DESTROY:
    PostQuitMessage(0);  // ������� �� ���������
    break;

  default:
    // ��� ��������� �� ������������ ���� ���������� ���� Windows
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}
