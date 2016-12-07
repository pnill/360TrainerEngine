#include "common.h"
#include "Mw3.h"
//#include "BO2_XUID.h"

DWORD dwPlayerIndex = 0;
XINPUT_STATE state;


#ifdef __cplusplus

extern "C"
{
#endif
	VOID NTAPI XeCryptRandom( OUT BYTE * pb, IN DWORD cb );
	DWORD __stdcall ExCreateThread(PHANDLE pHandle, DWORD dwStackSize, LPDWORD lpThreadId, VOID* apiThreadStartup , LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlagsMod);
	VOID __stdcall ExRegisterTitleTerminateNotification(PEX_TITLE_TERMINATE_REGISTRATION, BOOL);
	DWORD XamGetCurrentTitleId();
#ifdef __cplusplus
}
#endif



void InitializeMe()
{

	//Mw3 
	if( XamGetCurrentTitleId() == 0x415608CB )
	{
		//if(*(int*)(0x82111688) == 0x7D8802A6)
		//{ 
			PatchDrawVert();
			//printf("XamGetCurrenTitleId(): %08X\r\n",XamGetCurrentTitleId());
		//}
	}

	//if(*(int*)(0x8293D724) == 0x3D60816E )
	//{
	//	PatchDrawVert();
	//}

	
	
	Sleep(3000); 
	return;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
	switch( fdwReason )
	{
		case DLL_PROCESS_ATTACH:
			InitializeMe();

		break;
	}
	return(TRUE);
}