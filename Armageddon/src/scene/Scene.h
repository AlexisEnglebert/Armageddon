#pragma once
#include "entt.hpp"
#include "component.h"
#include "../Macros.h"
#include "../Graphics/3D/Renderable/RenderModels.h"
#include "../Application.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Includes.h"
#include "../Graphics/3D/Renderable/Object.h"
#include <vector>
#include <stdio.h>
#include <wrl\client.h>
#include "../Graphics/3D/Renderable/Object.h"
#include "..\ImGuizmo\ImGuizmo.h"
class Entity;
class DLL Scene
{
public:
	Scene() {};
	Scene(Microsoft::WRL::ComPtr <ID3D11Device> device,
		Microsoft::WRL::ComPtr <ID3D11DeviceContext> device_context) : device(device), device_context(device_context)
	{};
	void UpdateScene();
	Entity CreateEntity();
	Entity& GetEntityById(entt::entity ent);
	Entity& GetSelectedEntity();
	void DrawGameObjectScene();
	void DrawEntity(Entity& ent);
	void DrawEntityInfo(Entity& ent);
	entt::registry g_registry;
	std::map<int, std::unique_ptr<Object>> DrawableEntity;
	DirectX::XMFLOAT3A LightDir = { 0.25f, 0.5f, -1.0f };
	DirectX::XMMATRIX TestScale = DirectX::XMMatrixScaling(1,1,1);

private:
	void DrawTransFormComponent(Entity& ent);
	void DrawModelComponent(Entity& ent);
	friend class Entity;
	entt::entity SelectedEntity = entt::null;
	//Entity selectedEnt;
	int selectedEntID = -1;
	std::vector<Entity> EntityList;
	Microsoft::WRL::ComPtr <ID3D11Device> device;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> device_context;

	ImGuizmo::OPERATION CurrentOperation = ImGuizmo::OPERATION::TRANSLATE;
};