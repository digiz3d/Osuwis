#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

LRESULT CALLBACK procWindow(HWND fenetrePrincipale, UINT message, WPARAM wParam, LPARAM lParam);
HRGN ScanRegion(HBITMAP pBitmap, BYTE jTranspR, BYTE jTranspG , BYTE jTranspB);
BYTE* Get24BitPixels(HBITMAP pBitmap, WORD *pwWidth, WORD *pwHeight);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	// skip the unused instance warning
	(void)hPrevInstance;
	(void)lpCmdLine;
	(void)nCmdShow;
	
	WNDCLASSEX wcex;
	HWND hWindow;
	MSG message;
	HBITMAP hSkinBmp;
	
	const wchar_t *wWindowClass = L"MsnMsgrUIManager";
	
	wcex.cbSize 		= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= procWindow;
	wcex.cbClsExtra	 	= 0;
	wcex.cbWndExtra	 	= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, L"OSUWIS_ICON");
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= wWindowClass;
	wcex.hIconSm		= LoadIcon(hInstance, L"OSUWIS_ICON");
	
	if(!RegisterClassEx(&wcex))
	{
		return 1;
	}
	
	hWindow = CreateWindowEx(
		WS_EX_LEFT,
		wWindowClass,
		L"Osuwis",
		WS_BORDER | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		240,
		180,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	
	HRGN hCustomRegion;
	
	hSkinBmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(1));
	hCustomRegion = ScanRegion(hSkinBmp, 0, 0, 0);
	SetWindowRgn(hWindow, hCustomRegion, true);
	
	SetWindowLong(hWindow, GWL_STYLE, 0);
	
	ShowWindow(hWindow, SW_SHOW);
	UpdateWindow(hWindow);	

	while(GetMessage(&message,NULL,0,0) != 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	DeleteObject(hSkinBmp);
	return 0;
}

LRESULT CALLBACK procWindow(HWND hFenetre, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hSkinBmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(1));
	switch (message)
	{
		case WM_LBUTTONDOWN:
		{
			SendMessage(hFenetre, WM_NCLBUTTONDOWN, HTCAPTION, 0);
			return 0;
		}
		case WM_CREATE:
		{
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_PAINT:
		{
			HDC hDc;
			PAINTSTRUCT ps;

			hDc = BeginPaint(hFenetre, &ps);
			DrawState(hDc, NULL, NULL, reinterpret_cast<long long int>(hSkinBmp), 0, 0, 0, 0, 0, DST_BITMAP);
			EndPaint(hFenetre, &ps);
			return 0;
		}
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			}
			return 0;
		}
		
		case WM_COPYDATA:
		{
			COPYDATASTRUCT* pcds = (COPYDATASTRUCT*) lParam;
			if (pcds->dwData == 1351) {
				LPCTSTR lpszString = (LPCTSTR)(pcds->lpData);
				wstring str(lpszString);

				int sep1(str.find(L"\\0"));
				int sep2(str.find(L"\\0", sep1+2));
				int sep3(str.find(L"\\0", sep2+2));
				int sep4(str.find(L"\\0", sep3+2));
				int sep5(str.find(L"\\0", sep4+2));
				int sep6(str.find(L"\\0", sep5+2));
				
				wstring songName(str.substr(sep4+2, sep5-sep4-2));
				wstring artistName(str.substr(sep5+2, sep6-sep5-2));
				
				wofstream myfile;
				myfile.open("Osuwis.txt", ios::trunc);
				if (myfile.is_open())
				{
					myfile << songName << L" - " << artistName << endl;
					myfile.close();
				}
			}
			else {
				wcout << L"mince" << endl;
			}
			return 0;
		}
		default:
		{
			return DefWindowProc(hFenetre, message, wParam, lParam);
		}
	}
}


HRGN ScanRegion(HBITMAP pBitmap, BYTE jTranspR, BYTE jTranspG , BYTE jTranspB)
{
  WORD wBmpWidth,wBmpHeight;
  HRGN hRgn, hTmpRgn;

  BYTE *pPixels = Get24BitPixels(pBitmap, &wBmpWidth, &wBmpHeight);
  if (!pPixels) return NULL;

  hRgn = CreateRectRgn(0,0,wBmpWidth,wBmpHeight);
  if (!hRgn) { delete pPixels; return NULL; }

  DWORD p=0;
  for (WORD y=0; y<wBmpHeight; y++)
  {
    for (WORD x=0; x<wBmpWidth; x++)
    {
      BYTE jRed   = pPixels[p+2];
      BYTE jGreen = pPixels[p+1];
      BYTE jBlue  = pPixels[p+0];

      if (jRed == jTranspR && jGreen == jTranspG && jBlue == jTranspB)
      {
        hTmpRgn = CreateRectRgn(x,y,x+1,y+1);
        CombineRgn(hRgn, hRgn, hTmpRgn, RGN_XOR);
        DeleteObject(hTmpRgn);
      }
      p+=3;
    }
  }
  delete pPixels;
  return hRgn;
}
BYTE* Get24BitPixels(HBITMAP pBitmap, WORD *pwWidth, WORD *pwHeight)
{
    BITMAP bmpBmp;
    LPBITMAPINFO pbmiInfo;
	BITMAPINFO bmiInfo;
	WORD wBmpWidth, wBmpHeight;
	GetObject(pBitmap, sizeof(bmpBmp),&bmpBmp);
	pbmiInfo   = (LPBITMAPINFO)&bmpBmp;
	wBmpWidth  = (WORD)pbmiInfo->bmiHeader.biWidth;
	wBmpWidth -= (wBmpWidth%4);
	wBmpHeight = (WORD)pbmiInfo->bmiHeader.biHeight;
	*pwWidth  = wBmpWidth;
	*pwHeight = wBmpHeight;
	
	BYTE *pPixels = new BYTE[wBmpWidth*wBmpHeight*3];
    if (!pPixels)
	{
		return NULL;
	}
	
	HDC hDC = GetWindowDC(NULL);
 
	bmiInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiInfo.bmiHeader.biWidth = wBmpWidth;
	bmiInfo.bmiHeader.biHeight = -wBmpHeight;
	bmiInfo.bmiHeader.biPlanes = 1;
	bmiInfo.bmiHeader.biBitCount = 24;
	bmiInfo.bmiHeader.biCompression = BI_RGB;
	bmiInfo.bmiHeader.biSizeImage = wBmpWidth*wBmpHeight*3;
	bmiInfo.bmiHeader.biXPelsPerMeter = 0;
	bmiInfo.bmiHeader.biYPelsPerMeter = 0;
	bmiInfo.bmiHeader.biClrUsed = 0;
	bmiInfo.bmiHeader.biClrImportant = 0;

	int iRes = GetDIBits(hDC,pBitmap,0,wBmpHeight,(LPVOID)pPixels,&bmiInfo,DIB_RGB_COLORS);
 
	ReleaseDC(NULL,hDC);
	
	if (!iRes)
	{
		delete pPixels;
		return NULL;
	};

    return pPixels;
}
