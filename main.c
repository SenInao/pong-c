#include <windows.h>

#include <windows.h>

LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  static HDC hdcMem = NULL;
  static HBITMAP hBitmap = NULL;
  static HDC hdcWindow = NULL;

  switch (msg) {
    case WM_CREATE: {
      // Create a memory DC for double buffering
      hdcWindow = GetDC(hwnd);
      hdcMem = CreateCompatibleDC(hdcWindow);
      RECT rect;
      GetClientRect(hwnd, &rect);
      hBitmap = CreateCompatibleBitmap(hdcWindow, rect.right, rect.bottom);
      SelectObject(hdcMem, hBitmap);
      break;
    }

    case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd, &ps);

      HBRUSH hBlackBr = CreateSolidBrush(RGB(0,0,0));

      FillRect(hdcMem, &ps.rcPaint, hBlackBr);
      RECT rect = { 50, 50, 200, 150 };
      FillRect(hdcMem, &rect, (HBRUSH)(COLOR_WINDOW + 1));

      BitBlt(hdcWindow, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, hdcMem, 0, 0, SRCCOPY);

      EndPaint(hwnd, &ps);

      DeleteObject(hBlackBr);
      break;
    }

    case WM_DESTROY: {
      // Clean up resources
      DeleteDC(hdcMem);
      DeleteObject(hBitmap);
      ReleaseDC(hwnd, hdcWindow);
      PostQuitMessage(0);
      break;
    }

    default:
      return DefWindowProc(hwnd, msg, wParam, lParam);
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

  HWND windowHandle = CreateWindow("NirsClass", "Pong", WS_CAPTION | WS_POPUP | WS_SYSMENU, 50, 50, 500, 500, NULL, NULL, hInstance, NULL);
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
