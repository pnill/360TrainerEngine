#include "common.h"

UINT32 g_titleID = 0x4D530805;

bool OneHitKo = false;
bool HooksSet = false;
BOOL InfHealth = FALSE;

VOID __declspec(naked) CheckDamage(VOID)
{

		/*
			8250F9AC ED806828             fsubs fr12,fr0,fr13 
			8250F9B0 FDAC036E             fsel fr13,fr12,fr13,fr0 
			8250F9B4 FC00EB6E             fsel fr0,fr0,fr13,fr29 
			8250F9B8 D01F054C             stfs fr0,54Ch(r31) 
		*/

	DWORD Ptr;

	__asm
	{
		mr Ptr,r0
	}

	__asm{
		/* Replace the overwritten code */
		fsubs fr12,fr0,fr13
		fsel fr13,fr12,fr13,fr0
		fsel fr0,fr0,fr13,fr29
	}

	if( Ptr != 0x01B4 && InfHealth == TRUE )
	{

	

	}else{

		__asm{
			stfs fr0,0x54C(r31)
		}
	
	}

	if( Ptr == 0x01B4 && OneHitKo == TRUE )
	{
		float nValue = 0.0;
		__asm
		{
			stfs nValue,0x54C(r31)
		}
	}

	__asm{
			/* return back to the game */
			/* 8250F9B8 */
			lis r11,0x8250
			ori r11,r11,0xF9BC
			mtctr r11
			bctr
	}
}

VOID ShowTrainerMain()
{
	Prompt = XShowMessageBoxUI(0, L"TheDefaced.org - DeadlyData", L"Bioshock V0.1\r\nView Codes: DPAD-DOWN(x4)", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_PASSCODEMODE, &g_mb_result, &g_xol);
}

VOID SetHooks()
{
		DWORD Stuff[4];
		patchInJump( (PDWORD)Stuff, (DWORD)CheckDamage, FALSE );
		//DmSetMemory( (PDWORD)0x8250F9AC, 4, &Stuff[0], NULL );
		//DmSetMemory( (PDWORD)0x8250F9B0, 4, &Stuff[1], NULL );
		//DmSetMemory( (PDWORD)0x8250F9B4, 4, &Stuff[2], NULL );
		//DmSetMemory( (PDWORD)0x8250F9B8, 4, &Stuff[3], NULL );
		//HooksSet = true;

		memcpy( (PDWORD)0x8250F9AC, &Stuff[0], 4 );
		memcpy( (PDWORD)0x8250F9B0, &Stuff[1], 4 );
		memcpy( (PDWORD)0x8250F9B4, &Stuff[2], 4 );
		memcpy( (PDWORD)0x8250F9B8, &Stuff[3], 4 );
		HooksSet = true;
}

VOID ProcessButtonPress( MESSAGEBOX_RESULT g_mb_result )
{

	if( g_mb_result.rgwPasscode[0] == DPAD_DOWN && g_mb_result.rgwPasscode[1] == DPAD_DOWN && g_mb_result.rgwPasscode[2] == DPAD_DOWN && g_mb_result.rgwPasscode[3] == DPAD_DOWN )
	{
		Sleep(500);
		XShowMessageBoxUI(0, L"Viewing All Codes", L"Inf Health DPAD-UP(x4)\r\nOne Hit Kills Y(x4)\r\nInf Plasma(Energy) RB(x4)\r\nInf Ammo(Buggy) LB(x4)\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);

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
			XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"Inf Health - Disabled\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			InfHealth = FALSE;
		}else{
			XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"Inf Health - Enabled\r\nDescription: You take no damage from other monsters.\r\nNOTE: This hasn't been tested against smoking, or other damaging enviorments.", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
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
			XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"One Hit Kills - Disabled\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			OneHitKo = false;
		}else{
			XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"One Hit Kills - Enabled\r\nDescription: Any monster you hit will die within the first hit.\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			OneHitKo = true;
		}

		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;
	}

	if( g_mb_result.rgwPasscode[0] == LEFT_BUMPER && g_mb_result.rgwPasscode[1] == LEFT_BUMPER && g_mb_result.rgwPasscode[2] == LEFT_BUMPER && g_mb_result.rgwPasscode[3] == LEFT_BUMPER )
	{
		Sleep(500);
		XShowMessageBoxUI(0, L"Cheat Enabled", L"Inf Ammo\r\nDescription: !WARNING! - CANNOT BE DISABLED\r\nNOTE: This is not an permanent solution and is buggy your ammo will go negative and sometimes you won't be able to switch to the weapon.\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
		
		BYTE Nop[4] = { 0x60, 0x00, 0x00, 0x00 };
		//DmSetMemory( (void*)0x824775BC, 4, Nop, NULL );
		//DmSetMemory( (void*)0x824775B8, 4, Nop, NULL );
		memcpy( (void*)0x824775BC, Nop, 0x04 );
		memcpy( (void*)0x824775B8, Nop, 0x04 );

		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;

	}

	if( g_mb_result.rgwPasscode[0] == RIGHT_BUMPER && g_mb_result.rgwPasscode[1] == RIGHT_BUMPER && g_mb_result.rgwPasscode[2] == RIGHT_BUMPER && g_mb_result.rgwPasscode[3] == RIGHT_BUMPER )
	{

		Sleep(500);
		
	
			XShowMessageBoxUI(0, L"Cheat Enabled", L"Inf Plasma(Shock)Energy\r\nDescription: !WARNING! - CANNOT BE DISABLED\r\nWhen enabled you should have an inf ammount of the shock/plasma energy from your hands.\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			
			BYTE Nop[4] = { 0x60, 0x00, 0x00, 0x00 };
			//DmSetMemory( (void*)0x824CF6AC, 4, Nop, NULL );
			memcpy( (void*)0x824CF6AC, Nop, 4 );

			memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
			Prompt = 0x9999;
		

	}


}