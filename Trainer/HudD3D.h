#include "common.h"
#include "d3d9.h"

bool firstrun = true;

HRESULT render(void);
void initialize_data(void);

// The name of our application.  Used for window and MessageBox titles and error reporting
const char *g_app_name="Plotting Points";

// Our screen/window sizes and bit depth.  A better app would allow the user to choose the
// sizes.  I'll do that in a later tutorial, for now this is good enough.
const int g_width=640;
const int g_height=480;
const int g_depth=16; //16-bit colour

// Our global flag to track whether we should quit or not.  When it becomes true, we clean
// up and exit.
bool g_app_done=false;

// The D3DDevice is your main rendering interface.  It represents the display and all of its
// capabilities.  When you create, modify, or render any type of resource, you will likely
// do it through this interface.
IDirect3DDevice9 *g_d3d_device=NULL;


//Our presentation parameters.  They get set in our call to dhInitDevice, and we need them
//in case we need to reset our application.
D3DPRESENT_PARAMETERS g_pp;

struct point_vertex{
	float x, y, z, rhw;  // The transformed(screen space) position for the vertex.
	DWORD colour;        // The vertex colour.
};

#define D3DFVF_XYZRHW 0x004
#define D3DFVF_DIFFUSE 0x040

const DWORD point_fvf=D3DFVF_XYZRHW|D3DFVF_DIFFUSE;

point_vertex sine_data[g_width]; //Enough data to plot from one edge to the other
point_vertex random_data[g_width]; //A whole whack of data

bool random_mode=true;


void initialize_data(void){
	int count;
	float count_f;
	unsigned char red,green,blue;
	float y;

	for(count=0;count<g_width;count++){

		count_f=(float)count;

		sine_data[count].x=count_f;
		y=sinf(count_f/10.0f) ;
		sine_data[count].y= y * (g_height/4.0f) + (g_height/2.0f);
		sine_data[count].z=1.0f;
		sine_data[count].rhw=1.0f;

		blue= (char)ceil((count_f/g_width) * 200.0f) +55;
		green=(char)ceil(((g_width-count_f)/g_width) * 200.0f) +55;
		red=(char) ((fabsf(y)* 200.0f) + 55.0f);
		sine_data[count].colour=D3DCOLOR_XRGB(red,green,blue);

	}

	srand(GetTickCount());
	for(count=0;count<g_width;count++){
		random_data[count].x=(float)(rand()%g_width);
		random_data[count].y=(float)(rand()%g_height);
		random_data[count].z=1.0f;
		random_data[count].rhw=1.0f;
		random_data[count].colour=D3DCOLOR_XRGB(rand()%255,rand()%255,rand()%255);
	}

}

/*void preformd3d()
{
	

	data = random_data;
	DbgPrint("Clearing up the d3d buffer...");
	//Clear the buffer to our new colour.
	//g_pDevice->Clear(0,  //Number of rectangles to clear, we're clearing everything so set it to 0
	//	NULL, //Pointer to the rectangles to clear, NULL to clear whole display
	//	D3DCLEAR_TARGET,   //What to clear.  We don't have a Z Buffer or Stencil Buffer
	//	0x00000000, //Colour to clear to (AARRGGBB)
	//	1.0f,  //Value to clear ZBuffer to, doesn't matter since we don't have one
	//	0 );   //Stencil clear value, again, we don't have one, this value doesn't matter

	DbgPrint("g_pDevice: %08X\r\n",g_pDevice);

	
	DbgPrint("Setting FVF\r\n");
	g_pDevice->SetFVF(point_fvf);

	DbgPrint("Drawing..\r\n");
	g_pDevice->DrawPrimitiveUP(D3DPT_POINTLIST,        //PrimitiveType
		g_width,                //PrimitiveCount
		data,                   //pVertexStreamZeroData
		sizeof(point_vertex));  //VertexStreamZeroStride

	//printf("Presenting the captured scene..");
	//g_pDevice->Present(NULL,  //Source rectangle to display, NULL for all of it
	//	NULL,  //Destination rectangle, NULL to fill whole display
	//	NULL,  //Target window, if NULL uses device window set in CreateDevice
	//	NULL );

}*/

