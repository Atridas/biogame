#include <Windows.h>

#include <Engine.h>
#include <params.h>

#include "base.h"
#include "TestProcess.h"
#include "PhysXProcess.h"
#include "Utils/Exception.h"
#include "ParticleProcess.h"
#include "SoundTestProcess.h"
#include "IAProcess.h"
#include "EntityProcess.h"
#include "ParticleViewerProcess.h"
#include "Biotest.h"

#if defined( _DEBUG )  
#include "Utils/MemLeaks.h" 
#endif

//#define __T_PARTICULES__
//#define __T_PYSX__
#define __T_ENTITIES__
//#define __T_BIOTEST__
//#define __T_PARTICLE_VIEWER__



/*
                                               `:/+osyhhhhhhhyyss+:.`                               
                                        `:+o+sdhs+/:..`     ```.-/oyhdyo:`                          
                                     `+hddhs+-`        `            ``-/ohdy/.                      
                                  `:yds-              :+            :o`  -::ohds:`                  
                                .ody:`                .:.        `   /h` //:  `:yhy+.               
                              :ydo.                        -.   .-`   sy  ./.    `-odh/`            
                            `ym/`                                `::  `h/ .s/+/`    `-sdy-          
                            sm.                                    ./. -s `+```:+/.    `/ds.        
                           +N:                                ..     //`o`   -+:`        `sm/       
                          -No                          .:://:.`+/`    .:::`./.             :ms`     
                         `dh`                          `..--./+-:ds:`   .+/+       `        .hh.    
                         sm.                         -+::/:-.--:/-hNms:``+s+    `-/+         `hd`   
                        /N:                         `s``dNNy` .-.:`ys+/--``-:/ydhys+/:-.`     .hh`  
                       `my                           /` omdo..`: ``smo:``--+-:yy.-:-:-::--`    .ms  
                       +N-                           `---:+--:/``-oo//s:--.:./y-o-/:./+:-:`     +N- 
                      `my                           .://////---:::.` `:-``-+--+-o:``hMMd-/      `dy 
                      +N-                          `..--:::-..`` .-`   `   -`.-o/:-.:so/-:       /N-
                     `my                           .+/` ```----//-`.           +s---:--.`        `ms
                     `N+                            ``  .:+o+/-.   :o:--.```   `s:.-...`          hh
                     `N+                                           `hyshso/...``/+.  ``           hh
                     `N+                                           so/-:/:+s+::-`-h/              hh
                     `N+                                          `d/---.``:.`./`-o+`             hh
                     `No                                          /+-.-/oo/`   `  `/`             my
                      dd                      .                  .s```` `.s+.`   `:+             .N+
                      +N.             -.      .                  //-:++/. -ddyyyhd:              om.
                      .mo             ..             .-         `+dmNNds+::y/:/yho`             `dy 
                       dh                            ``         `:ymho....:///-:hh+.            -N/ 
                       oN.                         `            `++/.:+os/oyds/--ms:            oN` 
                       `Ny                         `            `s.-/++s:::/++/o y/`           `my  
                        +N:                  `/.                +h+hhhoy:/+-o/o-/m:            /N-  
                         sm:                  .`               -s+ysNmdhysyyoy/ooy:           -mo   
                          om/`                                .:.-/-+o:///+ys/so`.-`         -dy`   
                           /dy/.`                             .  ```.:-:-:::/ .o.`         `od+`    
                           .+mmdh:`                      -  `::/+/+/.`  ```.` -:         `/hy-      
                         .ods:`./yhs:`                 `+: `:-oo+///oos+/:---:+s`      .+hs:        
                       .sds-      ./yhs/.`          ``/s:  :o:`       `.-::::. so  `./shs-          
                    `-ydo.  +o-      `:ohho/-.`   .:os:`  `+.                  -Nosyyo:`            
                  ./hh/`    -+ms.        .:+syysosyh/.``   ``-            `.-+shy+-`                
                -sds-`        .yd+`           `-/osyyyyyyssooooo++///++osyyhs+-`                    
             ./yh+.             -hd:`                 ``.--::/hNy+/////:-.`             `.    //    
           .ods:`                `/dh-`                     -hmmo-        `             -ds-..ms./  
          +mo.                     `+dh:                  `omo`-ody:` .y. yo` -`         .mmddNhdo  
        .yd:                         `+dy-               `hm-    `/hh+:my`oM.`N+       .+dy. .--`   
       :dy`                            `+dy/`           -dd.    ``-+hNmNNdNNyhd:-   `/ydo.          
      +N/                                `/hd/`  ``...-oNdssssyhhhyo/..omy/oddydy./yds:`            
    :sm/                                    :dhshhhyshMh+/:---.``       .sms:-/yhds-                
  -yd/`                                      `/:.   -ms`                  `+ddy+.                   
.yd/`                                              `hy                                              
yh`                                               .dh`                                              
-`                                               -my`                                               
`                                               -ms            
*/



