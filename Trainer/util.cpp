#include "util.h"
#include <ppcintrinsics.h>
#include <string>
//#include <xfilecache.h>


UINT32 __declspec(naked) HvxSetState(UINT32 mode){ //2 = protection off, 3 = protection on
	__asm {
		li      r0, 0x7B
		sc
		blr
	}
}

VOID __declspec(naked) GLPR_FUN(VOID)
{
	__asm{
		std     r14, -0x98(sp)
		std     r15, -0x90(sp)
		std     r16, -0x88(sp)
		std     r17, -0x80(sp)
		std     r18, -0x78(sp)
		std     r19, -0x70(sp)
		std     r20, -0x68(sp)
		std     r21, -0x60(sp)
		std     r22, -0x58(sp)
		std     r23, -0x50(sp)
		std     r24, -0x48(sp)
		std     r25, -0x40(sp)
		std     r26, -0x38(sp)
		std     r27, -0x30(sp)
		std     r28, -0x28(sp)
		std     r29, -0x20(sp)
		std     r30, -0x18(sp)
		std     r31, -0x10(sp)
		stw     r12, -0x8(sp)
		blr
	}
}

DWORD interpretBranchDest(DWORD currAddr, DWORD brInst)
{
	DWORD ret;
	int destOff = brInst&0x3FFFFFC;
	int currOff = currAddr&~0x80000000; // make it a positive int
	if(brInst&0x2000000) // backward branch
		destOff = destOff|0xFC000000; // sign extend
	ret = (DWORD)(currOff+destOff);
	return (ret|(currAddr&0x80000000)); // put back the bit if it was used
}

DWORD findInterpretBranch(PDWORD startAddr, DWORD maxSearch)
{
	DWORD i;
	DWORD ret = 0;
	for(i = 0; i < maxSearch; i++)
	{
		if((startAddr[i]&0xFC000000) == 0x48000000)
		{
			ret = interpretBranchDest((DWORD)&startAddr[i], startAddr[i]);
			i = maxSearch;
		}
	}
	return ret;
}

/*DWORD relinkGPLR(int offset, PDWORD saveStubAddr, PDWORD orgAddr)
{
	DWORD saver[0x30];

	memcpy(saver,GLPR_FUN,0x30 * 4);
	//DmSetMemory(saver, 0x30 * 4, GLPR_FUN, 0);


	DWORD inst = 0, repl=0;
	int i;
	// if the msb is set in the instruction, set the rest of the bits to make the int negative
	if(offset&0x2000000)
		offset = offset|0xFC000000;
	//DbgPrint("frame save offset: %08x\n", offset);
	//repl = orgTemp[offset/4];

	memcpy(&repl,&orgAddr[((DWORD)offset)/4],4);
	//DmSetMemory(&repl, 4, &orgAddr[((DWORD)offset)/4], 0);
	
	//DbgPrint("replacing %08x\n", repl);
	for(i = 0; i < 20; i++)
	{
		if(repl == saver[i])
		{
			int newOffset = (int)((PDWORD)(GLPR_FUN)+(DWORD)i)-(int)saveStubAddr;
			inst = 0x48000001|(newOffset&0x3FFFFFC);
			//DbgPrint("saver addr: %08x savestubaddr: %08x\n", &saver[i], saveStubAddr);
		}
	}
	//DbgPrint("new instruction: %08x\n", inst);
	return inst;
}*/

DWORD relinkGPLR(int offset, PDWORD saveStubAddr, PDWORD orgAddr)
{
	DWORD inst = 0, repl;
	int i;
	PDWORD saver = (PDWORD)GLPR_FUN;

	if((saver[0] & 0x3D600000) == 0x3D600000)
	{
		//DebugPrint("Fixing Saver....");
		short* shit = (short*)saver;

		DWORD target = saver[0] << 16;
		//DebugPrint("Target Stage 1: 0x%X", target);
		target += shit[3];
		saver = (PDWORD)target;
	}

	//DebugPrint("Saver at: 0x%X", saver);

	// if the msb is set in the instruction, set the rest of the bits to make the int negative
	if(offset&0x2000000)
		offset = offset|0xFC000000;
	//DebugPrint("frame save offset: %08x\n", offset);
	repl = orgAddr[offset/4];
	//DebugPrint("replacing %08x\n", repl);
	for(i = 0; i < 20; i++)
	{
		if(repl == saver[i])
		{
			int newOffset = (int)&saver[i]-(int)saveStubAddr;
			inst = 0x48000001|(newOffset&0x3FFFFFC);
			//DebugPrint("saver addr: %08x savestubaddr: %08x\n", &saver[i], saveStubAddr);
		}
	}
	//DebugPrint("new instruction: %08x\n", inst);
	return inst;
}