HRESULT render(void){
HRESULT hr;
const void *data;

   //Clear the buffer to our new colour.
   hr=g_d3d_device->Clear(0,  //Number of rectangles to clear, we're clearing everything so set it to 0
                          NULL, //Pointer to the rectangles to clear, NULL to clear whole display
                          D3DCLEAR_TARGET,   //What to clear.  We don't have a Z Buffer or Stencil Buffer
                          0x00000000, //Colour to clear to (AARRGGBB)
                          1.0f,  //Value to clear ZBuffer to, doesn't matter since we don't have one
                          0 );   //Stencil clear value, again, we don't have one, this value doesn't matter
   if(FAILED(hr)){
      return hr;
   }

   //Notify the device that we're ready to render
   hr=g_d3d_device->BeginScene();
   if(FAILED(hr)){
      return hr;
   }


   g_d3d_device->SetFVF(point_fvf);

   if (random_mode) {
      data=random_data;
   }else{
      data=sine_data;
   }

   g_d3d_device->DrawPrimitiveUP(D3DPT_POINTLIST,        //PrimitiveType
                                 g_width,                //PrimitiveCount
                                 data,                   //pVertexStreamZeroData
                                 sizeof(point_vertex));  //VertexStreamZeroStride
   
   //Notify the device that we're finished rendering for this frame
   g_d3d_device->EndScene();

   //Show the results
   hr=g_d3d_device->Present(NULL,  //Source rectangle to display, NULL for all of it
                            NULL,  //Destination rectangle, NULL to fill whole display
                            NULL,  //Target window, if NULL uses device window set in CreateDevice
                            NULL );//Unused parameter, set it to NULL

   return hr;
}

VOID __declspec(naked) XuiRenderPresentHooked(VOID)
{
	
	/*
		819545E4 38800000             li r4,0 
		819545E8 807C007C             lwz r3,7Ch(r28) 
		819545EC 4BFC413D             bl CHUDBkgndModule::Render (81918728h) 
		819545F0 38800000             li r4,0 
		819545F4 807D0B0C             lwz r3,0B0Ch(r29) 
	*/
	__asm{
		/* Grab Direct3D Pointer for usage */
		lwz r3,0x0B0C(r29)
		mr g_d3d_device,r3
	}

	if(firstrun == true)
	{
		initialize_data();
		firstrun = false;
	}	

	render();

		DbgPrint("Returning to normal xam code\r\n");
	__asm{
		/* Replace Overwritten Code */
		li r4,0
		lwz r3,0x7C(r28)
		lis r11,0x8191
		ori r11,r11,0x8728
		mtctr r11
		bctrl
		li r4,0
		lwz r3,0x0B0C(r29) 
		
		/* Return back to xam */
		lis r11,0x8195
		ori r11,r11,0x45F4
		mtctr r11
		bctr
	}

}

void PatchCreateDC()
{
	DbgPrint("------ Hook Activated ------\r\n");

	//DbgPrint("---------------------- Present: %08X ------------------------------\r\n",present);
	DWORD Stuff[4];

	/*819545E4*/
	patchInJump((PDWORD)Stuff, (UINT32)XuiRenderPresentHooked, false);
	DmSetMemory( (PDWORD)0x819545E4, 4, &Stuff[0], NULL);
	DmSetMemory( (PDWORD)0x819545E8, 4, &Stuff[1], NULL);
	DmSetMemory( (PDWORD)0x819545EC, 4, &Stuff[2], NULL);
	DmSetMemory( (PDWORD)0x819545F0, 4, &Stuff[3], NULL);
}