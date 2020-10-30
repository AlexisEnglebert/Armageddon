#pragma once
#include<d3d11.h>
#include<wrl/client.h>
#include<vector>
#include"..\Macros.h"
#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")
/*
#####################################################################
Adaptateur Data permet de récupérer tout les adaptateurs possible pour
DirectX11 ( ADAPTATEUR GRAPHIQUE par exemple il vas lister ma carte 
Graphique)
#####################################################################
*/

class DLL AdaptaterData
{
public:
	AdaptaterData(IDXGIAdapter* ptrAdaptater);

	IDXGIAdapter* ptrAdaptater = nullptr;
	DXGI_ADAPTER_DESC Description;

};

class DLL AdaptaterReader // Peremet de get les adaptater
{
public:
	static std::vector<AdaptaterData> GetAdaptaterData(); // get all adaptaters
private:
	static std::vector<AdaptaterData> adaptaters;		// list de tout les adaptaters
};