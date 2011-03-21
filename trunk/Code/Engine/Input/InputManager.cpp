#include <base.h>
#include "InputManager.h"
#include "GamePad.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <Utils/Logger.h>
#include <Utils/Exception.h>
//#include "Script/ScriptManager.h"
//#include "luabind/luabind.hpp"



//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CInputManager::Done ()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CInputManager::Release () 
{
	LOGGER->AddNewLog(ELL_INFORMATION, "InputManager:: shutting down DirectInput");

	CHECKED_DELETE(m_pKB);
	CHECKED_DELETE(m_pMouse);
	CHECKED_DELETE(m_pGamePad);
	CHECKED_RELEASE(m_pDI);

	LOGGER->AddNewLog(ELL_INFORMATION, "InputManager:: offline (ok)");
}


//----------------------------------------------------------------------------
// Initializes at least keyboard and mouse, otherwise returns error.
// If present joystick will also be initialized, but is not mandatory.
//-> IN: HWND      - handle to main application window
//----------------------------------------------------------------------------
bool CInputManager::Init (HWND hWnd, const Vect2i& screenRes, bool exclusiveModeinMouse)
{
	HRESULT hr;
	m_hWndMain = hWnd;

	LOGGER->AddNewLog(ELL_INFORMATION, "InputManager:: calling initialization");

	// create main DirectInput object
	m_bIsOk = !FAILED (hr = DirectInput8Create(	GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&m_pDI, NULL));
	if (m_bIsOk)
	{
		// create all input device objects
		m_pKB				= new CKeyboard;
		m_pMouse		= new CMouse;
		m_pGamePad	= new CGamePad;

		// initialize all input device objects
		m_bIsOk = m_pKB->Init(m_pDI, hWnd);
		if (m_bIsOk)
		{
			m_bIsOk	= m_pMouse->Init(m_pDI, hWnd, screenRes, exclusiveModeinMouse);

			if (m_bIsOk)
			{
        m_bIsOk = m_pGamePad->Init(screenRes);
        
        if(m_bIsOk)
        {
				  m_pGamePad->Update();
				  if (m_pGamePad->IsConnected() )
				  {
					  LOGGER->AddNewLog(ELL_INFORMATION, "InputManager:: GamePad is connected");
				  }
				  else
				  {
					  LOGGER->AddNewLog(ELL_INFORMATION, "InputManager:: GamePad is not connected");
				  }
        }
			}

		}
	}

	if (!m_bIsOk)
	{
		Release();
		std::string msg_error = "Error al inicializar directinput";
		throw CException(__FILE__, __LINE__, msg_error);
	}

  InitString2Input();

	return m_bIsOk;
} 



//----------------------------------------------------------------------------
// Query if a Joystick is active and ready or not. Also returns
// Joytsicks name if any was found.
//----------------------------------------------------------------------------
bool CInputManager::HasGamePad (INPUT_DEVICE_TYPE device) const
{
	
	switch (device)
	{
		case IDV_GAMEPAD1:
			return m_pGamePad->IsConnected(0);
			break;
		case IDV_GAMEPAD2:
			return m_pGamePad->IsConnected(1);
			break;
		case IDV_GAMEPAD3:
      return m_pGamePad->IsConnected(2);
	    break;
		case IDV_GAMEPAD4:
      return m_pGamePad->IsConnected(3);
	    break;
		default:
			return false;
	}
} 



//----------------------------------------------------------------------------
// Update all input devices
//----------------------------------------------------------------------------
HRESULT CInputManager::Update(void)
{
	HRESULT hr;

	if (!IsOk()) return E_FAIL;

	if (m_pKB) 
	{
		if ( FAILED( hr=m_pKB->Update() ) )
			return hr;
	}

	if (m_pMouse) 
	{
		if ( FAILED( hr=m_pMouse->Update() ) )
			return hr;
	}

	if (m_pGamePad) 
	{
		m_pGamePad->Update();
	}

	return S_OK;
}

