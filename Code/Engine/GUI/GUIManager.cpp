#include "GUIManager.h"
#include "Core.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "GuiWindows.h"
#include "xml/XMLTreeNode.h"
#include "TextureManager.h"
#include "ScriptManager.h"

//--Includes GuiElements---
#include "Utils/Exception.h"
#include "TextBox.h"
#include "PointerMouse.h"
#include "Button.h"
#include "CheckButton.h"
#include "DialogBox.h"
#include "EditableTextBox.h"
#include "Image.h"
#include "TextBox.h"
#include "ProgressBar.h"
#include "RadioBox.h"
#include "Slider.h"
#include "Statictext.h"
//-------------------------

//#include "luabind/luabind.hpp"

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------
CGUIManager::CGUIManager(const Vect2i& resolution)
: m_sCurrentWindows("Main.xml")
, m_TextBox(NULL)
, m_PointerMouse(NULL)
, m_bRenderError(false)
, m_bUpdateError(false)
, m_ScreenResolution(resolution)
, m_bIsOk(false)
, m_bLoadedGuiFiles(false)
, m_sLastLoadpathGUI_XML("")
, m_bFirstUpdate(true)
, m_bVisiblePointerMouse(true)
{}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CGUIManager::Done ()
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
void CGUIManager::Release () 
{
	LOGGER->AddNewLog(ELL_INFORMATION, "GUIManager:: shutting down GUI");

	std::map<std::string, CGuiWindows*>::iterator it;
	std::map<std::string, CGuiWindows*>::iterator itEnd(m_WindowsMap.end());
	for( it = m_WindowsMap.begin(); it != itEnd; ++it)
	{
		CGuiWindows* windows = it->second;
		CHECKED_DELETE(windows);
	}
	m_WindowsMap.clear();

	CHECKED_DELETE(m_TextBox);
	CHECKED_DELETE(m_PointerMouse);

	LOGGER->AddNewLog(ELL_INFORMATION, "GUIManager:: offline (ok)");
}

