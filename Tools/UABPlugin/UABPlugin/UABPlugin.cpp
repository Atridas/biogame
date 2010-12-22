//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "UABPlugin.h"
#include "MAXScrpt/MAXScrpt.h"
#include "MAXScrpt/Numbers.h"
#include "MAXScrpt/definsfn.h"
#include <vector>
#include <map>

typedef std::vector<float> VFloats;

struct CompareVFloats
{
	bool operator()(const VFloats &v1, const VFloats &v2) const
	{
		for(size_t i=0; i<v1.size() && i<v2.size();++i)
		{
			if(v1[i]<v2[i])
				return true;
			else if(v1[i]>v2[i])
				return false;
		}
		return (v1.size()>v2.size());
	}
};

typedef std::map<VFloats, unsigned int, CompareVFloats> MInts;
std::vector<MInts> g_VertexBuffer;

def_visible_primitive(UABSayHello, "uab_say_hello");
def_visible_primitive(UABClearVertexBuffer, "uab_clear_vertex_buffer");
def_visible_primitive(UABAddVtxOnVertexBuffer, "uab_add_vtx_on_vertex_buffer");

Value* UABSayHello_cf(Value **arg_list, int count)
{
	MessageBox(NULL, _T("HOLA MUNDO"),_T("NOS VAMOS DE VACACIONES"), MB_OK);
	return &true_value;
}

Value* UABClearVertexBuffer_cf(Value **arg_list, int count)
{
	g_VertexBuffer.clear();
	return &true_value;
}

Value* UABAddVtxOnVertexBuffer_cf(Value **arg_list, int count)
{
	check_arg_count(UABAddVtxOnVertexBuffer, 2, count);
	type_check(arg_list[0], Integer, "uab_add_vtx_on_vertex_buffer");
	type_check(arg_list[1], Array, "uab_add_vtx_on_vertex_buffer");
	
	Value *v1=arg_list[0];
	Value *v2=arg_list[1];

	if(v2->is_kind_of(class_tag(Array)) && v1->is_kind_of(class_tag(Integer)))
	{
		Array *l_MaxVtx=(Array *)v2;
		VFloats l_Vertex;
		size_t l_IdMaterial=v1->to_int();
		while(l_IdMaterial>=g_VertexBuffer.size())
		{
			MInts l_Map;
			g_VertexBuffer.push_back(l_Map);
		}
		for(int i=0;i<l_MaxVtx->size;++i)
		{
			Value *l_Component=l_MaxVtx->get(i+1);
			l_Vertex.push_back(l_Component->to_float());
		}
		if(g_VertexBuffer[l_IdMaterial].find(l_Vertex)==g_VertexBuffer[l_IdMaterial].end())
		{
			int l_Idx=g_VertexBuffer[l_IdMaterial].size();
			g_VertexBuffer[l_IdMaterial][l_Vertex]=l_Idx;
			return Integer::intern(l_Idx);
		}
		else
		{
			return Integer::intern(g_VertexBuffer[l_IdMaterial][l_Vertex]);
		}
	}
	return &false_value;
}

/*#define UABPlugin_CLASS_ID	Class_ID(0x5e1f3163, 0x519f7cc1)


class UABPlugin : public UtilityObj 
{
public:
		
	//Constructor/Destructor
	UABPlugin();
	virtual ~UABPlugin();

	virtual void DeleteThis() { }		
	
	virtual void BeginEditParams(Interface *ip,IUtil *iu);
	virtual void EndEditParams(Interface *ip,IUtil *iu);

	virtual void Init(HWND hWnd);
	virtual void Destroy(HWND hWnd);
	
	// Singleton access
	static UABPlugin* GetInstance() { 
		static UABPlugin theUABPlugin;
		return &theUABPlugin; 
	}

private:

	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND			hPanel;
	IUtil			*iu;
	Interface		*ip;
};


class UABPluginClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL loading = FALSE) 	{ return UABPlugin::GetInstance(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return UTILITY_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return UABPlugin_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("UABPlugin"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetUABPluginDesc() { 
	static UABPluginClassDesc UABPluginDesc;
	return &UABPluginDesc; 
}




//--- UABPlugin -------------------------------------------------------
UABPlugin::UABPlugin()
{
	iu = NULL;
	ip = NULL;	
	hPanel = NULL;
}

UABPlugin::~UABPlugin()
{

}

void UABPlugin::BeginEditParams(Interface* ip,IUtil* iu) 
{
	this->iu = iu;
	this->ip = ip;
	hPanel = ip->AddRollupPage(
		hInstance,
		MAKEINTRESOURCE(IDD_PANEL),
		DlgProc,
		GetString(IDS_PARAMS),
		0);
}
	
void UABPlugin::EndEditParams(Interface* ip,IUtil* iu) 
{
	this->iu = NULL;
	this->ip = NULL;
	ip->DeleteRollupPage(hPanel);
	hPanel = NULL;
}

void UABPlugin::Init(HWND hWnd)
{

}

void UABPlugin::Destroy(HWND hWnd)
{

}

INT_PTR CALLBACK UABPlugin::DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) 
	{
		case WM_INITDIALOG:
			UABPlugin::GetInstance()->Init(hWnd);
			break;

		case WM_DESTROY:
			UABPlugin::GetInstance()->Destroy(hWnd);
			break;

		case WM_COMMAND:
			#pragma message(TODO("React to the user interface commands.  A utility plug-in is controlled by the user from here."))
			break;

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
			UABPlugin::GetInstance()->ip->RollupMouseMessage(hWnd,msg,wParam,lParam); 
			break;

		default:
			return 0;
	}
	return 1;
}
*/