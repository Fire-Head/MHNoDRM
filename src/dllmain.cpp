#include <Windows.h>
#include "CPatch.h"

#pragma comment(linker, "/ENTRY:DllMain")
#pragma comment(linker, "/nodefaultlib:libcmt")
#pragma comment(lib, "msvcrt.lib")

#define ARRAY_SIZE(array)                (sizeof(array) / sizeof(array[0]))


// 0x42BDC5 GetLastError();
void __declspec(naked) DrmCheck00()
{
	__asm
	{
		mov     eax, 1
		retn
	}
}

// 0x43A007 BOOL IsBadReadPtr(const VOID *lp, UINT_PTR   ucb);
void __declspec(naked) DrmCheck01()
{
	__asm
	{
		xor     eax, eax
		retn    8
	}
}

// 0x4667DE DWORD GetVersion();
void __declspec(naked) DrmCheck02()
{
	__asm
	{
		mov     eax, ecx
		retn
	}
}

// 0x46D68A BOOL IsBadCodePtr(FARPROC lpfn);
void __declspec(naked) DrmCheck03()
{
	__asm
	{
		mov     ecx, dword ptr [esp+4+0Ch]
		mov     eax, dword ptr [esp+4+8]
		mov     dword ptr [eax], ecx
		retn    4
	}
}

// 0x4732AC DWORD GetLastError();
void __declspec(naked) DrmCheck04()
{
	__asm
	{
		mov     eax, edx
		retn
	}
}

// 0x474EB9 DWORD GetVersion();
void __declspec(naked) DrmCheck05()
{
	__asm
	{
		mov     eax, dword ptr [esp+0Ch]
		retn
	}
}

// 0x47D05E BOOL IsBadWritePtr(LPVOID   lp, UINT_PTR ucb);
void __declspec(naked) DrmCheck06()
{
	__asm
	{
		mov     eax, 0
		retn    8
	}
}

// 0x4C78A2 DWORD GetVersion();
void __declspec(naked) DrmCheck07()
{
	__asm
	{
		mov     eax, 1
		retn
	}
}

// 0x4CC48E HANDLE GetCurrentThread();
void __declspec(naked) DrmCheck08()
{
	__asm
	{
		mov     dword ptr [ebx], ecx
		retn
	}
}

// 0x4D26F6 HANDLE GetCurrentThread();
void __declspec(naked) DrmCheck09()
{
	__asm
	{
		mov     dword ptr [esp], esi
		retn
	}
}

// 0x4D4065 BOOL IsBadReadPtr(const VOID *lp, UINT_PTR   ucb);
void __declspec(naked) DrmCheck10()
{
	__asm
	{
		mov     eax, dword ptr [esp+8+0Ch]
		retn    8
	}
}

// 0x4D7E7C DWORD GetLastError();
void __declspec(naked) DrmCheck11()
{
	__asm
	{
		mov     eax, edx
		retn
	}
}

// 0x4D84E0 BOOL IsBadCodePtr(FARPROC lpfn);
void __declspec(naked) DrmCheck12()
{
	__asm
	{
		mov     eax, dword ptr [esp+4+8]
		mov     ecx, [edx]
		mov     dword ptr [eax], ecx
		retn    4
	}
}

// 0x4F2230 BOOL IsBadWritePtr(LPVOID   lp, UINT_PTR ucb);
void __declspec(naked) DrmCheck13()
{
	__asm
	{
		mov     eax, dword ptr [esp+8+8]
		mov     dword ptr [eax], 1
		retn    8
	}
}

// 0x4F9B5E BOOL IsBadReadPtr(const VOID *lp, UINT_PTR   ucb);
void __declspec(naked) DrmCheck14()
{
	__asm
	{
		mov     dword ptr [esp], esi
		retn    8
	}
}

// 0x5FFCE8 DWORD GetVersion();
void __declspec(naked) DrmCheck15()
{
	__asm
	{
		mov     eax, ecx
		retn
	}
}

struct { int address; void *func; }
DrmCheckPatches[] =
{
	{ 0x42BDC5, DrmCheck00 },	// Better Vision
	{ 0x43A007, DrmCheck01 },	// Broken SaveGame EntityData
	{ 0x4667DE, DrmCheck02 },	// DropDeadBody Crash
	{ 0x46D68A, DrmCheck03 },	// More Damage
	{ 0x4732AC, DrmCheck04 },	// Broken Health 1
	{ 0x474EB9, DrmCheck05 },	// Ignore Control 1
	{ 0x47D05E, DrmCheck06 },	// Help Text Crash
	{ 0x4C78A2, DrmCheck07 },	// Drop Item Timer
	{ 0x4CC48E, DrmCheck08 },	// Broken Doors
	{ 0x4D26F6, DrmCheck09 },	// Broken Health 2
	{ 0x4D4065, DrmCheck10 },	// Broken Useables
	{ 0x4D7E7C, DrmCheck11 },	// Broken Level Initialization 1
	{ 0x4D84E0, DrmCheck12 },	// Broken Level Initialization 2
	{ 0x4F2230, DrmCheck13 },	// Ignore Control 2
	{ 0x4F9B5E, DrmCheck14 },	// Less Ammo
	{ 0x5FFCE8, DrmCheck15 }	// Broken SaveGame Button
};

// Unlike GTA III/VC mss32.dll from Manhunt unloads fake .asi's, so here is the fix
extern "C" __declspec(dllexport) int WINAPI RIB_Main(unsigned int, unsigned int) { return TRUE; }

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if(reason == DLL_PROCESS_ATTACH)
	{
		for ( int i = 0; i < ARRAY_SIZE(DrmCheckPatches); i++ )
		{
			if ( CPatch::CheckChar(DrmCheckPatches[i].address-2, 0xFF) && CPatch::CheckChar(DrmCheckPatches[i].address-1, 0x15) )
				CPatch::SetPointer(DrmCheckPatches[i].address, &DrmCheckPatches[i].func);
		}
	}

	return TRUE;
}