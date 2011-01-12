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
   * Amb el constructor per defecte és suficient per deixar la classe en Ok.
  **/
  CAnimatedModelManager(void)   {SetOk(true);};
  /**
   * Destructor.
   * Allibera els recursos abans de destruir-se.
  **/
  ~CAnimatedModelManager(void)  {Done();};
  /**
   * Mètode de càrrega desde fitxer.
   * Aquest mètode inicialitzarà tots els AnimatedModel que es trobin al fitxer especificat.
   * @param _szFileName Fitxer que conté els path d'AnimatedModel a carregar.
   * @return True si s'ha iniciat correctament. False si no.
  **/
  bool Load(const string &_szFileName);
  /**
   * Mètode de càrrega desde varis fitxers.
   * Aquest mètode inicialitzarà tots els AnimatedModel que es trobin a cada un dels fitxers especificats.
   * @param _XMLs Vector que conté els diferents fitxers des d'on carregar AnimatedModels.
   * @return True si s'ha iniciat correctament. False si no.
  **/
  bool Load(const vector<string>& _XMLs);
  /**
   * Mètode de recàrrega del Manager.
   * Aquest mètode tornarà a inicialitzar tots els AnimatedModel que es trobin als fitxers especificats anteriorment.
   * @return True si s'ha iniciat correctament. False si no.
   * @see Load(const string &_szFileName)
   * @see Load(const vector<string>& _XMLs)
  **/
  bool Reload();

  /**
   * Mètode per obtenir un AnimatedCoreModel.
   * Aquest mètode retornarà l'AnimatedCoreModel especificat si és que aquest ha estat carregat anteriorment.
   * @param _szName Identificador de l'AnimatedCoreModel a buscar.
   * @return L'AnimatedCoreModel si existeix, NULL sino.
  **/
  CAnimatedCoreModel*     GetCore(const std::string& _szName);
  /**
   * Mètode d'obtenció d'AnimatedInstanceModel.
   * Aquest mètode cercarà l'AnimatedCoreModel del nom especificat, i en cas d'existir en retornarà un AnimatedInstanceModel d'aquest.
   * Important tenir en compte que aquesta instància NO s'allivera al manager i cal que l'usuari l'alliveri.
   * @param _szName Identificador de l'AnimatedCoreModel a buscar.
   * @return L'AnimatedInstanceModel si existeix la core, NULL sino.
  **/
  CAnimatedInstanceModel* GetInstance(const std::string& _szName);
  
private:
  /**
   * Mètode per obtenir un AnimatedCoreModel.
   * Aquest mètode retornarà l'AnimatedCoreModel especificat si és que aquest ha estat carregat anteriorment. Si no estava carregat el crearà i carregarà segons el fitxer especificat.
   * @param _szName Identificador de l'AnimatedCoreModel a carregar.
   * @param _szPath Fitxer d'on carregar l'AnimatedCoreModel en cas de no estar carregat amb anterioritat.
   * @return L'AnimatedCoreModel.
  **/
  CAnimatedCoreModel*     GetCore(const std::string& _szName, const std::string& _szPath);

  /**
   * Conjunt de fitxers que indiquen on carregar AnimatedCoreModel.
   * Cada fitxer nou que s'especifiqui mitjançant el mètode de Load s'adjuntarà a la llista. D'aquesta forma ReLoad sabrà quins fitxers recarregar.
  **/
  set<string> m_vXMLFiles;
};

#endif