#include <Windows.h>

#include <Engine.h>

#include "base.h"
#include "TestProcess.h"
#include "Utils/Exception.h"

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

  // Register the window class
  WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

  RegisterClassEx( &wc );

  try {
    // Create the application's window
    HWND hWnd = CreateWindow(	APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, wc.hInstance, NULL );


    // Añadir aquí el Init de la applicación
    CEngine l_engine;
    CProcess* l_Test = new CTestProcess();
    l_engine.SetProcess(l_Test);

    l_engine.Init("", hWnd);


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
         // Main loop: Añadir aquí el Update y Render de la aplicación principal
	       l_engine.Update();
		     l_engine.Render();
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