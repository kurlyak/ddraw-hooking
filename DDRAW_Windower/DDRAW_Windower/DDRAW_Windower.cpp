#include <windows.h>
#include <ddraw.h>

#pragma comment (lib, "ddraw.lib")
#pragma comment (lib, "dxguid.lib")

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_BPP 32

bool g_bFocus = true;

#define INITGUID
#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

HWND g_hWnd;

DDSURFACEDESC Ddsd;
LPDIRECTDRAW lpDD;
LPDIRECTDRAW2 lpDD2;
LPDIRECTDRAWSURFACE lpDDSPrimary;
LPDIRECTDRAWSURFACE3 lpDDSPrimary3;
LPDIRECTDRAWSURFACE3 lpDDSBack3;

UCHAR *BackBuffer; // secondary back buffer
int BackLPitch;    // memory line pitch for back buffer

int DDraw_Fill_Surface(LPDIRECTDRAWSURFACE3 lpDDS, USHORT Color, RECT *client = 0)
{

	DDBLTFX DDBltFx; // this contains the DDBLTFX structure

	// clear out the structure and set the size field 
	DDRAW_INIT_STRUCT(DDBltFx);
	
	// set the dwfillcolor field to the desired Color
	DDBltFx.dwFillColor = Color; 

	lpDDS->Blt(NULL,     // ptr to dest rectangle
           NULL,       // ptr to source surface, NA            
           NULL,       // ptr to source rectangle, NA
           DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
           &DDBltFx);  // ptr to DDBLTFX structure

	// return success
	return 1;
}
void DDraw_Init()
{
	DirectDrawCreate(NULL, &lpDD, NULL);
	lpDD->QueryInterface(IID_IDirectDraw2, (LPVOID*)&lpDD2);

	lpDD2->SetCooperativeLevel(g_hWnd,
		  DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN );

	// set the display mode
	lpDD2->SetDisplayMode(WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_BPP,0,0);

	// Create the primary surface
	memset(&Ddsd,0,sizeof(Ddsd));
	Ddsd.dwSize = sizeof(Ddsd);
	// fullscreen mode
	Ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	Ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	// set the backbuffer count to 0 for windowed mode
	// 1 for fullscreen mode, 2 for triple buffering
	Ddsd.dwBackBufferCount = 1;
	// create the primary surface
	lpDD2->CreateSurface(&Ddsd,&lpDDSPrimary,NULL);

	lpDDSPrimary->QueryInterface(IID_IDirectDrawSurface3,(LPVOID*)&lpDDSPrimary3);

	// query for the backbuffer i.e the secondary surface
	DDSCAPS             ddscaps;
	ZeroMemory(&ddscaps, sizeof(ddscaps));
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	lpDDSPrimary3->GetAttachedSurface(&ddscaps,&lpDDSBack3);

	DDraw_Fill_Surface(lpDDSPrimary3,0);
	DDraw_Fill_Surface(lpDDSBack3,0);
}

UCHAR* DDraw_Lock_Back_Surface(void)
{
	// this function locks the secondary back surface and returns a pointer to it
	// and updates the global variables secondary buffer, and BackLPitch

	// is this surface already locked
	if (BackBuffer)
	{
		// return to current lock
		return(BackBuffer);
	} // end if

	// lock the primary surface
	DDRAW_INIT_STRUCT(Ddsd);
	lpDDSBack3->Lock(NULL,&Ddsd,DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,NULL); 

	// set globals
	BackBuffer = (UCHAR *)Ddsd.lpSurface;
	BackLPitch = Ddsd.lPitch;

	// return pointer to surface
	return(BackBuffer);

} // end DDraw_Lock_Back_Surface

int DDraw_Unlock_Back_Surface(void)
{
	// this unlocks the secondary

	// is this surface valid
	if (!BackBuffer)
		return(0);

	// unlock the secondary surface
	lpDDSBack3->Unlock(NULL);

	// reset the secondary surface
	BackBuffer = NULL;
	BackLPitch = 0;

	// return success
	return(1);
} // end DDraw_Unlock_Back_Surface

