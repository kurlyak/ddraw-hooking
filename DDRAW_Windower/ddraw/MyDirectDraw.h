#ifndef _MY_DIRECTDRAW_
#define _MY_DIRECTDRAW_

#include "MyDirectDrawSurface.h"
#include "MyDirectDraw2.h"
#include "WriteLog.h"

#include <ddraw.h>
#pragma comment (lib, "ddraw.lib")

class MyDirectDraw : public IDirectDraw
{
public:

	MyDirectDraw(IDirectDraw * device)
	{
		Write_Log("Entering MyDirectDraw constructor");
		m_device = device;
	}

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj)
	{
		Write_Log("Entering MyDirectDraw->QueryInterface");

		HRESULT hr;
		hr = m_device->QueryInterface(riid, ppvObj);

		if(SUCCEEDED(hr) && riid == IID_IDirectDraw2)
		{
			Write_Log("MyDirectDraw->QueryInterface riid == IID_IDirectDraw2");
			*ppvObj =new MyDirectDraw2((LPDIRECTDRAW2)*ppvObj);
		}

		return hr;
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{
		Write_Log("Entering MyDirectDraw->AddRef");
		return m_device->AddRef();
	}

	ULONG STDMETHODCALLTYPE Release()
	{
		Write_Log("Entering MyDirectDraw->Release");
		ULONG count = m_device->Release();
		if(0 == count)
			delete this;

		return count;
	}

	HRESULT STDMETHODCALLTYPE CreateSurface(LPDDSURFACEDESC lpDDSurfaceDesc2,
		LPDIRECTDRAWSURFACE FAR *lplpDDSurface,  
		IUnknown FAR *pUnkOuter)
	{
		Write_Log("Entering MyDirectDraw->CreateSurface");
		HRESULT hr = m_device->CreateSurface(lpDDSurfaceDesc2, lplpDDSurface,  pUnkOuter   );               

		return hr;
	}

	HRESULT STDMETHODCALLTYPE CreateClipper(DWORD dwFlags,
		LPDIRECTDRAWCLIPPER FAR *lplpDDClipper,
		IUnknown FAR *pUnkOuter)
	{
		Write_Log("Entering MyDirectDraw->CreateClipper");
		return m_device->CreateClipper(dwFlags, lplpDDClipper,   pUnkOuter);	
	}

	HRESULT STDMETHODCALLTYPE SetCooperativeLevel (HWND hWnd,
		DWORD dwFlags)
	{
		Write_Log("Entering MyDirectDraw->SetCooperativeLevel");
		return m_device->SetCooperativeLevel(hWnd,  dwFlags );
	}

	HRESULT STDMETHODCALLTYPE SetDisplayMode(DWORD dwWidth,
		DWORD dwHeight,
		DWORD dwBPP )
	{
		Write_Log("Entering MyDirectDraw->SetDisplayMode");
		return m_device->SetDisplayMode(dwWidth, dwHeight, dwBPP);
	}

	HRESULT STDMETHODCALLTYPE Compact()
	{
		Write_Log("Entering MyDirectDraw->Compact");
		return m_device->Compact();
	}

	HRESULT STDMETHODCALLTYPE Initialize(GUID FAR *lpGUID)
	{
		Write_Log("Entering MyDirectDraw->Initialize");
		return m_device->Initialize(lpGUID);
	}

	HRESULT STDMETHODCALLTYPE CreatePalette(DWORD dwFlags,
		LPPALETTEENTRY lpDDColorArray,           
		LPDIRECTDRAWPALETTE FAR *lplpDDPalette,  
		IUnknown FAR *pUnkOuter)
	{
		Write_Log("Entering MyDirectDraw->CreatePalette");
		return m_device->CreatePalette(dwFlags,                           
			lpDDColorArray,           
			lplpDDPalette,  
			pUnkOuter);
	}

	HRESULT STDMETHODCALLTYPE GetCaps(LPDDCAPS lpDDDriverCaps,  
		LPDDCAPS lpDDHELCaps)
	{
		Write_Log("Entering MyDirectDraw->GetCaps");
		return m_device->GetCaps(lpDDDriverCaps, lpDDHELCaps);
	}

	HRESULT STDMETHODCALLTYPE EnumDisplayModes(DWORD dwFlags,
		LPDDSURFACEDESC lpDDSurfaceDesc,          
		LPVOID lpContext,                           
		LPDDENUMMODESCALLBACK lpEnumModesCallback)
	{
		Write_Log("Entering MyDirectDraw->EnumDisplayModes");
		return m_device->EnumDisplayModes(dwFlags,                              
			lpDDSurfaceDesc,          
			lpContext,                           
			lpEnumModesCallback);
	}

	HRESULT STDMETHODCALLTYPE GetDisplayMode(LPDDSURFACEDESC lpDDSurfaceDesc)
	{
		Write_Log("Entering MyDirectDraw->GetDisplayMode");
		return m_device->GetDisplayMode(lpDDSurfaceDesc);
	}

	HRESULT STDMETHODCALLTYPE GetMonitorFrequency(LPDWORD lpdwFrequency)
	{
		Write_Log("Entering MyDirectDraw->GetMonitorFrequency");
		return m_device->GetMonitorFrequency(lpdwFrequency);
	}

	HRESULT STDMETHODCALLTYPE RestoreDisplayMode()
	{
		Write_Log("Entering MyDirectDraw->RestoreDisplayMode");
		return m_device->RestoreDisplayMode();
	}

	HRESULT STDMETHODCALLTYPE WaitForVerticalBlank(DWORD dwFlags,
		HANDLE hEvent)
	{
		Write_Log("Entering MyDirectDraw->WaitForVerticalBlank");
		return m_device->WaitForVerticalBlank(dwFlags, 
			hEvent);
	}

	HRESULT STDMETHODCALLTYPE GetScanLine(LPDWORD lpdwScanLine)
	{
		Write_Log("Entering MyDirectDraw->GetScanLine");
		return m_device->GetScanLine(lpdwScanLine);
	}

	HRESULT STDMETHODCALLTYPE GetVerticalBlankStatus(LPBOOL lpbIsInVB)
	{
		Write_Log("Entering MyDirectDraw->GetVerticalBlankStatus");
		return m_device->GetVerticalBlankStatus(lpbIsInVB);
	}

	HRESULT STDMETHODCALLTYPE GetFourCCCodes(LPDWORD lpNumCodes,
		LPDWORD lpCodes)
	{
		Write_Log("Entering MyDirectDraw->GetFourCCCodes");
		return m_device->GetFourCCCodes(lpNumCodes,  
			lpCodes);
	}


	HRESULT STDMETHODCALLTYPE DuplicateSurface(LPDIRECTDRAWSURFACE lpDDSurface,
		LPDIRECTDRAWSURFACE FAR *lplpDupDDSurface)
	{
		Write_Log("Entering MyDirectDraw->DuplicateSurface");
		return m_device->DuplicateSurface(lpDDSurface,             
			lplpDupDDSurface);
	}

	HRESULT STDMETHODCALLTYPE EnumSurfaces(DWORD dwFlags,
		LPDDSURFACEDESC lpDDSD,                        
		LPVOID lpContext,                                
		LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback)
	{
		Write_Log("Entering MyDirectDraw->EnumSurfaces");
		return m_device->EnumSurfaces(dwFlags,                                   
			lpDDSD,                        
			lpContext,                                
			lpEnumSurfacesCallback);
	}

	HRESULT STDMETHODCALLTYPE FlipToGDISurface()
	{
		Write_Log("Entering MyDirectDraw->FlipToGDISurface");
		return m_device->FlipToGDISurface();
	}

	HRESULT STDMETHODCALLTYPE GetGDISurface(LPDIRECTDRAWSURFACE FAR *lplpGDIDDSSurface)
	{
		Write_Log("Entering MyDirectDraw->GetGDISurface");
		return m_device->GetGDISurface(lplpGDIDDSSurface);
	}

private:
	IDirectDraw* m_device;	
};

#endif