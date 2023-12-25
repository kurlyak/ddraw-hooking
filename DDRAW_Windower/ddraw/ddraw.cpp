//==========================================================================
//	Ed Kurlyak 2023 DirectDraw Hook
//==========================================================================

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "MyDirectDraw.h"
#include "WriteLog.h"

#include <ddraw.h>
#pragma comment (lib, "dxguid.lib")


HHOOK hHook = NULL;

HMODULE hDdrawDll;
FARPROC	OrigAcquireDDThreadLock;
FARPROC	OrigCheckFullscreen;
FARPROC	OrigCompleteCreateSysmemSurface;
FARPROC	OrigD3DParseUnknownCommand;
FARPROC	OrigDDGetAttachedSurfaceLcl;
FARPROC	OrigDDInternalLock;
FARPROC	OrigDDInternalUnlock;
FARPROC	OrigDSoundHelp;
FARPROC	OrigDirectDrawCreate;
FARPROC	OrigDirectDrawCreateClipper;
FARPROC	OrigDirectDrawCreateEx;
FARPROC	OrigDirectDrawEnumerateA;
FARPROC	OrigDirectDrawEnumerateExA;
FARPROC	OrigDirectDrawEnumerateExW;
FARPROC	OrigDirectDrawEnumerateW;
FARPROC	OrigDllCanUnloadNow;
FARPROC	OrigDllGetClassObject;
FARPROC	OrigGetDDSurfaceLocal;
FARPROC	OrigGetOLEThunkData;
FARPROC	OrigGetSurfaceFromDC;
FARPROC	OrigRegisterSpecialCase;
FARPROC	OrigReleaseDDThreadLock;

typedef HRESULT (WINAPI *DirectDrawCreate_Type)( GUID FAR *lpGUID, LPVOID *lplpDD, IUnknown FAR *pUnkOuter );

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
	if (nCode < 0 || nCode != HC_ACTION ) return CallNextHookEx( hHook, nCode, wParam, lParam); 

	if(!(lParam & 0x40000000)) // Key Down
	{
		switch (wParam)
		{
			case VK_MENU:
			{
				return 1;
			};

		}
	}

	return CallNextHookEx( hHook, nCode, wParam, lParam); 
}


__declspec(naked) void MyAcquireDDThreadLock()
{
	_asm
	{
		jmp [OrigAcquireDDThreadLock] 
	} 
}
__declspec(naked) void MyCheckFullscreen()
{
	_asm
	{
		jmp [OrigCheckFullscreen] 
	} 
}
__declspec(naked) void MyCompleteCreateSysmemSurface()
{
	_asm
	{
		jmp [OrigCompleteCreateSysmemSurface] 
	} 
}
__declspec(naked) void MyD3DParseUnknownCommand()
{
	_asm
	{
		jmp [OrigD3DParseUnknownCommand] 
	} 
}
__declspec(naked) void MyDDGetAttachedSurfaceLcl()
{
	_asm
	{
		jmp [OrigDDGetAttachedSurfaceLcl] 
	} 
}
__declspec(naked) void MyDDInternalLock()
{
	_asm
	{
		jmp [OrigDDInternalLock] 
	} 
}
__declspec(naked) void MyDDInternalUnlock()
{
	_asm
	{
		jmp [OrigDDInternalUnlock] 
	} 
}
__declspec(naked) void MyDSoundHelp()
{
	_asm
	{
		jmp [OrigDSoundHelp] 
	} 
}

__declspec(dllexport) HRESULT WINAPI  MyDirectDrawCreate(GUID FAR *lpGUID, LPVOID *lplpDD, IUnknown FAR *pUnkOuter )
{ 
	Write_Log("DirectDrawCreate");

	DirectDrawCreate_Type OldFn = (DirectDrawCreate_Type)OrigDirectDrawCreate;
	HRESULT hr = OldFn( lpGUID, lplpDD, pUnkOuter );

	*lplpDD = new MyDirectDraw((LPDIRECTDRAW)*lplpDD);

	return hr;
}

