#pragma once
#include <xtl.h>
#include <xbdm.h>
#include <stdio.h>
#include <xui.h>
#include "buttons.h"
#include "util.h"
#include "Logging.h"
#include "KernelExports.h"

LPCWSTR g_strButtons[1] = { L"Continue" };
MESSAGEBOX_RESULT g_mb_result;
XOVERLAPPED g_xol;

bool PrintData = false;
bool PatchData = false;
bool PrintCur = false;
bool PrintSave = false;

unsigned int tx = 0;

std::vector<DWORD> AllTextures;
std::vector<DWORD> TexturesToLog;

Logging Tlog;

DWORD Prompt = 0x9999;


void __declspec(naked) HvxGetVersions( int magic, int mode, unsigned addr, __int64 outBuff, DWORD length )
{
	__asm
	{
			li    r0, 0
			sc
			blr
	}
}

BYTE* key=(BYTE*)XPhysicalAlloc(0x10,MAXULONG_PTR,NULL,PAGE_READWRITE);
