#pragma once
#ifndef __ANIMATED_MODEL_MANAGER_H__
#define __ANIMATED_MODEL_MANAGER_H__

#include "base.h"

#include <set>

#include "Utils/MapManager.h"

#include "AnimatedCoreModel.h"

// Forward declarations -------------
class CAnimatedInstanceModel;
// ----------------------------------

/**
 * Classe AnimatedModelManager.
 * Controlador de classes del tipus AnimatedCoreModel. S'encarrega de gestionar-les i fer la crida als seus render.
**/
class CAnimatedModelManager:
  public CMapManager<CAnimatedCoreModel>
{
public:
  /**
   * Constructor per defecte.
   * Amb el constructor per defecte �s suficient per deixar la classe en Ok.
  **/
  CAnimatedModelManager(void)   {SetOk(true);};
  /**
   * Destructor.
   * Allibera els recursos abans de destruir-se.
  **/
  ~CAnimatedModelManager(void)  {Done();};
  /**
   * M�tode de c�rrega desde fitxer.
   * Aquest m�tode inicialitzar� tots els AnimatedModel que es trobin al fitxer especificat.
   * @param _szFileName Fitxer que cont� els path d'AnimatedModel a carregar.
   * @return True si s'ha iniciat correctament. False si no.
  **/
  bool Load(const string &_szFileName);
  /**
   * M�tode de c�rrega desde varis fitxers.
   * Aquest m�tode inicialitzar� tots els AnimatedModel que es trobin a cada un dels fitxers especificats.
   * @param _XMLs Vector que cont� els diferents fitxers des d'on carregar AnimatedModels.
   * @return True si s'ha iniciat correctament. False si no.
  **/
  bool Load(const vector<string>& _XMLs);
  /**
   * M�tode de rec�rrega del Manager.
   * Aquest m�tode tornar� a inicialitzar tots els AnimatedModel que es trobin als fitxers especificats anteriorment.
   * @return True si s'ha iniciat correctament. False si no.
   * @see Load(const string &_szFileName)
   * @see Load(const vector<string>& _XMLs)
  **/
  bool Reload();

  /**
   * M�tode per obtenir un AnimatedCoreModel.
   * Aquest m�tode retornar� l'AnimatedCoreModel especificat si �s que aquest ha estat carregat anteriorment.
   * @param _szName Identificador de l'AnimatedCoreModel a buscar.
   * @return L'AnimatedCoreModel si existeix, NULL sino.
  **/
  CAnimatedCoreModel*     GetCore(const std::string& _szName);
  /**
   * M�tode d'obtenci� d'AnimatedInstanceModel.
   * Aquest m�tode cercar� l'AnimatedCoreModel del nom especificat, i en cas d'existir en retornar� un AnimatedInstanceModel d'aquest.
   * Important tenir en compte que aquesta inst�ncia NO s'allivera al manager i cal que l'usuari l'alliveri.
   * @param _szName Identificador de l'AnimatedCoreModel a buscar.
   * @return L'AnimatedInstanceModel si existeix la core, NULL sino.
  **/
  CAnimatedInstanceModel* GetInstance(const std::string& _szName);
  
private:
  /**
   * M�tode per obtenir un AnimatedCoreModel.
   * Aquest m�tode retornar� l'AnimatedCoreModel especificat si �s que aquest ha estat carregat anteriorment. Si no estava carregat el crear� i carregar� segons el fitxer especificat.
   * @param _szName Identificador de l'AnimatedCoreModel a carregar.
   * @param _szPath Fitxer d'on carregar l'AnimatedCoreModel en cas de no estar carregat amb anterioritat.
   * @return L'AnimatedCoreModel.
  **/
  CAnimatedCoreModel*     GetCore(const std::string& _szName, const std::string& _szPath);

  /**
   * Conjunt de fitxers que indiquen on carregar AnimatedCoreModel.
   * Cada fitxer nou que s'especifiqui mitjan�ant el m�tode de Load s'adjuntar� a la llista. D'aquesta forma ReLoad sabr� quins fitxers recarregar.
  **/
  set<string> m_vXMLFiles;
};

#endif