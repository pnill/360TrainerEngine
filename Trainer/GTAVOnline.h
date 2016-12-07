#include "common.h"

VOID __declspec(naked) UpdateBankCash(VOID)
{
	DWORD r3val;
	__asm{
		mr r3val,r3		
	}

	if( r3val == 0x00 )
	{
		 __asm
		 {
			//Duplicate the BEQ/CMP
			/* 0x82D86A7C*/
			/* New TU 82D877E4 */
			lis r3,0x82D8
			ori r3,r3,0x77E4
			mtctr r3
			bctr r3
		 }
	}

	DWORD bankOffset;
	__asm
	{
		mr bankOffset,r3
	}

	*(DWORD*)(bankOffset+0x0C) = 0x3B9AC9FF;


	__asm
	{
		lwz r11, 0(r3)
		lwz r10, 0x34(r11)
	}

	__asm
	{
		/*Return to normal branch in game*/
		/* 82D86A70 */
		/* 82D877D8 */
		lis r30,0x82D8
		ori r30,r30,0x6A70
		mtctr r30
		bctr r30
	}
}

VOID PatchGame()
{

	DbgPrint("Patched the Game\r\n");
	/*
		0x8299C36C // Health
		0xCB52811D // Money
		0x827D6EFC // Wanted Star 1
		0x827D6ED0 // Wanted Star 2
		0x827D6E94 // Wanted Star 3
		0x827D6E54 // Wanted Star 4
		0x827D6E14 // Wanted Star 5
		//STAT_BANK
		.text:82D86A70                 mtctr     r10
		.text:82D86A74                 bctrl
		
		//New TU
		0x8299D1FC // Health
		0x824C517C // Ammo
		0x822F1C14 // Ability
		0x827D6EBC // Wanted Star 1
		0x827D6E90 // Wanted Star 2
		0x827D6E54 // Wanted Star 3
		0x827D6E14 // Wanted Star 4
		0x827D6DD4 // Wanted Star 5


		Money // STAT_BANK
		.text:82D877D8                 mtctr     r10
		.text:82D877DC                 bctrl

		.text:822F441C                 lhz       r10, 0x50(r27)
		.text:822F4420                 mr        r17, r11
		.text:822F4424                 lhz       r9, 0x52(r27)

	*/

/*	*(DWORD*)0x8299D1FC = 0x60000000; // Health
	*(DWORD*)0x827D6EBC = 0x60000000; // Wanted 1
	*(DWORD*)0x827D6E90 = 0x60000000; // Wanted 2
	*(DWORD*)0x827D6E54 = 0x60000000; // Wanted 3
	*(DWORD*)0x827D6E14 = 0x60000000; // Wanted 4
	*(DWORD*)0x827D6DD4 = 0x60000000; // Wanted 5
*/

	//*(DWORD*)0x8368BBAC = 0x60000000;
	//patchInJump( (PDWORD)0x82D877C8, (DWORD)UpdateBankCash, FALSE );
		
		//HooksSet = true;

		//memcpy( (PDWORD)0x827004CC, &Stuff[0], 4 );
		//memcpy( (PDWORD)0x827004D0, &Stuff[1], 4 );
		//memcpy( (PDWORD)0x827004D4, &Stuff[2], 4 );
		//memcpy( (PDWORD)0x827004D8, &Stuff[3], 4 );

}

