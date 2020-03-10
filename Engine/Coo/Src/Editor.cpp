#include "Precompiled.h"
#include "Editor.h"
#include "GameObject.h"
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
		ImGui::Selectable("Targeting", false);
		ImGui::Selectable("Navigation", false);
		ImGui::Selectable("Physics", false);
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
			}
		}
		ImGui::TreePop();
	}
	ImGui::End();
}

void Coo::Editor::ShowInspectorView()
{
	ImGui::Begin("Inspector");
	if (mSelectedGameObject) 
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
						ImGui::DragFloat3(metaField->GetName(), data);
					}
				}
			}
		}
	}
	ImGui::End();
}
