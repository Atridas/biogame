#pragma once

#include <XML/XMLTreeNode.h>
#include "EnemyResource.h"
#include "EnemyInstance.h"
#include <string>
#include <vector>

using namespace std;

typedef vector<CEnemyResource> VectorEnemyResource;
typedef vector<CEnemyInstance> VectorEnemyInstance;

class CEnemyManager
{

public:
	CEnemyManager(void);
	virtual ~CEnemyManager(void);

	void Init(const string& _szPath);
	void Release();

private:
	VectorEnemyResource m_vResources;
	VectorEnemyInstance m_vEnemies;

};

