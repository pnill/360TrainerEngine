#include "common.h"

UINT32 g_titleID = 0x4D530805;

bool OneHitKo = false;

VOID __declspec(naked) CheckDamage(VOID)
{
	DWORD Ptr;

	__asm
	{
		mr Ptr,r29
	}

	__asm{
		/* Replace the overwritten code */
		lfs fr0, 0xC(r29)
		fadds fr0, fr0, fr30
	}

	if( Ptr != **(DWORD**)0x82B62568)
	{

		if(OneHitKo)
		{
			float n = 0.0f;
			__asm{
				stfs n, 0xC(r29)
			}
		}else{
			__asm{
				stfs fr0, 0xC(r29)
			}
		}

	}

	//DbgPrint("PlayerPTR: %08X\r\nPTR: %08X\r\n",**(DWORD**)0x82B62568,Ptr);
	
	__asm{
			fcmpu cr6, fr0, fr31

			/* return back to the game */
			lis r11,0x8217
			ori r11,r11,0xEB54
			mtctr r11
			bctr
	}
}

VOID ShowTrainerMain()
{
	Prompt = XShowMessageBoxUI(0, L"TheDefaced.org - DeadlyData", L"Kameo V0.2\r\nView Codes: DPAD-DOWN(x4)", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_PASSCODEMODE, &g_mb_result, &g_xol);
}

VOID ProcessButtonPress( MESSAGEBOX_RESULT g_mb_result )
{

	if( g_mb_result.rgwPasscode[0] == DPAD_DOWN && g_mb_result.rgwPasscode[1] == DPAD_DOWN && g_mb_result.rgwPasscode[2] == DPAD_DOWN && g_mb_result.rgwPasscode[3] == DPAD_DOWN )
	{
		Sleep(500);
		XShowMessageBoxUI(0, L"Viewing All Codes", L"Inf Coins - DPAD-UP(x4)\r\nInf Stamina - Y(x4)\r\nInf Health - X(x4)\r\nOne Hit Kills - RB(x4)\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
	
		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;

	}

	if( g_mb_result.rgwPasscode[0] == DPAD_UP && g_mb_result.rgwPasscode[1] == DPAD_UP && g_mb_result.rgwPasscode[2] == DPAD_UP && g_mb_result.rgwPasscode[3] == DPAD_UP )
	{
		Sleep(500);
		XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"Inf Runes(Coins) - Enabled\r\nDescription: Gives you 65535 coins and makes sure they don't go down from there.\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
		BYTE Nop[4] = { 0x60, 0x00, 0x00, 0x00 };
		
		memcpy( (void*)0x8217F2D0, Nop, 4 );
		memcpy( (void*)0x8217F314, Nop, 4 );
		*(DWORD*)(**(DWORD**)0x82B62568+0x18) = 0xFFFF;

		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;

	}

	if( g_mb_result.rgwPasscode[0] == Y_BUTTON && g_mb_result.rgwPasscode[1] == Y_BUTTON && g_mb_result.rgwPasscode[2] == Y_BUTTON && g_mb_result.rgwPasscode[3] == Y_BUTTON )
	{
		Sleep(500);
		XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"Inf Stamina - Enabled\r\nDescription: Makes it so your stamina never goes down ( also known as the spirit bar )\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);

		BYTE Nop[4] = { 0x60, 0x00, 0x00, 0x00 };
		memcpy( (void*)0x8217EDDC, Nop, 4 );
		memcpy( (void*)0x8217EDC0, Nop, 4 );
		//DmSetMemory( (void*)0x8217EDDC, 4, Nop, NULL );
		//DmSetMemory( (void*)0x8217EDC0, 4, Nop, NULL );

		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;
	}

	if( g_mb_result.rgwPasscode[0] == RIGHT_BUMPER && g_mb_result.rgwPasscode[1] == RIGHT_BUMPER && g_mb_result.rgwPasscode[2] == RIGHT_BUMPER && g_mb_result.rgwPasscode[3] == RIGHT_BUMPER )
	{

		Sleep(500);
		if( OneHitKo == false )
		{
			XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"One Hit Kills - Enabled\r\nDescription: Any monster you hit will die within the first hit.\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			OneHitKo = true;

			memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
			Prompt = 0x9999;
		}else{
			XShowMessageBoxUI(0, L"Cheat Disabled Successfully", L"One Hit Kills - Disabled", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			OneHitKo = false;

			memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
			Prompt = 0x9999;
		}
		
	}

	if( g_mb_result.rgwPasscode[0] == X_BUTTON && g_mb_result.rgwPasscode[1] == X_BUTTON && g_mb_result.rgwPasscode[2] == X_BUTTON && g_mb_result.rgwPasscode[3] == X_BUTTON )
	{
		Sleep(500);
		XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"Inf Health - Enabled\r\nDescription: Your health never goes down\r\n!WARNING!: Falling of a ledge can still force your health to go down this is only meant for being hit.", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);

		DWORD Stuff[4];
		patchInJump( (PDWORD)Stuff, (DWORD)CheckDamage, FALSE );
		memcpy( (PDWORD)0x8217EB44, &Stuff[0], 4 );
		memcpy( (PDWORD)0x8217EB48, &Stuff[1], 4 );
		memcpy( (PDWORD)0x8217EB4C, &Stuff[2], 4 );
		memcpy( (PDWORD)0x8217EB50, &Stuff[3], 4 );

		//DmSetMemory( (PDWORD)0x8217EB44, 4, &Stuff[0], NULL );
		//DmSetMemory( (PDWORD)0x8217EB48, 4, &Stuff[1], NULL );
		//DmSetMemory( (PDWORD)0x8217EB4C, 4, &Stuff[2], NULL );
		//DmSetMemory( (PDWORD)0x8217EB50, 4, &Stuff[3], NULL );

		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;
	}

}