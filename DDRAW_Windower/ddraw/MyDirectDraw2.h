//==========================================================================
//	Ed Kurlyak 2023 DirectDraw Hook
//==========================================================================

#ifndef _MY_DIRECTDRAW2_
#define _MY_DIRECTDRAW2_

#include <windows.h>

#include "MyDirectDrawSurface.h"
#include "WriteLog.h"

#include <ddraw.h>
#pragma comment (lib, "ddraw.lib")

class MyDirectDraw2 : public IDirectDraw2
{
public:

	HWND hwnd;

	MyDirectDraw2(IDirectDraw2 * device)
	{
		Write_Log("Entering MyDirectDraw2 constructor");
		m_device2 = (LPDIRECTDRAW2)device;
	}

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj)
	{
		Write_Log("Entering MyDirectDraw2->QueryInterface");
		return m_device2->QueryInterface(riid, ppvObj);
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{
		Write_Log("Entering MyDirectDraw2->AddRef");
		return m_device2->AddRef();
	}

	ULONG STDMETHODCALLTYPE Release()
	{
		Write_Log("Entering MyDirectDraw2->Release");
		
		ULONG count;
		count = m_device2->Release();
		if(0 == count)
			delete this;

		return count;
	}
	
	HRESULT STDMETHODCALLTYPE CreateSurface(LPDDSURFACEDESC lpDDSurfaceDesc,
		LPDIRECTDRAWSURFACE FAR *lplpDDSurface,  
		IUnknown FAR *pUnkOuter)
	{
		Write_Log("Entering MyDirectDraw2->CreateSurface");
	
		HRESULT hr;
		if( (lpDDSurfaceDesc->dwFlags == (DDSD_CAPS | DDSD_BACKBUFFERCOUNT)) &&
			//(lpDDSurfaceDesc->ddsCaps.dwCaps == (DDSCAPS_PRIMARYSURFACE|DDSCAPS_FLIP|DDSCAPS_3DDEVICE|DDSCAPS_COMPLEX)) )
			(lpDDSurfaceDesc->ddsCaps.dwCaps == (DDSCAPS_PRIMARYSURFACE|DDSCAPS_FLIP|DDSCAPS_COMPLEX)) )
		{
			Write_Log("MyDirectDraw2->CreateSurface PRIMARYSURFACE");
			
			lpDDSurfaceDesc->dwFlags = DDSD_CAPS;
			lpDDSurfaceDesc->dwBackBufferCount = 0;
			lpDDSurfaceDesc->ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

			hr = m_device2->CreateSurface(lpDDSurfaceDesc, lplpDDSurface,  pUnkOuter);               
			
			if (SUCCEEDED(hr))
				Write_Log("MyDirectDraw2->CreateSurface PRIMARYSURFACE OK");
			*lplpDDSurface = new MyDirectDrawSurface(m_hWnd, m_device2, *lplpDDSurface);
		}
		else if (lpDDSurfaceDesc->dwFlags == (DDSD_WIDTH|DDSD_HEIGHT|DDSD_CAPS|DDSD_ZBUFFERBITDEPTH))
		{
			Write_Log("MyDirectDraw2->CreateSurface ZBUFFER");
			hr = m_device2->CreateSurface(lpDDSurfaceDesc, lplpDDSurface,  pUnkOuter);               
			if (SUCCEEDED(hr))
				Write_Log("MyDirectDraw2->CreateSurface ZBUFFER OK");
			
		}
		else
		{
			Write_Log("MyDirectDraw2->CreateSurface NOT PRIMARY");
			 hr = m_device2->CreateSurface(lpDDSurfaceDesc, lplpDDSurface,  pUnkOuter);               
			if (SUCCEEDED(hr))
				Write_Log("MyDirectDraw2->CreateSurface NOT PRIMARY OK");
		}

		return hr;
	}
	
	HRESULT STDMETHODCALLTYPE CreateClipper(DWORD dwFlags, LPDIRECTDRAWCLIPPER FAR *lplpDDClipper, IUnknown FAR *pUnkOuter)
	{
		Write_Log("Entering MyDirectDraw2->CreateClipper");
		return m_device2->CreateClipper(dwFlags, lplpDDClipper,   pUnkOuter);	
	}

	HRESULT STDMETHODCALLTYPE SetCooperativeLevel(HWND hWnd, DWORD dwFlags )
	{
		m_hWnd = hWnd;

		Write_Log("Entering MyDirectDraw2->SetCooperativeLevel");
		
		if (dwFlags == (DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN) )
		{
			dwFlags = DDSCL_NORMAL;
		}

		HRESULT hr = m_device2->SetCooperativeLevel(hWnd,  dwFlags );

		if(SUCCEEDED(hr))
			Write_Log("Leave MyDirectDraw2->SetCooperativeLevel OK");
		else if(FAILED(hr))
			Write_Log("Leave MyDirectDraw2->SetCooperativeLevel FAILED");

		return hr;
	}
		
	HRESULT STDMETHODCALLTYPE Compact()
	{
		Write_Log("Entering MyDirectDraw2->Compact");
		return m_device2->Compact();
	}

	HRESULT STDMETHODCALLTYPE Initialize(GUID FAR *lpGUID  	)
	{
		Write_Log("Entering MyDirectDraw2->Initialize");
		return m_device2->Initialize(lpGUID  	);
	}

	HRESULT STDMETHODCALLTYPE  CreatePalette(DWORD dwFlags,                           
		LPPALETTEENTRY lpDDColorArray,           
		LPDIRECTDRAWPALETTE FAR *lplpDDPalette,  
		IUnknown FAR *pUnkOuter)
	{
		Write_Log("Entering MyDirectDraw2->CreatePalette");
		return m_device2->CreatePalette(	dwFlags,                           
			lpDDColorArray,           
			lplpDDPalette,  
			pUnkOuter);
	}

	HRESULT STDMETHODCALLTYPE  GetCaps(LPDDCAPS lpDDDriverCaps,  
		LPDDCAPS lpDDHELCaps)
	{
		Write_Log("Entering MyDirectDraw2->GetCaps");
		return m_device2->GetCaps(lpDDDriverCaps,  lpDDHELCaps  );
	}

	
	HRESULT STDMETHODCALLTYPE  EnumDisplayModes(DWORD dwFlags,                              
		LPDDSURFACEDESC lpDDSurfaceDesc,          
		LPVOID lpContext,                           
		LPDDENUMMODESCALLBACK lpEnumModesCallback)
	{
		Write_Log("Entering MyDirectDraw2->EnumDisplayModes");
		return m_device2->EnumDisplayModes(dwFlags,                              
			lpDDSurfaceDesc,          
			lpContext,                           
			lpEnumModesCallback);
		
	}

	HRESULT STDMETHODCALLTYPE  GetDisplayMode(LPDDSURFACEDESC lpDDSurfaceDesc)
	{
		Write_Log("Entering MyDirectDraw2->GetDisplayMode");
		return m_device2->GetDisplayMode(lpDDSurfaceDesc  );
	}

	HRESULT STDMETHODCALLTYPE  GetMonitorFrequency(LPDWORD lpdwFrequency)
	{
		Write_Log("Entering MyDirectDraw2->GetMonitorFrequency");
		return m_device2->GetMonitorFrequency(lpdwFrequency);
	}

	HRESULT STDMETHODCALLTYPE  RestoreDisplayMode()
	{
		Write_Log("Entering MyDirectDraw2->RestoreDisplayMode");
		return m_device2->RestoreDisplayMode();
	}

	HRESULT STDMETHODCALLTYPE  WaitForVerticalBlank(DWORD dwFlags,
		HANDLE hEvent)
	{
		Write_Log("Entering MyDirectDraw2->WaitForVerticalBlank");
		return m_device2->WaitForVerticalBlank(dwFlags,
			hEvent);
	}

	HRESULT STDMETHODCALLTYPE  GetScanLine(LPDWORD lpdwScanLine)
	{
		Write_Log("Entering MyDirectDraw2->GetScanLine");
		return m_device2->GetScanLine(lpdwScanLine);
	}

	HRESULT STDMETHODCALLTYPE  GetVerticalBlankStatus(LPBOOL lpbIsInVB)
	{
		Write_Log("Entering MyDirectDraw2->GetVerticalBlankStatus");
		return m_device2->GetVerticalBlankStatus(lpbIsInVB);
	}

	HRESULT STDMETHODCALLTYPE  GetFourCCCodes(LPDWORD lpNumCodes,
		LPDWORD lpCodes)
	{
		Write_Log("Entering MyDirectDraw2->GetFourCCCodes");
		return m_device2->GetFourCCCodes(lpNumCodes,  
			lpCodes);
	}

	HRESULT STDMETHODCALLTYPE  DuplicateSurface(LPDIRECTDRAWSURFACE lpDDSurface,             
		LPDIRECTDRAWSURFACE FAR *lplpDupDDSurface)
	{
		Write_Log("Entering MyDirectDraw2->DuplicateSurface");
		return m_device2->DuplicateSurface(lpDDSurface,             
			lplpDupDDSurface);
	}

	HRESULT STDMETHODCALLTYPE  EnumSurfaces(DWORD dwFlags,                                   
		LPDDSURFACEDESC lpDDSD,                        
		LPVOID lpContext,                                
		LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback)
	{
		Write_Log("Entering MyDirectDraw2->EnumSurfaces");
		return m_device2->EnumSurfaces(dwFlags,                                   
			lpDDSD,                        
			lpContext,                                
			lpEnumSurfacesCallback);
		
	}

	HRESULT STDMETHODCALLTYPE  FlipToGDISurface()
	{
		Write_Log("Entering MyDirectDraw2->FlipToGDISurface");
		return m_device2->FlipToGDISurface();
	}

	HRESULT STDMETHODCALLTYPE  GetGDISurface(LPDIRECTDRAWSURFACE FAR *lplpGDIDDSSurface)
	{
		Write_Log("Entering MyDirectDraw2->GetGDISurface");
		return m_device2->GetGDISurface(lplpGDIDDSSurface);
	}
	
	HRESULT STDMETHODCALLTYPE  GetAvailableVidMem(LPDDSCAPS lpDDSCaps,  
		LPDWORD    lpdwTotal,   
		LPDWORD    lpdwFree)
	{
		Write_Log("Entering MyDirectDraw2->GetAvailableVidMem");
		return m_device2->GetAvailableVidMem(
			lpDDSCaps,  
			lpdwTotal,   
			lpdwFree     
			);
	}

	HRESULT STDMETHODCALLTYPE SetDisplayMode(DWORD dwWidth,
		DWORD dwHeight,
		DWORD dwBPP,
		DWORD dwRefreshRate,
		DWORD dwFlags)
	{

		DWORD dwStyles = WS_OVERLAPPEDWINDOW|WS_VISIBLE;
		//dwStyles &= ~WS_MAXIMIZEBOX;
		
		SetWindowLong(m_hWnd,GWL_STYLE,dwStyles);
		SetWindowLong(m_hWnd,GWL_EXSTYLE, 0);

		RECT window_rect;
		window_rect.left = 0;
		window_rect.top = 0;
		window_rect.right = dwWidth;
		window_rect.bottom = dwHeight;

		AdjustWindowRectEx(&window_rect,
			GetWindowStyle(m_hWnd),
			GetMenu(m_hWnd) != NULL,
			GetWindowExStyle(m_hWnd));

		int wd = window_rect.right - window_rect.left;
		int hg = window_rect.bottom - window_rect.top;

		int nWidth  = GetSystemMetrics(SM_CXSCREEN);
		int nHeight = GetSystemMetrics(SM_CYSCREEN);

		int r_width = (nWidth - wd) / 2;
		int r_height = (nHeight - hg) / 2;

		MoveWindow(m_hWnd,r_width, r_height, wd, hg, true);

		Write_Log("Entering MyDirectDraw2->SetDisplayMode");
		
		ShowCursor(true);
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		dwWidth  = GetSystemMetrics(SM_CXSCREEN);
		dwHeight = GetSystemMetrics(SM_CYSCREEN);
		//dwBPP = 16;

		HRESULT hr = m_device2->SetDisplayMode(dwWidth,   dwHeight,   dwBPP,   0,   NULL );

		return hr;
	}

private:
	HWND m_hWnd;
	IDirectDraw2* m_device2;	
};

#endif