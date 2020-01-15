#include "Precompiled.h"
#include "Animator.h"
#include "Skeleton.h"
#include "ImGui/Inc/imgui.h"

using namespace Coo::Graphics;

// [offset][bone chain...][inverse_root][to_world][to_view][to_NDC]
void Skeleton::ApplyTransform(Bone* bone, const Coo::Math::Matrix4& transform, const Animator* animator)
{
	Coo::Math::Matrix4 trans = transform;
	if (bone == nullptr) return;
	if (bone->parent)
	{
		Coo::Math::Matrix4 toParent;
		if (animator && animator->GetBoneTransform(bone->index, toParent))
		{
			trans = toParent * transform;
		}
		else
		{
			trans = bone->toParentTransform * transform;
		}
	}
	mBoneTransform.boneTransforms[bone->index] = bone->offsetTransform * trans;
	for (size_t i = 0; i < bone->children.size(); ++i)
	{
		ApplyTransform(bone->children[i], trans, animator);
	}
}


bool Skeleton::Save(std::string fileName)
{
	std::filesystem::path path(fileName);
	path.replace_extension("skeleton");

	FILE* skeletonFile = nullptr;
	auto success = fopen_s(&skeletonFile, path.string().c_str(), "wb");
	if (success != 0 || skeletonFile == nullptr)
	{
		LOG("File to create file");
		return false;
	}

	Core::MemoryStream outStream;
	Core::StreamWriter sw(outStream);

	size_t numBone = bones.size();
	sw.Write(numBone);
	for (size_t i = 0; i < numBone; ++i)
	{
		// name
		sw.Write(bones[i]->name);
		// index
		sw.Write(bones[i]->index);
		// parentIndex 
		sw.Write(bones[i]->parentIndex);
		// children
		size_t numChild = bones[i]->childIndices.size();
		sw.Write(numChild);
		for (size_t j = 0; j < numChild; ++j)
		{
			sw.Write(bones[i]->childIndices[j]);
		}
		// toParentTransform
		for (size_t j = 0; j < 16; ++j)
		{
			sw.Write(bones[i]->toParentTransform.v[j]);
		}
		// offsetTransform
		for (size_t j = 0; j < 16; ++j)
		{
			sw.Write(bones[i]->offsetTransform.v[j]);
		}
	}
	fwrite(outStream.GetData(), outStream.GetSize(), 1, skeletonFile);
	fclose(skeletonFile);
	return true;
}

bool Skeleton::Load(std::string fileName)
{
	FILE* inputFile;
	auto success = fopen_s(&inputFile, fileName.c_str(), "rb");
	if (success != 0)
	{
		LOG("fail to open skeleton file");
		return false;
	}

	// loading bones
	size_t numBone;
	fread_s(&numBone, sizeof(size_t), sizeof(size_t), 1, inputFile);
	bones.resize(numBone);
	for (size_t i = 0; i < numBone; ++i)
	{
		bones[i] = std::make_unique<Bone>();
		// name
		int lenName;
		fread_s(&lenName, sizeof(int), sizeof(int), 1, inputFile);
		bones[i]->name.resize(lenName);
		fread_s(bones[i]->name.data(), lenName, lenName, 1, inputFile);
		// index
		fread_s(&bones[i]->index, sizeof(int), sizeof(int), 1, inputFile);
		// parentIndex
		fread_s(&bones[i]->parentIndex, sizeof(int), sizeof(int), 1, inputFile);
		// children
		size_t numChild;
		fread_s(&numChild, sizeof(size_t), sizeof(size_t), 1, inputFile);
		bones[i]->childIndices.resize(numChild);
		bones[i]->children.resize(numChild);
		for (size_t j = 0; j < numChild; ++j)
		{
			fread_s(&bones[i]->childIndices[j], sizeof(int), sizeof(int), 1, inputFile);
		}
		// toParentTransform
		for (size_t j = 0; j < 16; ++j)
		{
			fread_s(&bones[i]->toParentTransform.v[j], sizeof(float), sizeof(float), 1, inputFile);
		}
		// offsetTransform
		for (size_t j = 0; j < 16; ++j)
		{
			fread_s(&bones[i]->offsetTransform.v[j], sizeof(float), sizeof(float), 1, inputFile);
		}
	}
	fclose(inputFile);
	return true;
}

void Skeleton::Initialize()
{
	for (size_t i = 0; i < bones.size(); ++i)
	{
		auto parentIndex = bones[i]->parentIndex;
		// parent
		if (parentIndex != -1)
		{
			bones[i]->parent = bones[parentIndex].get();
		}
		else
		{
			root = bones[i].get();
		}
		// children
		for (size_t j = 0; j < bones[i]->childIndices.size(); ++j)
		{
			auto childIndex = bones[i]->childIndices[j];
			bones[i]->children[j] = bones[childIndex].get();
		}
	}
	mBoneTransformBuffer.Initialize();
}

void Skeleton::Terminate()
{
	mBoneTransformBuffer.Terminate();
}

void Skeleton::Bind(const Animator* animator)
{
	ApplyTransform(root, root->toParentTransform, animator);
	for (size_t i = 0; i < bones.size(); ++i)
	{
		mBoneTransform.boneTransforms[i] = Coo::Math::Transpose(mBoneTransform.boneTransforms[i]);
	}
	mBoneTransformBuffer.Set(mBoneTransform);
	mBoneTransformBuffer.BindVS(4);
}

void Coo::Graphics::Skeleton::DebugUI()
{
	ImGui::Begin("Skeleton", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	Bone* b = root;
	BoneDebugUI(b);
	ImGui::End();
}

void Coo::Graphics::Skeleton::BoneDebugUI(Bone* b)
{
	if (b == nullptr) { return; }
	if (ImGui::TreeNode( (b->name +" "+ std::to_string(b->index)).c_str()))
	{
		//ImGui::CollapsingHeader(b->name.c_str());
		for (size_t i = 0; i < b->children.size(); ++i)
		{
			BoneDebugUI(b->children[i]);
		}
		ImGui::TreePop();
	}
}


