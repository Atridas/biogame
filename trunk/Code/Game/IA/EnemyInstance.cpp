#include "EnemyInstance.h"



void CEnemyInstance::InitInstance(CXMLTreeNode& _XMLParams)
{
    m_vPosition = _XMLParams.GetVect3fProperty("pos",Vect3f(0.0f));
    m_vRotate = _XMLParams.GetVect3fProperty("rot",Vect3f(0.0f));

    SetOk(true);
}