#ifdef __T_PARTICULES__
#ifdef __T_PYSX__
#error "Definit particules i pysx a la vegada"
#endif
#ifdef __T_ENTITIES__
#error "Definit particules i entities a la vegada"
#endif
#ifdef __T_BIOTEST__
#error "Definit particules i biotest a la vegada"
#endif
#ifdef __T_PARTICLE_VIEWER__
#error "Definit particules i particle viewer a la vegada"
#endif
#endif

#ifdef __T_PYSX__
#ifdef __T_ENTITIES__
#error "Definit pysx i entities a la vegada"
#endif
#ifdef __T_BIOTEST__
#error "Definit pysx i biotest a la vegada"
#endif
#ifdef __T_PARTICLE_VIEWER__
#error "Definit pysx i particle viewer a la vegada"
#endif
#endif

#ifdef __T_ENTITIES__
#ifdef __T_BIOTEST__
#error "Definit entities i biotest a la vegada"
#endif
#ifdef __T_PARTICLE_VIEWER__
#error "Definit entities i particle viewer a la vegada"
#endif
#endif

#ifdef __T_BIOTEST__
#ifdef __T_PARTICLE_VIEWER__
#error "Definit biotest i particle viewer a la vegada"
#endif
#endif




#ifdef __T_PYSX__
#define APPLICATION_NAME	"TEST PhysX"
#endif
#ifdef __T_PARTICULES__
#define APPLICATION_NAME	"TEST Particulas"
#endif
#ifdef __T_ENTITIES__
#define APPLICATION_NAME	"TEST Entities"
#endif
#ifdef __T_BIOTEST__
#define APPLICATION_NAME	"BioTEST"
#endif
#ifdef __T_PARTICLE_VIEWER__
#define APPLICATION_NAME	"Particle Viewer"
#endif

#ifndef APPLICATION_NAME
#define APPLICATION_NAME	"TEST"
#endif

void ShowErrorMessage (const std::string& message);


