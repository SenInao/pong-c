#include <windows.h>
#include <stdio.h>

#define WIDTH 500
#define HEIGHT 500
#define PADDLE_WIDTH 20
#define PADDLE_HEIGHT 100
#define PADDLE_OFFSET 25
#define PADDLE_SPEED 8
#define PONG_WIDTH 10
#define PONG_SPEED 5

typedef struct Paddle {
  int x;
  int y;
} Paddle;

typedef struct Pong {
  int xvel;
  int yvel;
  int x;
  int y;
} Pong;

void update_pong(Pong* pong) {
  pong->x += pong->xvel * PONG_SPEED;
  pong->y += pong->yvel * PONG_SPEED;
}

Paddle paddle1;
Paddle paddle2;
Pong pong;

void init_objects(Paddle *paddle1, Paddle *paddle2, Pong *pong) {
  paddle1->x = PADDLE_OFFSET;
  paddle1->y = 0;
  paddle2->x = WIDTH - PADDLE_OFFSET - PADDLE_WIDTH;
  paddle2->y = 0;
  pong->x = (WIDTH-PONG_WIDTH)/2;
  pong->y = (HEIGHT-PONG_WIDTH)/2;
  pong->xvel = 1;
  pong->yvel = 0;
}

LRESULT WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hDc = BeginPaint(hWnd, &ps);
      HBRUSH black_brush = CreateSolidBrush(RGB(0,0,0));
      HBRUSH white_brush = CreateSolidBrush(RGB(255,255,255));

      RECT paddleRect = {
        paddle1.x, paddle1.y, paddle1.x+PADDLE_WIDTH, paddle1.y+PADDLE_HEIGHT
      };

      RECT paddleRect1 = {
        paddle2.x, paddle2.y, paddle2.x+PADDLE_WIDTH, paddle2.y+PADDLE_HEIGHT
      };

      RECT pongRect = {
        pong.x, pong.y, pong.x+PONG_WIDTH, pong.y+PONG_WIDTH
      };

      FillRect(hDc, &ps.rcPaint, black_brush);
      FillRect(hDc, &paddleRect, white_brush);
      FillRect(hDc, &paddleRect1, white_brush);
      FillRect(hDc, &pongRect, white_brush);
      DeleteObject(black_brush);
      ReleaseDC(hWnd, hDc);

      EndPaint(hWnd, &ps);
      return 0;
    }

    case WM_TIMER: {
      update_pong(&pong);
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
  RECT windowRect = {0,0,WIDTH,HEIGHT};
  AdjustWindowRect(&windowRect, WS_CAPTION | WS_POPUP | WS_SYSMENU, FALSE);

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

  HWND windowHandle = CreateWindow("NirsClass", "Pong", WS_CAPTION | WS_POPUP | WS_SYSMENU, 50, 50, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);
  SetTimer(windowHandle, 0, 60, NULL);

  init_objects(&paddle1, &paddle2, &pong);

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
