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

#if defined( _DEBUG )  
#include "Utils/MemLeaks.h" 
#endif

#define APPLICATION_NAME	"TEST"

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

    ReadXMLInitParams(l_InitParams,"./Data/XML/init_test_particulas.xml");

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

    l_Engine.AddProcess(l_Test);
    l_Engine.AddProcess(l_PhysX);
    l_Engine.AddProcess(l_Particle);
    l_Engine.AddProcess(l_pSoundTestProcess);
    l_Engine.AddProcess(l_pIAProcess);
    l_Engine.AddProcess(l_pEntityProcess);

    //l_Engine.ActivateProcess(l_Test);
    //l_Engine.ActivateProcess(l_pSoundTestProcess);
    //l_Engine.ActivateProcess(l_PhysX);
    l_Engine.ActivateProcess(l_Particle);
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