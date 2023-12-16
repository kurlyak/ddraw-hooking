#ifndef _MY_DIRECTDRAWSURFACE3_
#define _MY_DIRECTDRAWSURFACE3_


#include <windowsx.h>

#include "WriteLog.h"

#include <ddraw.h>
#pragma comment (lib, "ddraw.lib")

class MyDirectDrawSurface3 : public IDirectDrawSurface3
{
public:

	RECT rc1;

	MyDirectDrawSurface3(HWND hWnd, IDirectDraw2 *device, IDirectDrawSurface3 * surface)
	{
		m_hWnd = hWnd;
		Write_Log("Entering MyDirectDrawSurface3 constructor");
		
		m_device2 = device;

		GetClientRect(m_hWnd, &rc1);

		m_surface3 = (LPDIRECTDRAWSURFACE3)surface;
	}

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj)
	{
		Write_Log("Entering MyDirectDrawSurface3->QueryInterface");
		HRESULT hr = m_surface3->QueryInterface(riid, ppvObj);

		return hr;
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{
		Write_Log("Entering MyDirectDrawSurface3->AddRef");

		return m_surface3->AddRef();
	}

	ULONG STDMETHODCALLTYPE Release()
	{
		Write_Log("Entering MyDirectDrawSurface3->Release");

		ULONG count = m_surface3->Release();
		if(0 == count)
			delete this;

		return count;
	}

	HRESULT STDMETHODCALLTYPE AddAttachedSurface( LPDIRECTDRAWSURFACE3 lpDDSurface)
	{
		Write_Log("Entering MyDirectDrawSurface3->AddAttachedSurface");
		return m_surface3->AddAttachedSurface(lpDDSurface);
	}

	HRESULT STDMETHODCALLTYPE AddOverlayDirtyRect(LPRECT lpRect)
	{
		Write_Log("Entering MyDirectDrawSurface3->AddOverlayDirtyRect");
		return m_surface3->AddOverlayDirtyRect(lpRect);
	}



HRESULT STDMETHODCALLTYPE Blt(LPRECT lpDestRect,                    
		LPDIRECTDRAWSURFACE3 lpDDSrcSurface,  
		LPRECT lpSrcRect,                     
		DWORD dwFlags,                        
		LPDDBLTFX lpDDBltFx)
	{
		Write_Log("Entering MyDirectDrawSurface3->Blt");
		return m_surface3->Blt(lpDestRect,                    
			lpDDSrcSurface,  
			lpSrcRect,                     
			dwFlags,                        
			lpDDBltFx);
	}

	HRESULT STDMETHODCALLTYPE BltBatch(	LPDDBLTBATCH lpDDBltBatch,
		DWORD dwCount,              
		DWORD dwFlags)
	{
		Write_Log("Entering MyDirectDrawSurface3->BltBatch");
		return m_surface3->BltBatch(lpDDBltBatch,  
			dwCount,              
			dwFlags);
	}

	HRESULT STDMETHODCALLTYPE BltFast(DWORD dwX,                            
		DWORD dwY,                            
		LPDIRECTDRAWSURFACE3 lpDDSrcSurface,  
		LPRECT lpSrcRect,                     
		DWORD dwTrans)
	{
		Write_Log("Entering MyDirectDrawSurface3->BltFast");
		return m_surface3->BltFast(dwX,                            
			dwY,                            
			lpDDSrcSurface,  
			lpSrcRect,                     
			dwTrans);
	}


	HRESULT STDMETHODCALLTYPE DeleteAttachedSurface(DWORD dwFlags,	LPDIRECTDRAWSURFACE3 lpDDSAttachedSurface )
	{
		Write_Log("Entering MyDirectDrawSurface3->DeleteAttachedSurface");
		return m_surface3->DeleteAttachedSurface(dwFlags,lpDDSAttachedSurface);
	}

	HRESULT STDMETHODCALLTYPE EnumAttachedSurfaces(LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback)
	{
		Write_Log("Entering MyDirectDrawSurface3->EnumAttachedSurfaces");
		return m_surface3->EnumAttachedSurfaces(lpContext,lpEnumSurfacesCallback);
	}


	HRESULT STDMETHODCALLTYPE EnumOverlayZOrders(DWORD dwFlags,                          
		LPVOID lpContext,                       
		LPDDENUMSURFACESCALLBACK lpfnCallback)
	{
		Write_Log("Entering MyDirectDrawSurface3->EnumOverlayZOrders");
		return m_surface3->EnumOverlayZOrders(dwFlags, lpContext ,lpfnCallback);
	}

	HRESULT STDMETHODCALLTYPE Flip(	LPDIRECTDRAWSURFACE3 lpDDSurfaceTargetOverride,  
		DWORD dwFlags)
	{
		Write_Log("Entering MyDirectDrawSurface3->Flip");
		RECT window_rect;
		::GetClientRect(m_hWnd, &window_rect);

		int n_width = window_rect.right;
		int n_height = window_rect.bottom;

		AdjustWindowRectEx(&window_rect,
			GetWindowStyle(m_hWnd),
			GetMenu(m_hWnd) != NULL,
			GetWindowExStyle(m_hWnd));

		int wd = window_rect.right - window_rect.left;
		int hg = window_rect.bottom - window_rect.top;

		RECT rc;
		::GetWindowRect(m_hWnd, &rc);

		int nl = rc.left +(-window_rect.left);
		int nr = rc.left +(-window_rect.left) + n_width;
		int nt = rc.top +(-window_rect.top);
		int nb = rc.top +(-window_rect.top) + n_height;

		RECT    dest_rect;
		
		dest_rect.left = nl;
		dest_rect.right = nr;
		dest_rect.top = nt;
		dest_rect.bottom = nb;

		RECT    src_rect;
		src_rect.left = 0;
		src_rect.right = rc1.right;
		src_rect.top = 0;
		src_rect.bottom = rc1.bottom;

		HRESULT hr= Blt(&dest_rect, m_backsurface, &src_rect, DDBLT_WAIT, NULL);
		
		return hr;
	}



HRESULT STDMETHODCALLTYPE GetAttachedSurface(LPDDSCAPS lpDDSCaps, 
		LPDIRECTDRAWSURFACE3 FAR *lplpDDAttachedSurface)
	{
		HRESULT hr;

		Write_Log("Entering MyDirectDrawSurface3->GetAttachedSurface");

		if (lpDDSCaps->dwCaps == DDSCAPS_BACKBUFFER)
		{
			Write_Log("MyDirectDrawSurface3->GetAttachedSurface BACKBUFFER");
			DDSURFACEDESC ddsd;
		
			memset(&ddsd,0,sizeof(ddsd));
			ddsd.dwSize  = sizeof(ddsd);
			ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

			RECT rc;
			GetClientRect(m_hWnd, &rc);
			ddsd.dwWidth  =  rc.right;
			ddsd.dwHeight =  rc.bottom;

			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_3DDEVICE;

			IDirectDrawSurface* m_backsurface_t;
			
			hr = m_device2->CreateSurface(&ddsd, &m_backsurface_t,  NULL  );
			if(SUCCEEDED(hr))
				Write_Log("MyDirectDrawSurface3->GetAttachedSurface CreateSurface OK BACKBUFFER");

			hr = m_backsurface_t->QueryInterface(IID_IDirectDrawSurface3,(LPVOID*) &m_backsurface);
			
			if(SUCCEEDED(hr))
				Write_Log("MyDirectDrawSurface3->GetAttachedSurface QueryInterface OK BACKBUFFER");

			*lplpDDAttachedSurface = m_backsurface;

			return hr;
		}
		else
		{
			hr = m_surface3->GetAttachedSurface(lpDDSCaps,	lplpDDAttachedSurface  	);
		}

		return hr;
	}

HRESULT STDMETHODCALLTYPE GetBltStatus(	DWORD dwFlags)
	{
		Write_Log("Entering MyDirectDrawSurface3->GetBltStatus");
		return m_surface3->GetBltStatus(dwFlags);
	}

	HRESULT STDMETHODCALLTYPE GetCaps(LPDDSCAPS lpDDSCaps)
	{
		Write_Log("Entering MyDirectDrawSurface3->GetCaps");
		return m_surface3->GetCaps(lpDDSCaps);
	}

	HRESULT STDMETHODCALLTYPE GetClipper(LPDIRECTDRAWCLIPPER FAR *lplpDDClipper)
	{
		Write_Log("Entering MyDirectDrawSurface3->GetClipper");
		return m_surface3->GetClipper(lplpDDClipper);
	}

	HRESULT STDMETHODCALLTYPE GetColorKey(DWORD dwFlags,             
		LPDDCOLORKEY lpDDColorKey)
	{
		Write_Log("Entering MyDirectDrawSurface3->GetColorKey");
		return m_surface3->GetColorKey(dwFlags,             
			lpDDColorKey);
	}

	HRESULT STDMETHODCALLTYPE GetDC(HDC FAR *lphDC)
	{
		Write_Log("Entering MyDirectDrawSurface3->GetDC");
		return m_surface3->GetDC(lphDC);
	}

	HRESULT STDMETHODCALLTYPE GetFlipStatus(DWORD dwFlags )
	{
		Write_Log("Entering MyDirectDrawSurface3->GetFlipStatus");
		return m_surface3->GetFlipStatus(dwFlags);
	}

	HRESULT STDMETHODCALLTYPE GetOverlayPosition(LPLONG lplX, 
		LPLONG lplY)
	{
		Write_Log("Entering MyDirectDrawSurface3->GetOverlayPosition");
		return m_surface3->GetOverlayPosition(lplX, lplY);
	}

	HRESULT STDMETHODCALLTYPE GetPalette(LPDIRECTDRAWPALETTE FAR *lplpDDPalette	)
	{
		Write_Log("Entering MyDirectDrawSurface3->GetPalette");
		
		HRESULT hr;
		hr =  m_surface3->GetPalette(lplpDDPalette);
		return hr;
	}

	HRESULT STDMETHODCALLTYPE GetPixelFormat(LPDDPIXELFORMAT lpDDPixelFormat)
	{
		Write_Log("Entering MyDirectDrawSurface3->GetPixelFormat");
		return m_surface3->GetPixelFormat(lpDDPixelFormat);
	}

	HRESULT STDMETHODCALLTYPE GetSurfaceDesc(LPDDSURFACEDESC lpDDSurfaceDesc)
	{
		Write_Log("Entering MyDirectDrawSurface3->GetSurfaceDesc");
				
		HRESULT hr;
		hr = m_surface3->GetSurfaceDesc(lpDDSurfaceDesc);
		
		if(SUCCEEDED(hr))
			Write_Log("Leave MyDirectDrawSurface3->GetSurfaceDesc OK");

		if(FAILED(hr))
			Write_Log("Leave MyDirectDrawSurface3->GetSurfaceDesc FAILED");
		
		return hr;
	}

	HRESULT STDMETHODCALLTYPE Initialize(LPDIRECTDRAW lpDD,  LPDDSURFACEDESC lpDDSurfaceDesc  )
	{
		Write_Log("Entering MyDirectDrawSurface3->Initialize");
		return m_surface3->Initialize(lpDD, lpDDSurfaceDesc  );
	}

	HRESULT STDMETHODCALLTYPE IsLost()
	{
		Write_Log("Entering MyDirectDrawSurface3->IsLost");
		return m_surface3->IsLost();
	}

	HRESULT STDMETHODCALLTYPE Lock(	LPRECT lpDestRect,                
		LPDDSURFACEDESC lpDDSurfaceDesc,  
		DWORD dwFlags,                    
		HANDLE hEvent)
	{
		Write_Log("Entering MyDirectDrawSurface3->Lock");

		return m_surface3->Lock(lpDestRect,                
			lpDDSurfaceDesc,  
			dwFlags,                    
			hEvent);
	}

	HRESULT STDMETHODCALLTYPE ReleaseDC(HDC hDC )
	{
		Write_Log("Entering MyDirectDrawSurface3->ReleaseDC");
		return m_surface3->ReleaseDC( hDC);
	}

	HRESULT STDMETHODCALLTYPE Restore()
	{
		Write_Log("Entering MyDirectDrawSurface3->Restore");
		return m_surface3->Restore();
	}

	HRESULT STDMETHODCALLTYPE SetClipper(LPDIRECTDRAWCLIPPER lpDDClipper)
	{
		Write_Log("Entering MyDirectDrawSurface3->SetClipper");
		return m_surface3->SetClipper(lpDDClipper);
	}

	HRESULT STDMETHODCALLTYPE SetColorKey(	DWORD dwFlags,             
		LPDDCOLORKEY lpDDColorKey)
	{
		Write_Log("Entering MyDirectDrawSurface3->SetColorKey");
		return m_surface3->SetColorKey(dwFlags,             
			lpDDColorKey  );
	}

	HRESULT STDMETHODCALLTYPE SetOverlayPosition(LONG lX, 
		LONG lY)
	{
		Write_Log("Entering MyDirectDrawSurface3->SetOverlayPosition");
		return m_surface3->SetOverlayPosition(lX, lY);
	}

	HRESULT STDMETHODCALLTYPE SetPalette(	LPDIRECTDRAWPALETTE lpDDPalette)
	{
		Write_Log("Entering MyDirectDrawSurface3->SetPalette");
		return m_surface3->SetPalette(lpDDPalette);
	}

	HRESULT STDMETHODCALLTYPE Unlock(LPVOID lpRect	)
	{
		Write_Log("Entering MyDirectDrawSurface3->Unlock");
		return m_surface3->Unlock(lpRect);
	}

	HRESULT STDMETHODCALLTYPE UpdateOverlay(LPRECT lpSrcRect,                      
		LPDIRECTDRAWSURFACE3 lpDDDestSurface,  
		LPRECT lpDestRect,                     
		DWORD dwFlags,                         
		LPDDOVERLAYFX lpDDOverlayFx)
	{
		Write_Log("Entering MyDirectDrawSurface3->UpdateOverlay");
		return m_surface3->UpdateOverlay(lpSrcRect, lpDDDestSurface, lpDestRect ,dwFlags, lpDDOverlayFx);
	}

	
	HRESULT STDMETHODCALLTYPE UpdateOverlayDisplay(	DWORD dwFlags)
	{
		Write_Log("Entering MyDirectDrawSurface3->UpdateOverlayDisplay");
		return m_surface3->UpdateOverlayDisplay(dwFlags);
	}

	HRESULT STDMETHODCALLTYPE UpdateOverlayZOrder(DWORD dwFlags,                       
		LPDIRECTDRAWSURFACE3 lpDDSReference)
	{
		Write_Log("Entering MyDirectDrawSurface3->UpdateOverlayZOrder");
		return m_surface3->UpdateOverlayZOrder(dwFlags, lpDDSReference);
	}

    HRESULT STDMETHODCALLTYPE GetDDInterface(LPVOID FAR * arg1)
	{
		Write_Log("Entering MyDirectDrawSurface3->GetDDInterface");
		return m_surface3->GetDDInterface(arg1);
	}
    HRESULT STDMETHODCALLTYPE PageLock(DWORD arg1)
	{
		Write_Log("Entering MyDirectDrawSurface3->PageLock");
		return m_surface3->PageLock(arg1);
	}
    HRESULT STDMETHODCALLTYPE PageUnlock(DWORD arg1)
	{
		Write_Log("Entering MyDirectDrawSurface3->PageUnlock");
		return m_surface3->PageUnlock(arg1);
	}

    HRESULT STDMETHODCALLTYPE SetSurfaceDesc(LPDDSURFACEDESC arg1, DWORD arg2)
	{
		Write_Log("Entering MyDirectDrawSurface3->SetSurfaceDesc");
		return m_surface3->SetSurfaceDesc(arg1, arg2);
	}
	
private:
	HWND m_hWnd;
	IDirectDrawSurface3* m_backsurface;
	IDirectDraw2 *m_device2;
	IDirectDrawSurface3* m_surface3;	
};

#endif