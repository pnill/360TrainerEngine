#include "common.h"
#include <d3d9.h>
#include <sstream>
#include <xgraphics.h>
#include <winsockx.h>
#include <iostream>
#include <fstream>

UINT32 g_titleID = 0x4D530805;
bool RunOnce = true;
using namespace std;

#pragma region crc32
static UINT32 crc32_tab[] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
	0xe963a535, 0x9e6495a3,	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
	0xf3b97148, 0x84be41de,	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,	0x14015c4f, 0x63066cd9,
	0xfa0f3d63, 0x8d080df5,	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,	0x35b5a8fa, 0x42b2986c,
	0xdbbbc9d6, 0xacbcf940,	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
	0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,	0x76dc4190, 0x01db7106,
	0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
	0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
	0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
	0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
	0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
	0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
	0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
	0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
	0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
	0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
	0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
	0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

UINT32 crc32(UINT32 crc, const void *buf, size_t size)
{
	const UINT8 *p;

	p = (UINT8*)buf;
	crc = crc ^ ~0U;

	while (size--)
		crc = crc32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);

	return crc ^ ~0U;
}
#pragma endregion

#pragma region D3D Defines
const BYTE green[60] =
{
	0x42, 0x4D, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x12, 0x0B, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x20, 0xA0, 0x00, 0x00, 0xFF, 0xFF
};

const BYTE blue[60] =
{
	0x42, 0x4D, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x12, 0x0B, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0x00, 0x00, 0x00
};


const BYTE bYellow[60] =
{
	0x42, 0x4D, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
	0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x12, 0x0B, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00
};
const BYTE bAqua[58] =
{
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0x00, 0x00
};
const BYTE bPurple[58] =
{
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0x00, 0xFF, 0x00
};

const BYTE bWhite[58] =
{
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0x00
};

const BYTE bRed[60] =
{
	0x42, 0x4D, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x12, 0x0B, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xFF, 0x00, 0x00, 0x00
};

const BYTE orange[ 60 ] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA5, 0xFF, 0x00, 0x00, 0x00
};


LPDIRECT3DTEXTURE9 g_pTexGreen = NULL;
LPDIRECT3DTEXTURE9 g_pTexBlue = NULL;

#pragma endregion 

#pragma region Game Engine Hooks and Defintions
// Entity Types
#define ET_GENERAL 0
#define ET_PLAYER 1
#define ET_PLAYER_CORPSE 2
#define ET_ITEM 3
#define ET_MISSILE 4
#define ET_INVISIBLE 5
#define ET_SCRIPTMOVER 6
#define ET_SOUND_BLEND 7
#define ET_FX 8
#define ET_LOOP_FX 9
#define ET_PRIMARY_LIGHT 10
#define ET_TURRET 11
#define ET_HELICOPTER 12
#define ET_PLANE 13
#define ET_VEHICLE 14
#define ET_VEHICLE_CORPSE 15
#define ET_ACTOR 16
#define ET_ACTOR_SPAWNER 17
#define ET_ACTOR_CORPSE 18
#define ET_STREAMER_HINT 19
#define ET_ZBARRIER 20


typedef float vec3_t[3];
typedef float vec2_t[2];
typedef vec3_t Vec3;
typedef Vec3 Vector3;


typedef struct  
{
	char padding[0x1D4];
	DWORD isAlive;
	char padding2[0xD0];
	WORD eType;
	char padding3[0xCA];
}CEntity;

typedef struct {
	char Unknown0x0000[ 0x01B0 - 0x0000 ];
	int localClientNum;
} snapshot_t;

typedef struct {
	int isValid;
	char Unknown0x0004[ 0x002C - 0x0004 ];
	int team;
	char Unknown0x0030[ 0x05F0 - 0x0030 ];
	int clientNum;
	char Unknown0x05F4[ 0x0808 - 0x05F4 ];
} clientInfo_t;

typedef struct {
	char Unknown0x0000[ 0x0028 - 0x0000 ];
	snapshot_t* snapshot;
	char Unknown0x002C[ 0x69A90 - 0x002C ];
	clientInfo_t clientInfo[ 1 ];
} cgame_t;

typedef struct { 
	// 0x370 .text:821CFF3C lwz r11, 0x370(r31) 
	// 0x36C .text:8274D9D0 lwz r11, 0x36C(r3)
	// 0x1C0
	// .text:8274D9EC                 lbz       r11, 0x1C0(r3)
	// .text:8274D9F0                 li        r3, 1
	//	.text:8274D9F4                 clrlwi    r10, r11, 31
	//	.text:8274D9F8                 cmplwi    cr6, r10, 0

	char Unknown0x0000[ 0x01D0 - 0x0000 ];
	int clientNum; // 1D0 (4 bytes)
	unsigned char flags; // 1D5 (1 byte)
	char Unknown0x01D5[ 0x02A8 - 0x01D5 ];
	unsigned short state; // 2A8 (2 bytes)
	char Unknown0x01D6[ 0x0374 - 0x02AA ];
} entity_t;

cgame_t** ppGame = (cgame_t**) 0x82BBABE8;
entity_t** ppEntities = (entity_t**) 0x82BBC2D4;

void (__cdecl *R_DrawNameTags)( int Mtx, entity_t* ent, float Alpha, int Mode ) = (void (__cdecl *)( int, entity_t*, float, int ))0x821C28A0;

