// Dynamic Module System Core Interface Library
// David Rittenhouse 2003-2004
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
/*
#include "CRC32.h"

DWORD CDynamicCrc32::GetCrc32( PBYTE pbBuffer, unsigned int uiSize )
{
	DWORD dwCrc32 = NULL;
	if( pbBuffer == NULL || uiSize == NULL )
	{ 
		return( dwCrc32 ); 
	}
	Calculate( pbBuffer, uiSize, dwCrc32 );
	return( dwCrc32 );
}

DWORD CDynamicCrc32::GetCrc32( const char * szBuffer )
{
	DWORD dwCrc32 = NULL;
	if( szBuffer == NULL ) 
	{ 
		return( dwCrc32 );
	}
	unsigned int uiSize = strlen( szBuffer );
	Calculate( (PBYTE)szBuffer, uiSize, dwCrc32 );
	return( dwCrc32 );
}

DWORD CDynamicCrc32::GetCheckSum32( PBYTE pbBuffer, unsigned int uiSize )
{
	DWORD dwCheckSum32 = NULL;
	if( pbBuffer == NULL || uiSize == NULL )
	{ 
		return( dwCheckSum32 );
	}
	for( DWORD dw = 0; dw < uiSize ; dw++ ) 
	{ 
		dwCheckSum32 += *(DWORD*)( pbBuffer + dw ); 
	}
	return( dwCheckSum32 );
}

void CDynamicCrc32::Calculate( PBYTE pbBuffer, unsigned int uiSize, DWORD& dwCrc32 )
{
	if( dwTable != NULL )
	{
		if( pbBuffer == NULL || uiSize == NULL )
		{ 
			dwCrc32 = NULL;
		}
		PBYTE pbByte = pbBuffer;
		while( uiSize > 0 )
		{
			dwCrc32 = ( (dwCrc32 >> 8) ^ dwTable[(dwCrc32 & 0xFF) ^ *pbByte++] );
			uiSize--;
		}
	}
	else { dwCrc32 = NULL; }
}

DWORD CDynamicCrc32::Reflect( DWORD dw, BYTE by )
{
	DWORD dwValue = NULL;
	for( int i = 1 ; i < (by + 1); i++ )
	{
		if( dw & 1 )
		{ 
			dwValue |= 1 << (by - i);
		}
		dw >>= 1;
	}
	return( dwValue );
}

CDynamicCrc32::CDynamicCrc32( void )
{
	dwTable = new DWORD[257];
	for( int i = 0 ; i <= 256 ; i++ )
	{
		dwTable[i] = (Reflect( i, 8 ) << 24);
		for( int p = 0 ; p < 8 ; p++ )
		{
			dwTable[i] = (dwTable[i] << 1) ^ ((dwTable[i] & (1 << 31)) ? 0x04C11DB7 : 0);
		}
		dwTable[i] = Reflect( dwTable[i], 32 );
	}
}

CDynamicCrc32::~CDynamicCrc32( void )
{
	if( dwTable != NULL )
	{
		delete[] dwTable;
		dwTable = NULL;
	}
}*/