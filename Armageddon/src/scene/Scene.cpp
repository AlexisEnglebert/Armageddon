#include "Scene.h"
#include "Entity.h"
#include "imgui_internal.h"



void Scene::UpdateScene()
{
	auto group = g_registry.group<TransFormComponent>(entt::get<TagComponent>);
	//Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->Begin();
	for (auto ent : group) // iterator qui vas check tt les components
	{
		
		if (ent != entt::null)
		{
			
		//Armageddon::Log::GetLogger()->trace("test" );
			auto entity = GetEntityById(ent);
	
			if (entity)
			{
				
				/* Check if the entity has one drawable component*/
				if (entity.HasComponent<MaterialComponent>())
				{
					auto& Material = g_registry.get<MaterialComponent>(ent);
					//Material.Mat.BindRessources();

				}
				if (entity.HasComponent<ModelComponent>() || entity.HasComponent<MeshComponent>() || entity.HasComponent<SkyBoxComponent>())
				{

					auto object = this->DrawableEntity.find((int)ent);
					object->second.get()->LightDirection = LightDir;
					object->second.get()->UpDateConstantBuffer(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera());
					object->second.get()->Draw(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDeviceContext().Get());
				//	Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects()[i].get()->UpDateConstantBuffer(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera());

					
				}


				if (entity.HasComponent<TransFormComponent>())
				{
					auto& transform = g_registry.get<TransFormComponent>(ent);
					auto transforment = this->DrawableEntity.find((int)ent);
					transforment->second.get()->SetTransformMatrix(transform.GetTransformMatrix());

					//entity.GetEntityObject()->SetTransformMatrix(transform.GetTransformMatrix());
				}
				
				
			}
		}
		

	}
}

Entity Scene::CreateEntity()
{
	Entity ent = { g_registry.create(), this };
	EntityList.push_back(ent);
	return ent;
}

Entity& Scene::GetEntityById(entt::entity ent)
{
	for (auto& entity : EntityList)
	{

		if (entity.GetHandle() == ent)
		{
			return entity;
		}
	}
}
Entity& Scene::GetSelectedEntity()
{
	for (auto& entity : EntityList)
	{

		if (entity.selected)
		{
			return entity;
		}
	}
}
static bool objectSelected = false;