void DoStuff(void) {
	cgame_t* pGame = *ppGame;
	entity_t* pEntities = *ppEntities;

	if( pGame == nullptr || pEntities == nullptr )
		return;

	snapshot_t* pSnapshot = pGame->snapshot;

	if( pSnapshot ) {
		clientInfo_t* pLocalClientInfo = &pGame->clientInfo[pSnapshot->localClientNum];
		
		for( int i = 0; i < 1024; i++ ) {
		
			entity_t* pEntity = &pEntities[ i ];
			if( pSnapshot->localClientNum == pEntity->clientNum
				|| pEntity->flags & 0x20
				|| pEntity->state != 1 )
				continue;

			clientInfo_t* pClientInfo = &pGame->clientInfo[ pEntity->clientNum ];
			if( !pClientInfo->isValid )
				continue;

			int iTeam = pClientInfo->team;
			bool bEnemy = (iTeam != 0) ? (pLocalClientInfo->team != iTeam) : true;

			if(bEnemy)
				R_DrawNameTags( 0, pEntity, 1.0f, 0 );
		}
	}
}

VOID __declspec(naked) R_AddRefEntityToSceneSaveVar(VOID)
{
	__asm
	{
			li r3,458
			nop 
			nop 
			nop 
			nop 
			nop 
			nop 
			blr 
	} 
}


VOID __declspec(naked) PostRenderHookSaveVar(VOID)
{
	__asm
	{
			li r3,909
			nop 
			nop 
			nop 
			nop 
			nop 
			nop 
			blr 
	} 
}

int PostRenderHook(void* r3);
typedef int (*PostRenderHookFUN)(void* r3);
static DWORD PostRenderHookOld[4];
PostRenderHookFUN PostRenderHookSave = (PostRenderHookFUN)PostRenderHookSaveVar;

int PostRenderHook(void* r3)
{
	DoStuff();

	return PostRenderHookSave(r3);
}

int R_AddRefEntityToSceneHook(int unk, entity_t* cent, int entitNum, int RenderFx, int unk2, int unk3, int unk4, int Neg1, int unk5, int CentOffset, int Zero, int OneZero, int Zero2);
typedef int (*R_AddRefEntityToSceneFUN)(int unk, entity_t* cent, int entitNum, int RenderFx, int unk2, int unk3, int unk4, int Neg1, int unk5, int CentOffset, int Zero, int OneZero, int Zero2);
static DWORD R_AddRefEntityToSceneOld[4];
R_AddRefEntityToSceneFUN R_AddRefEntityToSceneSave = (R_AddRefEntityToSceneFUN)R_AddRefEntityToSceneSaveVar;


/*

	//cg_entities+0x2A6
    if(  cg_entities[entityNum].eType == 1  )//only wallhack on players ps:entityNum checks only for valid Entities
    {      
        RenderFx |= 1/*Fog | 18/*Depth | 19/*Shadow; //Default  |= 0x400000;  
    }  
 
     //TODO:
    
        coloured entities ( chameleons )
    
 
*/

int R_AddRefEntityToSceneHook( int unk, entity_t* cent, int entitNum, int RenderFx, int unk2, int unk3, int unk4, int Neg1, int unk5, int CentOffset, int Zero, int OneZero, int Zero2 )
{
	cgame_t* pGame = *ppGame;


	//Tlog.Add(AllTextures,RenderFx);
	//Tlog.Log(AllTextures);

	if( pGame == nullptr )
		return R_AddRefEntityToSceneSave(unk,cent,entitNum,RenderFx,unk2,unk3,unk4,Neg1,unk5,CentOffset,Zero,OneZero,Zero2);


	snapshot_t* pSnapshot = pGame->snapshot;

	if( pSnapshot ) {
		clientInfo_t* pLocalClientInfo = &pGame->clientInfo[pSnapshot->localClientNum];
			
		if( pSnapshot->localClientNum == cent->clientNum
			|| cent->flags & 0x20
			|| cent->state != 1 )
			return R_AddRefEntityToSceneSave(unk,cent,entitNum,RenderFx,unk2,unk3,unk4,Neg1,unk5,CentOffset,Zero,OneZero,Zero2);

		clientInfo_t* pClientInfo = &pGame->clientInfo[ cent->clientNum ];
		if( !pClientInfo->isValid )
			return R_AddRefEntityToSceneSave(unk,cent,entitNum,RenderFx,unk2,unk3,unk4,Neg1,unk5,CentOffset,Zero,OneZero,Zero2);

		int iTeam = pClientInfo->team;
		bool bEnemy = (iTeam != 0) ? (pLocalClientInfo->team != iTeam) : true;


		if( cent->state == ET_PLAYER && bEnemy )
		{

			////Tlog.Add(AllTextures,RenderFx);
			//Tlog.Log(AllTextures);
			/*
				RenderFx: 01400004 // Idk Sonar/Heat Signature if stand still
				 RenderFx: 00400184 // Thermal
				RenderFx: 00400004 // Default

				RenderFx: 40400001
				RenderFx: 00400004
				RenderFx: 01400004

				//
				FF00FF00
				FF00FFFF
				4000000
				0xFFFFFFFF
				'
				*/
			RenderFx |= -1;
		}

	}
	
	return R_AddRefEntityToSceneSave(unk,cent,entitNum,RenderFx,unk2,unk3,unk4,Neg1,unk5,CentOffset,Zero,OneZero,Zero2);
}
#pragma endregion

