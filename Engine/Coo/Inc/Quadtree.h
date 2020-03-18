#ifndef INCLUDED_COOENGINE_QUADTREE_H
#define INCLUDED_COOENGINE_QUADTREE_H

#include "Common.h"

namespace Coo 
{
	class Quadtree 
	{
	public:
		struct Node
		{
			std::array<std::unique_ptr<Node>, 4> children;
			std::vector<size_t> indices;
			Coo::Math::AABB aabb;
			bool isLeaf = false;
			void DrawNode() {}
		};

		using Visitor = std::function<bool(Node*)>;
		
		void Initialize(const Coo::Graphics::Mesh* mesh, size_t levels);
		void Visit(Visitor& visiter);
		bool Intersect(const Coo::Math::Ray& ray, float& distance) const;

	private:
		enum Quadrant{BL, BR, TL, TR};
		void GenerateNodeRecursive(std::unique_ptr<Node>& node, const Coo::Math::AABB& aabb, size_t levels);
		void VisitRecursive(Node* node, Visitor& visitor);
		bool IntersectRecursive(Node* node, const Coo::Math::Ray& ray, float& distance)const;

		const Graphics::Mesh* mMesh = nullptr;
		std::unique_ptr<Node> mRoot = nullptr;
	};
} // namespace Coo

#endif // !INCLUDED_COOENGINE_QUADTREE_H