void Scene::DrawGameObjectScene()
{
	ImGui::Begin("Game Objects", 0, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoDecoration);
	bool Entityselected = false;
	bool SkyBoxselected = false;


	DirectX::XMFLOAT4X4 ViewMat;
	DirectX::XMStoreFloat4x4(&ViewMat, Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetViewMatrix());
	ImGuiWindow* scene_window = ImGui::FindWindowByName("Scene");//291952657
	auto regionrect = scene_window->ContentRegionRect;

	ImVec2 vMin = regionrect.Min;
	ImVec2 vMax = regionrect.Max;
	ImGuizmo::ViewManipulate(&ViewMat.m[0][0], 3, { vMax.x - 100, vMin.y }, { 100,100 }, IM_COL32_BLACK_TRANS);
	if (ImGui::BeginMenu("Add"))
	{
		ImGui::MenuItem("Default Entity", NULL, &Entityselected);
		if (Entityselected)
		{	
			auto ent = CreateEntity();
			//ent.SetModel("..\\TestApplication\\assets\\Models\\suzanne.obj");
		//	ent.AddComponent<ModelComponent>("C:\\Users\\Desktop\\c++ project\\Armageddon\\TestApplication\\assets\\Models\\Cerberus_by_Andrew_Maximov\\Cerberus_LP.FBX", DrawableEntity, ent.GetHandle());
		//	ent.AddComponent<ModelComponent>("..\\TestApplication\\assets\\Models\\Cerberus_by_Andrew_Maximov\\Cerberus_LP.FBX", DrawableEntity, ent.GetHandle());
			//ent.AddComponent<ModelComponent>("..\\TestApplication\\assets\\Models\\sponza.obj",DrawableEntity,ent.GetHandle());
			ent.AddComponent<ModelComponent>("..\\TestApplication\\assets\\Models\\fbx\\Dragon_Baked_Actions_fbx_7.4_binary.fbx",DrawableEntity,ent.GetHandle());
			ent.AddComponent<TagComponent>("Model");
			ent.AddComponent<MaterialComponent>(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDevice(), Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDeviceContext());
			ent.AddComponent<TransFormComponent>();
		//	auto obj = DrawableEntity.find((int)ent.GetHandle());
		//	auto& mat =  obj->second.get().
		//	mat.SetColorMap(L"..\\TestApplication\\assets\\Models\\Cerberus_by_Andrew_Maximov\\Textures\\Cerberus_A.tif");
			//mat.SetNormalMap(L"..\\TestApplication\\assets\\Models\\Cerberus_by_Andrew_Maximov\\Textures\\Cerberus_N.tif");
			//mat.SetOcclusionMap(L"..\\TestApplication\\assets\\Models\\Cerberus_by_Andrew_Maximov\\Textures\\Cerberus_M(0).tif");
			//mat.SetSpecularMap(L"..\\TestApplication\\assets\\Models\\Cerberus_by_Andrew_Maximov\\Textures\\Cerberus_R(S).tif");
			//skybox = en
			//mat.Mat.IrSkyBoxTextRessource =  this->DrawableEntity.find((int)0)->second.get().;

		}
		/*ImGui::MenuItem("SkyBox", NULL, &SkyBoxselected);
		if (SkyBoxselected)
		{
			auto ent = CreateEntity();

			//ent.SetModel("..\\TestApplication\\assets\\Models\\suzanne.obj");
			//ent.AddComponent<TagComponent>("SkyBox");
			ent.AddComponent<SkyBoxComponent>(L"..\\TestApplication\\assets\\Textures\\skybox\\skybox1.dds");
			//ent.AddComponent<TransFormComponent>();
			Armageddon::Log::GetLogger()->trace("Create New Entity");
		}*/
		ImGui::EndMenu();
		ImGui::Separator();
	}


	/*if (ImGui::GetIO().MouseClicked[1])
	{
		if (ImGui::GetIO().MousePos.x > vMin.x && ImGui::GetIO().MousePos.x < vMax.x && ImGui::GetIO().MousePos.y> vMin.y && ImGui::GetIO().MousePos.y < vMax.y)
		{
			Armageddon::Log::GetLogger()->trace("click");

				ImGui::BeginPopup("test", ImGuiPopupFlags_AnyPopup | ImGuiPopupFlags_MouseButtonRight);
				ImGui::Text("oof"),

					Armageddon::Log::GetLogger()->trace("click");
			//	ImGui::EndPopup();


		}
	}*/
	/*if (ImGui::BeginPopupContextWindow("test",ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_AnyPopupId))
	{
		bool selected = false;
		if (ImGui::BeginMenu("Add"))
		{
			ImGui::MenuItem("Entity", NULL,&selected);
			if (selected)
			{
				auto ent = CreateEntity();
				Armageddon::Log::GetLogger()->trace("Create New Entity");
			}
			ImGui::EndMenu();
		}
		ImGui::EndPopup();

		//Armageddon::Log::GetLogger()->trace("click");
	}*/
	g_registry.each([&](auto EntId)
	{
		auto& ent = GetEntityById(EntId);

		DrawEntity(ent);
	});
	//ImGui::PushStyleColor(ImGuiColorEditFlags_)
	ImGui::End();

	ImGui::Begin("Entity Info", 0, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoDecoration);

	
	if (SelectedEntity != entt::null)
	{
		DrawEntityInfo(GetEntityById(SelectedEntity));
	}

		
	

	ImGui::End();

	ImGui::Begin(" Scene Debug", 0, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoDecoration);
	ImGui::SliderFloat("Sun X", &LightDir.x, 0.0f, 1.0f);
	//LightDir.x = DirectX::XMConvertToDegrees(LightDir.x);
	/*LightDir.y = DirectX::XMConvertToRadians(-LightDir.x);
	LightDir.z = DirectX::XMConvertToRadians(LightDir.x);*/
	//LightDir.y = DirectX::XMQuaternionRotationRollPitchYaw(LightDir.x,0.0f,0.0f).m128_f32[0];
	//LightDir.z = DirectX::XMQuaternionRotationRollPitchYaw(LightDir.x, 0.0f, 0.0f).m128_f32[0];
	ImGui::SliderFloat("Sun Y", &LightDir.y, 0.0f, 1.0f);
	ImGui::SliderFloat("Sun Z", &LightDir.z, -1.0f, 10.0f);

	bool TranslateOperation = ImGuizmo::OPERATION::TRANSLATE == CurrentOperation;
	bool RotateOperation = ImGuizmo::OPERATION::ROTATE == CurrentOperation;
	bool ScaleOperation = ImGuizmo::OPERATION::SCALE == CurrentOperation;

	if (ImGui::Checkbox("Transform", &TranslateOperation))
	{
		CurrentOperation = ImGuizmo::OPERATION::TRANSLATE;
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Rotate", &RotateOperation))
	{
		CurrentOperation = ImGuizmo::OPERATION::ROTATE;

	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Scale", &ScaleOperation))
	{
		CurrentOperation = ImGuizmo::OPERATION::SCALE;

	}
	ImGui::SameLine();
	ImGui::End();

	ImGui::Begin("Material", 0, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoDecoration);

	if (SelectedEntity != entt::null)
	{
		
		
			auto object = this->DrawableEntity.find((int)SelectedEntity);
			auto& vMat = object->second.get()->ObjectMaterials;
			for (int i = 0; i < vMat.size(); i++)
			{
				//Armageddon::Log::GetLogger()->trace(vMat.size());
				//ImGui::Text(vMat[i].MaterialName);
				if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)SelectedEntity, ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen, vMat[i].MaterialName))
				{
					//auto& ent = GetEntityById(SelectedEntity);

					//		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });

					ImGui::Columns(2, 0, false);
					ImGui::SetColumnWidth(0, 150.0f);
					char AlbedoName[255];
					ImGui::Image(vMat[i].GetColorMap(), { 50,50 });
					ImGui::SameLine();
					ImGui::Text("Albedo");
					//ImGui::SameLine();
					ImGui::NextColumn();
					strcpy_s(AlbedoName, vMat[i].Albedo.c_str());
					if (ImGui::InputText("##Albedomap", AlbedoName, sizeof(AlbedoName), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly))
					{
					}
					ImGui::SameLine();
					if (ImGui::Button("##1...", { 30.0f,20.0f }))
					{
						std::string test = Armageddon::Application::GetInstance()->GetWindow()->openfilename();
						if (!test.empty())
						{


							Armageddon::Log::GetLogger()->info(test.c_str());
							vMat[i].Albedo = test;

							size_t origsizes = strlen(vMat[i].Albedo.c_str()) + 1;
							const size_t newsizes = 500;
							size_t convertedCharP = 0;
							wchar_t constTowchar[500];
							mbstowcs_s(&convertedCharP, constTowchar, origsizes, vMat[i].Albedo.c_str(), _TRUNCATE);
							Armageddon::Log::GetLogger()->trace((void*)constTowchar);
							//vMat[i].SetColorMap(constTowchar);
							vMat[i].SetColorMap(constTowchar);
						}
					}
					ImGui::Columns();

					ImGui::Columns(2, 0, false);
					ImGui::SetColumnWidth(0, 150.0f);
					char NormalName[255];
					ImGui::Image(vMat[i].GetNormalMap(), { 50,50 });
					ImGui::SameLine();
					ImGui::Text("Normal");
					ImGui::NextColumn();

					strcpy_s(NormalName, vMat[i].Normal.c_str());
					if (ImGui::InputText("##NormalMap", NormalName, sizeof(NormalName), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly))
					{


					}
					ImGui::SameLine();
					if (ImGui::Button("##2...", { 30.0f,20.0f }))
					{
						std::string normalFilePath = Armageddon::Application::GetInstance()->GetWindow()->openfilename();
						if (!normalFilePath.empty())
						{
							Armageddon::Log::GetLogger()->info(normalFilePath.c_str());
							vMat[i].Normal = normalFilePath;

							size_t origsizes = strlen(vMat[i].Normal.c_str()) + 1;
							const size_t newsizes = 500;
							size_t convertedCharP = 0;
							wchar_t constTowchar[500];
							mbstowcs_s(&convertedCharP, constTowchar, origsizes, vMat[i].Normal.c_str(), _TRUNCATE);
							Armageddon::Log::GetLogger()->trace((void*)constTowchar);
							//vMat[i].SetColorMap(constTowchar);
							vMat[i].SetNormalMap(constTowchar);
						}
					}

					ImGui::Columns();

					ImGui::Columns(2, 0, false);
					ImGui::SetColumnWidth(0, 150.0f);
					char SpecName[255];

					ImGui::Image(vMat[i].GetSpecularMap(), { 50,50 });
					ImGui::SameLine();
					ImGui::Text("Specular map");
					ImGui::NextColumn();

					strcpy_s(SpecName, vMat[i].Spec.c_str());
					if (ImGui::InputText("##SpecularMap", SpecName, sizeof(SpecName), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly))
					{


					}
					ImGui::SameLine();
					if (ImGui::Button("##3...", { 30.0f,20.0f }))
					{
						std::string SpecularFilePath = Armageddon::Application::GetInstance()->GetWindow()->openfilename();
						if (!SpecularFilePath.empty())
						{
							Armageddon::Log::GetLogger()->info(SpecularFilePath.c_str());
							vMat[i].Spec = SpecularFilePath;



							size_t origsizes = strlen(vMat[i].Spec.c_str()) + 1;
							const size_t newsizes = 500;
							size_t convertedCharP = 0;
							wchar_t constTowchar[500];
							mbstowcs_s(&convertedCharP, constTowchar, origsizes, vMat[i].Spec.c_str(), _TRUNCATE);
							Armageddon::Log::GetLogger()->trace((void*)constTowchar);
							//vMat[i].SetColorMap(constTowchar);
							vMat[i].SetSpecMap(constTowchar);
						}
					}					
					ImGui::Columns();


					ImGui::Columns(2, 0, false);
					ImGui::SetColumnWidth(0, 150.0f);
					char AOname[255];

					ImGui::Image(vMat[i].GetAOMap(), { 50,50 });
					ImGui::SameLine();
					ImGui::Text("Ambiant occlusion");
					ImGui::NextColumn();

					strcpy_s(AOname, vMat[i].Occlusion.c_str());
					if (ImGui::InputText("##AOmap", AOname, sizeof(AOname), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly))
					{


					}
					ImGui::SameLine();
					if (ImGui::Button("##4...", { 30.0f,20.0f }))
					{
						std::string AOFilePath = Armageddon::Application::GetInstance()->GetWindow()->openfilename();
						if (!AOFilePath.empty())
						{
							Armageddon::Log::GetLogger()->info(AOFilePath.c_str());
							vMat[i].Occlusion = AOFilePath;

							size_t origsizes = strlen(vMat[i].Occlusion.c_str()) + 1;
							const size_t newsizes = 500;
							size_t convertedCharP = 0;
							wchar_t constTowchar[500];
							mbstowcs_s(&convertedCharP, constTowchar, origsizes, vMat[i].Occlusion.c_str(), _TRUNCATE);
							Armageddon::Log::GetLogger()->trace((void*)constTowchar);
							//vMat[i].SetColorMap(constTowchar);
							vMat[i].SetOcclusionMap(constTowchar);
						}
					}						
					ImGui::Columns();

					ImGui::TreePop();

				}
			}

	}
	
	ImGui::End();

}
static float test = 1;
static bool testa = false;
void Scene::DrawEntity(Entity& ent)
{
	if (ent)
	{	
		if (ent.HasComponent<TagComponent>())
		{

			auto& hasTag = ent.GetComponent<TagComponent>();
			
			
				if (ImGui::Selectable(hasTag.TagName.c_str(), selectedEntID == (int)ent.GetHandle(), ImGuiSelectableFlags_SelectOnClick))
				{
					selectedEntID = (int)ent.GetHandle();
					SelectedEntity = ent.GetHandle();
					ent.selected = true;

				}
				else
				{
					ent.selected = false;
				}
			
			
		/*	if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent.GetHandle(),ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Framed, hasTag.TagName.c_str()))
			{
				char entityName[255];
				ImGui::Text("Entity Name: ");
				ImGui::SameLine();
				strcpy_s(entityName, hasTag.TagName.c_str());
				ImGui::InputText("##EntityName", entityName, sizeof(entityName), ImGuiInputTextFlags_AllowTabInput);
				if (ent.HasComponent<TransFormComponent>())
				{
					
					DrawTransFormComponent(ent);
				}
				if (ent.HasComponent<ModelComponent>())
				{
					DrawModelComponent(ent);

				}
				ImGui::ButtonEx("AddComponent", { 100,25 });

				ImGui::TreePop();

			}*/
		}
	

	}
}