#pragma region D3D Hooks
VOID __declspec(naked) D3DQuery_GetDataSaveVar(VOID)
{
	__asm
	{
			li r3,457
			nop 
			nop 
			nop 
			nop 
			nop 
			nop 
			blr 
	} 
}


D3DQuery* D3DQuery_GetDataHook(D3DQuery *pThis, void *Size, unsigned int GetDataFlags, unsigned int unk1);
typedef D3DQuery* (*D3DQuery_GetDataFUN)(D3DQuery *pThis, void *Size, unsigned int GetDataFlags, unsigned int unk1);
static DWORD D3DQuery_GetDataOld[4];
D3DQuery_GetDataFUN CreateQueryTiledSave = (D3DQuery_GetDataFUN)D3DQuery_GetDataSaveVar;

D3DQuery* D3DQuery_GetDataHook(D3DQuery *pThis, void *Size, unsigned int GetDataFlags, unsigned int unk1)
{
	return NULL;
}

VOID __declspec(naked) SetTextureSaveVar(VOID)
{
	__asm{ 
			li r3, 455 //make this unique for each hook 
			nop 
			nop 
			nop 
			nop 
			nop 
			nop 
			blr 
	} 
}

D3DVOID SetTextureHook(IDirect3DDevice9* pDevice, unsigned int Sampler, D3DBaseTexture *pTexture, unsigned __int64 PendingMask3 );
typedef D3DVOID (*SetTextureFUN)(IDirect3DDevice9* pDevice, unsigned int Sampler, D3DBaseTexture *pTexture, unsigned __int64 PendingMask3);
static DWORD SetTextureOld[4];
SetTextureFUN SetTextureSave = (SetTextureFUN)SetTextureSaveVar;



D3DVOID SetTextureHook(IDirect3DDevice9* pDevice, unsigned int Sampler, D3DBaseTexture *pTexture, unsigned __int64 PendingMask3 )
{
	
	if(RunOnce == true)
	{
		RunOnce=false;
		D3DXCreateTextureFromFileInMemory(pDevice,(LPVOID)&green,sizeof(green),&g_pTexGreen);
		D3DXCreateTextureFromFileInMemory(pDevice,(LPVOID)&bRed,sizeof(blue),&g_pTexBlue);
	}

	if(pTexture != NULL)
	{

		UINT dwResourceMemSize, uiResourceMem;//, uiExtraMem, dwExtraMemSize;


		XGGetTextureLayout(pTexture,&uiResourceMem,(UINT*)&dwResourceMemSize,NULL,NULL,0,NULL,NULL,NULL,NULL,0);
		VOID* baseBuffer = (VOID*)uiResourceMem;

		if(baseBuffer != NULL)
		{

			if( Sampler == 3 )
			{
				DWORD m_stage3Crc = crc32(0,baseBuffer,1024);

				if( m_stage3Crc == 0x77DFC838 || m_stage3Crc == 0x34331E25 || m_stage3Crc == 0xE08CC47C || m_stage3Crc == 0x807506C8 ||
				m_stage3Crc == 0x93066E57 || m_stage3Crc == 0x0C302AF0 || m_stage3Crc == 0x108B0200 || m_stage3Crc == 0xFE1939CD ||
				m_stage3Crc == 0xFEA3C921 || m_stage3Crc == 0x7AA57A0A || m_stage3Crc == 0x4615159E || m_stage3Crc == 0xDE642528 || 
				m_stage3Crc == 0x9B4E531B || m_stage3Crc == 0xA3FC03D5 || m_stage3Crc == 0x0821ED60 || m_stage3Crc == 0xCCA345AB ||
				m_stage3Crc == 0x17D91026 || m_stage3Crc == 0x3BC884A2 || m_stage3Crc == 0xCD479B92 || m_stage3Crc == 0xDF08F226 ||
				m_stage3Crc == 0xA3C564A4 || m_stage3Crc == 0x2B85E715 || m_stage3Crc == 0x9FF16C33 || m_stage3Crc == 0xAC01A2EB )
				{
					DbgPrint("This is our texture\r\n");
				}
			}
		//	if( Sampler == 3 ) 
		//	{
		//		Tlog.Add(AllTextures,crc32(0,baseBuffer,1024));
		//	}
		}
	}

	return SetTextureSave(pDevice,Sampler,pTexture,PendingMask3);
}

VOID __declspec(naked) DrawIndexedVerticesSaveVar(VOID) 
{ 
	__asm{ 
			li r3, 454 //make this unique for each hook 
			nop 
			nop 
			nop 
			nop 
			nop 
			nop 
			blr 
	} 
}


D3DVOID DrawIndexedVerticesHook(IDirect3DDevice9* pDevice, int PrimitiveType, int BaseVertexIndex, unsigned int StartIndex, unsigned int IndexCount);
typedef D3DVOID (*DrawIndexedVerticesFUN)(IDirect3DDevice9* pDevice, int PrimitiveType, int BaseVertexIndex, unsigned int StartIndex, unsigned int IndexCount);
static DWORD DrawIndexedOld[4];
DrawIndexedVerticesFUN DrawIndexedVerticesSave = (DrawIndexedVerticesFUN)DrawIndexedVerticesSaveVar;