__declspec(naked) void MyDirectDrawCreateClipper()
{
	_asm
	{
		jmp [OrigDirectDrawCreateClipper]
	} 
}
__declspec(naked) void MyDirectDrawCreateEx()
{
	_asm
	{
		jmp [OrigDirectDrawCreateEx] 
	} 
}
__declspec(naked) void MyDirectDrawEnumerateA()
{
	_asm
	{
		jmp [OrigDirectDrawEnumerateA] 
	} 
}
__declspec(naked) void MyDirectDrawEnumerateExA()
{
	_asm
	{
		jmp [OrigDirectDrawEnumerateExA] 
	} 
}
__declspec(naked) void MyDirectDrawEnumerateExW()
{
	_asm
	{
		jmp [OrigDirectDrawEnumerateExW] 
	} 
}
__declspec(naked) void MyDirectDrawEnumerateW()
{
	_asm
	{
		jmp [OrigDirectDrawEnumerateW] 
	} 
}
__declspec(naked) void MyDllCanUnloadNow()
{
	_asm
	{
		jmp [OrigDllCanUnloadNow] 
	} 
}
__declspec(naked) void MyDllGetClassObject()
{
	_asm
	{
		jmp [OrigDllGetClassObject] 
	} 
}
__declspec(naked) void MyGetDDSurfaceLocal()
{
	_asm
	{
		jmp [OrigGetDDSurfaceLocal] 
	} 
}
__declspec(naked) void MyGetOLEThunkData()
{
	_asm
	{
		jmp [OrigGetOLEThunkData] 
	} 
}
__declspec(naked) void MyGetSurfaceFromDC()
{
	_asm
	{
		jmp [OrigGetSurfaceFromDC] 
	} 
}
__declspec(naked) void MyRegisterSpecialCase()
{
	_asm
	{
		jmp [OrigRegisterSpecialCase] 
	} 
}
__declspec(naked) void MyReleaseDDThreadLock()
{
	_asm
	{
		jmp [OrigReleaseDDThreadLock] 
	} 
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	char szPath[MAX_PATH];
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:

			HANDLE mut;
			mut = CreateMutex(NULL, FALSE, "TR3WINDOWMODE");
			DWORD result;
			result = WaitForSingleObject(mut,0);
			if (result == WAIT_OBJECT_0) 
			{
				/*
				FILE *f;
				fopen_s(&f, "log.txt", "w");
				fclose(f);
				*/	
			
				Write_Log("DLL_PROCESS_ATTACH");

				SetWindowsHookEx( WH_KEYBOARD, HookProc, NULL, GetCurrentThreadId());

				CopyMemory(szPath+GetSystemDirectory(szPath,MAX_PATH-10), "\\ddraw.dll",11);
				hDdrawDll = LoadLibrary(szPath);
				if (!hDdrawDll)
				{
					MessageBox(0, "Cannot load ddraw.dll", "TR3 DDRAW.DLL", MB_OK);
					ExitProcess(0);
				}
				
				OrigAcquireDDThreadLock			= GetProcAddress(hDdrawDll, "AcquireDDThreadLock");
				OrigCheckFullscreen				= GetProcAddress(hDdrawDll, "CheckFullscreen");
				OrigCompleteCreateSysmemSurface	= GetProcAddress(hDdrawDll, "CompleteCreateSysmemSurface");
				OrigD3DParseUnknownCommand		= GetProcAddress(hDdrawDll, "D3DParseUnknownCommand");
				OrigDDGetAttachedSurfaceLcl		= GetProcAddress(hDdrawDll, "DDGetAttachedSurfaceLcl");
				OrigDDInternalLock				= GetProcAddress(hDdrawDll, "DDInternalLock");
				OrigDDInternalUnlock			= GetProcAddress(hDdrawDll, "DDInternalUnlock");
				OrigDSoundHelp					= GetProcAddress(hDdrawDll, "DSoundHelp");
				OrigDirectDrawCreate			= GetProcAddress(hDdrawDll, "DirectDrawCreate");
				OrigDirectDrawCreateClipper		= GetProcAddress(hDdrawDll, "DirectDrawCreateClipper");
				OrigDirectDrawCreateEx			= GetProcAddress(hDdrawDll, "DirectDrawCreateEx");
				OrigDirectDrawEnumerateA		= GetProcAddress(hDdrawDll, "DirectDrawEnumerateA");
				OrigDirectDrawEnumerateExA		= GetProcAddress(hDdrawDll, "DirectDrawEnumerateExA");
				OrigDirectDrawEnumerateExW		= GetProcAddress(hDdrawDll, "DirectDrawEnumerateExW");
				OrigDirectDrawEnumerateW		= GetProcAddress(hDdrawDll, "DirectDrawEnumerateW");
				OrigDllCanUnloadNow				= GetProcAddress(hDdrawDll, "DllCanUnloadNow");
				OrigDllGetClassObject			= GetProcAddress(hDdrawDll, "DllGetClassObject");
				OrigGetDDSurfaceLocal			= GetProcAddress(hDdrawDll, "GetDDSurfaceLocal");
				OrigGetOLEThunkData				= GetProcAddress(hDdrawDll, "GetOLEThunkData");
				OrigGetSurfaceFromDC			= GetProcAddress(hDdrawDll, "GetSurfaceFromDC");
				OrigRegisterSpecialCase			= GetProcAddress(hDdrawDll, "RegisterSpecialCase");
				OrigReleaseDDThreadLock			= GetProcAddress(hDdrawDll, "ReleaseDDThreadLock");
			}
			else
			{
				FreeLibraryAndExitThread(hModule, 0);
				Write_Log("FreeLibraryAndExitThread");
			}
			break;

			case DLL_PROCESS_DETACH:
				Write_Log("DLL_PROCESS_DETACH");
				UnhookWindowsHookEx( hHook );
				FreeLibrary(hDdrawDll);
			break;
		}
	return TRUE;
}