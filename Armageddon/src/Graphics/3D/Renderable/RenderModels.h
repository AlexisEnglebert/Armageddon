#pragma once
#include "Object.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../../../Includes.h"
#include "Mesh.h"
class DLL RenderModels : public Object
{
public:
	
	RenderModels(const char* modelPath ,Microsoft::WRL::ComPtr <ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> device_context) : device(device), device_context(device_context)
	{
		Armageddon::Log::GetLogger()->trace("STATRING LAODING MODEL");
		SetName("Model");
		Assimp::Importer imp;
		const aiScene* model = imp.ReadFile(modelPath, aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_ConvertToLeftHanded |
			aiProcess_GenNormals |
			aiProcess_CalcTangentSpace);



		ProcessNode(model->mRootNode, model);
		const aiMesh* pMesh = model->mMeshes[0];
		Armageddon::Log::GetLogger()->trace(" LAODING MODEL : Enzo.obj");

		



		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
		{"POSITION",0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},

		};
		InitShaders(device, L"..\\bin\\Debug-x64\\Armageddon\\", L"VertexShader.cso", L"PixelShader.cso", layout, ARRAYSIZE(layout));

		InitConstantBuffer(device.Get(), device_context.Get());
		CustomDraw = true;
		Armageddon::Log::GetLogger()->error("FINISHED INIT RENDERMODEL");

	};
	

	void ProcessNode(aiNode* node, const aiScene* scene)
	{
		//Armageddon::Log::GetLogger()->trace("PROCESS NODE");

		for (UINT i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			modelMeshes.push_back(ProcessMesh(mesh, scene));
		}
		for (UINT i = 0; i < node->mNumChildren; i++)
		{
			this->ProcessNode(node->mChildren[i], scene);
		}
	};

	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> v;
		std::vector<DWORD> indices;
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			
			if (mesh->mTextureCoords[0])
			{
				if (mesh->HasNormals())
				{
				

					v.push_back({ mesh->mVertices[i].x,mesh->mVertices[i].y ,mesh->mVertices[i].z,mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y,mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z });
				}
				else
				{
				///	v.push_back({ mesh->mVertices[i].x,mesh->mVertices[i].y ,mesh->mVertices[i].z,mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y});
				}
			}
			
		}
		for (UINT o = 0; o < scene->mNumMaterials; o++)
		{
			
			Armageddon::Log::GetLogger()->info("Material {0}", o);
		}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (UINT j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}

		}
		
		return Mesh(device.Get(), device_context.Get(), v, indices);
	};


	virtual void Draw(ID3D11DeviceContext* device_contex)
	{
		BindShader(device_contex);
		BindConstantBuffer(device_contex);
		BindInputLayout(device_contex);
		for (UINT i = 0; i < modelMeshes.size(); i++)
		{
		 modelMeshes[i].Draw();
		}
	};
	virtual void UpDateConstantBuffer(Camera* cam,DirectX::XMMATRIX Transform)
	{
		DirectX::XMMATRIX WorldMat = DirectX::XMMatrixIdentity();

		f += 0.01f;

		Armageddon::Log::GetLogger()->trace("F = {0}", f);
		WorldMat = DirectX::XMMatrixRotationY(f);

		//ConstantBuffer.data.LightDir = {0.0f,0.0f,f};
		ConstantBuffer.data.color = {1.0f};
		ConstantBuffer.data.mat = WorldMat * cam->GetViewMatrix() * cam->GetProjectionMatrix() * Transform;
		ConstantBuffer.data.Yoffset = 0;
		ConstantBuffer.data.Xoffset = 0;
	}

	std::vector<Mesh> GetMeshArray()
	{
		return this->modelMeshes;
	};

	~RenderModels()
	{
		modelMeshes.clear();
		v.clear();
		indices.clear();
	}
private:
	std::vector<Vertex> v;
	std::vector<DWORD> indices;
	std::vector<Mesh> modelMeshes;
	Microsoft::WRL::ComPtr <ID3D11Device>& device;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext>& device_context;
};