D3DVOID DrawIndexedVerticesHook(IDirect3DDevice9* pDevice, int PrimitiveType, int BaseVertexIndex, unsigned int StartIndex, unsigned int IndexCount)
{


		DWORD m_stage3Crc;
		IDirect3DBaseTexture9* Stage3Tex;
		pDevice->GetTexture(3,&Stage3Tex);


		UINT dwResourceMemSize, uiResourceMem;//, uiExtraMem, dwExtraMemSize;

	if(Stage3Tex != NULL)
	{
		XGGetTextureLayout(Stage3Tex,&uiResourceMem,(UINT*)&dwResourceMemSize,NULL,NULL,0,NULL,NULL,NULL,NULL,0);
		VOID* baseBuffer = (VOID*)uiResourceMem;
	
		if(baseBuffer !=NULL)
		{
			m_stage3Crc = crc32(0,baseBuffer,1024);
			
		}
	}


	//if( AllTextures.size() != 0 )
	//{
		//if( tx >= 0 && tx < AllTextures.size() )
	    //{
			if( /*m_stage3Crc == AllTextures[tx] ||*/
				m_stage3Crc == 0x5FC1C37E || m_stage3Crc == 0x1CBF934B || m_stage3Crc == 0xE6578628 || m_stage3Crc == 0x95314B59 ||
				m_stage3Crc == 0xE8BD4AE2 || m_stage3Crc == 0xD2DF222E || m_stage3Crc == 0x9C96E0CE || m_stage3Crc == 0x0C302AF0 ||
				m_stage3Crc == 0x93066E57 || m_stage3Crc == 0x4518DFBE || m_stage3Crc == 0xF6309FE1 || m_stage3Crc == 0xD7071237 ||
				m_stage3Crc == 0x77DFC838 || m_stage3Crc == 0x34331E25 || m_stage3Crc == 0xE08CC47C || m_stage3Crc == 0x807506C8 ||
				m_stage3Crc == 0x93066E57 || m_stage3Crc == 0x0C302AF0 || m_stage3Crc == 0x108B0200 || m_stage3Crc == 0xFE1939CD ||
				m_stage3Crc == 0xFEA3C921 || m_stage3Crc == 0x7AA57A0A || m_stage3Crc == 0x4615159E || m_stage3Crc == 0xDE642528 || 
			    m_stage3Crc == 0x9B4E531B || m_stage3Crc == 0xA3FC03D5 || m_stage3Crc == 0x0821ED60 || m_stage3Crc == 0xCCA345AB ||
				m_stage3Crc == 0x17D91026 || m_stage3Crc == 0x3BC884A2 || m_stage3Crc == 0xCD479B92 || m_stage3Crc == 0xDF08F226 ||
				m_stage3Crc == 0xA3C564A4 || m_stage3Crc == 0x2B85E715 || m_stage3Crc == 0x9FF16C33 || m_stage3Crc == 0xAC01A2EB )
			{			
				pDevice->SetTexture( 0, g_pTexGreen);
				pDevice->SetTexture( 1, g_pTexGreen);
				pDevice->SetTexture( 2, g_pTexGreen);
				pDevice->SetTexture( 3, g_pTexGreen);
				pDevice->SetTexture( 4, g_pTexGreen);
				

				pDevice->SetRenderState( D3DRS_ZENABLE, false );
				pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
		
				DrawIndexedVerticesSave(pDevice,PrimitiveType,BaseVertexIndex,StartIndex,IndexCount);

				pDevice->SetTexture( 0, g_pTexBlue);
				pDevice->SetTexture( 1, g_pTexBlue);
				pDevice->SetTexture( 2, g_pTexBlue);
				pDevice->SetTexture( 3, g_pTexBlue);
				pDevice->SetTexture( 4, g_pTexBlue);

				pDevice->SetRenderState( D3DRS_ZENABLE, true );
				pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	

				//if(PrintData == true)
				//{
				//	DbgPrint("AllTextures[tx]: %08X\r\n",AllTextures[tx]);
				//	DbgPrint("m_stage0Crc: %08X\r\n",m_stage3Crc);
				//	PrintData=false;
				//}
			}
		//}
	//}
	
	D3DVOID ret = DrawIndexedVerticesSave(pDevice,PrimitiveType,BaseVertexIndex,StartIndex,IndexCount);
	return ret;
}
#pragma endregion

#pragma region GamerTag, XUID Hooks
VOID __declspec(naked) XUserGetNameSaveVar(VOID)
{
	__asm
	{
			li r3,135
			nop 
			nop 
			nop 
			nop 
			nop 
			nop 
			blr 
	} 
}

DWORD XUserGetNamehk( DWORD dwUserIndex, LPSTR szUserName, DWORD cchUserName );
typedef DWORD (*XUserGetNameFUN)(DWORD dwUserIndex, LPSTR szUserName, DWORD cchUserName);
static DWORD XUserGetNameOld[4];
XUserGetNameFUN XUserGetNameSave = (XUserGetNameFUN)XUserGetNameSaveVar;

DWORD XUserGetNamehk( DWORD dwUserIndex, LPSTR szUserName, DWORD cchUserName )
{
	//^3 Yellow
	//^4 Blue
	//^1 Red
	//^5 Aqua
	//^7 Gold
	//sprintf(szUserName,"^5Hax");
	memset(szUserName,0x00,cchUserName);
	sprintf(szUserName,"FakeName");

	//printf("[XAM] XUserGetName\r\n");
	return ERROR_SUCCESS;
}