inline void Mem_Set_DWORD(void *dest, DWORD data, int count)
{
	// this function fills or sets unsigned 16-bit aligned memory
	// count is number of words
	_asm 
	{ 
		mov edi, dest   ; edi points to destination memory
			mov ecx, count  ; number of 32-bit words to move
			mov eax,  data   ; 32-bit data
			rep stosd       ; move data
	} // end asm

} // end Mem_Set_WORD

int DDraw_Flip(void)
{
	while( 1 )
    {
		HRESULT DDRetVal;
        DDRetVal = lpDDSPrimary3->Flip( NULL, 0 );
        if( DDRetVal == DD_OK )
        {
			break;
        }
        
		if( DDRetVal == DDERR_SURFACELOST )
        {
			DDRetVal = lpDDSPrimary3->Restore();
            if( DDRetVal != DD_OK )
            {
				break;
            }
        }
        if( DDRetVal != DDERR_WASSTILLDRAWING )
        {
			break;
        }
	}

	return 1;
}
void Render_Scene()
{
	if(g_bFocus)
	{
		DDraw_Fill_Surface(lpDDSBack3, 0);
		DDraw_Lock_Back_Surface();

		//рисуем куб на экране тут, цвет белый
		if(BackBuffer)
		{
			for ( int y = 100; y < 300; y++ )
			{
				for ( int x = 100; x < 300; x++ )
				{
					int Index = x + y * (BackLPitch / 4);
					Mem_Set_DWORD((DWORD *)BackBuffer+(unsigned int)Index, 0xFFFFFFFF,(unsigned int)1);
				}
			}
		}

		DDraw_Unlock_Back_Surface();
		DDraw_Flip();
	}
}

int DDraw_Shutdown(void)
{
// this function release all the resources directdraw
// allocated, mainly to com objects
lpDD2->RestoreDisplayMode();

// release the secondary surface
if (lpDDSBack3)
    lpDDSBack3->Release();

// release the primary surface
if (lpDDSPrimary3)
   lpDDSPrimary3->Release();

// finally, the main dd object
if (lpDD2)
    lpDD2->Release();

// return success
return 1;
} // end DDraw_Shutdown

LRESULT CALLBACK WndProc(HWND hWnd,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_ACTIVATE:
			switch(LOWORD(wParam))
			{
				case WA_ACTIVE:
				case WA_CLICKACTIVE:
				g_bFocus = true;
				break;
			
				case WA_INACTIVE:
				g_bFocus = false;
				break;
			}
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int PASCAL WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASS wcl;
	wcl.style = CS_HREDRAW | CS_VREDRAW;
	wcl.lpfnWndProc = WndProc;
	wcl.cbClsExtra = 0L;
	wcl.cbWndExtra = 0L;
	wcl.hInstance = hInstance;
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcl.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcl.lpszMenuName = NULL;
	wcl.lpszClassName = "Sample";

	/*
	WNDCLASS wcl = { CS_HREDRAW|CS_VREDRAW, WndProc,
			0, 0, hInstance, NULL, LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)(COLOR_WINDOW+1),
			NULL, "Sample"};

	*/

	if(!RegisterClass(&wcl))
		return 0;

	g_hWnd = CreateWindow("Sample", "Sample Application",
					WS_OVERLAPPEDWINDOW,
					0, 0,
					640, 480,
					NULL,
					NULL,
					hInstance,
					NULL);
	if(!g_hWnd)
		return 0;

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	DDraw_Init();

	MSG msg;

	while(true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message ==	WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(GetKeyState(VK_ESCAPE) & 0xFF00)
			break;

		Render_Scene();
	}

	DDraw_Shutdown();

	DestroyWindow(g_hWnd);
	UnregisterClass(wcl.lpszClassName, wcl.hInstance);

	return (int)msg.wParam;
}