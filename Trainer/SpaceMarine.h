#include "common.h"

UINT32 g_titleID = 0x4D530805;

bool OneHitKo = false;
bool HooksSet = false;
BOOL InfHealth = FALSE;

VOID __declspec(naked) CheckDamage(VOID)
{

		/*
		826CF764 D3DF0018             stfs fr30,18h(r31) 
		826CF768 D3FF000C             stfs fr31,0Ch(r31) 
		826CF76C 995F00C0             stb r10,0C0h(r31) 
		826CF770 D3DF0020             stfs fr30,20h(r31) 

		r31 + 0x0D0 == 1 // Player
		else
		//AI
		
		r31 + 0x84 == 42749e10 // Player
		else
		AI

		*/

	DWORD Ptr;

	__asm
	{
		mr Ptr,r31
	}
	

	//DbgPrint("PTR+0x84: %08X\r\n",*(DWORD*)(Ptr+0x84));



	if( InfHealth == TRUE && *(DWORD*)(Ptr+0x84) == 0x42749e10 )
	{
		float pValue = 900.0f;
		__asm
		{
				stfs pValue,18h(r31) 
				stfs pValue,0Ch(r31) 
				stb r10,0C0h(r31) 
				stfs pValue,20h(r31) 
		}

	}else{
		if( *(DWORD*)(Ptr+0x84) == 0x42749e10 )
		{
			__asm{
				stfs fr30,18h(r31) 
				stfs fr31,0Ch(r31) 
				stb r10,0C0h(r31) 
				stfs fr30,20h(r31) 
			}
		}

	}
	
	if( OneHitKo == TRUE && *(DWORD*)(Ptr+0x84) != 0x42749e10 )
	{
		float nValue = 0.0f;
		__asm{
				stfs nValue,18h(r31) 
				stfs nValue,0Ch(r31) 
				stb r10,0C0h(r31) 
				stfs nValue,20h(r31) 
		}

	}else{
		
		if(  *(DWORD*)(Ptr+0x84) != 0x42749e10 )
		{	
			__asm{
				stfs fr30,18h(r31) 
				stfs fr31,0Ch(r31) 
				stb r10,0C0h(r31) 
				stfs fr30,20h(r31) 
			 }
		}

	}

	__asm{
			/* return back to the game */
			/* 826CF770 */
			lis r11,0x826C
			ori r11,r11,0xF774
			mtctr r11
			bctr
	}
}

VOID ShowTrainerMain()
{
	Prompt = XShowMessageBoxUI(0, L"TheDefaced.org - DeadlyData", L"Space MarineTrainer V0.1\r\nView Codes: DPAD-DOWN(x4)", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_PASSCODEMODE, &g_mb_result, &g_xol);
}

VOID SetHooks()
{

		/*
		826CF764 D3DF0018             stfs fr30,18h(r31) 
		826CF768 D3FF000C             stfs fr31,0Ch(r31) 
		826CF76C 995F00C0             stb r10,0C0h(r31) 
		826CF770 D3DF0020             stfs fr30,20h(r31) 
		*/

		DWORD Stuff[4];
		patchInJump( (PDWORD)Stuff, (DWORD)CheckDamage, FALSE );
		//DmSetMemory( (PDWORD)0x826CF764, 4, &Stuff[0], NULL );
		//DmSetMemory( (PDWORD)0x826CF768, 4, &Stuff[1], NULL );
		//DmSetMemory( (PDWORD)0x826CF76C, 4, &Stuff[2], NULL );
		//DmSetMemory( (PDWORD)0x826CF770, 4, &Stuff[3], NULL );
		//HooksSet = true;

		memcpy( (PDWORD)0x826CF764, &Stuff[0], 4 );
		memcpy( (PDWORD)0x826CF768, &Stuff[1], 4 );
		memcpy( (PDWORD)0x826CF76C, &Stuff[2], 4 );
		memcpy( (PDWORD)0x826CF770, &Stuff[3], 4 );
		HooksSet = true;
}

VOID ProcessButtonPress( MESSAGEBOX_RESULT g_mb_result )
{

	if( g_mb_result.rgwPasscode[0] == DPAD_DOWN && g_mb_result.rgwPasscode[1] == DPAD_DOWN && g_mb_result.rgwPasscode[2] == DPAD_DOWN && g_mb_result.rgwPasscode[3] == DPAD_DOWN )
	{
		Sleep(500);
		XShowMessageBoxUI(0, L"Viewing All Codes", L"Inf Health DPAD-UP(x4)\r\nOne Hit Kills Y(x4)\r\nInf Ammo(Includes Grenades) X(x4)\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);

		if( HooksSet == false )
		{
			SetHooks();
		}

		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;

	}

	if( g_mb_result.rgwPasscode[0] == DPAD_UP && g_mb_result.rgwPasscode[1] == DPAD_UP && g_mb_result.rgwPasscode[2] == DPAD_UP && g_mb_result.rgwPasscode[3] == DPAD_UP )
	{
		Sleep(500);
		
		if( InfHealth == TRUE )
		{
			XShowMessageBoxUI(0, L"Cheat Disabled Successfully", L"Inf Health - Disabled\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			InfHealth = FALSE;
		}else{
			XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"Inf Health - Enabled\r\nDescription: You take no damage from enemies/or your own nades/etc..\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			InfHealth = TRUE;
		}

		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;

	}

	if( g_mb_result.rgwPasscode[0] == Y_BUTTON && g_mb_result.rgwPasscode[1] == Y_BUTTON && g_mb_result.rgwPasscode[2] == Y_BUTTON && g_mb_result.rgwPasscode[3] == Y_BUTTON )
	{
		Sleep(500);

		if( OneHitKo == true )
		{
			XShowMessageBoxUI(0, L"Cheat Disabled Successfully", L"One Hit Kills - Disabled\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			OneHitKo = false;
		}else{
			XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"One Hit Kills - Enabled\r\nDescription: Any monster/enemy you hit will die within the first hit.\r\n!WARNING!: NPC(s)/Friendly AI on your side will also die in one hit.", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			OneHitKo = true;
		}

		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;
	}

	

	if( g_mb_result.rgwPasscode[0] == X_BUTTON && g_mb_result.rgwPasscode[1] == X_BUTTON && g_mb_result.rgwPasscode[2] == X_BUTTON && g_mb_result.rgwPasscode[3] == X_BUTTON )
	{
		Sleep(500);
		
		XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"Inf Ammo\r\n!WARNING! - CANNOT BE DISABLED\r\nWhen enabled you'll have unlimited ammo no reload, this includes all weapons (Grenades also)", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
		BYTE Nop[4] = { 0x60, 0x00, 0x00, 0x00 };
		//DmSetMemory( (void*)0x826472B8, 4, Nop, NULL );
	
		/*
		826472B0 EDA0F828             fsubs fr13,fr0,fr31 
		826472B4 C00B0A68             lfs fr0,0A68h(r11) 
		826472B8 FD806850             fneg fr12,fr13 
		826472BC FD6C682E             fsel fr11,fr12,fr0,fr13 
		826472C0 D17E0008             stfs fr11,8(r30) 
		*/

		memcpy( (void*)0x826472B0, Nop, 4);
		memcpy( (void*)0x826472B8, Nop, 4);
		memcpy( (void*)0x826472C0, Nop, 4);

		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;
	}


	


}