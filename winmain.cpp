#include <windows.h>
#include <cstring>
// ���������� �������
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);
//������������ ����
HWND hMainWnd, hInputWnd, hOutputWnd, hButtonWnd;
char inputString[80];

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
                10, 10, 570, 120,	
                hMainWnd, NULL,	hInstance, NULL );
 /*
    hOutputWnd = //���� ������
    CreateWindow ("EDIT",	NULL, 
                  WS_VISIBLE | WS_CHILD | ES_READONLY ,
                  10,	180, 570,	180,
                  hMainWnd, NULL, hInstance, NULL );
*/


  hButtonWnd = //������
  CreateWindow ("BUTTON",	"OK",
                WS_VISIBLE | WS_CHILD | WS_BORDER ,
                480, 140, 100, 30,
                hMainWnd, NULL, hInstance, NULL );

  SetFocus (hInputWnd); //��������� ������ ������� �� ���� �����

  // ���� ��������� ����������
  MSG msg = {0};    // ��������� ���������
  int cond = 0;   // ���������� ���������
  while ((cond = GetMessage(&msg, NULL, 0, 0 )) != 0) // ���� ���������
  {
    if (cond == -1) return 3;  // ���� GetMessage ������ ������ - �����
    TranslateMessage(&msg);    
    DispatchMessage(&msg);

    RECT rect;    
    HDC hdc = GetDC(hMainWnd); // �������� ���� ��� ����������
    GetClientRect(hMainWnd, &rect); 
    int length = strlen(inputString);
    Rectangle (hdc, 10, 180, 580, 360);
    TextOut ( hdc, 300 - length*3.4, 260, inputString, length );
    UpdateWindow (hMainWnd);
    ReleaseDC(hMainWnd, hdc); // ����������� ���� ��� ������ ����������
    
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
      GetWindowText(hInputWnd, inputString, 80);
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