#include "common.h"

UINT32 g_titleID = 0x4D530805;

BOOL InfHealth = false;
BOOL InfFlashLight = false;
BOOL SuperSprint = false;
BOOL SprintRecovery = false;

struct StructWeapon{

	struct{
		char Pad[0x20];

		struct{
			char Pad[0x16C];

			struct{
				char Pad[0x104];

				DWORD SelWeapon;
				DWORD Unk;
				DWORD Weapon1;
				DWORD Weapon2;
				DWORD Weapon3;
				DWORD Clip1;
				DWORD Ammo1;
				DWORD Clip2;
				DWORD Ammo2;
				DWORD Clip3;
				DWORD Ammo3;
				DWORD Ammo3_;
			}*Weapon____;

		}*Weapon___;

	}*Weapon__;

}*Weapon_;

void ShowTrainerMain()
{
	Prompt = XShowMessageBoxUI(0, L"TheDefaced.org - DeadlyData", L"Alan Wake Trainer v0.3\r\nEnter one of the following:\r\nLT-LT-LT-LT(1111) - View All codes\r\nRB-RB-RB-RB (3333) - Inf Health\r\nLB-LB-LB-LB (2222) - Inf FlashLight\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_PASSCODEMODE, &g_mb_result, &g_xol);
}

void ProcessButtonPress( MESSAGEBOX_RESULT g_mb_result )
{
	if( g_mb_result.rgwPasscode[0] == LEFT_TRIGGER && g_mb_result.rgwPasscode[1] == LEFT_TRIGGER && g_mb_result.rgwPasscode[2] == LEFT_TRIGGER && g_mb_result.rgwPasscode[3] == LEFT_TRIGGER )
	{
		Sleep(500);
		XShowMessageBoxUI(0, L"All Trainer Codes...", L"Inf Health - RB(x4)\r\nInf Flashlight - LB(x4)\r\nInf Sprint(No Recovery) Y(x4)\r\nSuper Sprint - X(x4)\r\nInf Ammo(Includes Flashbangs/Flares + NO RELOAD) - DPAD-LEFT(x4)\r\nUnlock All Weapons - DPAD-RIGHT(x4)", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result,  &g_xol );

		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;
	}


	if( g_mb_result.rgwPasscode[0] == DPAD_RIGHT && g_mb_result.rgwPasscode[1] == DPAD_RIGHT && g_mb_result.rgwPasscode[2] == DPAD_RIGHT && g_mb_result.rgwPasscode[3] == DPAD_RIGHT )
	{
		Sleep(500);

		StructWeapon *nWeapon = (StructWeapon*)0x82EEED48;

		nWeapon->Weapon__->Weapon___->Weapon____->Weapon1 = 0x00;
		nWeapon->Weapon__->Weapon___->Weapon____->Weapon2 = 0x01;
		nWeapon->Weapon__->Weapon___->Weapon____->Weapon3 = 0x03;
		nWeapon->Weapon__->Weapon___->Weapon____->Ammo1 = 0xFF;
		nWeapon->Weapon__->Weapon___->Weapon____->Ammo2 = 0xFF;
		nWeapon->Weapon__->Weapon___->Weapon____->Ammo3 = 0xFF;
		nWeapon->Weapon__->Weapon___->Weapon____->Clip1 = 0xFF;
		nWeapon->Weapon__->Weapon___->Weapon____->Clip2 = 0xFF;
		nWeapon->Weapon__->Weapon___->Weapon____->Clip3 = 0xFF;
		nWeapon->Weapon__->Weapon___->Weapon____->SelWeapon = 0x00;
		nWeapon->Weapon__->Weapon___->Weapon____->Ammo3_ = 0xFF;

		XShowMessageBoxUI(0, L"Cheat Activated!", L"Unlock All Weapons Activated!\r\nDescription: This gives you the revolver, highest grade rifle, and flare gun, with 255 ammo each and 255 bullets in the clip of each.\r\nNOTE: With Inf ammo the fact these guns come with ammo won't matter.", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);

		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;
	}

	if( g_mb_result.rgwPasscode[0] == DPAD_LEFT && g_mb_result.rgwPasscode[1] == DPAD_LEFT && g_mb_result.rgwPasscode[2] == DPAD_LEFT && g_mb_result.rgwPasscode[3] == DPAD_LEFT )
	{
		Sleep(500);


		BYTE Nop[4] = { 0x60, 0x00, 0x00, 0x00 };
		//DWORD dwOld;
		//VirtualProtect( (void*)0x82421310, 4, PAGE_EXECUTE_READWRITE, &dwOld );
		//VirtualProtect( (void*)0x821ED57C, 4, PAGE_EXECUTE_READWRITE, &dwOld );
		//VirtualProtect( (void*)0x824212D8, 4, PAGE_EXECUTE_READWRITE, &dwOld );

		//DmSetMemory( (void*)0x82421310, 4, Nop, NULL );
		//DmSetMemory( (void*)0x821ED57C, 4, Nop, NULL );
		//DmSetMemory( (void*)0x824212D8, 4, Nop, NULL );

		memcpy( (void*)0x82421310, Nop, 4 ); // Standard Ammo
		memcpy( (void*)0x821ED57C, Nop, 4 ); // Flares/Flash bangs Nop the decreasement
		memcpy( (void*)0x824212D8, Nop, 4 ); // Kill off reloading ;)

		XShowMessageBoxUI(0, L"Cheat Activated!", L"Infinite Ammo enabled\r\nDescription:\r\nThis works for all guns, it also disables the need for reloading on any weapon, also gives inf flashbangs and flares.\r\n!WARNING!: IN THIS VERSION THERE'S NO DE-ACTIVATE FOR THIS OPTION!", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);

		memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
		memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
		Prompt = 0x9999;
	}

	if( g_mb_result.rgwPasscode[0] == Y_BUTTON && g_mb_result.rgwPasscode[1] == Y_BUTTON && g_mb_result.rgwPasscode[2] == Y_BUTTON && g_mb_result.rgwPasscode[3] == Y_BUTTON )
	{

		Sleep(500);

		if( SprintRecovery == false )
		{
			BYTE RecoveryOn1[4] = { 0x44, 0x7A, 0x00, 0x00 };

			memcpy( (void*)0x82F11B08, RecoveryOn1, 4 );
			memcpy( (void*)0x82F11B0C, RecoveryOn1, 4 );
			memcpy( (void*)0x82F11B10, RecoveryOn1, 4 );

			memcpy( (void*)0x82F11BB0, RecoveryOn1, 4 );
			memcpy( (void*)0x82F11BB4, RecoveryOn1, 4 );
			memcpy( (void*)0x82F11BB8, RecoveryOn1, 4 );

			BYTE DelayOn1[4] = { 0x3D, 0xCC, 0xCC, 0xCD };

			memcpy( (void*)0x82F11C58, DelayOn1, 4 );
			memcpy( (void*)0x82F11C5C, DelayOn1, 4 );
			memcpy( (void*)0x82F11C60, DelayOn1, 4 );

			memcpy( (void*)0x82F11D00, DelayOn1, 4 );
			memcpy( (void*)0x82F11D08, DelayOn1, 4 );

			XShowMessageBoxUI(0, L"Cheat Activated!", L"No Sprint Recovery Enabled!\r\nDisables recovery time for sprinting so you can keep on going and going,\r\nThe Energizer Bunny comes to mind :D.", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);


			memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
			Prompt = 0x9999;

			SprintRecovery = true;
		}else{

			BYTE RecoveryOff1[4] = { 0x41, 0x00, 0x00, 0x00 };
			BYTE RecoveryOff2[4] = { 0x3D, 0xCC, 0xCC, 0xCD };
			BYTE RecoveryOff3[4] = { 0x41, 0xA0, 0x00, 0x00 };
			BYTE RecoveryOff4[4] = { 0x41, 0x60, 0x00, 0x00 };

			memcpy( (void*)0x82F11B08, RecoveryOff1, 4 );
			memcpy( (void*)0x82F11B0C, RecoveryOff2, 4 );
			memcpy( (void*)0x82F11B10, RecoveryOff3, 4 );

			memcpy( (void*)0x82F11BB0, RecoveryOff4, 4 );
			memcpy( (void*)0x82F11BB4, RecoveryOff2, 4 );
			memcpy( (void*)0x82F11BB8, RecoveryOff3, 4 );

			BYTE DelayOff1[4] = { 0x40, 0xC0, 0x00, 0x00 };
			BYTE DelayOff2[4] = { 0x40, 0x00, 0x00, 0x00 };
			BYTE DelayOff3[4] = { 0x41, 0x20, 0x00, 0x00 };

			memcpy( (void*)0x82F11C58, DelayOff1, 4 );
			memcpy( (void*)0x82F11C5C, RecoveryOff2, 4 );
			memcpy( (void*)0x82F11C60, RecoveryOff3, 4 );

			memcpy( (void*)0x82F11D00, DelayOff2, 4 );
			memcpy( (void*)0x82F11D08, DelayOff3, 4 );

			XShowMessageBoxUI(0, L"Cheat De-Activated!", L"Sprint Recovery has been re-enabled,\r\nYou killed the damn bunny!", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			SprintRecovery =  false;
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

		if( SuperSprint == false )
		{
			//0x82F151A0  40a00000 00000000 40a00000 - Speed
			BYTE SpeedOn[4] = { 0x40 ,0xA0, 0x00, 0x00 };
			memcpy( (void*)0x82F151A0, SpeedOn, 4 );
			memcpy( (void*)0x82F151A8, SpeedOn, 4 );

			BYTE RecoveryOn1[4] = { 0x44, 0x7A, 0x00, 0x00 };
			//Recovery Set Normal
			memcpy( (void*)0x82F11B08, RecoveryOn1, 4 );
			memcpy( (void*)0x82F11B0C, RecoveryOn1, 4 );
			memcpy( (void*)0x82F11B10, RecoveryOn1, 4 );

			//Recovery Set Casual
			memcpy( (void*)0x82F11BB0, RecoveryOn1, 4 );
			memcpy( (void*)0x82F11BB4, RecoveryOn1, 4 );
			memcpy( (void*)0x82F11BB8, RecoveryOn1, 4 );

			//0x82F11B08  447a0000 447a0000 447a0000
			//0x82F11BB0  447a0000 447a0000 447a0000

			BYTE DelayOn1[4] = { 0x3D, 0xCC, 0xCC, 0xCD };

			//0x82F11C58  3dcccccd 3dcccccd 3dcccccd
			memcpy( (void*)0x82F11C58, DelayOn1, 4 );
			memcpy( (void*)0x82F11C5C, DelayOn1, 4 );
			memcpy( (void*)0x82F11C60, DelayOn1, 4 );

			//0x82F11D00  3dcccccd 00000000 3dcccccd
			memcpy( (void*)0x82F11D00, DelayOn1, 4 );
			memcpy( (void*)0x82F11D08, DelayOn1, 4 );

			XShowMessageBoxUI(0, L"Cheat Activated!", L"Super Sprint Enabled!\r\n\r\nDescription:\r\nSpeeds up sprinting speed and disables sprint recovery.", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			SuperSprint = true;

			memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
			Prompt = 0x9999;

		}else{
			//0x82F11B08  41000000 3dcccccd 41a00000
			//0x82F11BB0  41600000 3dcccccd 41a00000
			//0x82F11C58  40c00000 3dcccccd 41a00000
			//0x82F11D00  40000000 00000000 41200000
			//0x82F151A0  3f733333 00000000 40000000 // Speed

			BYTE SpeedOff[4] = { 0x3F, 0x73, 0x33, 0x33 };
			BYTE SpeedOff2[4] = { 0x40, 0x00, 0x00, 0x00 };

			memcpy( (void*)0x82F151A0, SpeedOff, 4 );
			memcpy( (void*)0x82F151A8, SpeedOff2, 4 );

			BYTE RecoveryOff1[4] = { 0x41, 0x00, 0x00, 0x00 };
			BYTE RecoveryOff2[4] = { 0x3D, 0xCC, 0xCC, 0xCD };
			BYTE RecoveryOff3[4] = { 0x41, 0xA0, 0x00, 0x00 };
			BYTE RecoveryOff4[4] = { 0x41, 0x60, 0x00, 0x00 };

			//Recovery Set Normal
			memcpy( (void*)0x82F11B08, RecoveryOff1, 4 );
			memcpy( (void*)0x82F11B0C, RecoveryOff2, 4 );
			memcpy( (void*)0x82F11B10, RecoveryOff3, 4 );

			//Recovery Set Casual
			memcpy( (void*)0x82F11BB0, RecoveryOff4, 4 );
			memcpy( (void*)0x82F11BB4, RecoveryOff2, 4 );
			memcpy( (void*)0x82F11BB8, RecoveryOff3, 4 );

			//0x82F11B08  447a0000 447a0000 447a0000
			//0x82F11BB0  447a0000 447a0000 447a0000

			BYTE DelayOff1[4] = { 0x40, 0xC0, 0x00, 0x00 };
			BYTE DelayOff2[4] = { 0x40, 0x00, 0x00, 0x00 };
			BYTE DelayOff3[4] = { 0x41, 0x20, 0x00, 0x00 };


			//0x82F11C58  40c00000 3dcccccd 41a00000
			memcpy( (void*)0x82F11C58, DelayOff1, 4 );
			memcpy( (void*)0x82F11C5C, RecoveryOff2, 4 );
			memcpy( (void*)0x82F11C60, RecoveryOff3, 4 );

			//0x82F11D00  40000000 00000000 41200000
			memcpy( (void*)0x82F11D00, DelayOff2, 4 );
			memcpy( (void*)0x82F11D08, DelayOff3, 4 );



			XShowMessageBoxUI(0, L"Cheat De-Activated!", L"Super Sprint Disabled!", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			SuperSprint = false;

			memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
			Prompt = 0x9999;
		}

	}

	if( g_mb_result.rgwPasscode[0] == RIGHT_BUMPER && g_mb_result.rgwPasscode[1] == RIGHT_BUMPER && g_mb_result.rgwPasscode[2] == RIGHT_BUMPER && g_mb_result.rgwPasscode[3] == RIGHT_BUMPER )
	{
		Sleep(500);

		if( InfHealth == false )
		{
			BYTE HealthLowOn[4] = { 0x00, 0x00, 0x00, 0x00 };
			memcpy( (void*)0x82F12438, HealthLowOn, 4 );
			memcpy( (void*)0x82F1243C, HealthLowOn, 4 );
			memcpy( (void*)0x82F12440, HealthLowOn, 4 );

			XShowMessageBoxUI(0, L"Cheat Activated!", L"Inf Health enabled!\r\n\r\nDescription:\r\nChanges the lowest state of health possible, as well as increases re-generation time.", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			InfHealth = true;

			memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
			Prompt = 0x9999;
		}
		else
		{
			BYTE HealthOff1[4] = { 0x3E, 0x99, 0x99, 0x9A };
			BYTE HealthOff2[4] = { 0x3C, 0x23, 0xD7, 0x0A };
			BYTE HealthOff3[4] = { 0x3F, 0x80, 0x00, 0x00 };

			memcpy( (void*)0x82F12438, HealthOff1, 4 );
			memcpy( (void*)0x82F1243C, HealthOff2, 4 );
			memcpy( (void*)0x82F12440, HealthOff3, 4 );
			//3e 99 99 9a 3c 23 d7 0a 3f 80 00 00 - Low Health Defaults.

			XShowMessageBoxUI(0, L"Cheat De-activated", L"Inf Health disabled!\r\n", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
			InfHealth = false;

			memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
			Prompt = 0x9999;
		}

	}

	if( g_mb_result.rgwPasscode[0] == LEFT_BUMPER && g_mb_result.rgwPasscode[1] == LEFT_BUMPER && g_mb_result.rgwPasscode[2] == LEFT_BUMPER && g_mb_result.rgwPasscode[3] == LEFT_BUMPER )
	{

		Sleep(500);

		if( InfFlashLight == false )
		{
			BYTE LightPatchOn[4] = { 0x44, 0x7a, 0x00, 0x00 };
			BYTE LightPatchOn2[4] = { 0x45, 0x9c, 0x40, 0x00 };

			memcpy( (void*)0x82F12AE0, LightPatchOn, 4 );
			memcpy( (void*)0x82F12AE4, LightPatchOn, 4 );
			memcpy( (void*)0x82F12AE8, LightPatchOn2, 4 );
			InfFlashLight = true;
			XShowMessageBoxUI(0, L"Cheat Activated!", L"Inf flashlight enabled!\r\n\r\nDescription:\r\nMakes it so the flashlight dis-charges the battery at a slower rate.", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);

			memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
			Prompt = 0x9999;
		}
		else
		{
			//Off patch codes here...

			XShowMessageBoxUI(0, L"Cheat De-Activated", L"Inf flashlight disabled!", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);

			InfFlashLight = false;

			memset( &g_mb_result.rgwPasscode[0], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[1], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[2], 0x00, 0x02 );
			memset( &g_mb_result.rgwPasscode[3], 0x00, 0x02 );
			Prompt = 0x9999;
		}
	}

}