//----------------------------------------------------------------------------
// If mouse or joystick return current position.
//----------------------------------------------------------------------------
HRESULT CInputManager::GetPosition( INPUT_DEVICE_TYPE idType, Vect2i& pos) 
{
	switch (idType)
	{
		case IDV_MOUSE:
			pos = m_pMouse->GetPosition();
			return S_OK;
			break;
		case IDV_GAMEPAD1:
			m_pGamePad->GetPosition(pos,0);
			return true;
			break;
		case IDV_GAMEPAD2:
			m_pGamePad->GetPosition(pos,1);
			return true;
			break;
		case IDV_GAMEPAD3:
      m_pGamePad->GetPosition(pos,2);
			return true;
	    break;
		case IDV_GAMEPAD4:
      m_pGamePad->GetPosition(pos,3);
			return true;
			break;
		default:
			return E_FAIL;
	}
} 


//----------------------------------------------------------------------------
// If GamePad return current intensity on axes.
//----------------------------------------------------------------------------
bool CInputManager::GetGamePadLeftThumbDeflection(float *pfX, float *pfY, INPUT_DEVICE_TYPE device)
{
	switch (device)
	{
		case IDV_GAMEPAD1:
			return m_pGamePad->GetLeftThumbDeflection(pfX,pfY,0);
			break;
		case IDV_GAMEPAD2:
			return m_pGamePad->GetLeftThumbDeflection(pfX,pfY,1);
			break;
		case IDV_GAMEPAD3:
      return m_pGamePad->GetLeftThumbDeflection(pfX,pfY,2);
	    break;
		case IDV_GAMEPAD4:
      return m_pGamePad->GetLeftThumbDeflection(pfX,pfY,3);
	    break;
		default:
			return false;
	}  
}

bool CInputManager::GetGamePadRightThumbDeflection(float *pfX, float *pfY, INPUT_DEVICE_TYPE device)
{
	switch (device)
	{
		case IDV_GAMEPAD1:
			return m_pGamePad->GetRightThumbDeflection(pfX,pfY,0);
			break;
		case IDV_GAMEPAD2:
			return m_pGamePad->GetRightThumbDeflection(pfX,pfY,1);
			break;
		case IDV_GAMEPAD3:
      return m_pGamePad->GetRightThumbDeflection(pfX,pfY,2);
	    break;
		case IDV_GAMEPAD4:
      return m_pGamePad->GetRightThumbDeflection(pfX,pfY,3);
	    break;
		default:
			return false;
	}
}

bool CInputManager::GetGamePadDeltaTriggers(float *pfLeft, float *pfRight, INPUT_DEVICE_TYPE device)
{
	switch (device)
	{
		case IDV_GAMEPAD1:
			return m_pGamePad->GetDeltaTriggers(pfLeft,pfRight,0);
			break;
		case IDV_GAMEPAD2:
			return m_pGamePad->GetDeltaTriggers(pfLeft,pfRight,1);
			break;
		case IDV_GAMEPAD3:
      return m_pGamePad->GetDeltaTriggers(pfLeft,pfRight,2);
	    break;
		case IDV_GAMEPAD4:
      return m_pGamePad->GetDeltaTriggers(pfLeft,pfRight,3);
	    break;
		default:
			return false;
	}
}

void	CInputManager::SetGamePadLeftMotorSpeed (uint32 speed, INPUT_DEVICE_TYPE device)
{
	switch (device)
	{
		case IDV_GAMEPAD1:
			m_pGamePad->SetLeftMotorSpeed(speed,0);
			break;
		case IDV_GAMEPAD2:
			m_pGamePad->SetLeftMotorSpeed(speed,1);
			break;
		case IDV_GAMEPAD3:
      m_pGamePad->SetLeftMotorSpeed(speed,2);
	    break;
		case IDV_GAMEPAD4:
      m_pGamePad->SetLeftMotorSpeed(speed,3);
	    break;
	}
}

