#include "common.h"

UINT32 g_titleID = 0x4D530805;

bool OneHitKo = false;
bool HooksSet = false;
BOOL InfHealth = FALSE;

VOID __declspec(naked) CheckDamage(VOID)
{

		/*
			827004CC FD4B036E             fsel fr10,fr11,fr13,fr0 
			827004D0 FD20EAAE             fsel fr9,fr0,fr10,fr29 
			827004D4 FD004818             frsp fr8,fr9 
			827004D8 D11F05C4             stfs fr8,5C4h(r31)  
		*/

	DWORD Ptr;

	__asm
	{
		mr Ptr,r0
	}

	__asm{
		/* Replace the overwritten code */
		fsel fr10,fr11,fr13,fr0
		fsel fr9,fr0,fr10,fr29
		frsp fr8,fr9
	}

	if( Ptr != 0x010C && InfHealth == TRUE )
	{
		float pValue = 1000.0f;
		__asm
		{
			stfs pValue,0x5C4(r31)
		}

	}else{
		if( Ptr != 0x010C )
		{
			__asm{
				stfs fr8,0x5C4(r31)
			}
		}
	
	}

	if( Ptr == 0x010C && OneHitKo == TRUE )
	{

		float nValue = 0.0;
		__asm
		{
			stfs nValue,0x5C4(r31)
		}

	}else{

		if( Ptr == 0x010C)
		{
			__asm
			{
				stfs fr8,0x5C4(r31)
			}
		}


	}

	__asm{
			/* return back to the game */
			/* 827004D8 */
			lis r11,0x8270
			ori r11,r11,0x04DC
			mtctr r11
			bctr
	}
}

VOID ShowTrainerMain()
{
	Prompt = XShowMessageBoxUI(0, L"TheDefaced.org - DeadlyData", L"Bioshock 2 V0.1\r\nView Codes: DPAD-DOWN(x4)", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_PASSCODEMODE, &g_mb_result, &g_xol);
}

VOID SetHooks()
{
		DWORD Stuff[4];
		patchInJump( (PDWORD)Stuff, (DWORD)CheckDamage, FALSE );
		//DmSetMemory( (PDWORD)0x827004CC, 4, &Stuff[0], NULL );
		//DmSetMemory( (PDWORD)0x827004D0, 4, &Stuff[1], NULL );
		//DmSetMemory( (PDWORD)0x827004D4, 4, &Stuff[2], NULL );
		//DmSetMemory( (PDWORD)0x827004D8, 4, &Stuff[3], NULL );
		//HooksSet = true;

		memcpy( (PDWORD)0x827004CC, &Stuff[0], 4 );
		memcpy( (PDWORD)0x827004D0, &Stuff[1], 4 );
		memcpy( (PDWORD)0x827004D4, &Stuff[2], 4 );
		memcpy( (PDWORD)0x827004D8, &Stuff[3], 4 );
		HooksSet = true;
}

VOID ProcessButtonPress( MESSAGEBOX_RESULT g_mb_result )
{

	if( g_mb_result.rgwPasscode[0] == DPAD_DOWN && g_mb_result.rgwPasscode[1] == DPAD_DOWN && g_mb_result.rgwPasscode[2] == DPAD_DOWN && g_mb_result.rgwPasscode[3] == DPAD_DOWN )
	{
		Sleep(500);
		XShowMessageBoxUI(0, L"Viewing All Codes", L"Inf Health DPAD-UP(x4)\r\nOne Hit Kills Y(x4)\r\nInf Plasma(Energy) RB(x4)\r\nInf Drill Fuel LB(x4)\r\nInf Ammo X(x4)\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);

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
			XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"Inf Health - Enabled\r\nDescription: You take no damage from other monsters.\r\nNOTE: This hasn't been tested against smoking, or other damaging environments.", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
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
		XShowMessageBoxUI(0, L"Cheat Enabled", L"Inf Drill Fuel\r\nDescription: !WARNING! - CANNOT BE DISABLED\r\nThis is just a random option that seems to have a big impact on the game, I.E. After enabling it I noticed having Inf Eve Injections and other things as well.\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
		
		BYTE Nop[4] = { 0x60, 0x00, 0x00, 0x00 };
		//DmSetMemory( (void*)0x8274550C, 4, Nop, NULL );

		memcpy( (void*)0x8274550C, Nop, 0x04 );
	
		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;

	}

	if( g_mb_result.rgwPasscode[0] == RIGHT_BUMPER && g_mb_result.rgwPasscode[1] == RIGHT_BUMPER && g_mb_result.rgwPasscode[2] == RIGHT_BUMPER && g_mb_result.rgwPasscode[3] == RIGHT_BUMPER )
	{

		Sleep(500);
		
	
			XShowMessageBoxUI(0, L"Cheat Enabled", L"Inf Plasma(Shock)Energy\r\nDescription: !WARNING! - CANNOT BE DISABLED\r\nWhen enabled you should have an inf amount of the shock/plasma energy from your hands.\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			
			BYTE Nop[4] = { 0x60, 0x00, 0x00, 0x00 };
			//DmSetMemory( (void*)0x826B06AC, 4, Nop, NULL );
			memcpy( (void*)0x826B06AC, Nop, 4 );

			memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
			Prompt = 0x9999;
		

	}

	if( g_mb_result.rgwPasscode[0] == X_BUTTON && g_mb_result.rgwPasscode[1] == X_BUTTON && g_mb_result.rgwPasscode[2] == X_BUTTON && g_mb_result.rgwPasscode[3] == X_BUTTON )
	{
		Sleep(500);
		
		XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"Inf Ammo\r\n!WARNING! - CANNOT BE DISABLED\r\nWhen enabled you should have a inf amount of ammo in any gun you're carrying only issue is it's slightly buggy and makes your clip go negative or etc sometimes.", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
		BYTE Nop[4] = { 0x60, 0x00, 0x00, 0x00 };
		//DmSetMemory( (void*)0x828F1EC8, 4, Nop, NULL );
		//DmSetMemory( (void*)0x828F1ECC, 4, Nop, NULL );
		
		memcpy( (void*)0x828F1EC8, Nop, 4);
		memcpy( (void*)0x828F1ECC, Nop, 4);

		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;
	}


}