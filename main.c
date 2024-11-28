#include <windows.h>

LRESULT WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hDc = BeginPaint(hWnd, &ps);
      HBRUSH black_brush = CreateSolidBrush(RGB(0,0,0));
      HBRUSH white_brush = CreateSolidBrush(RGB(255,255,255));

      RECT pong1 = {
        500, 210, 520, 315
      };
      RECT pong2 = {
        30, 210, 50, 315
      };

      FillRect(hDc, &ps.rcPaint, black_brush);
      FillRect(hDc, &pong1, white_brush);
      FillRect(hDc, &pong2, white_brush);
      DeleteObject(black_brush);
      ReleaseDC(hWnd, hDc);

      EndPaint(hWnd, &ps);
      return 0;
    }

    case WM_DESTROY: {
      PostQuitMessage(0);
      return 0;
    }
    default: {
      return DefWindowProcA(hWnd, uMsg, wParam, lParam);
    }
  }
  return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
  WNDCLASSA class = {
    0,
    WinProc,
    0,
    0,
    hInstance,
    NULL,
    NULL,
    NULL,
    NULL,
    "NirsClass"
  };

  RegisterClassA(&class);

  HWND windowHandle = CreateWindow("NirsClass", "Pong", WS_CAPTION | WS_POPUP | WS_SYSMENU, 50, 50, 550, 550, NULL, NULL, hInstance, NULL);
  ShowWindow(windowHandle, nShowCmd);

  MSG msg;

  for (;;) {
    if (GetMessageA(&msg, NULL, 0, 0) == 0) {
      break;
    }
    TranslateMessage(&msg);
    DispatchMessageA(&msg);
  }

  return 0;
}