void	CInputManager::SetGamePadRightMotorSpeed (uint32 speed, INPUT_DEVICE_TYPE device)
{
	switch (device)
	{
		case IDV_GAMEPAD1:
			m_pGamePad->SetRightMotorSpeed(speed,0);
			break;
		case IDV_GAMEPAD2:
			m_pGamePad->SetRightMotorSpeed(speed,1);
			break;
		case IDV_GAMEPAD3:
      m_pGamePad->SetRightMotorSpeed(speed,2);
	    break;
		case IDV_GAMEPAD4:
      m_pGamePad->SetRightMotorSpeed(speed,3);
	    break;
	}
}

//----------------------------------------------------------------------------
// Return the change of mouse cursor since last call to Update().
//----------------------------------------------------------------------------
const Vect3i& CInputManager::GetMouseDelta(void) 
{ 
	return m_pMouse->GetMouseDelta();
}


//----------------------------------------------------------------------------
// Ask about button state.
//----------------------------------------------------------------------------
bool CInputManager::IsDown(INPUT_DEVICE_TYPE idType, uint32 nBtn)
{
	bool result = false;

  switch(idType)
  {
    case IDV_MOUSE:
      result = m_pMouse->IsDown(nBtn);
      break;
    case IDV_KEYBOARD:
      result = m_pKB->IsDown(nBtn);
      break;
		case IDV_GAMEPAD1:
			result = m_pGamePad->IsDown(nBtn,0);
			break;
		case IDV_GAMEPAD2:
			result = m_pGamePad->IsDown(nBtn,1);
			break;
		case IDV_GAMEPAD3:
      result =  m_pGamePad->IsDown(nBtn,2);
	    break;
		case IDV_GAMEPAD4:
      result = m_pGamePad->IsDown(nBtn,3);
	    break;
  }

	return result;
}

//----------------------------------------------------------------------------
// Ask about button/key state for mouse, joytsick or keyboard.
//----------------------------------------------------------------------------
bool CInputManager::IsDownUp(INPUT_DEVICE_TYPE idType, uint32 nBtn)
{
	 bool result = false;

  switch(idType)
  {
    case IDV_MOUSE:
      result = m_pMouse->IsDownUp(nBtn);
      break;
    case IDV_KEYBOARD:
      result = m_pKB->IsDownUp(nBtn);
      break;
    case IDV_GAMEPAD1:
			result = m_pGamePad->IsDownUp(nBtn,0);
			break;
		case IDV_GAMEPAD2:
			result = m_pGamePad->IsDownUp(nBtn,1);
			break;
		case IDV_GAMEPAD3:
      result = m_pGamePad->IsDownUp(nBtn,2);
	    break;
		case IDV_GAMEPAD4:
      result = m_pGamePad->IsDownUp(nBtn,3);
	    break;
  }

	return result;
} 

//----------------------------------------------------------------------------
// Ask about button/key state for mouse, joytsick or keyboard.
//----------------------------------------------------------------------------
bool CInputManager::IsUpDown(INPUT_DEVICE_TYPE idType, uint32 nBtn)
{
	 bool result = false;

  switch(idType)
  {
    case IDV_MOUSE:
      result = m_pMouse->IsUpDown(nBtn);
      break;
    case IDV_KEYBOARD:
      result = m_pKB->IsUpDown(nBtn);
      break;
    case IDV_GAMEPAD1:
			result = m_pGamePad->IsUpDown(nBtn,0);
			break;
		case IDV_GAMEPAD2:
			result = m_pGamePad->IsUpDown(nBtn,1);
			break;
		case IDV_GAMEPAD3:
      result = m_pGamePad->IsUpDown(nBtn,2);
	    break;
		case IDV_GAMEPAD4:
      result = m_pGamePad->IsUpDown(nBtn,3);
	    break;
  }

	return result;
} 


int32 CInputManager::Scan2ascii (uint32 scancode, uint16* result)
{
   static HKL layout=GetKeyboardLayout(0);
   static uint8 State[256];

   if (GetKeyboardState(State)==FALSE)
      return 0;
   uint32 vk=MapVirtualKeyEx(scancode,1,layout);
   return ToAsciiEx(vk,scancode,State,result,0,layout);
}

