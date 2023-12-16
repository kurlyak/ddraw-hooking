#ifndef _MY_DIRECTDRAWSURFACE_
#define _MY_DIRECTDRAWSURFACE_

#include "MyDirectDrawSurface3.h"
#include "WriteLog.h"

#include <ddraw.h>
#pragma comment (lib, "ddraw.lib")

class MyDirectDrawSurface : public IDirectDrawSurface
{
public:
	
	MyDirectDrawSurface(HWND hWnd, IDirectDraw2 *device, IDirectDrawSurface * surface) : m_hWnd(hWnd), m_device2(device), m_surface(surface)
	{
		Write_Log("Entering MyDirectDrawSurface constructor");
	}

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj)
	{
		Write_Log("Entering MyDirectDrawSurface->QueryInterface");

		HRESULT hr;
		hr = m_surface->QueryInterface(riid, ppvObj);
		
		if (SUCCEEDED(hr) && riid == IID_IDirectDrawSurface3)
		{

			Write_Log("MyDirectDrawSurface->QueryInterface riid == IID_IDirectDrawSurface3");
			*ppvObj = new MyDirectDrawSurface3(m_hWnd, m_device2, (LPDIRECTDRAWSURFACE3)*ppvObj);
		}

		return hr;
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{
		Write_Log("Entering MyDirectDrawSurface->AddRef");
		return m_surface->AddRef();
	}

	ULONG STDMETHODCALLTYPE Release()
	{
		Write_Log("Entering MyDirectDrawSurface->Release");
		ULONG count = m_surface->Release();
		if(0 == count)
			delete this;

		return count;
	}

	
	HRESULT STDMETHODCALLTYPE AddAttachedSurface(LPDIRECTDRAWSURFACE lpDDSurface)
	{
		Write_Log("Entering MyDirectDrawSurface->AddAttachedSurface");
		return m_surface->AddAttachedSurface(lpDDSurface);
	}

	HRESULT STDMETHODCALLTYPE AddOverlayDirtyRect(LPRECT lpRect)
	{
		Write_Log("Entering MyDirectDrawSurface->AddOverlayDirtyRect");
		return m_surface->AddOverlayDirtyRect(lpRect);
	}


	HRESULT STDMETHODCALLTYPE Initialize(LPDIRECTDRAW lpDD,  LPDDSURFACEDESC lpDDSurfaceDesc)
	{
		Write_Log("Entering MyDirectDrawSurface->Initialize");
		return m_surface->Initialize(lpDD, lpDDSurfaceDesc  );
	}

	HRESULT STDMETHODCALLTYPE IsLost()
	{
		Write_Log("Entering MyDirectDrawSurface->IsLost");
		return m_surface->IsLost();
	}

	HRESULT STDMETHODCALLTYPE Restore()
	{
		Write_Log("Entering MyDirectDrawSurface->Restore");
		return m_surface->Restore();
	}

	HRESULT STDMETHODCALLTYPE DeleteAttachedSurface(DWORD dwFlags,	LPDIRECTDRAWSURFACE lpDDSAttachedSurface)
	{
		Write_Log("Entering MyDirectDrawSurface->DeleteAttachedSurface");
		return m_surface->DeleteAttachedSurface(dwFlags,lpDDSAttachedSurface);
	}

	HRESULT STDMETHODCALLTYPE EnumAttachedSurfaces(LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback)
	{
		Write_Log("Entering MyDirectDrawSurface->EnumAttachedSurfaces");
		return m_surface->EnumAttachedSurfaces(lpContext,lpEnumSurfacesCallback);
	}


	HRESULT STDMETHODCALLTYPE GetAttachedSurface(LPDDSCAPS lpDDSCaps,
		LPDIRECTDRAWSURFACE FAR *lplpDDAttachedSurface)
	{
		Write_Log("Entering MyDirectDrawSurface->GetAttachedSurface");
		HRESULT hr = m_surface->GetAttachedSurface(lpDDSCaps, 
			lplpDDAttachedSurface);

		return hr;
	}

	HRESULT STDMETHODCALLTYPE Blt(LPRECT lpDestRect,                    
		LPDIRECTDRAWSURFACE lpDDSrcSurface,  
		LPRECT lpSrcRect,                     
		DWORD dwFlags,                        
		LPDDBLTFX lpDDBltFx)
	{
		Write_Log("Entering MyDirectDrawSurface->Blt");

		return m_surface->Blt(lpDestRect,                    
			lpDDSrcSurface,  
			lpSrcRect,                     
			dwFlags,                        
			lpDDBltFx);
	}

	HRESULT STDMETHODCALLTYPE BltBatch(	LPDDBLTBATCH lpDDBltBatch,  
		DWORD dwCount,              
		DWORD dwFlags               
		)
	{
		Write_Log("Entering MyDirectDrawSurface->BltBatch");
		return m_surface->BltBatch(lpDDBltBatch,  
			dwCount,              
			dwFlags);
	}

	HRESULT STDMETHODCALLTYPE BltFast(	DWORD dwX,                            
		DWORD dwY,                            
		LPDIRECTDRAWSURFACE lpDDSrcSurface,  
		LPRECT lpSrcRect,                     
		DWORD dwTrans                         
		)
	{
		Write_Log("Entering MyDirectDrawSurface->BltFast");
		return m_surface->BltFast(dwX,                            
			dwY,                            
			lpDDSrcSurface,  
			lpSrcRect,                     
			dwTrans);
	}

	HRESULT STDMETHODCALLTYPE GetBltStatus(DWORD dwFlags)
	{
		Write_Log("Entering MyDirectDrawSurface->GetBltStatus");
		return m_surface->GetBltStatus(dwFlags);
	}

	HRESULT STDMETHODCALLTYPE GetColorKey(DWORD dwFlags,             
		LPDDCOLORKEY lpDDColorKey)
	{
		Write_Log("Entering MyDirectDrawSurface->GetColorKey");
		return m_surface->GetColorKey(dwFlags,             
			lpDDColorKey  );
	}

	HRESULT STDMETHODCALLTYPE SetColorKey(DWORD dwFlags,
		LPDDCOLORKEY lpDDColorKey)
	{
		Write_Log("Entering MyDirectDrawSurface->SetColorKey");
		return m_surface->SetColorKey(dwFlags,             
			lpDDColorKey  );
	}

	HRESULT STDMETHODCALLTYPE GetDC(HDC FAR *lphDC)
	{
		Write_Log("Entering MyDirectDrawSurface->GetDC");
		return m_surface->GetDC(lphDC);
	}

	HRESULT STDMETHODCALLTYPE ReleaseDC(HDC hDC )
	{
		Write_Log("Entering MyDirectDrawSurface->ReleaseDC");
		return m_surface->ReleaseDC( hDC);
	}

	
	HRESULT STDMETHODCALLTYPE Flip(LPDIRECTDRAWSURFACE lpDDSurfaceTargetOverride, 
		DWORD dwFlags)
	{
		Write_Log("Entering MyDirectDrawSurface->Flip");
		return m_surface->Flip(lpDDSurfaceTargetOverride,  
			dwFlags);
	}

	HRESULT STDMETHODCALLTYPE GetFlipStatus(DWORD dwFlags )
	{
		Write_Log("Entering MyDirectDrawSurface->GetFlipStatus");
		return m_surface->GetFlipStatus(dwFlags);
	}

	HRESULT STDMETHODCALLTYPE Lock(LPRECT lpDestRect,                
		LPDDSURFACEDESC lpDDSurfaceDesc,  
		DWORD dwFlags,                    
		HANDLE hEvent)
	{
		Write_Log("Entering MyDirectDrawSurface->Lock");
		return m_surface->Lock(lpDestRect,                
			lpDDSurfaceDesc,  
			dwFlags,                    
			hEvent);
	}

	HRESULT STDMETHODCALLTYPE Unlock(LPVOID lpRect)
	{
		Write_Log("Entering MyDirectDrawSurface->Unlock");
		return m_surface->Unlock(lpRect);
	}

	HRESULT STDMETHODCALLTYPE EnumOverlayZOrders(DWORD dwFlags,
		LPVOID lpContext,                       
		LPDDENUMSURFACESCALLBACK lpfnCallback)
	{
		Write_Log("Entering MyDirectDrawSurface->EnumOverlayZOrders");
		return m_surface->EnumOverlayZOrders(dwFlags, lpContext ,lpfnCallback);
	}

	HRESULT STDMETHODCALLTYPE GetOverlayPosition(LPLONG lplX,
		LPLONG lplY)
	{
		Write_Log("Entering MyDirectDrawSurface->GetOverlayPosition");
		return m_surface->GetOverlayPosition(lplX, lplY);
	}

	HRESULT STDMETHODCALLTYPE SetOverlayPosition(LONG lX, 
		LONG lY)
	{
		Write_Log("Entering MyDirectDrawSurface->SetOverlayPosition");
		return m_surface->SetOverlayPosition(lX, lY);
	}

	HRESULT STDMETHODCALLTYPE UpdateOverlay(LPRECT lpSrcRect,                      
		LPDIRECTDRAWSURFACE lpDDDestSurface,  
		LPRECT lpDestRect,                     
		DWORD dwFlags,                         
		LPDDOVERLAYFX lpDDOverlayFx)
	{
		Write_Log("Entering MyDirectDrawSurface->UpdateOverlay");
		return m_surface->UpdateOverlay(lpSrcRect, lpDDDestSurface, lpDestRect ,dwFlags, lpDDOverlayFx);
	}

	HRESULT STDMETHODCALLTYPE UpdateOverlayDisplay(DWORD dwFlags)
	{
		Write_Log("Entering MyDirectDrawSurface->UpdateOverlayDisplay");
		return m_surface->UpdateOverlayDisplay(dwFlags);
	}

	HRESULT STDMETHODCALLTYPE UpdateOverlayZOrder(DWORD dwFlags,
		LPDIRECTDRAWSURFACE lpDDSReference)
	{
		Write_Log("Entering MyDirectDrawSurface->UpdateOverlayZOrder");
		return m_surface->UpdateOverlayZOrder(dwFlags, lpDDSReference);
	}
	
	HRESULT STDMETHODCALLTYPE GetCaps(LPDDSCAPS lpDDSCaps)
	{
		Write_Log("Entering MyDirectDrawSurface->GetCaps");
		return m_surface->GetCaps(lpDDSCaps);
	}

	HRESULT STDMETHODCALLTYPE GetClipper(LPDIRECTDRAWCLIPPER FAR *lplpDDClipper)
	{
		Write_Log("Entering MyDirectDrawSurface->GetClipper");
		return m_surface->GetClipper(lplpDDClipper);
	}

	HRESULT STDMETHODCALLTYPE SetClipper(LPDIRECTDRAWCLIPPER lpDDClipper)
	{
		Write_Log("Entering MyDirectDrawSurface->SetClipper");
		return m_surface->SetClipper(lpDDClipper);
	}


	HRESULT STDMETHODCALLTYPE GetPixelFormat(LPDDPIXELFORMAT lpDDPixelFormat)
	{
		Write_Log("Entering MyDirectDrawSurface->GetPixelFormat");
		return m_surface->GetPixelFormat(lpDDPixelFormat);
	}

	HRESULT STDMETHODCALLTYPE GetSurfaceDesc(LPDDSURFACEDESC lpDDSurfaceDesc)
	{
		Write_Log("Entering MyDirectDrawSurface->GetSurfaceDesc");
		return m_surface->GetSurfaceDesc(lpDDSurfaceDesc);
	}

	HRESULT STDMETHODCALLTYPE GetPalette(LPDIRECTDRAWPALETTE FAR *lplpDDPalette)
	{
		Write_Log("Entering MyDirectDrawSurface->GetPalette");
		return m_surface->GetPalette(lplpDDPalette);
	}

	HRESULT STDMETHODCALLTYPE SetPalette(LPDIRECTDRAWPALETTE lpDDPalette)
	{
		Write_Log("Entering MyDirectDrawSurface->SetPalette");
		return m_surface->SetPalette(lpDDPalette);
	}
	
private:
	HWND m_hWnd;
	IDirectDraw2 *m_device2;
	IDirectDrawSurface* m_surface;	
};

#endif