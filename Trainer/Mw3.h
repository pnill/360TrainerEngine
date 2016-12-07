#include "common.h"
#include <d3d9.h>
#include <xgraphics.h>

UINT32 g_titleID = 0x4D530805;
bool RunOnce = true;
using namespace std;

#pragma region Engine Defintions

enum entity_type_t
{
	ET_GENERAL			= 0,
	ET_PLAYER           = 1,
	ET_PLAYER_CORPSE    = 2,
	ET_ITEM             = 3,
	ET_EXPLOSIVE        = 4,
	ET_INVISIBLE        = 5,
	ET_SCRIPTMOVER      = 6,
	ET_SOUND_BLEND      = 7,
	ET_FX               = 8,
	ET_LOOP_FX          = 9,
	ET_PRIMARY_LIGHT    = 10,
	ET_TURRET           = 11,
	ET_HELICOPTER       = 12,
	ET_PLANE            = 13,
	ET_VEHICLE          = 14,
	ET_VEHICLE_COLLMAP  = 15,
	ET_VEHICLE_CORPSE   = 16,
	ET_VEHICLE_SPAWNER  = 17
};  

#define STYLE_NONE			0
#define STYLE_SHADOW		3
#define STYLE_OUTLINE		7


class CVector
{
public:
	float x;
	float y;
	float z;

	CVector() : x( 0 ), y( 0 ), z( 0 ) {}
	CVector( float _x, float _y, float _z ) : x( _x ), y( _y ), z( _z ) {}
};

class CColor
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	CColor( unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a )
		: r( _r ), g( _g ), b( _b ), a( _a )
	{
	}

	unsigned int ABGR() const
	{
		return ( ( a << 24 ) | ( b << 16 ) | ( g << 8 ) | ( r ) );
	}

	unsigned int RGBA() const
	{
		return ( ( r << 24 ) | ( g << 16 ) | ( b << 8 ) | ( a ) );
	}

	float* FloatPtr() const
	{
		static float flColor[ 4 ];

		flColor[ 0 ] = float( r ) / 255.0f;
		flColor[ 1 ] = float( g ) / 255.0f;
		flColor[ 2 ] = float( b ) / 255.0f;
		flColor[ 3 ] = float( a ) / 255.0f;

		return flColor;
	}
};


class CRotator
{
public:
	float pitch;
	float yaw;
	float roll;

	CRotator() : pitch( 0 ), yaw( 0 ), roll( 0 ) { }
};



typedef struct{
	char _0x0000[8];
	__int32 Width; //0x0008 
	__int32 Height; //0x000C 
	char _0x0010[12];
	__int32 FrameCount; //0x001C 
	__int32 LocalGame; //0x0020 
	char GameType[4]; //0x0024 
	char _0x0028[28];
	char Hostname[64]; //0x0044 
	char _0x0084[196];
	__int32 MaxClients; //0x0148 
	char _0x014C[4];
	char MapPath[64]; //0x0150 
	char _0x0190[128];
} cgs_t;

typedef struct {
	int Index;
	char Unknown_0x004[ 0x0100 - 0x004 ];
} SpecialEntity_t;

typedef struct {
	char Unknown_0x330C[ 0x330C - 0x0000 ];
	int PlayerCount;
	char Unknown_0x3314[ 0x3314 - 0x3310 ];

	SpecialEntity_t Entities[ 1 ];

} snapshot_t;

typedef struct{
	int bValid;
	char Unknown_0x000C[ 0x000C - 0x0004];
	char szPlayerName[ 16 ]; // 0x00C
	char Unknown_0x002C[ 0x002C - 0x001C ];
	int iTeam; // 0x002C
	char Unknown_0x0590[ 0x590 - 0x002C ];

} clientInfo_t; // 0x590 Size


//cg_t + 0x105900 = ScreenMatrix?
typedef struct{
	int Ticks;
	char Unknown_0x0150[ 0x150 - 0x04 ];
	int LocalClient;
	char Unknown_0x019C[ 0x19C - 0x154 ];
	int Health;
	char Unknown_0x0204[ 0x204 - 0x1A0 ];
	int AdvancedUAV;
	char Unknown_0x335C[ 0x335C - 0x208 ];
	snapshot_t* Snapshot;
	char Unknown_0xFDE8C[ 0xFDF8C - 0x3364 ];
	clientInfo_t clientInfo[ 1 ];

} cg_t; // 0x105A80


