#include <windows.h>

// ���������� �������
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);
HWND hMainWnd, hInputWnd, hOutputWnd, hButtonWnd;
////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////// 
// ������� ��������� ��������� WinMain
int APIENTRY WinMain(HINSTANCE hInstance,
  HINSTANCE         hPrevInstance,
  LPSTR             lpCmdLine,
  int               nCmdShow)
{

  // ��� �������� ������
  LPCTSTR WinClass = TEXT("My Window Class!");
  RegMyWindowClass(hInstance, WinClass);  // ����������� ������


  // ���������� ��������� ������ ������
  RECT screen_rect;
  GetWindowRect(GetDesktopWindow(), &screen_rect); // ���������� ������
  int x = screen_rect.right / 2 - 300;
  int y = screen_rect.bottom / 2 - 200;

  // �������� ����������� ����

  /*
  HWND CreateWindow
    (
    LPCTSTR lpClassName,		// ��������� �� ������������������ ��� ������
    LPCTSTR lpWindowName,		// ��������� �� ��� ����
    DWORD dwStyle,			// ����� ����
    int x,				// �������������� ������� ����
    int y,				// ������������ ������� ���� 
    int nWidth,			// ������ ����
    int nHeight,			// ������ ����
    HWND hWndParent,			// ���������� ������������� ��� ���� ���������
    HMENU hMenu,			// ���������� ���� ��� ������������� ��������� ����
    HANDLE hInstance,			// ���������� ���������� ����������
    LPVOID lpParam 			// ��������� �� ������ �������� ����
    );
   */

  //������� ����
  hMainWnd = 
  CreateWindow (WinClass, "Dialog Window",
                WS_SYSMENU  | WS_VISIBLE    ,
                x, y, 1000, 700,
                NULL, NULL, hInstance, NULL);

  //���� �����
  hInputWnd = 
  CreateWindow ("EDIT", NULL,
                WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_WANTRETURN,	
                10, 10, 570, 100,	
                hMainWnd, NULL,	hInstance, NULL );
  
  //���� ������
  hOutputWnd = 
  CreateWindow ("EDIT",	NULL, 
                WS_VISIBLE | WS_CHILD | ES_READONLY ,
                10,	120, 570,	150,
                hMainWnd, NULL, hInstance, NULL );

  //������
  hButtonWnd = 
  CreateWindow ("BUTTON",	"OK",
                WS_VISIBLE | WS_CHILD | WS_BORDER ,
                230, 300, 120, 30,
                hMainWnd, NULL, hInstance, NULL );

 
  // ���� ��������� ����������
  MSG msg = {0};    // ��������� ���������
  int iGetOk = 0;   // ���������� ���������
  while ((iGetOk = GetMessage(&msg, NULL, 0, 0 )) != 0) // ���� ���������
  {
    if (iGetOk == -1) return 3;  // ���� GetMessage ������ ������ - �����
    TranslateMessage(&msg);    
    DispatchMessage(&msg);
  }

  return msg.wParam;  // ���������� ��� ���������� ���������
}

////////////////////////////////////////////////////////////////////////// 
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

////////////////////////////////////////////////////////////////////////// 
// ������� ��������� ��������� ��� �������� ����
LRESULT CALLBACK MainWndProc(
  HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  // ������� � ��������� ���������
  switch (message)
  {
  case WM_LBUTTONUP:
    // ������� �� ���������
    UpdateWindow (hWnd);
    break;

  case WM_DESTROY:
    PostQuitMessage(0);  // ������� �� ���������
    break;

  case WM_COMMAND:
    switch (wParam)
    {
    case BN_CLICKED:
      
      //InvalidateRect(hMainWnd,NULL,true);
      //UpdateWindow(hMainWnd);

      HDC hdc;
      RECT rect;    
      PAINTSTRUCT ps;  // ��������� ��������� ��� ���������.
      hdc = GetDC(hMainWnd);
      GetClientRect(hMainWnd, &rect); 
      Rectangle (hdc, 500, 500, 800, 600);
      ReleaseDC(hMainWnd, hdc);

      //CDC::TextOut(500, 500, "play");

      UpdateWindow(hMainWnd);

      for(int i = 0; i < 100; ++i) // ���� ������ ������ � ��������� 0;2pi �� ��������.
      {
       // SetPixel(hOutputWnd, i, i, RGB(0, 0, 0)); 
      }
      char str[80];
      GetWindowText(hInputWnd, str, 80);
      SetWindowText(hOutputWnd, str);
      UpdateWindow (hMainWnd);
      //MessageBox(hWnd, TEXT("������!"), TEXT("�������"), 0);
    }
    break;

  default:  
    // ��� ��������� �� ������������ ���� ���������� ���� Windows
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}