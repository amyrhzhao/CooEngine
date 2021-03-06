#ifndef INCLUDED_COOENGINE_EDITOR_H
#define INCLUDED_COOENGINE_EDITOR_H

namespace Coo 
{
	class World;
	class GameObject;
	class Service;

	class Editor 
	{
	public:
		Editor(World& world);

		void ShowHierarchyView();
		void ShowInspectorView();

	private:
		World& mWorld;

		Service* mSelectedService;
		GameObject* mSelectedGameObject;
	};
}

#endif // !INCLUDED_COOENGINE_EDITOR_H
