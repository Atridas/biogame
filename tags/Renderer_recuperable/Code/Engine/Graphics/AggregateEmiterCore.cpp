#include "AggregateEmiterCore.h"
#include "Core.h"
#include "EmiterCoreManager.h"

#include <XML\XMLTreeNode.h>

bool CAggregateEmiterCore::Init(CXMLTreeNode& _xmlEmiter)
{
  if(strcmp(_xmlEmiter.GetName(), "AggregateEmiter") != 0)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CCoreAggregateEmiter::Init XML element not correct: \"%s\" and should be \"AggregateEmiter\"", _xmlEmiter.GetName());
  }

  CEmiterCoreManager *l_pCEM = CORE->GetEmiterCoreManager();

  SetName(_xmlEmiter.GetPszISOProperty("id"));
  LOGGER->AddNewLog(ELL_INFORMATION, "CCoreAggregateEmiter::Init Reading Particle Core: \"%s\"",GetName().c_str());
  
  m_ChildNames.insert(GetName());

  int l_iNumChildren = _xmlEmiter.GetNumChildren();
  for(int i = 0; i < l_iNumChildren; ++i)
  {
    CXMLTreeNode l_xmlEmiterRef = _xmlEmiter(i);
    if(strcmp(l_xmlEmiterRef.GetName(), "Emiter") == 0)
    {
      string l_szEmiterName = l_xmlEmiterRef.GetPszISOProperty("id");
      Vect3f l_vPosition    = l_xmlEmiterRef.GetVect3fProperty("position", Vect3f(0,0,0), false);
      Vect3f l_vVolume      = l_xmlEmiterRef.GetVect3fProperty("volume",   Vect3f(1,1,1), false);
      float  l_fYaw         = l_xmlEmiterRef.GetFloatProperty ("yaw",0,false)   * FLOAT_PI_VALUE / 180.0f;
      float  l_fPitch       = l_xmlEmiterRef.GetFloatProperty ("pitch",0,false) * FLOAT_PI_VALUE / 180.0f;
      float  l_fRoll        = l_xmlEmiterRef.GetFloatProperty ("roll",0,false)  * FLOAT_PI_VALUE / 180.0f;
      
      LOGGER->AddNewLog(ELL_INFORMATION, "\tNew Child Core: \"%s\"", l_szEmiterName.c_str());
      LOGGER->AddNewLog(ELL_INFORMATION, "\t\tpos: (%f, %f, %f) volume: (%f, %f, %f) yaw: %f pitch: %f, roll: %f", 
                                          l_vPosition.x, l_vPosition.y, l_vPosition.z, 
                                          l_vVolume.x, l_vVolume.y, l_vVolume.z,
                                          l_fYaw, l_fPitch, l_fRoll);

      SEmiters l_ChildEmiter;
      l_ChildEmiter.movement.SetPosition(l_vPosition);
      l_ChildEmiter.movement.SetYaw  (l_fYaw);
      l_ChildEmiter.movement.SetPitch(l_fPitch);
      l_ChildEmiter.movement.SetRoll (l_fRoll);

      l_ChildEmiter.volume = l_vVolume;

      l_ChildEmiter.emiter = l_szEmiterName;
        
      const CEmiterCore *l_pChildEmiter = l_pCEM->GetEmiterCore(l_szEmiterName);

      if(l_pChildEmiter->IsSimpleEmiter())
      {
        m_Childs.push_back(l_ChildEmiter);
        m_ChildNames.insert(l_szEmiterName);
      }
      else if(l_pChildEmiter->IsAggregateEmiter())
      {
        const set<string>& l_GrandchildNames = dynamic_cast<const CAggregateEmiterCore*>(l_pChildEmiter)->GetChildNames();
        if(l_GrandchildNames.find(GetName()) == l_GrandchildNames.end())
        {
          m_Childs.push_back(l_ChildEmiter);
          
          set<string>::const_iterator l_it  = l_GrandchildNames.begin();
          set<string>::const_iterator l_end = l_GrandchildNames.end  ();
          for(; l_it != l_end; ++l_it)
          {
            m_ChildNames.insert(*l_it);
          }
        }
        else
        {
          LOGGER->AddNewLog(ELL_WARNING, "\tBucles are not allowed.");
        }
      }
      else
      {
        LOGGER->AddNewLog(ELL_ERROR, "\tChild Core is neither simple nor aggregate!");
      }


    }
    else if(!l_xmlEmiterRef.IsComment())
    {
      LOGGER->AddNewLog(ELL_WARNING, "Unknown element: %s", l_xmlEmiterRef.GetName());
    }
  }
  return true;
}
