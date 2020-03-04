#include "Precompiled.h"
#include "Editor.h"
#include "GameObject.h"
#include "World.h"

using namespace Coo;

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
		for (auto& Component : mSelectedGameObject->mComponents) 
		{

		}
	}
}
