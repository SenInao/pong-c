#include <windows.h>
#include <stdio.h>

#define WIDTH 500
#define HEIGHT 500
#define PADDLE_WIDTH 20
#define PADDLE_HEIGHT 100
#define PADDLE_OFFSET 25
#define PADDLE_SPEED 10

typedef struct Paddle {
  int x;
  int y;
} Paddle;

Paddle paddle1 = {PADDLE_OFFSET, 0};
Paddle paddle2 = {WIDTH - PADDLE_OFFSET, 0};

LRESULT WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hDc = BeginPaint(hWnd, &ps);
      HBRUSH black_brush = CreateSolidBrush(RGB(0,0,0));
      HBRUSH white_brush = CreateSolidBrush(RGB(255,255,255));

      RECT pong1 = {
        paddle1.x, paddle1.y, paddle1.x+PADDLE_WIDTH, paddle1.y+PADDLE_HEIGHT
      };

      RECT pong2 = {
        paddle2.x, paddle2.y, paddle2.x+PADDLE_WIDTH, paddle2.y+PADDLE_HEIGHT
      };

      FillRect(hDc, &ps.rcPaint, black_brush);
      FillRect(hDc, &pong1, white_brush);
      FillRect(hDc, &pong2, white_brush);
      DeleteObject(black_brush);
      ReleaseDC(hWnd, hDc);

      EndPaint(hWnd, &ps);
      return 0;
    }

    case WM_TIMER: {
      paddle1.y+=PADDLE_SPEED;
      InvalidateRect(hWnd, NULL, 1);
      return 0;
    }

    case WM_KEYDOWN: {
      switch (LOWORD(wParam))
      {
        case 40:
          paddle2.y+=PADDLE_SPEED;
          break;
        case 38:
          paddle2.y-=PADDLE_SPEED;
          break;
      }
      InvalidateRect(hWnd, NULL, 1);
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
  SetTimer(windowHandle, 0, 1000, NULL);

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
