#include "common.h"

UINT32 g_titleID = 0x4D530805;

bool OneHitKo = false;
bool HooksSet = false;
BOOL InfHealth = FALSE;

VOID __declspec(naked) CheckDamage(VOID)
{

		/*
			82B480D4 C1BD005C             lfs fr13,5Ch(r29) 
			82B480D8 ED80F828             fsubs fr12,fr0,fr31 
			82B480DC D19D003C             stfs fr12,3Ch(r29) 
			82B480E0 D1BD0060             stfs fr13,60h(r29) 

			r0 = FFFFFFFFFFFFFFA0  r1 = 00000000704FD5C0  r2 = 0000000020000000  r3 = 000000004548E7D0  r4 = 00000000457374B8  r5 = 00000000704FD770  r6 = 0000000000000000  r7 = 0000000000000000  r8 = 00000001260E93CD  r9 = 000000000000001D r10 = 000000000000001E r11 = 0000000000000000 r12 = 0000000082B47F68 r13 = 0000000080202000 r14 = 0000000000000000 r15 = 0000000000000000 r16 = 0000000000000000 r17 = 00000000454D7B20 r18 = 0000000000000000 r19 = 0000000045493200 r20 = 000000000000000B r21 = 0000000000000000 r22 = 0000000000000000 r23 = 0000000040FA6560 
		Player -	r24 = 0000000040F61810 r25 = 0000000000000000 r26 = FFFFFFFF82108CE0 r27 = 0000000000000000 r28 = 0000000000000000 r29 = 000000004548E7D0 r30 = 00000000704FD770 r31 = 0000000000000000 cr = 48000424 xer = 20000000 
			r0 = FFFFFFFFFFFFFFA0  r1 = 00000000704FD5C0  r2 = 0000000020000000  r3 = 000000004548E7D0  r4 = 00000000457374B8  r5 = 00000000704FD770  r6 = 0000000000000000  r7 = 0000000000000000  r8 = 00000001260E93CD  r9 = 000000000000001D r10 = 000000000000001E r11 = 0000000000000000 r12 = 0000000082B47F68 r13 = 0000000080202000 r14 = 0000000000000000 r15 = 0000000000000000 r16 = 0000000000000000 r17 = 00000000454D7B20 r18 = 0000000000000000 r19 = 0000000045493200 r20 = 000000000000000B r21 = 0000000000000000 r22 = 0000000000000000 r23 = 0000000040FA6560 
		Player -	r24 = 0000000040F61810 r25 = 0000000000000000 r26 = FFFFFFFF82108CE0 r27 = 0000000000000000 r28 = 0000000000000000 r29 = 000000004548E7D0 r30 = 00000000704FD770 r31 = 0000000000000000 cr = 48000424 xer = 20000000 
			r0 = FFFFFFFFFFFFFFA0  r1 = 00000000704FD5C0  r2 = 0000000020000000  r3 = 0000000044F93CF0  r4 = 00000000457374B8  r5 = 00000000704FD770  r6 = 0000000000000000  r7 = 0000000000000000  r8 = 0000000082C96210  r9 = 0000000045BC89D0 r10 = 0000000045BC8740 r11 = 0000000000000000 r12 = 0000000082B47F68 r13 = 0000000080202000 r14 = 0000000000000000 r15 = 0000000000000000 r16 = 0000000000000000 r17 = 00000000454D7B20 r18 = 0000000000000000 r19 = 0000000045493200 r20 = 0000000000000002 r21 = 0000000000000000 r22 = 0000000000000000 r23 = 0000000040FA6560 
	NON-Player -	r24 = 0000000000000000 r25 = 0000000000000000 r26 = FFFFFFFF82108CE0 r27 = 0000000000000000 r28 = 0000000000000000 r29 = 0000000044F93CF0 r30 = 00000000704FD770 r31 = 0000000000000000 cr = 48000424 xer = 20000000 
			r0 = FFFFFFFFFFFFFFA0  r1 = 00000000704FD5B0  r2 = 0000000020000000  r3 = 00000000454AB020  r4 = 00000000457372C8  r5 = 00000000704FD760  r6 = 0000000000000000  r7 = 0000000000000000  r8 = 0000000082C96210  r9 = 0000000045C530F0 r10 = 0000000045C52E60 r11 = 0000000000000000 r12 = 0000000082B47F68 r13 = 0000000080202000 r14 = 0000000000000000 r15 = 0000000000000000 r16 = 0000000000000000 r17 = 00000000454A6FE0 r18 = 0000000000000000 r19 = 000000004548CBB0 r20 = 0000000000000004 r21 = 0000000000000000 r22 = 0000000000000000 r23 = 0000000040FA6560 
	NON-Player -	r24 = 0000000000000000 r25 = 0000000000000000 r26 = FFFFFFFF82108CE0 r27 = 0000000000000000 r28 = 0000000000000000 r29 = 00000000454AB020 r30 = 00000000704FD760 r31 = 0000000000000000 cr = 48000424 xer = 20000000 
		*/

	DWORD Ptr;

	__asm
	{
		mr Ptr,r24
	}

	if( InfHealth == TRUE && Ptr != 0x00)
	{
		//printf("Player Shouldn't have been damaged here\r\n");
		float pValue = 900.0f;
		__asm{
			lfs fr13,0x5C(r29)
			fsubs fr12,fr0,fr31
			stfs pValue,0x3C(r29)
			stfs pValue,0x60(r29)
		}

	}else{
		if( Ptr != 0x00 )
		{
			//printf("Player Should be damaged here Ptr != 0x00\r\n");
			__asm{
				lfs fr13,0x5C(r29)
				fsubs fr12,fr0,fr31
				stfs fr12,0x3C(r29)
				stfs fr13,0x60(r29)
			}
		}

	}
	
	if( OneHitKo == TRUE && Ptr == 0x00 )
	{
		float nValue = 0.0f;
		__asm{
			lfs fr13,0x5C(r29)
			fsubs fr12,fr0,fr31
			stfs nValue,0x3C(r29)
			stfs nValue,0x60(r29)
		}
	}else{
		
		if( Ptr == 0x00 )
		{	
			__asm{
				lfs fr13,0x5C(r29)
				fsubs fr12,fr0,fr31
				stfs fr12,0x3C(r29)
				stfs fr13,0x60(r29)
			 }
		}

	}

	__asm{
			/* return back to the game */
			/* 82B480E0 */
			lis r11,0x82B4
			ori r11,r11,0x80E4
			mtctr r11
			bctr
	}
}

