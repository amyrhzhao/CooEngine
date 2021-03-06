#include "Precompiled.h"
#include "Editor.h"
#include "GameObject.h"
#include "Service.h"
#include "World.h"

using namespace Coo;
using namespace Coo::Math;

Coo::Editor::Editor(World& world) 
	:mWorld(world)
{
}

void Coo::Editor::ShowHierarchyView()
{
	ImGui::Begin("World");
	if (ImGui::TreeNode("Services")) 
	{
		for (auto& service : mWorld.mServices)
		{
			if (ImGui::Selectable(service->GetMetaClass()->GetName(), service.get() == mSelectedService)) 
			{
				mSelectedService = service.get();
				mSelectedGameObject = nullptr;
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Cameras"))
	{
		ImGui::Selectable("Main", false);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("GameObjects")) 
	{
		for (auto gameObject : mWorld.mUpdateList) 
		{
			if (ImGui::Selectable(gameObject->GetName(), gameObject == mSelectedGameObject)) 
			{
				mSelectedGameObject = gameObject;
				mSelectedService = nullptr;
			}
		}
		ImGui::TreePop();
	}
	ImGui::End();
}

void Coo::Editor::ShowInspectorView()
{
	ImGui::Begin("Inspector");
	if (mSelectedService) 
	{
		mSelectedService->DebugUI();
	}
	else if (mSelectedGameObject) 
	{
		for (auto& component : mSelectedGameObject->mComponents) 
		{
			auto metaClass = component->GetMetaClass();
			if (ImGui::CollapsingHeader(metaClass->GetName(), ImGuiTreeNodeFlags_DefaultOpen)) 
			{
				for (size_t i = 0; i < metaClass->GetFieldsCount(); ++i) 
				{
					auto metaField = metaClass->GetField(i);
					if (metaField->GetMetaType() == Core::Meta::DeduceType<Vector3>()) 
					{
						float* data = (float*)((uint8_t*)component.get() + metaField->GetOffset());
						ImGui::DragFloat3(metaField->GetName(), data, 0.01f);
					}
				}
			}
		}
	}
	ImGui::End();
}
