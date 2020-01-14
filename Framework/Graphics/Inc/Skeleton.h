#ifndef INCLUDED_COOENGINE_GRAPHICS_SKELETON_H
#define INCLUDED_COOENGINE_GRAPHICS_SKELETON_H

#include "Bone.h"
#include "ConstantBuffer.h"

namespace Coo::Graphics 
{
	class Animator;
	struct Skeleton
	{
		Bone* root = nullptr;
		std::vector<std::unique_ptr<Bone>> bones;

		bool Save(std::string fileName);
		bool Load(std::string fileName);

		void Initialize();
		void Terminate();
		void Bind(const Animator* animator);
		void DebugUI();

	private:
		void BoneDebugUI(Bone* b);
		struct BoneTransformData
		{
			Coo::Math::Matrix4 boneTransforms[128];
		};
		using BoneTransformBuffer = TypedConstantBuffer<BoneTransformData>;
		BoneTransformBuffer mBoneTransformBuffer;
		BoneTransformData mBoneTransform;
		
		void ApplyTransform(Bone* bone, const Coo::Math::Matrix4& transform,const Animator* animator);
	};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_SKELETON_H