//----------------------------------------------------------------------------
// Init
//----------------------------------------------------------------------------
bool CGUIManager::Init (const std::string& initGuiXML)
{
	
	m_bIsOk = false;

	LOGGER->AddNewLog(ELL_INFORMATION, "GUIManager:: calling initialization");

	CXMLTreeNode parser;
	if (!parser.LoadFile(initGuiXML.c_str()))
	{
		std::string msg_error = "CGUIManager::Init-> Error al leer el archivo de configuracion GUI: " + initGuiXML;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	else
	{

		CTextureManager* textureM = CORE->GetTextureManager();

		CXMLTreeNode  m = parser["GuiFiles"];
		if (m.Exists())
		{
			std::string path = m.GetPszProperty("path","");
			if (path.compare("") != 0)
				m_bIsOk = LoadGuiFiles(path);
			else
			{
				m_bIsOk = true;
			}
		}
		else
		{
			std::string msg_error = "CGUIManager::Init-> Error al intentar leer el tag <GuiFiles> del archivo de configuracion GUI: " + initGuiXML;
			LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
			throw CException(__FILE__, __LINE__, msg_error);
		}

		if (m_bIsOk)
		{
			m = parser["TextBox"];
			if (m.Exists())
			{
				float posx													= m.GetFloatProperty("posx",									30.f);
				float posy													= m.GetFloatProperty("posy",									30.f);
				float w															= m.GetFloatProperty("width",									35.f);
				float h															= m.GetFloatProperty("height",								35.f);
				float button_w											= m.GetFloatProperty("button_width",					5.f);
				float button_h											= m.GetFloatProperty("button_height",					5.f);
				std::string buttonClose_normal			= m.GetPszProperty("buttonClose_normal",			"");
				std::string buttonClose_over				= m.GetPszProperty("buttonClose_over",				"");
				std::string buttonClose_clicked			= m.GetPszProperty("buttonClose_clicked",			"");
				std::string buttonClose_deactivated	= m.GetPszProperty("buttonClose_deactivated", "" );
				std::string buttonMove_normal				= m.GetPszProperty("buttonMove_normal",				"");
				std::string buttonMove_over					= m.GetPszProperty("buttonMove_over",					"");
				std::string buttonMove_clicked			= m.GetPszProperty("buttonMove_clicked",			"");
				std::string buttonMove_deactivated	= m.GetPszProperty("buttonMove_deactivated",	"");
				std::string quad										= m.GetPszProperty("quad",										"");

				CTexture* Close_normal			= textureM->GetResource(buttonClose_normal);
				CTexture* Close_over				= textureM->GetResource(buttonClose_over);
				CTexture* Close_clicked			= textureM->GetResource(buttonClose_clicked);
				CTexture* Close_deactivated	= textureM->GetResource(buttonClose_deactivated);
				CTexture* Move_normal				= textureM->GetResource(buttonMove_normal);
				CTexture* Move_over					= textureM->GetResource(buttonMove_over);
				CTexture* Move_clicked			= textureM->GetResource(buttonMove_clicked);
				CTexture* Move_deactivated	= textureM->GetResource(buttonMove_deactivated);
				CTexture* back							= textureM->GetResource(quad);

				m_TextBox =	new CTextBox(	m_ScreenResolution.x, m_ScreenResolution.y, h, w, Vect2f(posx,posy), button_w, button_h);
				assert(m_TextBox);
				m_TextBox->SetName("TextBox");
				m_TextBox->SetCloseButtonTextures(Close_normal, Close_over, Close_clicked, Close_deactivated);
				m_TextBox->SetMoveButtonTextures(Move_normal, Move_over, Move_clicked, Move_deactivated);
				m_TextBox->SetDialogTexture(back);
				m_TextBox->SetVisible(false);
			}
			else
			{
				std::string msg_error = "CGUIManager::Init-> Error al intentar leer el tag <TextBox> del archivo de configuracion GUI: " + initGuiXML;
				LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
				throw CException(__FILE__, __LINE__, msg_error);
			}

			m = parser["PointerMouse"];
			if (m.Exists())
			{
				float posx													= m.GetFloatProperty("posx",			5.f);
				float posy													= m.GetFloatProperty("posy",			5.f);
				float w															= m.GetFloatProperty("width",			5.f);
				float h															= m.GetFloatProperty("height",		5.f);
				std::string texture									= m.GetPszProperty("texture",			"");
				bool isQuadrant											= m.GetBoolProperty("isQuadrant",	true);
				
				CTexture* texture_pointer = textureM->GetResource(texture);
				
				m_PointerMouse	= new CPointerMouse(m_ScreenResolution.x, m_ScreenResolution.y, h,w,Vect2f(posx,posy));
				assert(m_PointerMouse);
				m_PointerMouse->SetTexture(texture_pointer,"default");
				m_PointerMouse->SetActiveTexture("default");
				m_PointerMouse->SetQuadrant(isQuadrant);
			}
			else
			{
				std::string msg_error = "CGUIManager::Init-> Error al intentar leer el tag <PointerMouse> del archivo de configuracion GUI: " + initGuiXML;
				LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
				throw CException(__FILE__, __LINE__, msg_error);
			}

			m_bIsOk = m_TextBox && m_PointerMouse;
			
		}//END if (m_bIsOk)
		
	} //END if (!parser.LoadFile(initGuiXML.c_str()))

	if (!m_bIsOk)
	{
		Release();
	}
	else
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CSoundManager:: online (ok)");
	}

	return m_bIsOk;

}

//----------------------------------------------------------------------------
// Render
//----------------------------------------------------------------------------
void CGUIManager::Render (CRenderManager *renderManager, CFontManager* fm)
{
	if (m_bIsOk)
	{
		if (m_bLoadedGuiFiles)
		{
			std::map<std::string, CGuiWindows*>::iterator it;
			it = m_WindowsMap.find( m_sCurrentWindows );
			if( it != m_WindowsMap.end() )
			{
				CGuiWindows * currentWindows = it->second;
				currentWindows->Render(renderManager, fm);
				RenderTransitionEffect(renderManager);
				m_bRenderError = false;
			}
			else
			{
				if (!m_bRenderError)
				{
					LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: Se ha intentado pintar la windows %s no registrada ", m_sCurrentWindows.c_str());
					m_bRenderError = true;
				}
			}
		}//END if (m_bLoadedGuiFiles)
		
		//Siempre los �ltimos en pintarse
		assert(m_TextBox);
		m_TextBox->Render(renderManager, fm);
	}//END if (m_bIsOk)
}


