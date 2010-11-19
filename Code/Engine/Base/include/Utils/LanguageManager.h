//----------------------------------------------------------------------------------
// CLanguageManager class
// Author: Enric Vergara
//
// Description:
// Esta clase será la que se encargue de dar un string en el lenguage que este seteado actualmente
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_LANGUAGE_MANAGER_H_
#define INC_LANGUAGE_MANAGER_H_

#include "Math/Color.h"
#include "base.h"
#include <vector>
#include <map>

// Forward Declarations -----------------------------------------------
struct SLanguageManagerParams;
// --------------------------------------------------------------------

struct SLiteral
{
	std::string	m_sFontId;
	CColor			m_cColor;
	std::string	m_value;
};


class CLanguageManager:
  public CBaseControl
{
public:
  CLanguageManager():m_sCurrentLanguage(""){};
	virtual ~CLanguageManager(void) {Done();}

  bool Init(const SLanguageManagerParams&);

	void						LoadXMLs						();																//se puede hacer un reload
	void						SetXmlFile					(const std::string& pathFile);
	bool						GetLiteral					(const std::string& id, SLiteral& sliteral) const;
	void						SetCurrentLanguage	(const std::string& id);
	
private:
	void						LoadXML							(const std::string& pathFile);
	
protected:
  void            Release             ();
private:
	typedef std::map<std::string,SLiteral>      TLanguage;

	std::vector<std::string>					m_vXML_Files;
	std::map<std::string, TLanguage>	m_Languages;
	std::string												m_sCurrentLanguage;


};

#endif //INC_LANGUAGE_MANAGER_H_