typedef struct{
	char Unknown_0x0000[ 0x0014 - 0x0000 ];
	CVector WorldLocation;
	char Unknown_0x00C8[ 0x00C8 - 0x0020 ];
	DWORD Type;				
	DWORD Flags;							
	char Unknown_0x0154[ 0x0154 - 0x00D0 ];
	int clientNum;
	char Unknown_0x01EC[ 0x01EC - 0x0158 ];
} entity_t; // 0x1EC Size

char* Bones[20] = {
	/*Head*/          "j_helmet"     , "j_head"            , "j_neck" 
	/*Arms*/        , "j_shoulder_le", "j_shoulder_ri"  , "j_elbow_le"     , "j_elbow_ri", "j_wrist_le", "j_wrist_ri", "j_gun"
	/*Back*/        , "j_mainroot"   , "j_spineupper"   , "j_spinelower" , "j_spine4" 
	/*Legs*/        , "j_hip_ri"     , "j_hip_le"       , "j_knee_le"    , "j_knee_ri" , "j_ankle_ri", "j_ankle_le"};

#define FONT_SMALL_DEV    "fonts/smallDevFont"
#define FONT_BIG_DEV    "fonts/bigDevFont"
#define FONT_CONSOLE    "fonts/consoleFont"
#define FONT_BIG        "fonts/bigFont"
#define FONT_SMALL        "fonts/smallFont"
#define FONT_BOLD        "fonts/boldFont"
#define FONT_NORMAL        "fonts/normalFont"
#define FONT_EXTRA_BIG    "fonts/extraBigFont"
#define FONT_OBJECTIVE    "fonts/objectiveFont" 

CColor colWhite( 255, 255, 255, 255 );
CColor colBlack( 0, 0, 0, 255 );
CColor colRed( 255, 0, 0, 255 );
CColor colGreen( 0, 255, 0, 255 );
CColor colBlue( 0, 0, 255, 255 );
CColor colYellow( 255, 255, 0, 255 );
CColor colCyan( 0, 255, 255, 255 );
CColor colPurple( 255, 0, 255, 255 );

typedef float vec3_t[3];
typedef float vec2_t[2];
typedef vec3_t Vec3;
typedef Vec3 Vector3;

float Vec3DistanceSq(const float *p1, const float *p2)
{
	return ((p2[0] - p1[0]) * (p2[0] - p1[0]) + ((p2[2] - p1[2]) * (p2[2] - p1[2]) + ((p2[1] - p1[1]) * (p2[1] - p1[1]))));
}
float Distance(float* p1, float* p2)
{
	return sqrt(Vec3DistanceSq(p1, p2)) / 48;


}


#pragma endregion 

#pragma region Engine Functions


#define OffCalcName 0x82112908
#define OffDrawTextPhysical 0x82174C78
#define OffRegisterFont 0x82174F88
#define OffDrawStrechPic 0x82164AF8 
#define OffMaterialRegisterHandle 0x82413E80 
#define OffTargetVisible 0x820D5010 
#define OffGetTargetPos 0x820D4C40
#define OffAllocString 0x82251320
#define OffSameTeam 0x8226CE28 
#define RenderOffset 0x82115308

entity_t** ppEntities = (entity_t**)0x826436B8; 
cgs_t** ppGame_s = (cgs_t**)0x8263EE44; 

cg_t* GetCGame( int localClientNum )
{
	return (cg_t*)(*(DWORD*)0x8263EE60 + (localClientNum*0x105A80));
}



void(__cdecl *CG_CalcNamePosition)(int localClientNum, CVector* origin, float *xOut, float *yOut) = (void(__cdecl*)(int,CVector*,float*,float*))OffCalcName;
void(__cdecl *CL_DrawTextPhysical)(const char* text, int maxChars, DWORD pFont, float x, float y, float xScale, float yScale, float *color, int style ) = (void (__cdecl *)(const char*, int, DWORD, float, float,float,float,float *,int))OffDrawTextPhysical;
DWORD(__cdecl *CL_RegisterFont)(const char* fontName, int imageTrack) = (DWORD(__cdecl*)(const char*,int))OffRegisterFont;