void	CGUIManager::RenderPointerMouse	(CRenderManager *renderManager, CFontManager* fontManager)
{
	if(m_bVisiblePointerMouse)
			m_PointerMouse->Render(renderManager, fontManager);
}
//----------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------

void CGUIManager::Update (float elapsedTime)
{
	if (m_bIsOk)
	{
		assert(m_TextBox);
		assert(m_PointerMouse);

		//Si es la primera vez que actualizamos la GUI debemos hacer un load de la main.xml:
		if (m_bFirstUpdate)
		{
			std::map<std::string, CGuiWindows*>::iterator itCurrentWindows;
			itCurrentWindows = m_WindowsMap.find( m_sCurrentWindows );
			if( itCurrentWindows != m_WindowsMap.end() )
			{
				CGuiWindows * currentWindow = itCurrentWindows->second;
				currentWindow->LoadWindows();
			}
			m_bFirstUpdate = false;
		}

		CInputManager* intputManager = CORE->GetInputManager();

		m_PointerMouse->Update(intputManager, elapsedTime);

		m_TextBox->Update(intputManager, elapsedTime);


		if( !m_TextBox->IsVisible() && m_bLoadedGuiFiles)
		{
			std::map<std::string, CGuiWindows*>::iterator it;
			it = m_WindowsMap.find( m_sCurrentWindows );
			if( it != m_WindowsMap.end() )
			{
				CGuiWindows * currentWindow = it->second;
				if (!UpdateTransitionEffect(elapsedTime))
				{
					currentWindow->Update(intputManager, elapsedTime);
				}
				m_bUpdateError = false;
			}
			else
			{
				if (!m_bUpdateError)
				{
					LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: Se ha intentado updatear la windows %s no registrada ", m_sCurrentWindows.c_str());
					m_bUpdateError = true;
				}
			}
		}//END if( !m_TextBox.IsVisible() )
	}
}

void CGUIManager::RenderTransitionEffect(CRenderManager *renderManager)
{
	if (m_sTransitionEffect.m_bDoEffect)
	{
		switch (m_sTransitionEffect.m_eType)
		{
			case TE_SHADOW:
			{
				//Dibujamos un quad2d en toda la pantalla:
				//	- Durante la primera mitad de tiempo ir� de totalmente transparente a totalmente opaco negro
				//	- Durante la segunda mitad de tiempo ir� de totalmente opaco negro a totalmente transparente
				CColor color = colBLACK;
				float alpha;
				if (m_sTransitionEffect.m_fTimeCounter < m_sTransitionEffect.m_fTransitionTime*0.5f)
				{
					//Durante la primera mitad del tiempo: alpha de 0.f -> 1.f
					alpha = m_sTransitionEffect.m_fTimeCounter / m_sTransitionEffect.m_fTransitionTime*0.5f;
					color.SetAlpha(alpha);
				}
				else
				{
					//Durante la segunda mitad del tiempo: alpha de 1.f -> 0.f
					alpha = m_sTransitionEffect.m_fTimeCounter / m_sTransitionEffect.m_fTransitionTime*0.5f; //esto va de 1->2
					color.SetAlpha(abs(alpha-2));
				}
				renderManager->DrawQuad2D(Vect2i(0,0),m_ScreenResolution.x,m_ScreenResolution.y,UPPER_LEFT,color);
			}
			break;
		
			default:
			{
				LOGGER->AddNewLog(ELL_ERROR,"CGUIManager::RenderTransitionEffect-> No se reconoce el efecto a realizar!");
			}
		}
	}
}

bool CGUIManager::UpdateTransitionEffect (float elapsedTime)
{
	if (m_sTransitionEffect.m_bDoEffect)
	{
		m_sTransitionEffect.m_fTimeCounter += elapsedTime;

		if (!m_sTransitionEffect.m_bActiveWindows && m_sTransitionEffect.m_fTimeCounter > m_sTransitionEffect.m_fTransitionTime*0.5f)
		{
			ActiveWindows(m_sTransitionEffect.m_sWindowsName);
			m_sTransitionEffect.m_bActiveWindows = true;

		}
		if (m_sTransitionEffect.m_fTimeCounter > m_sTransitionEffect.m_fTransitionTime)
		{
			m_sTransitionEffect.m_bDoEffect = false;
			m_sTransitionEffect.m_fTimeCounter = 0.f;
			m_sTransitionEffect.m_bActiveWindows = false;
		}
		return true;
	}
	
	return false;
}

