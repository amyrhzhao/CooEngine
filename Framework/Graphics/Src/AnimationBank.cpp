#include "Precompiled.h"
#include "AnimationClip.h"
#include "AnimationBank.h"

using namespace Coo::Graphics;
namespace
{
	std::string remove_extension(const std::string& filename) {
		size_t lastdot = filename.find_last_of(".");
		if (lastdot == std::string::npos) return filename;
		return filename.substr(0, lastdot);
	}
}
bool AnimationBank::Save(std::string fileName)
{
	std::filesystem::path path(fileName);
	path.replace_extension("animation");

	FILE* animationFile = nullptr;
	auto success = fopen_s(&animationFile, path.string().c_str(), "wb");
	if(success != 0 || animationFile == nullptr)
	{
		LOG("File to create file");
		return false;
	}

	Core::MemoryStream outStream;
	Core::StreamWriter sw(outStream);

	size_t numAnimClip = animationClips.size();
	sw.Write(numAnimClip);
	for (size_t clipIndex = 0; clipIndex < numAnimClip; ++clipIndex) 
	{
		auto& animClip = animationClips[clipIndex];
		// name 
		sw.Write(animClip->name);
		// duration
		sw.Write(animClip->duration);
		// ticksPersecond
		sw.Write(animClip->ticksPersecond);
		// boneAnimations
		size_t numAnimations = animClip->boneAnimations.size();
		sw.Write(numAnimations);
		for (size_t animIndex = 0; animIndex < numAnimations; ++animIndex) 
		{
			auto& boneAnim = animClip->boneAnimations[animIndex];
			// check empty
			bool isEmpty;
			isEmpty = (boneAnim.get() == nullptr);
			sw.Write(isEmpty);
			if (isEmpty) { continue; }
			// Bone animation
			sw.Write(boneAnim->boneName);
			auto& anim = boneAnim->animation;
			// position
			size_t numPos = anim.mPositionKeyframes.size();
			sw.Write(numPos);
			for (size_t i = 0; i < numPos; ++i) 
			{
				auto& key = anim.mPositionKeyframes[i];
				// time
				sw.Write(key.time);
				// value
				sw.Write(key.value.x);
				sw.Write(key.value.y);
				sw.Write(key.value.z);
			}
			// rotation
			size_t numRot = anim.mRotationKeyframes.size();
			sw.Write(numRot);
			for (size_t i = 0; i < numRot; ++i) 
			{
				auto& key = anim.mRotationKeyframes[i];
				// time
				sw.Write(key.time);
				// value
				sw.Write(key.value.x);
				sw.Write(key.value.y);
				sw.Write(key.value.z);
				sw.Write(key.value.w);
			}
			// scale
			size_t numScale = anim.mScaleKeyframes.size();
			sw.Write(numScale);
			for (size_t i = 0; i < numScale; ++i) 
			{
				auto& key = anim.mScaleKeyframes[i];
				// time
				sw.Write(key.time);
				// value
				sw.Write(key.value.x);
				sw.Write(key.value.y);
				sw.Write(key.value.z);
			}
		}
	}
	fwrite(outStream.GetData(), outStream.GetSize(), 1, animationFile);
	fclose(animationFile);
	return true;
}