void CInputManager::InitString2Input ()
{
  typedef std::pair<std::string, uint32> PairString2Code;

  //----------------INPUT_DEVICE_TYPE---------------//
  m_String2Code.insert( PairString2Code("IDV_KEYBOARD", IDV_KEYBOARD) );
  m_String2Code.insert( PairString2Code("IDV_MOUSE", IDV_MOUSE) );
  m_String2Code.insert( PairString2Code("IDV_GAMEPAD1", IDV_GAMEPAD1) );
  m_String2Code.insert( PairString2Code("IDV_GAMEPAD2", IDV_GAMEPAD2) );
  m_String2Code.insert( PairString2Code("IDV_GAMEPAD3", IDV_GAMEPAD3) );
  m_String2Code.insert( PairString2Code("IDV_GAMEPAD4", IDV_GAMEPAD4) );
  m_String2Code.insert( PairString2Code("IDV_NOTHING", IDV_NOTHING) );

  //----------------INPUT_AXIS_TYPE-----------------//
  m_String2Code.insert( PairString2Code("AXIS_MOUSE_X", AXIS_MOUSE_X) );
  m_String2Code.insert( PairString2Code("AXIS_MOUSE_Y", AXIS_MOUSE_Y) );
  m_String2Code.insert( PairString2Code("AXIS_MOUSE_Z", AXIS_MOUSE_Z) );
  m_String2Code.insert( PairString2Code("AXIS_LEFT_THUMB_X", AXIS_LEFT_THUMB_X) );
  m_String2Code.insert( PairString2Code("AXIS_LEFT_THUMB_Y", AXIS_LEFT_THUMB_Y) );
  m_String2Code.insert( PairString2Code("AXIS_RIGHT_THUMB_X", AXIS_RIGHT_THUMB_X) );
  m_String2Code.insert( PairString2Code("AXIS_RIGHT_THUMB_Y", AXIS_RIGHT_THUMB_Y) );
  m_String2Code.insert( PairString2Code("AXIS_DELTA_TRIGGER_RIGHT", AXIS_DELTA_TRIGGER_RIGHT) );
  m_String2Code.insert( PairString2Code("AXIS_DELTA_TRIGGER_LEFT", AXIS_DELTA_TRIGGER_LEFT) );
  m_String2Code.insert( PairString2Code("AXIS_NOTHING", AXIS_NOTHING) );

  //----------------INPUT_EVENT_TYPE-----------------//
  m_String2Code.insert( PairString2Code("EVENT_DOWN", EVENT_DOWN) );
  m_String2Code.insert( PairString2Code("EVENT_UP_DOWN", EVENT_UP_DOWN) );
  m_String2Code.insert( PairString2Code("EVENT_DOWN_UP", EVENT_DOWN_UP) );
  m_String2Code.insert( PairString2Code("EVENT_NOTHING", EVENT_NOTHING) );

  //----------------MOUSE_BUTTON---------------------//
  m_String2Code.insert( PairString2Code("MOUSE_BUTTON_LEFT", MOUSE_BUTTON_LEFT) );
  m_String2Code.insert( PairString2Code("MOUSE_BUTTON_RIGHT", MOUSE_BUTTON_RIGHT) );
  m_String2Code.insert( PairString2Code("MOUSE_BUTTON_MIDDLE", MOUSE_BUTTON_MIDDLE) );
  m_String2Code.insert( PairString2Code("MOUSE_BUTTON_MIDDLE", MOUSE_BUTTON_MIDDLE) );
  m_String2Code.insert( PairString2Code("MOUSE_BUTTON_NOTHING", MOUSE_BUTTON_NOTHING) );


  //----------------Gamepad codes from XInput.h------//
  m_String2Code.insert( PairString2Code("PAD_DPAD_UP", PAD_DPAD_UP) );
  m_String2Code.insert( PairString2Code("PAD_DPAD_DOWN", PAD_DPAD_DOWN) );
  m_String2Code.insert( PairString2Code("PAD_DPAD_LEFT", PAD_DPAD_LEFT) );
  m_String2Code.insert( PairString2Code("PAD_DPAD_RIGHT", PAD_DPAD_RIGHT) );
  m_String2Code.insert( PairString2Code("PAD_DPAD_START", PAD_DPAD_START) );
  m_String2Code.insert( PairString2Code("PAD_DPAD_BACK", PAD_DPAD_BACK) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_LEFT_THUMB", PAD_BUTTON_LEFT_THUMB) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_RIGHT_THUMB", PAD_BUTTON_RIGHT_THUMB) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_LEFT_SHOULDER", PAD_BUTTON_LEFT_SHOULDER) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_RIGHT_SHOULDER",PAD_BUTTON_RIGHT_SHOULDER) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_A", PAD_BUTTON_A) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_B", PAD_BUTTON_B) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_X", PAD_BUTTON_X) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_Y", PAD_BUTTON_Y) );

  //-----------Keyboard scan codes from dinput.h ------//
  m_String2Code.insert( PairString2Code("KEY_ESCAPE", KEY_ESCAPE) );
  m_String2Code.insert( PairString2Code("KEY_TAB", KEY_TAB) );
  m_String2Code.insert( PairString2Code("KEY_SPACE", KEY_SPACE) );
  m_String2Code.insert( PairString2Code("KEY_RETURN", KEY_RETURN) );
  m_String2Code.insert( PairString2Code("KEY_BACK", KEY_BACK) );
  m_String2Code.insert( PairString2Code("KEY_CAPITAL", KEY_CAPITAL) );
  m_String2Code.insert( PairString2Code("KEY_MINUS", KEY_MINUS) );
  m_String2Code.insert( PairString2Code("KEY_EQUALS", KEY_EQUALS) );
  m_String2Code.insert( PairString2Code("KEY_LBRACKET", KEY_LBRACKET) );
  m_String2Code.insert( PairString2Code("KEY_RBRACKET", KEY_RBRACKET) );
  m_String2Code.insert( PairString2Code("KEY_SEMICOLON", KEY_SEMICOLON) );
  m_String2Code.insert( PairString2Code("KEY_APOSTROPHE", KEY_APOSTROPHE) );
  m_String2Code.insert( PairString2Code("KEY_GRAVE", KEY_GRAVE) );
  m_String2Code.insert( PairString2Code("KEY_BACKSLASH", KEY_BACKSLASH) );
  m_String2Code.insert( PairString2Code("KEY_COMMA", KEY_COMMA) );
  m_String2Code.insert( PairString2Code("KEY_PERIOD", KEY_PERIOD) );
  m_String2Code.insert( PairString2Code("KEY_SLASH", KEY_SLASH) );
  m_String2Code.insert( PairString2Code("KEY_A", KEY_A) );
  m_String2Code.insert( PairString2Code("KEY_S", KEY_S) );
  m_String2Code.insert( PairString2Code("KEY_D", KEY_D) );
  m_String2Code.insert( PairString2Code("KEY_F", KEY_F) );
  m_String2Code.insert( PairString2Code("KEY_G", KEY_G) );
  m_String2Code.insert( PairString2Code("KEY_H", KEY_H) );
  m_String2Code.insert( PairString2Code("KEY_J", KEY_J) );
  m_String2Code.insert( PairString2Code("KEY_K", KEY_K) );
  m_String2Code.insert( PairString2Code("KEY_L", KEY_L) );
  m_String2Code.insert( PairString2Code("KEY_X", KEY_X) );
  m_String2Code.insert( PairString2Code("KEY_C", KEY_C) );
  m_String2Code.insert( PairString2Code("KEY_B", KEY_B) );
  m_String2Code.insert( PairString2Code("KEY_N", KEY_N) );
  m_String2Code.insert( PairString2Code("KEY_M", KEY_M) );
  m_String2Code.insert( PairString2Code("KEY_Q", KEY_Q) );
  m_String2Code.insert( PairString2Code("KEY_W", KEY_W) );
  m_String2Code.insert( PairString2Code("KEY_E", KEY_E) );
  m_String2Code.insert( PairString2Code("KEY_R", KEY_R) );
  m_String2Code.insert( PairString2Code("KEY_T", KEY_T) );
  m_String2Code.insert( PairString2Code("KEY_Y", KEY_Y) );
  m_String2Code.insert( PairString2Code("KEY_U", KEY_U) );
  m_String2Code.insert( PairString2Code("KEY_I", KEY_I) );
  m_String2Code.insert( PairString2Code("KEY_O", KEY_O) );
  m_String2Code.insert( PairString2Code("KEY_P", KEY_P) );
  m_String2Code.insert( PairString2Code("KEY_1", KEY_1) );
  m_String2Code.insert( PairString2Code("KEY_2", KEY_2) );
  m_String2Code.insert( PairString2Code("KEY_3", KEY_3) );
  m_String2Code.insert( PairString2Code("KEY_4", KEY_4) );
  m_String2Code.insert( PairString2Code("KEY_5", KEY_5) );
  m_String2Code.insert( PairString2Code("KEY_6", KEY_6) );
  m_String2Code.insert( PairString2Code("KEY_7", KEY_7) );
  m_String2Code.insert( PairString2Code("KEY_8", KEY_8) );
  m_String2Code.insert( PairString2Code("KEY_9", KEY_9) );
  m_String2Code.insert( PairString2Code("KEY_0", KEY_0) );
  m_String2Code.insert( PairString2Code("KEY_F1", KEY_F1) );
  m_String2Code.insert( PairString2Code("KEY_F2", KEY_F2) );
  m_String2Code.insert( PairString2Code("KEY_F3", KEY_F3) );
  m_String2Code.insert( PairString2Code("KEY_F4", KEY_F4) );
  m_String2Code.insert( PairString2Code("KEY_F5", KEY_F5) );
  m_String2Code.insert( PairString2Code("KEY_F6", KEY_F6) );
  m_String2Code.insert( PairString2Code("KEY_F7", KEY_F7) );
  m_String2Code.insert( PairString2Code("KEY_F8", KEY_F8) );
  m_String2Code.insert( PairString2Code("KEY_F9", KEY_F9) );
  m_String2Code.insert( PairString2Code("KEY_F10", KEY_F10) );
  m_String2Code.insert( PairString2Code("KEY_F11", KEY_F11) );
  m_String2Code.insert( PairString2Code("KEY_F12", KEY_F12) );
  m_String2Code.insert( PairString2Code("KEY_NPPLUS", KEY_NPPLUS) );
  m_String2Code.insert( PairString2Code("KEY_NPMINUS", KEY_NPMINUS) );
  m_String2Code.insert( PairString2Code("KEY_NPDECIMAL", KEY_NPDECIMAL) );
  m_String2Code.insert( PairString2Code("KEY_NPCOMMA", KEY_NPCOMMA) );
  m_String2Code.insert( PairString2Code("KEY_NPDIVIDE", KEY_NPDIVIDE) );
  m_String2Code.insert( PairString2Code("KEY_NPMULTIPLY", KEY_NPMULTIPLY) );
  m_String2Code.insert( PairString2Code("KEY_NPENTER", KEY_NPENTER) );
  m_String2Code.insert( PairString2Code("KEY_NUMLOCK", KEY_NUMLOCK) );
  m_String2Code.insert( PairString2Code("KEY_NP1", KEY_NP1) );
  m_String2Code.insert( PairString2Code("KEY_NP2", KEY_NP2) );
  m_String2Code.insert( PairString2Code("KEY_NP3", KEY_NP3) );
  m_String2Code.insert( PairString2Code("KEY_NP4", KEY_NP4) );
  m_String2Code.insert( PairString2Code("KEY_NP5", KEY_NP5) );
  m_String2Code.insert( PairString2Code("KEY_NP6", KEY_NP6) );
  m_String2Code.insert( PairString2Code("KEY_NP7", KEY_NP7) );
  m_String2Code.insert( PairString2Code("KEY_NP8", KEY_NP8) );
  m_String2Code.insert( PairString2Code("KEY_NP9", KEY_NP9) );
  m_String2Code.insert( PairString2Code("KEY_NP0", KEY_NP0) );
  m_String2Code.insert( PairString2Code("KEY_RSHIFT", KEY_RSHIFT) );
  m_String2Code.insert( PairString2Code("KEY_LSHIFT", KEY_LSHIFT) );
  m_String2Code.insert( PairString2Code("KEY_RCTRL", KEY_RCTRL) );
  m_String2Code.insert( PairString2Code("KEY_LCTRL", KEY_LCTRL) );
  m_String2Code.insert( PairString2Code("KEY_RALT", KEY_RALT) );
  m_String2Code.insert( PairString2Code("KEY_LALT", KEY_LALT) );
  m_String2Code.insert( PairString2Code("KEY_LWIN", KEY_LWIN) );
  m_String2Code.insert( PairString2Code("KEY_RWIN", KEY_RWIN) );
  m_String2Code.insert( PairString2Code("KEY_UP", KEY_UP) );
  m_String2Code.insert( PairString2Code("KEY_DOWN", KEY_DOWN) );
  m_String2Code.insert( PairString2Code("KEY_LEFT", KEY_LEFT) );
  m_String2Code.insert( PairString2Code("KEY_RIGHT", KEY_RIGHT) );
  m_String2Code.insert( PairString2Code("KEY_INSERT", KEY_INSERT) );
  m_String2Code.insert( PairString2Code("KEY_DELETE", KEY_DELETE) );
  m_String2Code.insert( PairString2Code("KEY_HOME", KEY_HOME) );
  m_String2Code.insert( PairString2Code("KEY_END", KEY_END) );
  m_String2Code.insert( PairString2Code("KEY_PGDOWN", KEY_PGDOWN) );
  m_String2Code.insert( PairString2Code("KEY_PGUP", KEY_PGUP) );
  m_String2Code.insert( PairString2Code("KEY_PAUSE", KEY_PAUSE) );
  m_String2Code.insert( PairString2Code("KEY_SCROLL", KEY_SCROLL) );
  m_String2Code.insert( PairString2Code("KEY_NULL", KEY_NULL) );
}