void(__cdecl *CL_DrawStretchPicPhysical)(float flX, float flY, float flWidth, float flHeight, float flU, float flUH, float flV, float flVH, float* vecColor, DWORD pTexture)=(void(__cdecl*)(float,float,float,float,float,float,float,float,float*, DWORD))OffDrawStrechPic;
DWORD(__cdecl *Material_RegisterHandle)(const char* name, int imageTrack)=(DWORD(__cdecl*)(const char*,int))OffMaterialRegisterHandle;

bool(__cdecl *AimTarget_IsTargetVisible)(int localClientNum, entity_t *targetEnt, unsigned int visBone )=(bool(__cdecl*)(int,entity_t*,unsigned int))OffTargetVisible;
void(__cdecl *AimTarget_GetTagPos)(int localClientNum, entity_t *cent, unsigned int tagName, CVector *pos)=(void(__cdecl*)(int,entity_t*,unsigned int, CVector*))OffGetTargetPos;

unsigned int(__cdecl *GScr_AllocString)(const char *s)=(unsigned int(__cdecl*)(const char *s))OffAllocString;

int(__cdecl *OnSameTeam)(entity_t *ent1, entity_t *ent2)=(int(__cdecl*)(entity_t*,entity_t*))OffSameTeam;


bool IsBoneVisible( int localClientNum, entity_t *targetEnt, const char  *VisBone)
{
	unsigned int iVisBone = GScr_AllocString( VisBone );
	return AimTarget_IsTargetVisible( localClientNum, targetEnt, iVisBone );
}

bool IsVisible(int localClientNum, entity_t *targetEnt)
{
	for(int j=0;j<20;j++)
	{
		unsigned int iVisBone = GScr_AllocString( Bones[j] );

		if( AimTarget_IsTargetVisible( localClientNum, targetEnt, iVisBone ) )
		{
			return true;
		}

	}

	return false;
}


bool GetTagPos( int localClientNum, entity_t *cent, const char *tagName, CVector* pos)
{
	if(localClientNum != 0 && localClientNum != 1)
		return false;

	unsigned int iTagName = GScr_AllocString(tagName);
	
	AimTarget_GetTagPos( localClientNum, cent, iTagName, pos );
	return true;
}

void JustHereForBP(void)
{

	int DoNothing = 1;
	if(PrintCur == true)
	{
		printf("DoNothing: %i",DoNothing);
	}

}

void DrawRect( float flX, float flY, float flWidth, float flHeight, const CColor& colDrawColor )
{
	static DWORD pMaterial = Material_RegisterHandle( "white" , 7);

	CL_DrawStretchPicPhysical( flX, flY, flWidth, flHeight, 0, 0, 1, 1, colDrawColor.FloatPtr(),pMaterial);
}

void DrawBox( float flX, float flY, float flWidth, float flHeight, const CColor& colDrawColor, float flThickness )
{
	DrawRect( flX, flY, flWidth, flThickness, colDrawColor );
	DrawRect( flX, flY, flThickness, flHeight, colDrawColor );
	DrawRect( flX + flWidth - flThickness, flY, flThickness, flHeight, colDrawColor );
	DrawRect( flX, flY + flHeight - flThickness, flWidth, flThickness, colDrawColor );

}

void DrawCorners( float flX, float flY, float flWidth, float flHeight, const CColor& colDrawColor )
{
	float flDH = flHeight / 4;
	float flDW = flWidth / 4;

	if ( flDW < 1 )
		flDW = 1;

	DrawRect( flX, flY, flDW, 1, colDrawColor );
	DrawRect( flX + flWidth - flDW + 1, flY, flDW, 1, colDrawColor );
	DrawRect( flX, flY + flHeight, flDW, 1, colDrawColor );
	DrawRect( flX + flWidth - flDW + 1, flY + flHeight, flDW, 1, colDrawColor );

	if ( flDH > 0 )
	{
		DrawRect( flX, flY + 1, 1, flDH, colDrawColor );
		DrawRect( flX + flWidth, flY + 1, 1, flDH, colDrawColor );
		DrawRect( flX, flY + flHeight - flDH, 1, flDH, colDrawColor );
		DrawRect( flX + flWidth, flY + flHeight - flDH, 1, flDH, colDrawColor );
	}
}