void CGUIManager::ActiveWindowsWithEffect	(const std::string& inNameWindow, EtypeTransitionEffect type, float transitionTime )
{
	m_sTransitionEffect.m_bActiveWindows	= false; 
	m_sTransitionEffect.m_bDoEffect				= true;
	m_sTransitionEffect.m_eType						= type;
	m_sTransitionEffect.m_fTransitionTime	= transitionTime;
	m_sTransitionEffect.m_fTimeCounter		= 0.f;
	m_sTransitionEffect.m_sWindowsName		= inNameWindow;
}

void CGUIManager::ActiveWindows( const std::string& inNameWindow )
{
	std::map<std::string, CGuiWindows*>::iterator it;
	it = m_WindowsMap.find( inNameWindow );
	if( it != m_WindowsMap.end() )
	{
		//Primero finalizamos la ventana actual
		std::map<std::string, CGuiWindows*>::iterator itCurrentWindows;
		itCurrentWindows = m_WindowsMap.find( m_sCurrentWindows );
		if( itCurrentWindows != m_WindowsMap.end() )
		{
			CGuiWindows * currentWindow = itCurrentWindows->second;
			currentWindow->SaveWindows();

			//A continuaci�n leemos los valores de la nueva ventana
			CGuiWindows* windows = it->second;
			windows->LoadWindows();
			m_sCurrentWindows = inNameWindow;
		}
		else
		{
			LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: Al intentar cambiar de windows, la actual-->%s no se ha encontrado registrada", m_sCurrentWindows.c_str());
		}
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: Al intentar cambiar a la windows-->%s esta no se ha encontrado registrada", inNameWindow.c_str());
	}
}


void CGUIManager::PushWindows (const std::string& inNameWindow )
{
  std::map<std::string, CGuiWindows*>::iterator it;
  it = m_WindowsMap.find( inNameWindow );
  if( it != m_WindowsMap.end() )
  {
		m_PrevWindows.push_back(m_sCurrentWindows);
    ActiveWindows(inNameWindow);    
  }
  else
  {
    LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::PushWindows Al intentar cambiar a la windows-->%s etsa no se ha encontrado registrada", inNameWindow.c_str());
  }
}

void CGUIManager::PopWindows ()
{
  if (m_PrevWindows.size() == 0)
  {
    LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::PopWindows -> El vector de PrevWindows esta vac�o!");
  }
  else
  {
    std::string popWindows = m_PrevWindows[m_PrevWindows.size()-1];
    m_PrevWindows.pop_back();
    ActiveWindows(popWindows);
  }
}


void CGUIManager::SetScreenResolution(const Vect2i& resolution)
{
	std::map<std::string, CGuiElement*>::iterator it(m_ElementsMap.begin());
	std::map<std::string, CGuiElement*>::iterator itEnd(m_ElementsMap.end());

	while (it != itEnd)
	{
		CGuiElement* guiElement = it->second;
		guiElement->SetWindowsWidth(resolution.x);
		guiElement->SetWindowsHeight(resolution.y);
		it++;
	}
	
}

