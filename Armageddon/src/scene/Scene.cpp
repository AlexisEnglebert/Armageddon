#include "Scene.h"
#include "Entity.h"
#include "imgui_internal.h"

Scene::Scene() // on vas initialiser l'entity component system
{
}

void Scene::UpdateScene()
{
	auto group = g_registry.group<TransFormComponent>(entt::get<TagComponent>);

	for (auto ent : group) // iterator qui vas check tt les components
	{
		
		if (ent != entt::null)
		{
			
			auto entity = GetEntityById(ent);
	
			if (entity)
			{
				if (entity.HasComponent<TransFormComponent>())
				{
					auto& transform = g_registry.get<TransFormComponent>(ent);

				
					


					Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects()[(int)entity.GetHandle()]->SetTransformMatrix(transform.GetTransformMatrix());
					//entity.GetEntityObject()->SetTransformMatrix(transform.GetTransformMatrix());
				}
			}
		/*	auto& transform = g_registry.get<TransFormComponent>(ent);
			auto& model = g_registry.get<ModelComponent>(ent);
			Armageddon::Log::GetLogger()->trace(transform.Translation.x);
			
			model.object->SetTransformMatrix(transform.GetTransformMatrix());*/
		}
		
		

	}
}

Entity Scene::CreateEntity()
{
	Entity ent = { g_registry.create(), this };
	ent.AddComponent<TagComponent>("Default");
	EntityList.push_back(ent);
	return ent;
}

Entity Scene::GetEntityById(entt::entity ent)
{
	for (auto& entity : EntityList)
	{

		if (entity.GetHandle() == ent)
		{
			return entity;
		}
	}
}

void Scene::DrawGameObjectScene()
{
	
	ImGui::Begin("Game Objects",0,ImGuiWindowFlags_HorizontalScrollbar |ImGuiWindowFlags_NoDecoration);
	bool selected = false;

	if (ImGui::BeginMenu("Add"))
	{
		ImGui::MenuItem("Entity", NULL, &selected);
		if (selected)
		{
			auto ent = CreateEntity();
			
			//ent.SetModel("..\\TestApplication\\assets\\Models\\suzanne.obj");
			ent.AddComponent<ModelComponent>("..\\TestApplication\\assets\\Models\\suzanne.obj",ent.GetHandle());
			ent.AddComponent<TransFormComponent>();
			Armageddon::Log::GetLogger()->trace("Create New Entity");
		}
		ImGui::EndMenu();
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
		Entity ent = GetEntityById(EntId);

		DrawEntity(ent);
	});
	//ImGui::PushStyleColor(ImGuiColorEditFlags_)
	ImGui::End();
}
static float test = 1;
void Scene::DrawEntity(Entity& ent)
{
	if (ent)
	{
		if (ent.HasComponent<TagComponent>())
		{

			auto& hasTag = ent.GetComponent<TagComponent>();
			if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent.GetHandle(),ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_Framed, hasTag.TagName.c_str()))
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

			}
		}
	

	}
}

void Scene::DrawTransFormComponent(Entity& ent)
{
	auto& component = ent.GetComponent<TransFormComponent>();

	if (ImGui::TreeNodeEx(component.ComponentName, ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Selected))
	{
		ImGui::Text("Transform Component");
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });
		ImGui::Text("Translation");
		ImGui::SameLine();
		ImGui::Button("X", { 20.0f,20.0f });
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::DragFloat("##X", &component.Translation.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::Button("Y", { 20.0f,20.0f });
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("##Y", &component.Translation.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::Button("Z", { 20.0f,20.0f });
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("##Z", &component.Translation.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		//ImGui::PopItemWidth();


		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::Button("X", { 20.0f,20.0f });
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::DragFloat("###X##", &component.Rotation.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::Button("Y", { 20.0f,20.0f });
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("###Y##", &component.Rotation.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::Button("Z", { 20.0f,20.0f });
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("###Z##", &component.Rotation.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();


		ImGui::Text("Scale");
		ImGui::SameLine();
		ImGui::Button("X", { 20.0f,20.0f });
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::DragFloat("####X", &component.Scale.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::Button("Y", { 20.0f,20.0f });
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("####Y", &component.Scale.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::Button("Z", { 20.0f,20.0f });
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("####Z", &component.Scale.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::NewLine();

		ImGui::PopStyleVar();


		ImGui::TreePop();
	}
}

void Scene::DrawModelComponent(Entity& ent)
{
	auto& component = ent.GetComponent<ModelComponent>();

	if (ImGui::TreeNodeEx(component.ComponentName, ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Selected))
	{
		char ModelName[255];
		ImGui::Text("Model Path ");
		ImGui::SameLine();
		strcpy_s(ModelName, component.ModelPath);
		ImGui::InputText("##EntityName", ModelName, sizeof(ModelName), ImGuiInputTextFlags_AllowTabInput);

		ImGui::TreePop();
	}
}