void DrawString( float flX, float flY, const CColor& colDrawColor, bool bCenter, char* pszString )
{
	static DWORD pFont = CL_RegisterFont("fonts/consoleFont",1);
	float flScale = 1;
	CL_DrawTextPhysical( pszString, 0x7FFFFFFF, pFont, flX, flY + 14, flScale, flScale, colDrawColor.FloatPtr(), STYLE_SHADOW );
}

void DrawStringEx( float flX, float flY, const CColor& colDrawColor, bool bCenter, char* pszString, ... )
{
	char szStringBuffer[ 1024 ];
	va_list vArgList;

	va_start( vArgList, pszString );
	sprintf( szStringBuffer, pszString, vArgList );
	va_end( vArgList );

	DrawString( flX, flY, colDrawColor, bCenter, szStringBuffer );
	memset( szStringBuffer, 0, sizeof( szStringBuffer ) );
}


CVector BonePos[20];

void DoStuff(int localClientNum) 
{

	if(localClientNum != 1 && localClientNum !=0)
		return;

	DrawString( 20.0f,20.0f, colGreen, false, "exbls' MW3 Hook");

	entity_t* pEntities = *ppEntities;
	cgs_t* pGame_s = *ppGame_s;
	cg_t* pGame = GetCGame(localClientNum);

	if( pGame == nullptr || pEntities == nullptr || pGame_s == nullptr )
		return;

	snapshot_t* pSnapshot = pGame->Snapshot;

	if(pSnapshot == nullptr )
		return;
	
	int iLocalIndex = pGame->LocalClient;

	entity_t* pLocalEntity = &pEntities[ iLocalIndex ];

	clientInfo_t* pLocalClient = &pGame->clientInfo[ iLocalIndex ];


		for( int i=0; i < pGame->Snapshot->PlayerCount; i++ )
		{
			int iEntityIndex = pGame->Snapshot->Entities[ i ].Index;

			entity_t* pEntity = &pEntities[ iEntityIndex ];
			
			if( iEntityIndex == iLocalIndex
				|| pEntity->Flags == 0x20 )
				continue;

			if( pEntity->Type != ET_PLAYER 
				&& pEntity->Type != ET_EXPLOSIVE )
				continue; 

			
			clientInfo_t* pClient = &pGame->clientInfo[ iEntityIndex ];

			if( pClient->bValid == false && pEntity->Type != ET_EXPLOSIVE )
				continue;

			float TotalDistance = Distance( (float*)&pEntity->WorldLocation,(float*)&pLocalEntity->WorldLocation );				
			char DistanceString[255];

			sprintf(DistanceString,"Distance: %i m",(int)TotalDistance);

			// Draw enemy explosives
			if(pEntity->Type == ET_EXPLOSIVE )
			{	
				if( OnSameTeam( pEntity, pLocalEntity ) == 0 )
				{
					float expX;
					float expY;

					CG_CalcNamePosition( localClientNum, &pEntity->WorldLocation, &expX, &expY );
					
					static DWORD nFont = CL_RegisterFont( FONT_CONSOLE, 1);


					//if( IsVisible( pEntity ) )
					//{
					//	DrawBox( expX,expY, 100.0f, 100.0f, colPurple, 1.0f);
					//	
					//	DrawString( expX,expY-100.0f, colPurple, false, DistanceString);

					//}
					//else
					//{
						//DrawCorners( expX,expY, 100.0f, 100.0f, colBlue)
					if( TotalDistance <= 20 && TotalDistance > 10 )
					{
						DrawBox(expX-10.0f,expY-10.0f,20.0f,20.0f,colBlue,1.0f);

						DrawString( expX, expY-30.0f, colBlue, false, "Explosive");
						DrawString( expX, expY-20.0f, colBlue, false, DistanceString);
					}

					if( TotalDistance <= 10 )
					{
						DrawBox(expX-10.0f,expY-10.0f,20.0f,20.0f,colCyan,1.0f);

						DrawString( expX, expY-30.0f, colCyan, false, "Explosive - !WARNING!");
						DrawString( expX, expY-20.0f, colCyan, false, DistanceString);
					}



				}

				//}

		}


		if( pEntity->Type == ET_PLAYER )
		{
			bool bEnemy = ( pLocalClient->iTeam != 0 ) ? ( pClient->iTeam != pLocalClient->iTeam ) : true;

			if( bEnemy)
			{
						CVector vecLocation;
						CVector vecBaseLocation;


						if( GetTagPos( localClientNum, pEntity, "j_head", &vecLocation ) == false ) return;
						vecBaseLocation = pEntity->WorldLocation;

						CVector vecScreen;
						CVector vecBox;

						CG_CalcNamePosition( localClientNum,&vecLocation,&vecScreen.x,&vecScreen.y );
						CG_CalcNamePosition( localClientNum,&vecBaseLocation,&vecBox.x,&vecBox.y );

						float flHeight = ( vecBox.y - vecScreen.y );
						float flWidth = flHeight / 2.0f;


						vecScreen.x = vecBox.x;
						static DWORD nFont = CL_RegisterFont( FONT_CONSOLE, 1);

						/* Draw Name */
						if( IsVisible( localClientNum, pEntity ) )
						{
							DrawBox( vecScreen.x - (flWidth / 2.0f ), vecScreen.y, flWidth, flHeight, colRed, 1.0f );

							DrawString( vecScreen.x-40.0f, vecScreen.y-20.0f, colRed, false, pClient->szPlayerName );
							DrawString( vecScreen.x-40.0f, vecScreen.y-30.0f, colRed, false, DistanceString);
						}	
						else
						{
							DrawCorners( vecScreen.x - (flWidth / 2.0f), vecScreen.y, flWidth,flHeight,colYellow );
						
							DrawString( vecScreen.x-40.0f, vecScreen.y-20.0f, colYellow, false, pClient->szPlayerName );
							DrawString( vecScreen.x-40.0f, vecScreen.y-30.0f, colYellow, false, DistanceString);
			
						}

				}
		}
				
				memset( DistanceString, 0, sizeof( DistanceString ) );
			
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

void PostRenderHook(int localClientNum);
typedef void (*PostRenderHookFUN)(int localClientNum);
static DWORD PostRenderHookOld[4];
PostRenderHookFUN PostRenderHookSave = (PostRenderHookFUN)PostRenderHookSaveVar;

void PostRenderHook(int localClientNum)
{
	
	PostRenderHookSave(localClientNum);
	DoStuff(localClientNum);
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

#pragma region D3D Hooks
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
		D3DXCreateTextureFromFileInMemory(pDevice,(LPVOID)&bPurple,sizeof(green),&g_pTexGreen);
		D3DXCreateTextureFromFileInMemory(pDevice,(LPVOID)&bAqua,sizeof(blue),&g_pTexBlue);
	}
	/*
	if(pTexture != NULL && PrintData == true )
	{

		UINT dwResourceMemSize, uiResourceMem;//, uiExtraMem, dwExtraMemSize;


		XGGetTextureLayout(pTexture,&uiResourceMem,(UINT*)&dwResourceMemSize,NULL,NULL,0,NULL,NULL,NULL,NULL,0);
		VOID* baseBuffer = (VOID*)uiResourceMem;

		if(baseBuffer != NULL)
		{
				if( Sampler == 0 ) 
				{
					DWORD m_Stage3Crc = crc32(0, baseBuffer, 1024);

					Tlog.Add(AllTextures,m_Stage3Crc);

					if(PrintSave == true )
					{
						printf("-- SetTextureHook --\r\n");
						printf("m_Stage3Crc = %08X\r\n",m_Stage3Crc);
						printf("Sampler = %08X\r\n",Sampler);
						printf("PendingMask3 = %08X\r\n",PendingMask3);
						PrintSave = false;
					}
				}
		}
	}*/



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
	/*
	DWORD m_stage3Crc;
	IDirect3DBaseTexture9* Stage3Tex;
	pDevice->GetTexture(0,&Stage3Tex);


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
			//if( m_stage3Crc == AllTextures[tx]
			if(	 m_stage3Crc == 0xCBAFCB95 || m_stage3Crc == 0x93afa9dc || m_stage3Crc == 0xAE014801 || m_stage3Crc == 0xC1FA2C69 || m_stage3Crc == 0xB30FAA33 || m_stage3Crc == 0xD69A6F8E
				|| m_stage3Crc == 0xE2D914F8 || m_stage3Crc == 0xC0BDC0BF || m_stage3Crc == 0x13BC656A || m_stage3Crc == 0x88CD7CE3 || m_stage3Crc == 0x0BA6C957 || m_stage3Crc == 0x8C2760BD
				|| m_stage3Crc == 0xC01AD527 || m_stage3Crc == 0xAA9D196F || m_stage3Crc == 0xB2B5A5F9 || m_stage3Crc == 0xDA87CA61 || m_stage3Crc == 0xD06C8AF6 || m_stage3Crc == 0xD0B069BB
				|| m_stage3Crc == 0x0FD4B9FB || m_stage3Crc == 0xB755B136 || m_stage3Crc == 0x4B793B2E || m_stage3Crc == 0xA8190A7F || m_stage3Crc == 0xC99520C8 || m_stage3Crc == 0x2AE0D806
				|| m_stage3Crc == 0xBEA0D9AB || m_stage3Crc == 0xB79AB113 || m_stage3Crc == 0x8F9DABC7 || m_stage3Crc == 0xD262D9F8 || m_stage3Crc == 0x6ACD3F80 || m_stage3Crc == 0x03CE863C
				|| m_stage3Crc == 0x592F6226 || m_stage3Crc == 0x23CF3042 || m_stage3Crc == 0x633BAADF || m_stage3Crc == 0x6E6B7434 || m_stage3Crc == 0x1F10E490 || m_stage3Crc == 0x1020633F
				|| m_stage3Crc == 0xDDB280B8 || m_stage3Crc == 0xEE831612 || m_stage3Crc == 0x40AECD49 || m_stage3Crc == 0xFA7E1558 || m_stage3Crc == 0x86CAD2F0 || m_stage3Crc == 0x7EEDEA38
 																		 )
			{




				pDevice->SetTexture( 0, g_pTexBlue);
				//SetTextureSave( pDevice, 1, g_pTexBlue, 0 );
				//SetTextureSave( pDevice, 2, g_pTexBlue, 0 );
				//SetTextureSave( pDevice, 3, g_pTexBlue, 0 );
				//SetTextureSave( pDevice, 4, g_pTexBlue, 0 );

			
				pDevice->SetRenderState(D3DRS_ZENABLE , false );


				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	

				DrawIndexedVerticesSave(pDevice,PrimitiveType,BaseVertexIndex,StartIndex,IndexCount);

				pDevice->SetTexture( 0, g_pTexGreen);

				pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
				pDevice->SetRenderState ( D3DRS_ZENABLE , true );


				//SetTextureSave( pDevice, 1, g_pTexGreen, 0 );
				//SetTextureSave( pDevice, 2, g_pTexGreen, 0 );
				//SetTextureSave( pDevice, 3, g_pTexGreen, 0 );
				//SetTextureSave( pDevice, 4, g_pTexGreen, 0 );



			}	

			/*if(	m_stage3Crc == 0xA3CF81B7 )
			{

				pDevice->SetRenderState ( D3DRS_ZENABLE , false);
				pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
				pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVDESTCOLOR);
				pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVSRCALPHA);

				DrawIndexedVerticesSave(pDevice,PrimitiveType,BaseVertexIndex,StartIndex,IndexCount);

				pDevice->SetRenderState ( D3DRS_ZENABLE , true );
				pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
				pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVDESTCOLOR);
				pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVSRCALPHA);


			}*/
	//	}
	//}


//	if(PrintCur == true)
//	{
//		printf("-- DrawIndexedVertices --");
//		DbgPrint("AllTextures[tx]: %08X\r\n",AllTextures[tx]);
//		DbgPrint("m_stage0Crc: %08X\r\n",m_stage3Crc);
//		DbgPrint("tx: %i\r\n",tx);
//		DbgPrint("AllTextures.size(): %i",AllTextures.size());
//		PrintCur = false;
//	}

	D3DVOID ret = DrawIndexedVerticesSave(pDevice,PrimitiveType,BaseVertexIndex,StartIndex,IndexCount);
	return ret;
}
#pragma endregion




VOID PatchDrawVert()
{

	
	


	if(*(int*)RenderOffset  == 0x7D8802A6)
	{
		hookFunctionStart((PDWORD)RenderOffset,(PDWORD)PostRenderHookSaveVar,PostRenderHookOld,(DWORD)PostRenderHook);
		XShowMessageBoxUI(0, L"Rainbow World On", L"Hax Activated - WallHax+Name ESP", ARRAYSIZE(g_strButtons), g_strButtons, 0, XMB_NOICON, &g_mb_result, &g_xol);
	}	
}