VOID __declspec(naked) XUserGetXUIDSaveVar(VOID)
{
	__asm
	{
			li r3,235
			nop 
			nop 
			nop 
			nop 
			nop 
			nop 
			blr 
	} 
}

DWORD XUserGetXUIDhk( DWORD dwUserIndex, DWORD Unk1, PXUID pxuid );
typedef DWORD(*XUserGetXUIDFUN)(DWORD dwUserIndex, DWORD Unk1, PXUID pxuid);
static DWORD XUserGetXUIDOld[4];
XUserGetXUIDFUN XUserGetXUIDSave = (XUserGetXUIDFUN)XUserGetXUIDSaveVar;

DWORD XUserGetXUIDhk( DWORD dwUserIndex, DWORD Unk1, PXUID pxuid )
{

	XUID nXUID;
	DWORD ret = XUserGetXUIDSave( dwUserIndex, Unk1, &nXUID);

	printf("[XAM] XUserGetXUID - Real XUID: 0x%016I64X\r\n",nXUID);


	nXUID = 0x00090000061F8F05;
	//0x 00 09 00 00 0A 3617C3	
	
	memcpy(pxuid,&nXUID, 0x08);

	printf("[XAM] XUserGetXUID - Spoofed XUID: 0x%016I64X\r\n",nXUID);

	return ret;
}

VOID __declspec(naked) XUserGetSigninInfoSaveVar(VOID)
{
	__asm
	{
			li r3,745
			nop 
			nop 
			nop 
			nop 
			nop 
			nop 
			blr 
	} 
}


DWORD XUserGetSignInfohk(  DWORD dwUserIndex, DWORD dwFlags, PXUSER_SIGNIN_INFO pSigninInfo );
typedef DWORD(*XUserGetSigninInfoFUN)( DWORD dwUserIndex, DWORD dwFlags, PXUSER_SIGNIN_INFO pSigninInfo );
static DWORD XUserGetSigninInfoOld[4];
XUserGetSigninInfoFUN XUserGetSigninInfoSave = (XUserGetSigninInfoFUN)XUserGetSigninInfoSaveVar;

DWORD XUserGetSigninInfohk( DWORD dwUserIndex, DWORD dwFlags, PXUSER_SIGNIN_INFO pSigninInfo  )
{


	PXUSER_SIGNIN_INFO npSigninInfo = new _XUSER_SIGNIN_INFO;

	DWORD ret = XUserGetSigninInfoSave( dwUserIndex, dwFlags, npSigninInfo );

	sprintf(npSigninInfo->szUserName,"FakeName");
	npSigninInfo->xuid = 0x00090000061F8F05;

	memcpy( pSigninInfo, npSigninInfo, sizeof(_XUSER_SIGNIN_INFO) );

	delete[] npSigninInfo;

	return ret;
}

#pragma endregion

#pragma region XOnline, NetDLL Hooks
VOID __declspec(naked) XeKeysGetConsoleIdSaveVar(VOID)
{
	__asm
	{
			li r3,385
			nop 
			nop 
			nop 
			nop 
			nop 
			nop 
			blr 
	} 
}

DWORD XeKeysGetConsoleIdhk(  PBYTE databuffer OPTIONAL, char* szBuffer OPTIONAL );
typedef DWORD(*XeKeysGetConsoleIdFUN)( PBYTE databuffer OPTIONAL, char* szBuffer OPTIONAL );
static DWORD XeKeysGetConsoleIdOld[4];
XeKeysGetConsoleIdFUN XeKeysGetConsoleIdSave = (XeKeysGetConsoleIdFUN)XeKeysGetConsoleIdSaveVar;

DWORD XeKeysGetConsoleIdhk( PBYTE databuffer OPTIONAL, char* szBuffer OPTIONAL )
{

	//BYTE nConsoleID[11];
	//XeKeysGetConsoleID( 0, szBuffer);

	//for(int i=0;i<11;i++)
	//{
	//	printf("%02X",nConsoleID[i]);
	//}
	//printf("\r\n");
	//memcpy(databuffer,ConsoleID,0x11);
	//memcpy(szBuffer,ConsoleID,0x11);

	return ERROR_SUCCESS;
}

VOID __declspec(naked) NetDll_recvSaveVar(VOID)
{
	__asm
	{
			li r3,631
			nop 
			nop 
			nop 
			nop 
			nop 
			nop 
			blr 
	} 
}


int NetDll_recvhk( void* unk1, SOCKET s, char FAR *buf, int len, int flags );
typedef int(*NetDll_recvFUN)( void* unk1, SOCKET s, char FAR *buf, int len, int flags );
static DWORD NetDll_recvOld[4];
NetDll_recvFUN NetDll_recvSave = (NetDll_recvFUN)NetDll_recvSaveVar;

int NetDll_recvhk( void* unk1, SOCKET s, char FAR *buf, int len, int flags )
{

	//printf("[WSARecv] Recving Data\r\n");

	DWORD ret = NetDll_recvSave(unk1, s, buf, len, flags );

	/*
	char* recvData = new char[len*2];
	memset(recvData,0x00,len*2);
	
	printf("RecvData: ");
	for(int i = 0; i<len; i++ )
	{
		printf("%02X",buf[i]);
		sprintf(&recvData[i*2],"%02X",buf[i]);
	}
		printf("\r\n");
	
	ofstream *RecvFile = new ofstream;
	RecvFile->open("Game:\\RecvLog.txt",ios::binary | ios::app );
	RecvFile->write( recvData, len*2 );
	RecvFile->write( "\r\n", 2 );
	RecvFile->close();*/

	ofstream *RecvRaw = new ofstream;
	RecvRaw->open("Game:\\RecvRaw.bin", ios::binary | ios::app );
	RecvRaw->write( buf, len );
	RecvRaw->close();
	delete[] RecvRaw;


	//delete[] recvData;
	//printf("[WSARecv] WSAGetLastError(): %i\r\n",WSAGetLastError());


	return ret;
}



