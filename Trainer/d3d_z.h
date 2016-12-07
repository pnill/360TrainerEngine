//--------------------------------------------------------------------------------------
// XuiDrawText.cpp
//
// The sample demonstrates how to draw text using Xui, outside of the Xui runtime 
// control framework.
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#include <xtl.h>
#include <xui.h>
#include <stdio.h>


//--------------------------------------------------------------------------------------
// Global variables and definitions
//--------------------------------------------------------------------------------------

HXUIDC                  g_hDC = 0;                    // Xui device context
HXUIFONT                g_hFont = 0;                    // Handle to Xui font
IDirect3D9*             g_pD3D = 0;                    // D3D interface
IDirect3DDevice9*       g_pDevice = 0;                    // D3D device interface
D3DPRESENT_PARAMETERS   g_d3dpp =
{
	0
};                  // D3D presentation paramters
bool                    g_fXuiRenderInitialized = false;  // Initialization flags
bool                    g_fXuiInitialized = false;


HRESULT InitD3D();
void UninitD3D();
HRESULT InitXui();
void UninitXui();
void RenderScene();
void DrawText( HXUIDC hdc, HXUIFONT hFont, D3DCOLOR color, float x, float y, LPCWSTR text );


//--------------------------------------------------------------------------------------
// Name: main
// Desc: Entry point to the program
//--------------------------------------------------------------------------------------
int __cdecl RunTextRender()
{
	InitD3D();

	if( FAILED( InitXui() ) )
		return -1;

	DWORD startTime = GetTickCount();
	while( GetTickCount() < startTime + 10000 )
	{
		RenderScene();
	}

	UninitXui();
	//UninitD3D();
}


//--------------------------------------------------------------------------------------
// Name: InitD3D
// Desc: Initialize D3D and create the D3D device.
//--------------------------------------------------------------------------------------
HRESULT InitD3D()
{
	HRESULT hr = S_OK;

	//g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	//if( g_pD3D == 0 )
	//	return E_FAIL;

	ZeroMemory( &g_d3dpp, sizeof( g_d3dpp ) );

	g_d3dpp.BackBufferWidth = 1280;
	g_d3dpp.BackBufferHeight = 720;

	g_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	g_d3dpp.BackBufferCount = 1;
	g_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//hr = g_pD3D->CreateDevice( 0, D3DDEVTYPE_HAL, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pDevice );
	//if( FAILED( hr ) )
	//{
	//	UninitD3D();
	//}

	return hr;
}


//--------------------------------------------------------------------------------------
// Name: UninitD3D
// Desc: Release all resources used by the D3D device.
//--------------------------------------------------------------------------------------
void UninitD3D()
{
	if( g_pDevice != 0 )
	{
		g_pDevice->Release();
		g_pDevice = 0;
	}

	if( g_pD3D != 0 )
	{
		g_pD3D->Release();
		g_pD3D = 0;
	}
}


