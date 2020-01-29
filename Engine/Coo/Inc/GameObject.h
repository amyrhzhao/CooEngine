#ifndef INCLUDED_COOENGINE_GAMEOBJECT_H
#define INCLUDED_COOENGINE_GAMEOBJECT_H

namespace Coo 
{
	class Component;

	class GameObject final 
	{
	public:
		GameObject() = default;

		void Initialize();
		void Terminate();

		void Update(float deltaTime);
		void Render();
		void DebugUI();

		void SetName(const char* name) { mName = name; }
		const char* GetName() const { return mName.c_str(); }

		template<class T, class = std::void_t<std::is_base_of<Component,T>>>
		T* AddComponent()
		{
			auto& newComponent = mComponents.emplace_back(std::make_unique<T>());
			newComponent->mOwner = this;
			return static_cast<T*>(newComponent.get());
		}

	private:
		using Components = std::vector<std::unique_ptr<Component>>;

		Components mComponents;
		std::string mName;
	};

} // namespace Coo

#endif // !INCLUDED_COOENGINE_GAMEOBJECT_H
