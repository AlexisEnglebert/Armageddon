#pragma once
#include "Object.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <DDSTextureLoader.h>
#include "../Material.h"
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
			aiProcess_CalcTangentSpace)
			;
		if (model == nullptr)
		{
			Armageddon::Log::GetLogger()->error(imp.GetErrorString());
		}

		ProcessNode(model->mRootNode, model);

		const aiMesh* pMesh = model->mMeshes[0];
		Armageddon::Log::GetLogger()->trace(" LAODING MODEL :{0}",modelPath);

		



		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
		{"POSITION",0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	    {"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},



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
			for (UINT i = 0; i < mesh->mNumBones; i++)
			{
				Armageddon::Log::GetLogger()->trace("Bones name: {0}", (const char*)mesh->mBones[i]->mName.C_Str());
			}
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

					if (mesh->HasTangentsAndBitangents())
					{
						v.push_back({ mesh->mVertices[i].x,mesh->mVertices[i].y ,mesh->mVertices[i].z,mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y,mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z ,(float)mesh->mTangents->x, (float)mesh->mTangents->y,(float)mesh->mTangents->z,mesh->mBitangents->x,mesh->mBitangents->y ,mesh->mBitangents->z });
					}
					else
					{
						v.push_back({ mesh->mVertices[i].x,mesh->mVertices[i].y ,mesh->mVertices[i].z,mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y,mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z});

					}
				}
				else
				{
				///	v.push_back({ mesh->mVertices[i].x,mesh->mVertices[i].y ,mesh->mVertices[i].z,mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y});
				}
			}
			
		}
		//for (UINT o = 0; o < scene->mNumMaterials; o++)
		//{
			
			if (mesh->mMaterialIndex >= 0)
			{
				auto& mat = scene->mMaterials[mesh->mMaterialIndex];
				//for (int i = 0; i < mat->mNumProperties; i++)
				//{

				auto& property =  mat->mProperties[0];
				
					mat->Get(AI_MATKEY_NAME, m_mname);
					
				Armageddon::Log::GetLogger()->info("Material {0}", (const char*)m_mname.C_Str());
				
					mat->GetTexture(aiTextureType_DIFFUSE, 0,&(aiString)Albedoname);
					mat->GetTexture(aiTextureType_NORMALS, 0,&(aiString)Normalname);
					mat->GetTexture(aiTextureType_SPECULAR, 0,&(aiString)Specname);
					mat->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0,&(aiString)Occlusionname);
					//Armageddon::Log::GetLogger()->info("Material {0}", textname.C_Str());
						

				//}

			}
		//	Armageddon::Log::GetLogger()->info("Material {0}", o);
		//}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (UINT j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}

		}
		
		const auto i_mesh = Mesh(device.Get(), device_context.Get(), v, indices, 
			(const char*)m_mname.C_Str(),"a", "a", "a", "a");
			ObjectMaterials.push_back(i_mesh.mat);

		return i_mesh;
	};


	virtual void Draw(ID3D11DeviceContext* device_contex)
	{

		
		BindShader(device_contex);
		BindConstantBuffer(device_contex);
		BindInputLayout(device_contex);
		ObjectMaterials[0].BindRessources();
		for (UINT i = 0; i < modelMeshes.size(); i++)
		{
		 modelMeshes[i].Draw();
		}
	};
	float rot = 0;
	virtual void UpDateConstantBuffer(Camera* cam)
	{
		DirectX::XMMATRIX WorldMat = DirectX::XMMatrixIdentity();
	//	Armageddon::Log::GetLogger()->trace("METALNESS : {0}", metalNess);
	//	rot += 45;
		//ConstantBuffer.data.LightDir = { 0.0f,0.0f,DirectX::XMConvertToRadians(rot) };
		//ConstantBuffer.data.color = {1.0f};
		//QConstantBuffer.data.LightDir = { 0.25f, 0.5f, -1.0f };
	
		ConstantBuffer.data.WorldMat = (WorldMat * this->TransformMat);
		ConstantBuffer.data.ProjectionMat = cam->GetProjectionMatrix();
		ConstantBuffer.data.ViewMat = cam->GetViewMatrix();
		ConstantBuffer.data.MVP = cam->GetViewMatrix() * cam->GetProjectionMatrix();
		//ConstantBuffer.data.TransformMat = WorldMat;

		ConstantBuffer.data.Yoffset = 0;
		ConstantBuffer.data.Xoffset = 0;
		ConstantBuffer.data.LightDir = LightDirection;
		ConstantBuffer.data.CameraPos = { cam->GetPos().x * cam->GetRotation().x ,cam->GetPos().y ,cam->GetPos().z};
		//ConstantBuffer.data.EyePos = cam->getpo
		ConstantBuffer.data.RoughNess = Roughness;
		ConstantBuffer.data.MetalNess = metalNess;

		lightBuffer.data.LightIntensity = 1.0f;

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
	aiString m_mname;
	std::string Albedoname;
	std::string Normalname;
	std::string Specname;
	std::string Occlusionname;
	Microsoft::WRL::ComPtr <ID3D11Device>& device;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext>& device_context;


	Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState;
	/*SkyBox*/

	
};

