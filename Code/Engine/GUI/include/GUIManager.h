//----------------------------------------------------------------------------------
// CWindows class
// Author: Enric Vergara
//
// Description:
// Esta clase gestiona el pintado y update de una ventana de la GUI. Esta ventana es simplemente una agrupacion
// de GuiElements que se lee al inicio de la aplicacion de un fichero .xml
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_GUI_MANAGER_H
#define INC_GUI_MANAGER_H

#include <string>
#include <map>
#include "base.h"
#include "TextBox.h"

//---Forward Declarations---
class CPointerMouse;
class CGuiElement;
class CGuiWindows;
class CRenderManager;
class CFontManager;
class CScriptManager;
//--------------------------

//----------Declaracion de nuevos tipos------------------------------------
typedef enum EtypeTransitionEffect {TE_SHADOW = 0};
struct TransitionEffect
{
	TransitionEffect::TransitionEffect(): m_bDoEffect(false), m_fTransitionTime(0.f), m_eType(TE_SHADOW), m_sWindowsName(""), m_fTimeCounter(0.f), m_bActiveWindows(false){}
	bool									m_bDoEffect;
	float									m_fTransitionTime;
	float									m_fTimeCounter;
	std::string						m_sWindowsName;
	bool									m_bActiveWindows;
	EtypeTransitionEffect	m_eType;

};
//---------------------------------------------------------------------------

class CGUIManager//:public CScriptRegister
{

public:
	// Init and End protocols
	CGUIManager(const Vect2i& resolution);
	virtual ~CGUIManager() {Done();}  


	bool								Init											(const std::string& initGuiXML);
	void								Done											();
	bool								IsOk											() const																						{ return m_bIsOk; }
	void								Render										(CRenderManager *renderManager, CFontManager* fontManager);
	void								RenderPointerMouse				(CRenderManager *renderManager, CFontManager* fontManager);
	void								Update										(float elapsedTime);
	void								SetScreenResolution				(const Vect2i& resolution);
	bool								LoadGuiFiles							(const std::string& pathWindows);
	bool								ReloadGuiFiles						();
	void								SetMessageBox							(const std::string& text);
	bool								IsVisibleMessage					() const																						{assert(m_TextBox); return m_TextBox->IsVisible();}
	void								ActiveWindows							(const std::string& inNameWindow);
  void                PushWindows               (const std::string& inNameWindow);
  void                PopWindows                ();
	void								ActiveWindowsWithEffect		(const std::string& inNameWindow, EtypeTransitionEffect type, float transitionTime );
	void								SetVisiblePointerMouse		(bool flag)																					{m_bVisiblePointerMouse = flag;}

	//-------Funciones para modificar los GuiElements--------------------------
	void								SetActiveGuiElement				(const std::string& inNameGuiElement, bool flag);
	void								SetVisibleGuiElement			(const std::string& inNameGuiElement, bool flag);

	bool								GetProgressBarValue				(const std::string& inNameGuiElement, float& outValue);
	bool								SetProgressBarValue				(const std::string& inNameGuiElement, float inValue);

	bool								NextBlockInRadioBox				(const std::string& inNameRadioBox);
	bool								PrevBlockInRadioBox				(const std::string& inNameRadioBox);

	std::string					GetButtonCheckInRadioBox	(const std::string& inNameRadioBox);
	void								SetButtonCheckInRadioBox	(const std::string& inNameRadioBox, const std::string& button);

	void								SetStateCheckButton 			(const std::string& inCheckButtonName, bool state);
	bool								GetStateCheckButton 			(const std::string& inCheckButtonName);

	void								SetStateSlider						(const std::string& inSliderName, float amount);
	float								GetStateSlider						(const std::string& inSliderName);

	void								SetLiteralInStaticText		(const std::string& inStaticText, const std::string& lit);
	
	void								SetEditableTextBox				(const std::string& inEditableTextName, const std::string& text);
	std::string					GetEditableTextBox				(const std::string& inEditableTextName);

	void								SetImage									(const std::string& inImageName, const std::string& activeImage);
	std::string					GetImage									(const std::string& inImageName);
	void								PlayImage									(const std::string& inImageName, float timePerImage, bool loop);
	//-----------------------------------------------------------------------------------------------------------//

private:
	void								Release										();
	void								RenderTransitionEffect		(CRenderManager *renderManager);
	bool								UpdateTransitionEffect		(float elapsedTime);

private:
	Vect2i															m_ScreenResolution;
	bool																m_bIsOk;
	std::map<std::string, CGuiWindows*> m_WindowsMap;
	std::map<std::string, CGuiElement*>	m_ElementsMap;
	std::string													m_sCurrentWindows;
  std::vector<std::string>            m_PrevWindows;
	bool																m_bVisiblePointerMouse;
	bool																m_bRenderError;
	bool																m_bUpdateError;
	CPointerMouse*											m_PointerMouse;
	CTextBox*														m_TextBox;
	bool																m_bLoadedGuiFiles;
	std::string													m_sLastLoadpathGUI_XML;
	TransitionEffect										m_sTransitionEffect;
	bool																m_bFirstUpdate;
};

#endif //INC_GUI_MANAGER_H