bool CGUIManager::LoadGuiFiles (const std::string& pathGUI_XML)
{
	m_bLoadedGuiFiles = false;
	LOGGER->AddNewLog(ELL_INFORMATION, "GUIManager:: Empezando a leer los .xml del directorio->%s",pathGUI_XML.c_str());
	//Read xml files:
	std::map<std::string, CGuiWindows*>::iterator it(m_WindowsMap.begin());
	std::map<std::string, CGuiWindows*>::iterator itEnd(m_WindowsMap.end());
	while (it != itEnd)
	{
		CGuiWindows* windows = it->second;
		CHECKED_DELETE(windows);
		++it;
	}

	m_WindowsMap.clear();
	m_ElementsMap.clear();
	m_sCurrentWindows = "Main.xml";

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	// We read the XmlGui files	
	m_sLastLoadpathGUI_XML = pathGUI_XML;
	std::string path_xmls = pathGUI_XML+"/*.xml";
	hFind = FindFirstFile(path_xmls.c_str(), &FindFileData);

	// we check the existence of the XmlGui directory
	if ( hFind == INVALID_HANDLE_VALUE ) 
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: Error al intentar leer los .xml del directorio %s", pathGUI_XML.c_str());
		return false;
	}
	else 
	{
		std::string FileName = FindFileData.cFileName;
		CGuiWindows* windows = new CGuiWindows();
		std::string pathFile = pathGUI_XML+"/";
		pathFile += FileName;
		bool isOk = false;
		isOk = windows->LoadXML(pathFile,m_ScreenResolution);
		if (!isOk)
		{
			return false;
		}
		windows->RegisterElements(m_ElementsMap);
		windows->SetName(FileName);
		m_WindowsMap.insert( std::pair<std::string,CGuiWindows*>(FileName,windows) );

		while( FindNextFile(hFind, &FindFileData) != 0 )
		{
			std::string FileName = FindFileData.cFileName;
			CGuiWindows* windows = new CGuiWindows();
			std::string pathFile = pathGUI_XML+"/";
			pathFile += FileName;
			isOk = windows->LoadXML(pathFile,m_ScreenResolution);
			if (!isOk)
			{
				return false;
			}
			windows->RegisterElements(m_ElementsMap);
			windows->SetName(FileName);
			m_WindowsMap.insert( std::pair<std::string,CGuiWindows*>(FileName,windows) );
		}

		FindClose(hFind);
	}	

	m_bLoadedGuiFiles = true;
	return true;
}

bool CGUIManager::ReloadGuiFiles ()
{
	bool isOk = false;
	std::string windows = m_sCurrentWindows;
	isOk = LoadGuiFiles(m_sLastLoadpathGUI_XML);
	
	std::map<std::string, CGuiWindows*>::iterator it;
	it = m_WindowsMap.find( windows );
	if (it != m_WindowsMap.end())
	{
		m_sCurrentWindows = windows;
	}
	
	return isOk;
}

void CGUIManager::SetMessageBox( const std::string& text )
{
	if (m_bIsOk)
	{
		assert(m_TextBox);
		if( !m_TextBox->IsVisible() )
		{
			m_TextBox->SetMessage( text );
			m_TextBox->SetVisible( true );
			m_TextBox->SetActive( true );
			m_TextBox->SetPosition(Vect2i(30,30));
		}
	}
}


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//				Funciones para modificar los GuiElements
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void CGUIManager::SetActiveGuiElement (const std::string& inNameGuiElement, bool flag)
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameGuiElement);
	if( it!= m_ElementsMap.end() )
	{
		it->second->SetActive(flag);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion SetActiveGuiElement", inNameGuiElement.c_str());
	}
}

void CGUIManager::SetVisibleGuiElement (const std::string& inNameGuiElement, bool flag)
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameGuiElement);
	if( it!= m_ElementsMap.end() )
	{
		it->second->SetVisible(flag);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion SetVisibleGuiElement", inNameGuiElement.c_str());
	}
}

bool CGUIManager::GetProgressBarValue (const std::string& inNameGuiElement, float& outValue)
{

	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameGuiElement);
	if( it!= m_ElementsMap.end() )
	{
		CProgressBar * progress = (CProgressBar*) (it->second);
		outValue = progress->GetProgress();
		return true;
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion GetProgressBarValue", inNameGuiElement.c_str());
	}

	return false;
}

bool CGUIManager::SetProgressBarValue (const std::string& inNameGuiElement, float inValue)
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameGuiElement);
	if( it!= m_ElementsMap.end() )
	{
		CProgressBar * progress = (CProgressBar*) (it->second);
		progress->SetProgress(inValue);
		return true;
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion SetProgressBarValue", inNameGuiElement.c_str());
	}
	return false;
}

std::string CGUIManager::GetButtonCheckInRadioBox (const std::string& inNameRadioBox)
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameRadioBox);
	if( it!= m_ElementsMap.end() )
	{
		CRadioBox * rb = (CRadioBox*) (it->second);
		return rb->GetDefaultCheckButton();
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion GetButtonCheckInRadioBox", inNameRadioBox.c_str());
	}

	return "";
}

void CGUIManager::SetButtonCheckInRadioBox(  const std::string& inNameRadioBox, const std::string& button )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameRadioBox);
	if( it!= m_ElementsMap.end() )
	{
		CRadioBox * rb = (CRadioBox*) (it->second);
		rb->SetDefaultCheckButton(button);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion GetButtonCheckInRadioBox", inNameRadioBox.c_str());
	}
}