VOID __declspec(naked) NetDll_sendSaveVar(VOID)
{
	__asm
	{
			li r3,195
			nop 
			nop 
			nop 
			nop 
			nop 
			nop 
			blr 
	} 
}


int NetDll_sendhk( void* unk1, SOCKET s, const char FAR *buf, int len, int flags );
typedef int(*NetDll_sendFUN)( void* unk1, SOCKET s, const char FAR *buf, int len, int flags );
static DWORD NetDll_sendOld[4];
NetDll_sendFUN NetDll_sendSave = (NetDll_sendFUN)NetDll_sendSaveVar;

int NetDll_sendhk( void* unk1, SOCKET s, const char FAR *buf, int len, int flags )
{

 	printf("[WSASend] Sending Data\r\n");

	/*char* sendData = new char[len*2];
	memset(sendData,0x00,len*2);

	printf("Data: ");
	for(int i = 0; i<len; i++ )
	{
		printf("%02X",buf[i]);
		sprintf(&sendData[i*2],"%02X",buf[i]);
	}
	printf("\r\n");

	ofstream *sendFile = new ofstream;
	sendFile->open("Game:\\sendLog.txt",ios::binary | ios::app );
	sendFile->write( sendData, len*2 );
	sendFile->write( "\r\n", 2 );
	sendFile->close();*/

	ofstream *sendRaw = new ofstream;
	sendRaw->open("Game:\\sendLog.bin",ios::binary | ios::app );
	sendRaw->write( buf, len );
	sendRaw->close();
	delete[] sendRaw;

	//delete[] sendData;

	DWORD ret = NetDll_sendSave(unk1, s, buf, len, flags );

	//printf("[WSASend] WSAGetLastError(): %i\r\n",WSAGetLastError());

	
	return ret;
}


VOID __declspec(naked) NetDll_ConnectSaveVar(VOID)
{
		__asm
		{
			li r3,995
			nop 
			nop 
			nop 
			nop 
			nop 
			nop 
			blr 
	} 
}


int NetDll_Connecthk(  void* unk1, SOCKET s, struct sockaddr *name, int namelen );
typedef int(*NetDll_ConnectFUN)( void* unk1, SOCKET s, struct sockaddr *name, int namelen );
static DWORD NetDll_ConnectOld[4];
NetDll_ConnectFUN NetDll_ConnectSave = (NetDll_ConnectFUN)NetDll_ConnectSaveVar;

int NetDll_Connecthk( void* unk1, SOCKET s, struct sockaddr *name, int namelen )
{
	
	printf("unk1: %i\r\n",unk1);
	printf("s: %08X\r\n",s);
	printf("name: %08X\r\n",name);
	printf("namelen: %08X\r\n",namelen);

	sockaddr_in* client = (sockaddr_in*)name;

	int ret = NetDll_ConnectSave(unk1, s, name, namelen ); 
	int err = WSAGetLastError();

	if( err == WSAEISCONN || err == WSAEWOULDBLOCK)
	{
		printf("[WSAConnect] Port: %i\r\n",client->sin_port);
		printf("[WSAConnect] IP: %d.%d.%d.%d\n",
			int(client->sin_addr.s_addr&0xFF),
			int((client->sin_addr.s_addr&0xFF00)>>8),
			int((client->sin_addr.s_addr&0xFF0000)>>16),
			int((client->sin_addr.s_addr&0xFF000000)>>24));

		return INVALID_SOCKET;
	}

	if( ret != INVALID_SOCKET )
	{
		printf("[WSAConnect] Port: %i\r\n",client->sin_port);
		printf("[WSAConnect] IP: %d.%d.%d.%d\n",
			int(client->sin_addr.s_addr&0xFF),
			int((client->sin_addr.s_addr&0xFF00)>>8),
			int((client->sin_addr.s_addr&0xFF0000)>>16),
			int((client->sin_addr.s_addr&0xFF000000)>>24));

	}else{
		printf("[WSAConnect] Failed, Got INVALID_SOCKET\r\n");
		printf("[WSAConnect] WSAGetLastError(): %08X\r\n",WSAGetLastError() );

		return INVALID_SOCKET;
	}
	

	return 0;
}


VOID __declspec(naked) XHTTPConnectSaveVar(VOID)
{
	__asm
	{
			li r3,535
			nop 
			nop 
			nop 
			nop 
			nop 
			nop 
			blr 
	} 
}

PVOID XHTTPConnecthk(  PVOID hSession, const CHAR *pcszServerName, WORD nServerPort, DWORD dwFlags );
typedef PVOID(*XHTTPConnectFUN)( PVOID hSession, const CHAR *pcszServerName, WORD nServerPort, DWORD dwFlags );
static DWORD XHTTPConnectOld[4];
XHTTPConnectFUN XHTTPConnectSave = (XHTTPConnectFUN)XHTTPConnectSaveVar;

