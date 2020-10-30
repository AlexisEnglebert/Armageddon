#include "AdaptaterData.h"
#pragma comment(lib, "dxgi.lib")

/*
 Init la Variable [@]
*/
std::vector<AdaptaterData> AdaptaterReader::adaptaters;

AdaptaterData::AdaptaterData(IDXGIAdapter* ptrAdaptater)
{
    this->ptrAdaptater = ptrAdaptater;
    HRESULT hr = ptrAdaptater->GetDesc(&this->Description);
    if (FAILED(hr))
    {
        OutputDebugString("ERREUR CRITIQUE, FAILED HR");
        exit(-1);
    }
}


std::vector<AdaptaterData> AdaptaterReader::GetAdaptaterData()
{
    if (adaptaters.size() > 0)  // L'array a déjà un élément ce qui veut dire quelle est déjà initialisée
    {
        return adaptaters;
    }

    Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;  // pointeur factory
    HRESULT handleR = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
    if (FAILED(handleR))
    {
        OutputDebugString("ERREUR CRITIQUE, Handle to IDXGIFactory failed");
        exit(-1);
    }
    IDXGIAdapter* pAdapter;
    UINT Index = 0;
    while (SUCCEEDED(pFactory->EnumAdapters(Index, &pAdapter)))
    {
        adaptaters.push_back(AdaptaterData(pAdapter));
        Index++;

    }
    return adaptaters;
}