//----------------------------------------------------------------------------
// LUA BIND
//----------------------------------------------------------------------------
//void CInputManager::RegisterFunctions (CScriptManager* scriptManager)
//{
//	lua_State* l_pLUAState = scriptManager->GetLuaState();
//
//	using namespace luabind;
//
//	// ahora registramos lo que querramos
//	module(l_pLUAState)
//		[
//			// registramos la clase CInputManager
//			class_<CInputManager>(CScriptRegister::SetClassName("CInputManager"))
//			// registramos su constructor
//			.def(constructor<>())
//
//			//-----------GENERAL FUNCTIONS---------------------
//			.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)
//
//			.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
//						"Muestra todas las funciones de esta clase"),
//						&CScriptRegister::Help)
//
//			.def(	CScriptRegister::PushFunctionName("isDown","bool","INPUT_DEVICE_TYPE idType, uint32 nBtn", 
//						"Informa si se esta presionando un boton de un disp. de entrada (0:keyb,1:joys,2:mouse)"),
//						&CInputManager::IsDown)
//
//			.def(	CScriptRegister::PushFunctionName("isDownUp","bool","INPUT_DEVICE_TYPE idType, uint32 nBtn", 
//						"Informa si se justo ahora se ha despresionado un boton de un disp. de entrada (0:keyb,1:joys,2:mouse)"),
//						&CInputManager::IsDownUp)
//
//			.def(	CScriptRegister::PushFunctionName("isUpDown","bool","INPUT_DEVICE_TYPE idType, uint32 nBtn", 
//						"Informa si se justo ahora se ha presionado un boton de un disp. de entrada (0:keyb,1:joys,2:mouse)"),
//						&CInputManager::IsUpDown)						
//
//		];
//}