//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

  switch( msg )
  {
  case WM_DESTROY:
    {
      PostQuitMessage( 0 );
      return 0;
    }
    break;
  case WM_KEYDOWN:
    {
      switch( wParam )
      {
      case VK_ESCAPE:
        //Cleanup();
        PostQuitMessage( 0 );
        return 0;
        break;
      }
    }
    break;
  }//end switch( msg )

  return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
  #if defined( _DEBUG )
    MemLeaks::MemoryBegin();
  #endif //defined(_DEBUG)

  CLogger* l_pLogger = new CLogger();

  // Register the window class
  WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

  RegisterClassEx( &wc );

  try {
    CEngine l_Engine;
    SInitParams l_InitParams;

#ifdef __T_PYSX__
    ReadXMLInitParams(l_InitParams,"./Data/XML/init_test_physx.xml");
#endif
#ifdef __T_PARTICULES__
    ReadXMLInitParams(l_InitParams,"./Data/XML/init_test_particulas.xml");
#endif
#ifdef __T_ENTITIES__
    ReadXMLInitParams(l_InitParams,"./Data/XML/init_test_entities.xml");
#endif
#ifdef __T_BIOTEST__
    ReadXMLInitParams(l_InitParams,"./Data/XML/init.xml");
#endif
#ifdef __T_PARTICLE_VIEWER__
    ReadXMLInitParams(l_InitParams,"./Data/XML/init_test_entities.xml");
#endif

    // Create the application's window
    HWND hWnd = CreateWindow(	
                    APPLICATION_NAME, 
                    APPLICATION_NAME, 
                    WS_OVERLAPPEDWINDOW, 
                    l_InitParams.RenderManagerParams.v2iPosition.x, 
                    l_InitParams.RenderManagerParams.v2iPosition.y, 
                    l_InitParams.RenderManagerParams.v2iResolution.x,
                    l_InitParams.RenderManagerParams.v2iResolution.y,
                    NULL, NULL, wc.hInstance, NULL );


    // Añadir aquí el Init de la applicación
    CProcess* l_Test = new CTestProcess();
    CPhysXProcess* l_PhysX = new CPhysXProcess();
    CParticleProcess* l_Particle = new CParticleProcess();
    CSoundTestProcess* l_pSoundTestProcess = new CSoundTestProcess();
    CIAProcess* l_pIAProcess = new CIAProcess();
    CEntityProcess* l_pEntityProcess = new CEntityProcess();
    CBiotestProcess* l_pBiotestProcess = new CBiotestProcess();
    CParticleViewerProcess* l_pParticleViewerProcess = new CParticleViewerProcess();

    l_Engine.AddProcess(l_Test);
    l_Engine.AddProcess(l_PhysX);
    l_Engine.AddProcess(l_Particle);
    l_Engine.AddProcess(l_pSoundTestProcess);
    l_Engine.AddProcess(l_pIAProcess);
    l_Engine.AddProcess(l_pEntityProcess);
    l_Engine.AddProcess(l_pBiotestProcess);
    l_Engine.AddProcess(l_pParticleViewerProcess);


#ifdef __T_PYSX__
    l_Engine.ActivateProcess(l_PhysX);
#endif
#ifdef __T_PARTICULES__
    l_Engine.ActivateProcess(l_Particle);
#endif
#ifdef __T_ENTITIES__
    l_Engine.ActivateProcess(l_pEntityProcess);
#endif
#ifdef __T_BIOTEST__
    l_Engine.ActivateProcess(l_pBiotestProcess);
#endif
#ifdef __T_PARTICLE_VIEWER__
    l_Engine.ActivateProcess(l_pParticleViewerProcess);
#endif

    //l_Engine.ActivateProcess(l_Test);
    //l_Engine.ActivateProcess(l_pSoundTestProcess);
    //l_Engine.ActivateProcess(l_PhysX);
    //l_Engine.ActivateProcess(l_Particle);
    //l_Engine.ActivateProcess(l_pIAProcess);
    //l_Engine.ActivateProcess(l_pEntityProcess);


    l_Engine.Init(l_InitParams, hWnd);


    ShowWindow( hWnd, SW_SHOWDEFAULT );
    UpdateWindow( hWnd );
    MSG msg;
    ZeroMemory( &msg, sizeof(msg) );

    // Añadir en el while la condición de salida del programa de la aplicación

    while( msg.message != WM_QUIT )
    {
      if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
      {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
      }
      else
      {
			l_Engine.Update();
		    l_Engine.Render();

      }
    }
  } catch(CException& e)
  {
    ShowErrorMessage(e.GetDescription());
  } catch (...)   
  {

    ShowErrorMessage("Exception Occured");

  }
  UnregisterClass( APPLICATION_NAME, wc.hInstance );

  // Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos

  CHECKED_DELETE(l_pLogger)
  return 0;
}

void ShowErrorMessage (const std::string& message)
{
  bool logSaved = false;
  logSaved = LOGGER->SaveLogsInFile();
  std::string end_message = "";
  if (logSaved)    
  {
    end_message += "Sorry, Application failed. Logs saved\n";
  } else
  {    
    end_message += "Sorry, Application failed. Logs could not be saved\n";
  }
  end_message += message;
  MessageBox(0, end_message.c_str(), "FlostiProject Report", MB_OK | MB_ICONERROR);
}