void Scene::DrawEntityInfo(Entity& ent)
{
		
		if (ent.HasComponent<TagComponent>())
		{

			auto& hasTag = ent.GetComponent<TagComponent>();
			char entityName[255];
			ImGui::Text("Entity Name: ");
			ImGui::SameLine();
			strcpy_s(entityName, hasTag.TagName.c_str());
			if (ImGui::InputText("##EntityName", entityName, sizeof(entityName), ImGuiInputTextFlags_AllowTabInput))
			{
				std::string tagname(entityName);
				hasTag.TagName = tagname;
			}
			ImGui::Separator();
			if (ent.HasComponent<TransFormComponent>())
			{

				DrawTransFormComponent(ent);
				ImGui::Separator();
			}
			if (ent.HasComponent<ModelComponent>())
			{
				DrawModelComponent(ent);
				ImGui::Separator();

			}
		
		}
		if (ImGui::ButtonEx("AddComponent", { 100,25 }))
		{
			Armageddon::Log::GetLogger()->trace("BUTTON");
			if (ImGui::BeginPopup("test", ImGuiPopupFlags_AnyPopup | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoDecoration))
			{
				Armageddon::Log::GetLogger()->trace("TEST PEPOPE");
			}
			
			
		//	ImGui::EndPopup();
			
		}
	


}