//--------------------------------------------------------------------------------------
// Name: InitXui
// Desc: Initialize the Xui runtime and render libraries.
//--------------------------------------------------------------------------------------
HRESULT InitXui()
{
	if( g_pDevice == 0 )
		return E_FAIL;

	XUIInitParams initparams =
	{
		0
	};
	XUI_INIT_PARAMS( initparams );

	// Typeface descriptor specifies a name and file location for a font
	TypefaceDescriptor desc =
	{
		0
	};
	desc.szTypeface = L"Recovery";
	desc.szLocator = L"file://flash:/recovery.ttf";


	// Initialize Xui render library with our D3D device, 
	// and use a Xui-provided texture loader.
	//HRESULT hr = XuiRenderInitShared( g_pDevice, &g_d3dpp, XuiD3DXTextureLoader );
	//if( FAILED( hr ) )
	//	goto error;
	HRESULT hr = XuiRenderInit( NULL, 
		XuiD3DXTextureLoader );

	//HRESULT hr = XuiRenderInitShared( g_pDevice, &g_d3dpp, XuiD3DXTextureLoader );
	//HRESULT hr;

	g_fXuiRenderInitialized = true;

	// Create a Xui device context. The Xui text renderer uses many attributes
	// from this device context (position, color, shaders, etc.).
	hr = XuiRenderCreateDC( &g_hDC );
	if( FAILED( hr ) )
		goto error;

	printf("g_hDC: %08X\r\n",g_hDC);

	// Initialize the Xui runtime library.  Typeface descriptors are registered
	// by the runtime library, and consumed by the render library.
	hr = XuiInit( &initparams );
	if( FAILED( hr ) )
		goto error;
	g_fXuiInitialized = true;

	// Register our typeface name and font location.
	hr = XuiRegisterTypeface( &desc, TRUE );
	if( FAILED( hr ) )
		goto error;

	// Instantiate an 18pt font.
	hr = XuiCreateFont( L"Recovery", 18.0f, XUI_FONT_STYLE_NORMAL, 0, &g_hFont );
	if( FAILED( hr ) )
		goto error;

	return hr;

error:
	UninitXui();
	return hr;
}


//--------------------------------------------------------------------------------------
// Name: UninitXui
// Desc: Release resources used by the Xui font, device context and render libraries.
//--------------------------------------------------------------------------------------
void UninitXui()
{
	if( g_hFont != 0 )
	{
		XuiReleaseFont( g_hFont );
		g_hFont = 0;
	}

	if( g_hDC != 0 )
	{
		XuiRenderDestroyDC( g_hDC );
		g_hDC = 0;
	}

	if( g_fXuiRenderInitialized )
	{
		XuiRenderUninit();
		g_fXuiRenderInitialized = false;
	}

	if( g_fXuiInitialized )
	{
		XuiUninit();
		g_fXuiInitialized = false;
	}
}


//--------------------------------------------------------------------------------------
// Name: RenderScene
// Desc: Render and present a frame.
//--------------------------------------------------------------------------------------
void RenderScene()
{
	g_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0, 1.0f, 0L );

	// Begin Xui rendering
	XuiRenderBegin( g_hDC, D3DCOLOR_ARGB( 255, 0, 0, 0 ) );

	// Set the view
	D3DXMATRIX matView;
	D3DXMatrixIdentity( &matView );
	XuiRenderSetViewTransform( g_hDC, &matView );


	//Xui::XuiTextCommon::Render(struct _XUIOBJ *, wchar_t const *, int, unsigned long, float, int, struct XUITextFormatInfo *, struct XUIMessageRender *)
	// Draw the text somewhere

	DrawText( g_hDC, g_hFont, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 200, 200, L"Some Text" );

	// Complete Xui rendering
	XuiRenderEnd( g_hDC );
	XuiRenderPresent( g_hDC, NULL, NULL, NULL );

	g_pDevice->Present( NULL, NULL, NULL, NULL );
}


//--------------------------------------------------------------------------------------
// Name: DrawText
// Desc: Draw text at the given coordinates with the given color.
//--------------------------------------------------------------------------------------
void DrawText( HXUIDC hdc, HXUIFONT hFont, D3DCOLOR color, float x, float y, LPCWSTR text )
{
	// Measure the text
	XUIRect clipRect( 0, 0, g_d3dpp.BackBufferWidth - x, g_d3dpp.BackBufferHeight - y );
	XuiMeasureText( hFont, text, -1, XUI_FONT_STYLE_NORMAL, 0, &clipRect );

	// Set the text position in the device context
	D3DXMATRIX matXForm;
	D3DXMatrixIdentity( &matXForm );
	matXForm._41 = x;
	matXForm._42 = y;
	XuiRenderSetTransform( hdc, &matXForm );

	// Select the font and color into the device context
	XuiSelectFont( hdc, hFont );
	XuiSetColorFactor( hdc, ( DWORD )color );

	// Draw the text
	XuiDrawText( hdc, text, XUI_FONT_STYLE_NORMAL, 0, &clipRect );
}






