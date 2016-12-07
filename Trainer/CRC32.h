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
#ifndef CRC32_H
#define CRC32_H

#pragma warning( disable: 4786 )
#include <windows.h>

class CDynamicCrc32
{
public:
	CDynamicCrc32		( void );
	~CDynamicCrc32		( void );
	DWORD GetCrc32		( PBYTE pbBuffer, unsigned int uiSize );
	DWORD GetCrc32		( const char * szBuffer );
	DWORD GetCheckSum32	( PBYTE pbBuffer, unsigned int uiSize );

private:
	void Calculate		( PBYTE pbBuffer, unsigned int uiSize, DWORD& dwCrc );
	DWORD Reflect		(  DWORD dw, BYTE by );
	DWORD * dwTable;
};

#endif // CRC32_H*/