VOID ShowTrainerMain()
{
	Prompt = XShowMessageBoxUI(0, L"TheDefaced.org - DeadlyData", L"Body Count Trainer V0.1\r\nView Codes: DPAD-DOWN(x4)", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_PASSCODEMODE, &g_mb_result, &g_xol);
}

VOID SetHooks()
{

	/*
		82B480D4 C1BD005C             lfs fr13,5Ch(r29) 
		82B480D8 ED80F828             fsubs fr12,fr0,fr31 
		82B480DC D19D003C             stfs fr12,3Ch(r29) 
		82B480E0 D1BD0060             stfs fr13,60h(r29) 
	*/

		DWORD Stuff[4];
		patchInJump( (PDWORD)Stuff, (DWORD)CheckDamage, FALSE );
		//DmSetMemory( (PDWORD)0x82B480D4, 4, &Stuff[0], NULL );
		//DmSetMemory( (PDWORD)0x82B480D8, 4, &Stuff[1], NULL );
		//DmSetMemory( (PDWORD)0x82B480DC, 4, &Stuff[2], NULL );
		//DmSetMemory( (PDWORD)0x82B480E0, 4, &Stuff[3], NULL );
		//HooksSet = true;

		memcpy( (PDWORD)0x82B480D4, &Stuff[0], 4 );
		memcpy( (PDWORD)0x82B480D8, &Stuff[1], 4 );
		memcpy( (PDWORD)0x82B480DC, &Stuff[2], 4 );
		memcpy( (PDWORD)0x82B480E0, &Stuff[3], 4 );
		HooksSet = true;
}

VOID ProcessButtonPress( MESSAGEBOX_RESULT g_mb_result )
{

	if( g_mb_result.rgwPasscode[0] == DPAD_DOWN && g_mb_result.rgwPasscode[1] == DPAD_DOWN && g_mb_result.rgwPasscode[2] == DPAD_DOWN && g_mb_result.rgwPasscode[3] == DPAD_DOWN )
	{
		Sleep(500);
		XShowMessageBoxUI(0, L"Viewing All Codes", L"Inf Health DPAD-UP(x4)\r\nOne Hit Kills Y(x4)\r\nInf Ammo X(x4)\r\nInf Grenades RB(x4)", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);

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
			XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"One Hit Kills - Enabled\r\nDescription: Any monster/enemy you hit will die within the first hit.\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
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
		
		XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"Inf Ammo\r\n!WARNING! - CANNOT BE DISABLED\r\nWhen enabled you should have unlimited ammo in all weapons, Note this doesn't include the clip just the current ammo in the weapon.", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
		BYTE Nop[4] = { 0x60, 0x00, 0x00, 0x00 };
		//DmSetMemory( (void*)0x82D04104, 4, Nop, NULL );
	
		
		memcpy( (void*)0x82D04104, Nop, 4);
	

		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;
	}

	if( g_mb_result.rgwPasscode[0] == RIGHT_BUMPER && g_mb_result.rgwPasscode[1] == RIGHT_BUMPER && g_mb_result.rgwPasscode[2] == RIGHT_BUMPER && g_mb_result.rgwPasscode[3] == RIGHT_BUMPER )
	{
		Sleep(500);

		XShowMessageBoxUI(0, L"Cheat Enabled Successfully", L"Inf Grenades\r\n!WARNING! - CANNOT BE DISABLED(Seemed to cause issues with picking up c4)\r\nWhen enabled you should have unlimited grenades.", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
		BYTE Nop[4] = { 0x60, 0x00, 0x00, 0x00 };
		//DmSetMemory( (void*)0x82D04104, 4, Nop, NULL );

		memcpy( (void*)0x82AF66D0, Nop, 4);


		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;
	}
	


}