PVOID XHTTPConnecthk(  PVOID hSession, const CHAR *pcszServerName, WORD nServerPort, DWORD dwFlags )
{
	printf("[XHTTP] Connect: %s:%i\r\n",pcszServerName,nServerPort);
	return XHTTPConnectSave( hSession, pcszServerName, nServerPort, dwFlags );
}

#pragma endregion


#pragma region XNetInAddrToXnAddr HookSetup
VOID __declspec(naked) XNetInAddrToXnAddrSaveVar(VOID)
{
	__asm
	{
			li r3,335
			nop 
			nop 
			nop 
			nop 
			nop 
			nop 
			blr 
	} 
}


DWORD XNetInAddrToXnAddrhk( void* unk1, const IN_ADDR ina, XNADDR* pxna, XNKID *pxnkid );
typedef DWORD(*XNetInAddrToXnAddrFUN)( void* unk1, const IN_ADDR ina, XNADDR* pxna, XNKID *pxnkid );
static DWORD XNetInAddrToXnAddrold[4];
XNetInAddrToXnAddrFUN XNetInAddrToXnAddrSave = (XNetInAddrToXnAddrFUN)XNetInAddrToXnAddrSaveVar;
#pragma endregion

DWORD XNetInAddrToXnAddrhk( void* unk1, const IN_ADDR ina, XNADDR* pxna, XNKID *pxnkid )
{
	printf("[XNET] XNetInAddrToXnAddr\r\n");
	return XNetInAddrToXnAddrSave( unk1, ina, pxna, pxnkid );
}

#pragma region XNetGetTitleXnAddr Hook Setup
VOID __declspec(naked) XNetGetTitleXnAddrSaveVar(VOID)
{
	__asm
	{
			li r3,695
			nop 
			nop 
			nop 
			nop 
			nop 
			nop 
			blr 
	} 
}


DWORD XNetGetTitleXnAddrhk( void* unk1, XNADDR* pxna );
typedef DWORD(*XNetGetTitleXnAddrFUN)( void* unk1, XNADDR* pxna );
static DWORD XNetGetTitleXnAddrold[4];
XNetGetTitleXnAddrFUN XNetGetTitleXnAddrSave = (XNetGetTitleXnAddrFUN)XNetGetTitleXnAddrSaveVar;
#pragma endregion


DWORD XNetGetTitleXnAddrhk( void* unk1, XNADDR* pxna )
{
	printf("[XNET] XNetGetTitleXnAddr\r\n");
	XNADDR npxna;

	DWORD ret = XNetGetTitleXnAddrSave( unk1, &npxna );
	
	printf("[XNET] XNetGetTitleXnAddr - abOnline: 0x");
		for(int i=0;i<19;i++)
		{
			printf("%02X",npxna.abOnline[i]);
		}
		printf("\r\n");

		//BYTE nabNet[6] = { 0x00, 0xDD, 0xD8, 0x24, 0xE8, 0x00 };
		//memcpy(npxna.abEnet,&nabNet,6);
		//npxna.abEnet = { 0x00, 0x1D, 0xD8, 0x24, 0xE8 };
	printf("[XNET] XNetGetTitleXnAddr - abEnet: 0x");
		for(int i=0;i<5;i++)
		{
			printf("%02X",npxna.abEnet[i]);
		}
		printf("\r\n");

	memcpy(pxna,&npxna,sizeof(XNADDR));

	return ret;
}

#pragma region XNetXnAddrToInAddr Hook Setup
VOID __declspec(naked) XNetXnAddrToInAddrSaveVar(VOID)
{
	__asm
	{
			li r3,991
			nop 
			nop 
			nop 
			nop 
			nop 
			nop 
			blr 
	} 
}


DWORD XNetXnAddrToInAddrhk( void* unk1, XNADDR* pxna, XNKID* pxnkid, IN_ADDR *pina  );
typedef DWORD(*XNetXnAddrToInAddrFUN)( void* unk1, XNADDR* pxna, XNKID* pxnkid, IN_ADDR *pina );
static DWORD XNetXnAddrToInAddrold[4];
XNetXnAddrToInAddrFUN XNetXnAddrToInAddrSave = (XNetXnAddrToInAddrFUN)XNetXnAddrToInAddrSaveVar;
#pragma endregion

DWORD XNetXnAddrToInAddr( void* unk1, XNADDR *pxna, XNKID *pxnkid, IN_ADDR *pina )
{
	printf("[XNET] XNetXnAddrToInAddr\r\n");
	return XNetXnAddrToInAddrSave( unk1, pxna, pxnkid, pina );
}

