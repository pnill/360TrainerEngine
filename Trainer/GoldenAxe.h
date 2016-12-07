#include "common.h"

UINT32 g_titleID = 0x4D530805;

bool OneHitKo = false;
bool HooksSet = false;
bool InfBeast = false;
BOOL InfHealth = FALSE;

VOID __declspec(naked) CheckDamage(VOID)
{

		/*
			82102AC0 81780000             lwz r11,0(r24) 
			82102AC4 917C0000             stw r11,0(r28) 
			82102AC8 81580004             lwz r10,4(r24) 
			82102ACC 915C0004             stw r10,4(r28) 
		*/

	DWORD Ptr;
	DWORD Ptr2;
	//DWORD Ptr3;

	__asm
	{
		mr Ptr,r28
		mr Ptr2,r24
	}

	if( *(DWORD*)(Ptr+0x30) == 0xbed52430  && *(DWORD*)(Ptr+0x4) == 0x03  )
	{
		DbgPrint("r28: %08X\r\n", Ptr);
		DbgPrint("r24: %08X\r\n", Ptr2);
		DbgPrint("Ptr+0x14: %08X\r\n", *(DWORD*)(Ptr+0x14) );
	}

	if( *(DWORD*)(Ptr+0x30) == 0xbed52430  && *(DWORD*)(Ptr+0x4) == 0x03 && *(DWORD*)(Ptr+0x14) == 0x44228000 && InfBeast == true )
	{

		float nValue = 1000.0f;
		__asm{
				lwz r11,0(r24)
				stw nValue,0(r28)
				lwz r10,4(r24)
				stw r10,4(r28)
		}
		DbgPrint("Set Beast health 1000\r\n");
	}


	if( *(DWORD*)(Ptr+0x30) == 0xbed52430 && *(DWORD*)(Ptr+0x4) == 0x03 && *(DWORD*)(Ptr+0x14) == 0x447a0000 && InfHealth == TRUE )
	{
		//DbgPrint("PTR: %08X\r\nPTR2: %08X\r\n",Ptr,Ptr2);
	
	}else{
	
		if( OneHitKo == true && *(DWORD*)(Ptr+0x14) != 0x44228000 && *(DWORD*)(Ptr+0x30) == 0xbed52430 && *(DWORD*)(Ptr+0x4) == 0x03 )
		{
			WORD pValue = 0x00;
			__asm
			{
				lwz r11, 0(r24)
				stw pValue,0(r28)
				lwz r10,4(r24)
				stw r10,4(r28)
			}

		}else{

			__asm{
					lwz r11,0(r24)
					stw r11,0(r28)
					lwz r10,4(r24)
					stw r10,4(r28)
			}

		}
	}
	

	__asm{
			/* return back to the game */
			/* 82102ACC */
			lis r11,0x8210
			ori r11,r11,0x2AD0
			mtctr r11
			bctr
	}
}

VOID ShowTrainerMain()
{
	Prompt = XShowMessageBoxUI(0, L"TheDefaced.org - DeadlyData", L"Golden Axe V0.1\r\nView Codes: DPAD-DOWN(x4)", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_PASSCODEMODE, &g_mb_result, &g_xol);
}

VOID SetHooks()
{
		DWORD Stuff[4];
		patchInJump( (PDWORD)Stuff, (DWORD)CheckDamage, FALSE );
		DmSetMemory( (PDWORD)0x82102AC0, 4, &Stuff[0], NULL );
		DmSetMemory( (PDWORD)0x82102AC4, 4, &Stuff[1], NULL );
		DmSetMemory( (PDWORD)0x82102AC8, 4, &Stuff[2], NULL );
		DmSetMemory( (PDWORD)0x82102ACC, 4, &Stuff[3], NULL );
		//HooksSet = true;

		//memcpy( (PDWORD)0x827004CC, &Stuff[0], 4 );
		//memcpy( (PDWORD)0x827004D0, &Stuff[1], 4 );
		//memcpy( (PDWORD)0x827004D4, &Stuff[2], 4 );
		//memcpy( (PDWORD)0x827004D8, &Stuff[3], 4 );
		HooksSet = true;
}

VOID ProcessButtonPress( MESSAGEBOX_RESULT g_mb_result )
{

	if( g_mb_result.rgwPasscode[0] == DPAD_DOWN && g_mb_result.rgwPasscode[1] == DPAD_DOWN && g_mb_result.rgwPasscode[2] == DPAD_DOWN && g_mb_result.rgwPasscode[3] == DPAD_DOWN )
	{
		Sleep(500);
		XShowMessageBoxUI(0, L"Viewing All Codes", L"Inf Health DPAD-UP(x4)\r\nOne Hit Kills Y(x4)\r\nInf Beast Energy LB(x4)\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);

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

		if( InfBeast == true )
		{
			XShowMessageBoxUI(0, L"Cheat Disabled Successfully", L"Inf Beast Health/Energy - Disabled\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			InfBeast = false;
		}else{
			XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"Inf Beast Health/Energy - Enabled\r\n!WARNING!: Be careful with this one when the enemy has a beast or takes yours it will still have inf health\r\nDescription: Gives ANY beast inf health/energy.", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			InfBeast = true;
		}

	
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