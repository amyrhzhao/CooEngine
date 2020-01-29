#ifndef INCLUDED_COOENGINE_COMPONENT_H
#define INCLUDED_COOENGINE_COMPONENT_H

namespace Coo 
{
	class GameObject;

	class Component
	{
	public:
		Component();
		virtual ~Component() {}

		virtual void Initialize() {}
		virtual void Terminate() {}
		
		virtual void Update(float deltaTime) {}
		virtual void Render() {}
		virtual void DebugUI() {}

		GameObject& GetOwner() { return *mOwner; }
		const GameObject& GetOwner() const { return *mOwner; }

	private:
		friend class GameObject;
		GameObject* mOwner = nullptr;
	};
} // namespace Coo

#endif // !INCLUDED_COOENGINE_COMPONENT_H