VOID PatchDrawVert()
{
	    //xp 843419e4
		
		//hookFunctionStart((PDWORD)0x8293CFFC,(PDWORD)XeKeysGetConsoleIdSaveVar,XeKeysGetConsoleIdOld,(DWORD)XeKeysGetConsoleIdhk);

		// DrawIndexedVertices 
		// 0x8211A618 - NO TU
	    // 0x8216B1C8 - TU14
		// 0x8216B1C8 - TU15

		//hookFunctionStart((PDWORD)0x8216B1C8,(PDWORD)DrawIndexedVerticesSaveVar,DrawIndexedOld,(DWORD)DrawIndexedVerticesHook);
		//hookFunctionStart((PDWORD)0x821225B0,(PDWORD)D3DQuery_GetDataSaveVar,D3DQuery_GetDataOld,(DWORD)D3DQuery_GetDataHook);
		//hookFunctionStart((PDWORD)0x82113928,(PDWORD)SetTextureSaveVar,SetTextureOld,(DWORD)SetTextureHook);

	//if(PrintData == true)
//	{
		

		hookFunctionStart((PDWORD)0x8226BA00,(PDWORD)PostRenderHookSaveVar,PostRenderHookOld,(DWORD)PostRenderHook);
		hookFunctionStart((PDWORD)0x828A4E08,(PDWORD)R_AddRefEntityToSceneSaveVar,R_AddRefEntityToSceneOld,(DWORD)R_AddRefEntityToSceneHook);


		//hookFunctionStart((PDWORD)0x8293CC5C,(PDWORD)XUserGetXUIDSaveVar,XUserGetXUIDOld,(DWORD)XUserGetXUIDhk);
		//hookFunctionStart((PDWORD)0x8293DB8C,(PDWORD)XNetGetTitleXnAddrSaveVar,XNetGetTitleXnAddrold,(DWORD)XNetGetTitleXnAddrhk);
		//hookFunctionStart((PDWORD)0x8293DACC,(PDWORD)XNetInAddrToXnAddrSaveVar,XNetInAddrToXnAddrold,(DWORD)XNetInAddrToXnAddrhk);
		//hookFunctionStart((PDWORD)0x8293CE9C,(PDWORD)XUserGetSigninInfoSaveVar,XUserGetSigninInfoOld,(DWORD)XUserGetSigninInfohk);

		//hookFunctionStart((PDWORD)0x8293DDCC,(PDWORD)XHTTPConnectSaveVar,XHTTPConnectOld,(DWORD)XHTTPConnecthk);
		//hookFunctionStart((PDWORD)0x8293DC6C,(PDWORD)NetDll_sendSaveVar,NetDll_sendOld,(DWORD)NetDll_sendhk);
		//hookFunctionStart((PDWORD)0x8293DC4C,(PDWORD)NetDll_recvSaveVar,NetDll_recvOld,(DWORD)NetDll_recvhk);
		//hookFunctionStart((PDWORD)0x8293DC2C,(PDWORD)NetDll_ConnectSaveVar,NetDll_ConnectOld,(DWORD)NetDll_Connecthk);

		
	//	DbgPrint("Patched eeverything else\r\n");
///	}
	//	if(PrintData == false)
	//	{
			//hookFunctionStart((PDWORD)0x8293CCEC,(PDWORD)XUserGetNameSaveVar,XUserGetNameOld,(DWORD)XUserGetNamehk);
			//BYTE PatchVal[8] = { 0x3E, 0x60, 0x00, 0x01, 0x4E, 0x80, 0x00, 0x20 };
			//memcpy((PVOID)0x825DD5E8,PatchVal,8);
	//		PrintData = true;
			//DbgPrint("Patched GetName\r\n");
	//	}
	//*/
		
		//No Recoil
		*(int*)(0x82259C00)=0x60000000;

		/*
		demonware1 = 0x825990F8, 
		demonware2 = 0x825DC994, 
		demonware3 = 0x822D19E8, 
		demonware4 = 0x825990B8, 
		*/

		//*(int*)(0x825990F8)=0x60000000;				 // Coder123's patches, Hits a breakpoint inside of it too doing some odd stuff.
		//*(int*)(0x825DC994)=0x60000000;				 // Coder123's patches
		//*(LONGLONG*)(0x822D19E8)=0x386000004E800020; // EXE_COD_ONLINE_PERM_BAN_CONSOLE
		//*(LONGLONG*)(0x82599668)=0x386000004E800020; // banCheck
		//*(int*)(0x825DF880)=0x60000000; // EXE_COD_PIRACY_TEMP_BAN_PLAYER - ETC.
		//*(int*)(0x825DC9E4)=0x60000000; // challengeResponseResendInterval / banCheck / lanChallengeResponse / Makes game run smoother tbh.
		//*(LONGLONG*)(0x825DAC00)=0x386000004E800020; // EXE_COD_PIRACY_TEMP_BAN_PLAYER - ETC.
		//*(LONGLONG*)(0x825D9C28)=0x386000004E800020; // fakeDWDisconnect

		//*(LONGLONG*)(0x825996C8)=0x386000004E800020; // PLATFORM_DEMONWARE_DISCONNECT - Constant Loops - makes login not work anymore. 

		//*(LONGLONG*)(0x825997C0)=0x386000004E800020; // PLATFORM_DEMONWARE_DISCONNECT - Constant Loops
		//*(LONGLONG*)(0x82599088)=0x386000004E800020; // Original instant ban bypass attempt at improving.
		
	
		//*(LONGLONG*)(0x82595060)=0x386000014E800020; // FIRST_SIGNIN - Not really needed no  idea what we were trying to do here.
													 // 0x82599668 banCheck
													 // 0x822A0E0C iLocalJoinFailed banned

		/*
			 EXE_COD_PIRACY_TEMP_BAN_PLAYER - ETC.
			.text:825DAE0C                 li        r3, 5
			.text:825DAE10                 bl        sub_825993A0
		*/

		DbgPrint("Patched the system\r\n");


		
		XShowMessageBoxUI(0, L"Rainbow World On", L"Hax Activated - WallHax+Name ESP", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
}