bool AnimationBank::Load(std::string fileName, bool useFileName)
{
	FILE* animationFile = nullptr;
	auto success = fopen_s(&animationFile, fileName.c_str(), "rb");
	if (success != 0) 
	{
		LOG("fail to open animation file");
		return false;
	}

	// loading animation clips
	size_t numAnimClip;
	size_t startIndex = animationClips.size();
	fread_s(&numAnimClip, sizeof(size_t), sizeof(size_t), 1, animationFile);
	numAnimClip = numAnimClip + animationClips.size();
	animationClips.resize(numAnimClip);
	for (size_t clipIndex = startIndex; clipIndex < numAnimClip; ++clipIndex) 
	{
		auto& animClip = animationClips[clipIndex];
		animClip = std::make_unique<AnimationClip>();
		// name
		int lenName;
		fread_s(&lenName, sizeof(int), sizeof(int), 1, animationFile);
		std::string animName;
		animName.resize(lenName);
		fread_s(animName.data(), lenName, lenName, 1, animationFile);
		if(useFileName)
		{
			std::string file = std::filesystem::path(fileName).filename().string();
			animName = remove_extension(file);
		}
		animClip->name = animName;
		// duration
		fread_s(&animClip->duration, sizeof(float), sizeof(float), 1, animationFile);
		// ticksPersecond
		fread_s(&animClip->ticksPersecond, sizeof(float), sizeof(float), 1, animationFile);
		// boneAnimations
		size_t numAnimations;
		fread_s(&numAnimations, sizeof(size_t), sizeof(size_t), 1, animationFile);
		animClip->boneAnimations.resize(numAnimations);
		for(size_t animIndex = 0; animIndex < numAnimations; ++animIndex)
		{
			// check empty
			bool isEmpty;
			fread_s(&isEmpty, sizeof(bool), sizeof(bool), 1, animationFile);
			if (isEmpty) { continue; }
			// Bone animation
			auto& boneAnim = animClip->boneAnimations[animIndex];
			boneAnim = std::make_unique<BoneAnimation>();
			int lenBoneName;
			fread_s(&lenBoneName, sizeof(int), sizeof(int), 1, animationFile);
			boneAnim->boneName.resize(lenBoneName);
			fread_s(boneAnim->boneName.data(), lenBoneName, lenBoneName, 1, animationFile);
			// position
			size_t numPos;
			fread_s(&numPos, sizeof(size_t), sizeof(size_t), 1, animationFile);
			boneAnim->animation.mPositionKeyframes.resize(numPos);
			for (size_t i = 0; i < numPos; ++i) 
			{
				auto& key = boneAnim->animation.mPositionKeyframes[i];
				// time
				fread_s(&key.time, sizeof(float), sizeof(float), 1, animationFile);
				// value
				fread_s(&key.value.x, sizeof(float), sizeof(float), 1, animationFile);
				fread_s(&key.value.y, sizeof(float), sizeof(float), 1, animationFile);
				fread_s(&key.value.z, sizeof(float), sizeof(float), 1, animationFile);
			}
			// Rotation
			size_t numRot;
			fread_s(&numRot, sizeof(size_t), sizeof(size_t), 1, animationFile);
			boneAnim->animation.mRotationKeyframes.resize(numRot);
			for (size_t i = 0; i < numRot; ++i)
			{
				auto& key = boneAnim->animation.mRotationKeyframes[i];
				// time
				fread_s(&key.time, sizeof(float), sizeof(float), 1, animationFile);
				// value
				fread_s(&key.value.x, sizeof(float), sizeof(float), 1, animationFile);
				fread_s(&key.value.y, sizeof(float), sizeof(float), 1, animationFile);
				fread_s(&key.value.z, sizeof(float), sizeof(float), 1, animationFile);
				fread_s(&key.value.w, sizeof(float), sizeof(float), 1, animationFile);
			}
			// Scale
			size_t numScale;
			fread_s(&numScale, sizeof(size_t), sizeof(size_t), 1, animationFile);
			boneAnim->animation.mScaleKeyframes.resize(numScale);
			for (size_t i = 0; i < numScale; ++i)
			{
				auto& key = boneAnim->animation.mScaleKeyframes[i];
				// time
				fread_s(&key.time, sizeof(float), sizeof(float), 1, animationFile);
				// value
				fread_s(&key.value.x, sizeof(float), sizeof(float), 1, animationFile);
				fread_s(&key.value.y, sizeof(float), sizeof(float), 1, animationFile);
				fread_s(&key.value.z, sizeof(float), sizeof(float), 1, animationFile);
			}
		}
	}
	fclose(animationFile);
	return true;
}
