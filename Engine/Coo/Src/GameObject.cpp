#include "Precompiled.h"
#include "GameObject.h"
#include "Component.h"

using namespace Coo;

META_CLASS_BEGIN(Coo::GameObject)
	META_NO_FIELD
META_CLASS_END

void GameObject::Initialize()
{
	for (auto& component : mComponents) 
	{
		component->Initialize();
	}
}

void GameObject::Terminate()
{
	for (auto& component : mComponents)
	{
		component->Terminate();
	}
}

void GameObject::Update(float deltaTime)
{
	for (auto& component : mComponents)
	{
		component->Update(deltaTime);
	}
}

void GameObject::Render()
{
	for (auto& component : mComponents)
	{
		component->Render();
	}
}

void GameObject::DebugUI()
{
	for (auto& component : mComponents)
	{
		component->DebugUI();
	}
}
