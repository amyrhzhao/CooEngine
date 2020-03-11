#include "Precompiled.h"
#include "Quadtree.h"

using namespace Coo;

void Quadtree::Initialize(const Graphics::Mesh* mesh, size_t levels)
{
	mMesh = mesh;

	// Get the bound of the mesh
	Math::AABB bound = Graphics::MeshBuilder::ComputeBound(*mesh);

	// Generate nodes up to number of levels
	GenerateNodeRecursive(mRoot, bound, levels);

	// Assign triangle
	const auto& vertices = mesh->vertices;
	const auto& indices = mesh->indices;
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		const auto& a = vertices[indices[i + 0]].position;
		const auto& b = vertices[indices[i + 1]].position;
		const auto& c = vertices[indices[i + 2]].position;
	
		const float minX = Math::min(Math::min(a.x, b.x), c.x);
		const float maxX = Math::max(Math::max(a.x, b.x), c.x);
		const float minY = Math::min(Math::min(a.y, b.y), c.y);
		const float maxY = Math::max(Math::max(a.y, b.y), c.y);
		const float minZ = Math::min(Math::min(a.z, b.z), c.z);
		const float maxZ = Math::max(Math::max(a.z, b.z), c.z);
	
		const Math::AABB aabb = Math::AABB::FromMinMax({ minX, minY, minZ }, { maxX, maxY, maxZ });

		Visitor visitor = [a, b, c, aabb, i](Node* node) 
		{
			if (!Math::Intersect(aabb, node->aabb)) 
			{
				return false;
			}
			if (!node->isLeaf) 
			{
				return true;
			}
			if (Math::Intersect(a, node->aabb) ||
				Math::Intersect(b, node->aabb) ||
				Math::Intersect(c, node->aabb) ||
				Math::Intersect((a + b) * 0.5f, node->aabb) ||
				Math::Intersect((a + c) * 0.5f, node->aabb) ||
				Math::Intersect((b + c) * 0.5f, node->aabb))
			{
				node->indices.push_back(i);
			}
			return false;
		};
		Visit(visitor);
	}
}

void Coo::Quadtree::Visit(Visitor& visiter)
{
	VisitRecursive(mRoot.get(), visiter);
}

bool Coo::Quadtree::Intersect(const Coo::Math::Ray& ray, float &distance) const
{
	if (mRoot == nullptr) { return false; }
	
	distance = std::numeric_limits<float>::max();
	return IntersectRecursive(mRoot.get(), ray, distance);
}

void Coo::Quadtree::GenerateNodeRecursive(std::unique_ptr<Node>& node, const Coo::Math::AABB& aabb, size_t levels)
{
	node = std::make_unique<Node>();
	node->aabb = aabb;

	if (--levels == 0) 
	{
		node->isLeaf = true;
	}
	else
	{
		const Math::Vector3& center = aabb.center;
		Math::Vector3 half = {aabb.extend.x * 0.5f, aabb.extend.y, aabb.extend.z * 0.5f};
		Math::AABB bl = { {center.x - half.x, center.y, center.z - half.z}, half };
		Math::AABB br = { {center.x + half.x, center.y, center.z - half.z}, half };
		Math::AABB tl = { {center.x - half.x, center.y, center.z + half.z}, half };
		Math::AABB tr = { {center.x + half.x, center.y, center.z + half.z}, half };
		GenerateNodeRecursive(node->children[BL], bl, levels);
		GenerateNodeRecursive(node->children[BR], br, levels);
		GenerateNodeRecursive(node->children[TL], tl, levels);
		GenerateNodeRecursive(node->children[TR], tr, levels);
	}
}

void Coo::Quadtree::VisitRecursive(Node* node, Visitor& visitor)
{
	if (visitor(node) && !node->isLeaf)
	{
		for (auto& child : node->children) 
		{
			VisitRecursive(child.get(), visitor);
		}
	}
}

bool Coo::Quadtree::IntersectRecursive(Node* node, const Coo::Math::Ray& ray, float& distance) const
{
	float distEntry, distExit;
	if (!Math::Intersect(ray, node->aabb, distEntry, distExit)) 
	{
		return false;
	}
	if (!node->isLeaf) 
	{
		return IntersectRecursive(node->children[0].get(), ray, distance) ||
			IntersectRecursive(node->children[1].get(), ray, distance) ||
			IntersectRecursive(node->children[2].get(), ray, distance) ||
			IntersectRecursive(node->children[3].get(), ray, distance);
	}

	bool intersect = false;
	for (size_t i = 0; i < node->indices.size(); ++i) 
	{
		size_t faceIndex = node->indices[i];
		const auto& a = mMesh->vertices[mMesh->indices[faceIndex + 0]];
		const auto& b = mMesh->vertices[mMesh->indices[faceIndex + 1]];
		const auto& c = mMesh->vertices[mMesh->indices[faceIndex + 2]];

		float d = 0.0f;
		if (Math::Intersect(ray, a.position, b.position, c.position, distance))
		{
			distance = Math::min(distance, d);
			intersect = true;
		}
	}
	return intersect;
}