void CGUIManager::SetStateCheckButton ( const std::string& inCheckButtonName, bool state )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inCheckButtonName);
	if( it!= m_ElementsMap.end() )
	{
		CCheckButton * checkButton = (CCheckButton*) (it->second);
		checkButton->SetOn(state);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion SetStateCheckButton", inCheckButtonName.c_str());
	}
}

bool CGUIManager::GetStateCheckButton( const std::string& inCheckButtonName )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inCheckButtonName);
	if( it!= m_ElementsMap.end() )
	{
		CCheckButton * checkButton = (CCheckButton*) (it->second);
		return checkButton->GetState();

	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion GetStateCheckButton", inCheckButtonName.c_str());
	}
	return false;
}

void CGUIManager::SetEditableTextBox( const std::string& inEditableText, const std::string& text )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inEditableText);
	if( it!= m_ElementsMap.end() )
	{
		CEditableTextBox * editableTextBox = (CEditableTextBox*) (it->second);
		editableTextBox->SetBuffer(text);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion SetEditableTextBox", inEditableText.c_str());
	}
}

std::string CGUIManager::GetEditableTextBox( const std::string& inEditableText )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inEditableText);
	if( it!= m_ElementsMap.end() )
	{
		CEditableTextBox * editableTextBox = (CEditableTextBox*) (it->second);
		return editableTextBox->GetBuffer();
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion GetEditableTextBox", inEditableText.c_str());
	}
	return "";
}

void CGUIManager::SetImage( const std::string& inImageName, const std::string& activeImage )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inImageName);
	if( it!= m_ElementsMap.end() )
	{
		CImage * image = (CImage*) (it->second);
		image->SetActiveTexture(activeImage);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion SetImage", inImageName.c_str());
	}
}

std::string CGUIManager::GetImage( const std::string& inImageName )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inImageName);
	if( it!= m_ElementsMap.end() )
	{
		CImage * image = (CImage*) (it->second);
		return image->GetActiveTexture();

	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion GetImage", inImageName.c_str());
	}
	return "";
}

void CGUIManager::PlayImage	(const std::string& inImageName, float timePerImage, bool loop)
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inImageName);
	if( it!= m_ElementsMap.end() )
	{
		CImage * image = (CImage*) (it->second);
		image->PlayAnimation(timePerImage, loop);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion GetImage", inImageName.c_str());
	}
}

void CGUIManager::SetStateSlider( const std::string& inSliderName, float amount )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inSliderName);
	if( it!= m_ElementsMap.end() )
	{
		CSlider * slider= (CSlider*) (it->second);
		slider->SetValue(amount);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion GetStateSlider", inSliderName.c_str());
	}
}

float CGUIManager::GetStateSlider( const std::string& inSliderName )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inSliderName);
	if( it!= m_ElementsMap.end() )
	{
		CSlider * slider= (CSlider*) (it->second);
		float kk=slider->GetValue();
		return slider->GetValue();

	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion GetStateSlider", inSliderName.c_str());
	}
	return 0.f;
}


void CGUIManager::SetLiteralInStaticText( const std::string& inStaticText, const std::string& lit )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inStaticText);
	if( it!= m_ElementsMap.end() )
	{
		CStaticText * st = (CStaticText*) (it->second);
		st->SetLiteral(lit);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion SetVariableText", inStaticText.c_str());
	}
}


bool CGUIManager::NextBlockInRadioBox(  const std::string& inNameRadioBox )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameRadioBox);
	if( it!= m_ElementsMap.end() )
	{
		CRadioBox * rb = (CRadioBox*) (it->second);
		rb->NextBlock();
		return true;
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion NextBlockInRadioBox", inNameRadioBox.c_str());
	}
	return false;
}

bool CGUIManager::PrevBlockInRadioBox(  const std::string& inNameRadioBox )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameRadioBox);
	if( it!= m_ElementsMap.end() )
	{
		CRadioBox * rb = (CRadioBox*) (it->second);
		rb->PrevBlock();
		return true;
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: No se ha encontrado el guiElement %s al ejecutar la funcion PrevBlockInRadioBox", inNameRadioBox.c_str());
	}
	return false;
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------