void Scene::DrawTransFormComponent(Entity& ent)
{
	auto& component = ent.GetComponent<TransFormComponent>();

	if (ImGui::TreeNodeEx(component.ComponentName, ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen))
	{
		DirectX::XMFLOAT4X4 ViewMat;
		DirectX::XMFLOAT4X4 ProjMat;
		DirectX::XMFLOAT4X4 WorldMat;
		DirectX::XMFLOAT4X4 TransformMat;
		const auto& camera = Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera();
		DirectX::XMMATRIX projection = camera->GetProjectionMatrix();
		DirectX::XMMATRIX View = camera->GetViewMatrix();
	//	const float* retest = camera->GetProjectionMatrix().r->m128_f32;
		auto object = this->DrawableEntity.find((int)ent);
		
		DirectX::XMStoreFloat4x4(&ViewMat, View);
		DirectX::XMStoreFloat4x4(&ProjMat, projection);
		DirectX::XMStoreFloat4x4(&WorldMat, object->second.get()->GetWorldMat()); //WorldMat
		DirectX::XMStoreFloat4x4(&TransformMat, component.GetTransformMatrix());

		ImGuiWindow* scene_window = ImGui::FindWindowByName("Scene");//291952657
	

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::Enable(true);
	
	
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text("Translate");
		//ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f,0.3215686275f,0.3215686275f,1.0f});
		ImGui::NextColumn();
		ImGui::Button("X", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::DragFloat("##X", &component.Translation.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333333333f,0.968627451f,0.1607843137,1.0f });
		ImGui::Button("Y", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("##Y", &component.Translation.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333333333f,0.5764705882f,0.968627451,1.0f });
		ImGui::Button("Z", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("##Z", &component.Translation.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		//ImGui::PopItemWidth();
		ImGui::Columns();


		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text("Rotation");
	//	ImGui::SameLine();
		ImGui::NextColumn();
		ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f,0.3215686275f,0.3215686275f,1.0f });
		ImGui::Button("X", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::DragFloat("###X##", &component.Rotation.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333333333f,0.968627451f,0.1607843137,1.0f });
		ImGui::Button("Y", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("###Y##", &component.Rotation.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333333333f,0.5764705882f,0.968627451,1.0f });
		ImGui::Button("Z", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("###Z##", &component.Rotation.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::Columns();
			

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text("Scaling");
		//ImGui::SameLine();
		ImGui::NextColumn();
		ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f,0.3215686275f,0.3215686275f,1.0f });
		ImGui::Button("X", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::DragFloat("####X", &component.Scale.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333333333f,0.968627451f,0.1607843137,1.0f });
		ImGui::Button("Y", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("####Y", &component.Scale.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333333333f,0.5764705882f,0.968627451,1.0f });
		ImGui::Button("Z", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("####Z", &component.Scale.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::NewLine();

		ImGui::PopStyleVar();
		ImGui::Columns();
		

		auto regionrect = scene_window->ContentRegionRect;
		ImVec2 vMin = regionrect.Min;
		ImVec2 vMax = regionrect.Max;
		scene_window->DrawList->AddRect(vMin, vMax, IM_COL32(255, 255, 0, 255));
	
		
		ImGuizmo::SetRect(vMin.x, vMin.y, static_cast<float>(Armageddon::Application::GetInstance()->GetWindow()->GetWindowWidth()),
			static_cast<float>(Armageddon::Application::GetInstance()->GetWindow()->GetWindowHeight()));
		if (ImGuizmo::Manipulate(*ViewMat.m, *ProjMat.m, CurrentOperation, ImGuizmo::LOCAL, *TransformMat.m))
		{
			switch (CurrentOperation)
			{
			case ImGuizmo::OPERATION::TRANSLATE : 
				component.Translation.x = TransformMat.m[3][0];
				component.Translation.y = TransformMat.m[3][1];
				component.Translation.z = TransformMat.m[3][2];
				break;
			case ImGuizmo::OPERATION::ROTATE : 
				component.Rotation.x = TransformMat.m[0][2];
				component.Rotation.y = TransformMat.m[1][1];
				component.Rotation.z = TransformMat.m[2][2];
				break;
			case ImGuizmo::OPERATION::SCALE :
				component.Scale.x = TransformMat.m[0][0];
				component.Scale.y = TransformMat.m[1][1];
				component.Scale.z = TransformMat.m[2][2];
				break;
			default:
				break;
			}

		}
		
		
			
		ImGui::TreePop();
	}
}

void Scene::DrawModelComponent(Entity& ent)
{
	auto& component = ent.GetComponent<ModelComponent>();

	if (ImGui::TreeNodeEx(component.ComponentName, ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen))
	{
		char ModelName[255];
		ImGui::Text("Model Path ");
		ImGui::SameLine();
		strcpy_s(ModelName, component.ModelPath);
		ImGui::InputText("##EntityName", ModelName, sizeof(ModelName), ImGuiInputTextFlags_AllowTabInput);

		ImGui::TreePop();
	}
}