VOID hookFunctionStart(PDWORD addr, PDWORD saveStub, PDWORD oldData, DWORD dest)
{
	DWORD temp[0x10];
	DWORD addrtemp[0x10];

	memcpy(addrtemp,addr,0x10 * 4);
	//DmSetMemory( addrtemp, 0x10 * 4, addr, 0);

	if((saveStub != NULL)&&(addr != NULL))
	{
		int i;
		DWORD addrReloc = (DWORD)(&addr[4]);// replacing 4 instructions with a jump, this is the stub return address
		//DbgPrint("hooking addr: %08x savestub: %08x dest: %08x addreloc: %08x\n", addr, saveStub, dest, addrReloc);
		// build the stub
		// make a jump to go to the original function start+4 instructions
		if(addrReloc & 0x8000){ // If bit 16 is 1
			//setmemdm( &saveStub[0], (0x3D600000 + (((addrReloc >> 16) & 0xFFFF) + 1)));
			temp[0] = 0x3D600000 + (((addrReloc >> 16) & 0xFFFF) + 1); // lis %r11, dest>>16 + 1printf("  - one\r\n");
		}else{
			//setmemdm( &saveStub[0], (0x3D600000 + (((addrReloc >> 16) & 0xFFFF) + 1)));
			temp[0] = 0x3D600000 + ((addrReloc >> 16) & 0xFFFF); // lis %r11, dest>>16
		}


		temp[1] = 0x396B0000 + (addrReloc & 0xFFFF); // addi %r11, %r11, dest&0xFFFF
		temp[2] = 0x7D6903A6; // mtctr %r11
		// instructions [3] through [6] are replaced with the original instructions from the function hook
		// copy original instructions over, relink stack frame saves to local ones
		if(oldData != NULL)
		{
			for(i = 0; i<4; i++)
				oldData[i] = addrtemp[i];
		}
		for(i = 0; i<4; i++)
		{
			if((addrtemp[i]&0x48000003) == 0x48000001) // branch with link
			{
				//DbgPrint("relink %08x\n", addr[i]);
				temp[i+3] = relinkGPLR((addrtemp[i]&~0x48000003), &saveStub[i+3], &addr[i]);
			}
			else
			{
				//DbgPrint("copy %08x\n", addr[i]);
				temp[i+3] = addrtemp[i];
			}
		}

		temp[7] = 0x4E800420; // bctr
		//doSync(temp);
		//DbgPrint("savestub:\n");
		//for(i = 0; i < 8; i++)
		//{
		//	DbgPrint("PatchDword(0x%08x, 0x%08x);\n", &saveStub[i], saveStub[i]);
		//}
		// patch the actual function to jump to our replaced one
		//DmSetMemory( saveStub, 8 * 4, temp, 0);
		memcpy(saveStub,temp,8*4);

		patchInJump(addr, dest, FALSE);
	}
}

VOID unhookFunctionStart(PDWORD addr, PDWORD oldData)
{
	if((addr != NULL)&&(oldData != NULL))
	{
		int i;
		for(i = 0; i < 4; i++)
		{
			addr[i] = oldData[i];
		}
		doSync(addr);
	}
}





VOID patchInJump(DWORD* addr, DWORD dest, BOOL linked){
	DWORD temp[4];

	if(dest & 0x8000) // If bit 16 is 1
		temp[0] = 0x3D600000 + (((dest >> 16) & 0xFFFF) + 1); // lis 	%r11, dest>>16 + 1
	else
		temp[0] = 0x3D600000 + ((dest >> 16) & 0xFFFF); // lis 	%r11, dest>>16

	temp[1] = 0x396B0000 + (dest & 0xFFFF); // addi	%r11, %r11, dest&0xFFFF
	temp[2] = 0x7D6903A6; // mtctr	%r11

	if(linked)
		temp[3] = 0x4E800421; // bctrl
	else
		temp[3] = 0x4E800420; // bctr
	
	//DmSetMemory(addr, 0x10,temp, 0);
	memcpy(addr,temp,0x10);
}



DWORD makeBranch(DWORD branchAddr, DWORD destination, BOOL linked) {
	return (0x48000000)|((destination-branchAddr)&0x03FFFFFF)|(DWORD)linked;
}


void swap_endian(BYTE* src, DWORD size)
{

	BYTE* temp = new BYTE[size];
	for(int i=size-4, b=0; i>=0; i-=4, b+=4){
		*(DWORD*)&temp[b]=(DWORD)src[i];
	}
	memcpy(src, temp, size);
	delete[size] temp;
}




