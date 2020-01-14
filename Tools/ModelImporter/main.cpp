#include <Graphics/Inc/Graphics.h>
#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags
#include <cstdio>

using Coo::Math::Vector3;
using BoneIndexLookup = std::map<std::string, int>; // Used to lookup bone by name

namespace 
{
	bool debug = true;
	const char* input = "../../Assets/Models/ybot.fbx";
	const char* output = "../../Assets/Models/ybot.coo";
}

struct Arguments
{
	const char* inputFileName = nullptr;
	const char* outputFileName = nullptr;
};

std::optional<Arguments> ParseArgs(int argc, char* argv[])
{
	if (debug) 
	{
		Arguments args;
		args.inputFileName = input;
		args.outputFileName = output;
		return args;
	}

	// We need at least 3 arguments
	if (argc < 3)
	{
		return std::nullopt;
	}

	Arguments args;
	args.inputFileName = argv[argc - 2];
	args.outputFileName = argv[argc - 1];
	return args;
}

void PrintUsage()
{
	printf(
		"== ModuleImporterHelp == \n"
		"\n"
		"Usage:\n"
		"    ModelImporter.exe [Options] <InputFile> <OutputFile> \n"
		"\n"
		"Options:\n"
		"    <none>\n"
		"\n"
	);
}

Coo::Math::Vector3 Convert(const aiVector3D& v)
{
	return Coo::Math::Vector3(v.x, v.y, v.z);
}

Coo::Math::Quaternion Convert(const aiQuaternion& q)
{
	return Coo::Math::Quaternion(q.x, q.y, q.z, q.w);
}

Coo::Math::Matrix4 Convert(const aiMatrix4x4& m)
{
	Coo::Math::Matrix4 mat = *(reinterpret_cast<const Coo::Math::Matrix4*>(&m));
	return Coo::Math::Transpose(mat);
}

// Check if inputBone exists in skeleton, if so just return the index
// Otherwise, add it to the skeleton. The aiBone mush have a name
int TryAddBone(const aiBone* inputBone, Coo::Graphics::Skeleton& skeleton, BoneIndexLookup& boneIndexLookup) 
{
	std::string name = inputBone->mName.C_Str();
	ASSERT(!name.empty(), "Error: inputBone has no name!");
	auto iter = boneIndexLookup.find(name);
	if (iter != boneIndexLookup.end())
		return iter->second;

	// Add a new bone in the skeleton for this
	auto& newBone = skeleton.bones.emplace_back(std::make_unique<Coo::Graphics::Bone>());
	newBone->name = std::move(name);
	newBone->index = static_cast<int>(skeleton.bones.size()) - 1;
	newBone->offsetTransform = Convert(inputBone->mOffsetMatrix);

	// Cache the bone index
	boneIndexLookup.emplace(newBone->name, newBone->index);
	return newBone->index;
}

// Recursively walk the aiScene tree and add/link bones to our skeleton as we find them 
Coo::Graphics::Bone* BuildSkeleton(const aiNode& sceneNode, Coo::Graphics::Bone* parent, Coo::Graphics::Skeleton& skeleton, BoneIndexLookup& boneIndexLookUp) 
{
	Coo::Graphics::Bone* bone = nullptr;
	std::string name = sceneNode.mName.C_Str();
	auto iter = boneIndexLookUp.find(name);
	if (iter != boneIndexLookUp.end()) 
	{
		// Bone already exists
		bone = skeleton.bones[iter->second].get();
	}
	else
	{
		// Add a new bone in the skeleton for this (possible need to generate a name for it)
		bone = skeleton.bones.emplace_back(std::make_unique<Coo::Graphics::Bone>()).get();
		bone->index = static_cast<int>(skeleton.bones.size()) - 1;
		bone->offsetTransform = Coo::Math::Matrix4::Identity();
		if (name.empty()) 
		{
			bone->name = "NoName" + std::to_string(bone->index);
		}
		else
		{
			bone->name = std::move(name);
		}

		// Cache the bone index
		boneIndexLookUp.emplace(bone->name, bone->index);
	}

	bone->parent = parent;
	bone->parentIndex = bone->parent ? bone->parent->index : -1;
	bone->toParentTransform = Convert(sceneNode.mTransformation);

	for (uint32_t i = 0; i < sceneNode.mNumChildren; ++i) 
	{
		Coo::Graphics::Bone* child = BuildSkeleton(*sceneNode.mChildren[i], bone, skeleton, boneIndexLookUp);
		bone->children.push_back(child);
		bone->childIndices.push_back(child->index);
	}
	return bone;
}

void SaveTexture(const aiMaterial* inputMaterial, uint32_t materialIndex, const aiScene* scene, const Arguments& args, Coo::Graphics::Model& model, aiTextureType textureType);
const char* GetTextureName(aiTextureType textureType);
const uint32_t Ai2CooTextureType(aiTextureType textureType);

int main(int argc, char* argv[])
{
	const auto argsOpt = ParseArgs(argc, argv);
	if (!argsOpt.has_value())
	{
		PrintUsage();
		return -1;
	}

	const auto& args = argsOpt.value();

	// Create an instance of the importer class to do the parsing for us
	Assimp::Importer importer;

	// Try to import the model into a scene
	const aiScene* scene = importer.ReadFile(args.inputFileName,
		aiProcessPreset_TargetRealtime_Quality |
		aiProcess_ConvertToLeftHanded);
	if (scene == nullptr)
	{
		printf("Error: %s\n", importer.GetErrorString());
		return -1;
	}

	Coo::Graphics::Model model;
	Coo::Graphics::Skeleton skeleton;
	BoneIndexLookup boneIndexLookUp;
	Coo::Graphics::AnimationBank animationBank;
	//std::vector<std::unique_ptr<Coo::Graphics::AnimationClip>> animationClips;

	// Look for mesh data
	if (scene->HasMeshes())
	{
		printf("Reading mesh data...\n");
		uint32_t numMeshes = scene->mNumMeshes;
		
		model.mMeshData.resize(numMeshes);

		for (uint32_t meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
		{
			const aiMesh* inputMesh = scene->mMeshes[meshIndex];
			model.mMeshData[meshIndex].materialIndex = inputMesh->mMaterialIndex;

			uint32_t numVertices = inputMesh->mNumVertices;
			printf("Reading vertices...\n");

			std::vector<Coo::Graphics::BoneVertex> vertices(numVertices);
			std::vector<uint32_t> indices;

			const aiVector3D* positions = inputMesh->mVertices;
			const aiVector3D* normals = inputMesh->mNormals;
			const aiVector3D* tangents = inputMesh->mTangents;
			const auto textureCoords = inputMesh->mTextureCoords;

			for (uint32_t i = 0; i < numVertices; ++i)
			{
				Coo::Graphics::BoneVertex vertex;
				if (inputMesh->HasPositions())
				{
					vertex.position = Convert(positions[i]);
				}
				if (inputMesh->HasNormals())
				{
					vertex.normal = Convert(normals[i]);
				}
				if (inputMesh->HasTangentsAndBitangents())
				{
					vertex.tangent = Convert(tangents[i]);
				}
				if (inputMesh->HasTextureCoords(0))
				{
					Vector3 texcoord = Convert(inputMesh->mTextureCoords[0][i]);
					vertex.texcoord = Coo::Math::Vector2(texcoord.x, texcoord.y);
				}
				vertices[i] = vertex;
			}

			const aiFace* faces = inputMesh->mFaces;
			for (uint32_t i = 0; i < inputMesh->mNumFaces; ++i)
			{
				for (uint32_t j = 0; j < inputMesh->mFaces[i].mNumIndices; ++j) 
				{
					indices.push_back(static_cast<uint32_t>(faces[i].mIndices[j]));
				}
			}
			
			if (inputMesh->HasBones()) 
			{
				printf("Reading bone weights...\n");
				
				// Track how may weights have we added to each vertex so far
				std::vector<int> numWeight(vertices.size(), 0);
				for (uint32_t meshBoneIndex = 0; meshBoneIndex < inputMesh->mNumBones; ++meshBoneIndex)
				{
					aiBone* inputBone = inputMesh->mBones[meshBoneIndex];
					int boneIndex = TryAddBone(inputBone, skeleton, boneIndexLookUp);

					for (uint32_t weightIndex = 0; weightIndex < inputBone->mNumWeights; ++weightIndex) 
					{
						const aiVertexWeight& weight = inputBone->mWeights[weightIndex];
						auto& vertex = vertices[weight.mVertexId];
						auto& count = numWeight[weight.mVertexId];
						
						// our vertices can hold at most up to 4 weights
						// todo: save all of them and normalize when saving
						if (count < 4) 
						{
							vertex.boneIndices[count] = boneIndex;
							vertex.boneWeights[count] = weight.mWeight;
							++count;
						}
					}
				}
			}

			model.mMeshData[meshIndex].mesh.vertices = vertices;
			model.mMeshData[meshIndex].mesh.indices = indices;
		}
	}

	// Look for material data
	if (scene->HasMaterials())
	{
		printf("Reading materials...\n");
		uint32_t numMaterials = scene->mNumMaterials;
		model.mMaterialData.resize(numMaterials);
		for (uint32_t materialIndex = 0; materialIndex < numMaterials; ++materialIndex)
		{
			const aiMaterial* inputMaterial = scene->mMaterials[materialIndex];
	
			model.mMaterialData[materialIndex].textureData.resize(static_cast<size_t>(Coo::Graphics::Model::TextureType::Unknown));
			// For now we are only interested in the first diffuse maps
			for (uint32_t textureIndex = 0; textureIndex < aiTextureType_UNKNOWN; ++textureIndex)
			{
				SaveTexture(inputMaterial, materialIndex, scene, args, model, static_cast<aiTextureType>(textureIndex));
			}
		}
	
	}

	// Check if we have skeleton information
	if (!skeleton.bones.empty())
	{
		printf("Building skeleton...\n");
		BuildSkeleton(*scene->mRootNode, nullptr, skeleton, boneIndexLookUp);
	}

	// Look for animation data
	if (scene->HasAnimations()) 
	{
		printf("Reading animations...\n");
		for (uint32_t animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex) 
		{
			const aiAnimation* inputAnim = scene->mAnimations[animIndex];
			auto& animClip = animationBank.animationClips.emplace_back(std::make_unique<Coo::Graphics::AnimationClip>());

			if (inputAnim->mName.length > 0) 
			{
				animClip->name = inputAnim->mName.C_Str();
			}
			else
			{
				animClip->name = "Anim" + std::to_string(animIndex);
			}

			animClip->duration = static_cast<float>(inputAnim->mDuration);
			animClip->ticksPersecond = static_cast<float>(inputAnim->mTicksPerSecond);

			printf("Reading bone animations for %s ...\n", animClip->name.c_str());
			
			// Reserve space so we have oen animation slot per bone, note that not 
			// all bones will have animations so some slot will stay empty. However,
			// keeping them the same size means we can use bone index directly to look
			// up animations.
			animClip->boneAnimations.resize(skeleton.bones.size());
			for (uint32_t boneAnimIndex = 0; boneAnimIndex < inputAnim->mNumChannels; ++boneAnimIndex) 
			{
				const aiNodeAnim* inputBoneAnim = inputAnim->mChannels[boneAnimIndex];
				int slotIndex = boneIndexLookUp[inputBoneAnim->mNodeName.C_Str()];
				auto& boneAnim = animClip->boneAnimations[slotIndex];
				boneAnim = std::make_unique<Coo::Graphics::BoneAnimation>();
				boneAnim->boneName = inputBoneAnim->mNodeName.C_Str();
				Coo::Graphics::AnimationBuilder builder;
				for (uint32_t keyIndex = 0; keyIndex < inputBoneAnim->mNumPositionKeys; ++keyIndex) 
				{
					auto& key = inputBoneAnim->mPositionKeys[keyIndex];
					builder.AddPositionKey(Convert(key.mValue), static_cast<float>(key.mTime));
				}
				for (uint32_t keyIndex = 0; keyIndex < inputBoneAnim->mNumRotationKeys; ++keyIndex)
				{
					auto& key = inputBoneAnim->mRotationKeys[keyIndex];
					builder.AddRotationKey(Convert(key.mValue), static_cast<float>(key.mTime));
				}
				for (uint32_t keyIndex = 0; keyIndex < inputBoneAnim->mNumScalingKeys; ++keyIndex)
				{
					auto& key = inputBoneAnim->mScalingKeys[keyIndex];
					builder.AddScaleKey(Convert(key.mValue), static_cast<float>(key.mTime));
				}

				boneAnim->animation = builder.Build();
			}
		}
	}

	// Save Model
	if (!model.Save(args.outputFileName)) 
	{
		printf("Failed saving model");
	}

	// Save Skeleton
	if (!skeleton.Save(args.outputFileName)) 
	{
		printf("Failed saving skeleton");
	}

	// Save AnimationBank
	/// AnimationGroup/AnimationBank/AnimationSet ==> AnimClip ==> BoneAnim ==> PosKey, RotKey, ScaleKey
	if(!animationBank.Save(args.outputFileName))
	{
		printf("Failed saving animation");
	}

	printf("All done!");
	return 0;
}

void SaveTexture(const aiMaterial* inputMaterial, uint32_t materialIndex, const aiScene* scene, const Arguments& args, Coo::Graphics::Model& model, aiTextureType textureType)
{
	const uint32_t textureCount = inputMaterial->GetTextureCount(textureType);
	if (textureCount > 0)
	{
		aiString texturePath;
		if (inputMaterial->GetTexture(textureType, 0, &texturePath) == aiReturn::aiReturn_SUCCESS)
		{
			const aiTexture* inputTexture = scene->GetEmbeddedTexture(texturePath.C_Str());

			if (inputTexture)
			{
				std::string fileName = args.inputFileName;
				fileName.erase(fileName.length() - 4); // remove ".fbx" extension
				fileName += std::to_string(materialIndex);
				fileName += ".";
				fileName += GetTextureName(textureType);

				ASSERT(scene->HasTextures(), "Error: No embedded texture found!");

				if (!inputTexture->CheckFormat("jpg") && !inputTexture->CheckFormat("png"))
				{
					ASSERT(false, "Error: Unrecogized texture format.");
				}

				printf("Extracting embedded texture %s\n", fileName.c_str());

				std::string fullFileName = args.outputFileName;
				fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
				fullFileName += fileName;

				FILE* file = nullptr;
				fopen_s(&file, fullFileName.c_str(), "wb");
				size_t written = fwrite(inputTexture->pcData, 1, inputTexture->mWidth, file);
				ASSERT(written == inputTexture->mWidth, "Error: Failed to extract embedded texture!");
				fclose(file);

				model.mMaterialData[materialIndex].textureData[Ai2CooTextureType(textureType)].textureName = fileName;
			}
			else
			{
				std::filesystem::path filePath = texturePath.C_Str();
				std::string filename = filePath.filename().u8string();
				printf("Adding texture %s\n", filename.c_str());
				model.mMaterialData[materialIndex].textureData[Ai2CooTextureType(textureType)].textureName = filename.c_str();
			}
		}
	}
}

const char* GetTextureName(aiTextureType textureType)
{
	switch (textureType)
	{
	case aiTextureType_DIFFUSE:
		return "diffuse";
		break;
	case aiTextureType_SPECULAR:
		return "specular";
		break;
	case aiTextureType_AMBIENT:
		return "ambient";
		break;
	case aiTextureType_EMISSIVE:
		return "emissive";
		break;
	case aiTextureType_HEIGHT:
		return "height";
		break;
	case aiTextureType_NORMALS:
		return "normals";
		break;
	case aiTextureType_SHININESS:
		return "shininess";
		break;
	case aiTextureType_OPACITY:
		return "opacity";
		break;
	case aiTextureType_DISPLACEMENT:
		return "displacement";
		break;
	case aiTextureType_LIGHTMAP:
		return "lightmap";
		break;
	case aiTextureType_REFLECTION:
		return "reflection";
		break;
	case aiTextureType_BASE_COLOR:
		return "base_color";
		break;
	case aiTextureType_NORMAL_CAMERA:
		return "normal_camera";
		break;
	case aiTextureType_EMISSION_COLOR:
		return "emission_color";
		break;
	case aiTextureType_METALNESS:
		return "metalness";
		break;
	case aiTextureType_DIFFUSE_ROUGHNESS:
		return "diffuse_roughness";
		break;
	case aiTextureType_AMBIENT_OCCLUSION:
		return "abient_occlusion";
		break;
	case aiTextureType_UNKNOWN:
		return "unknown";
		break;
	case _aiTextureType_Force32Bit:
		return "force32bit";
		break;
	default:
		return "none";
		break;
	}
}

const uint32_t Ai2CooTextureType(aiTextureType textureType) 
{
	Coo::Graphics::Model::TextureType type;
	switch (textureType)
	{
	case aiTextureType_DIFFUSE:
		type = Coo::Graphics::Model::TextureType::Diffuse;
		break;
	case aiTextureType_SPECULAR:
		type = Coo::Graphics::Model::TextureType::Specular;
		break;
	case aiTextureType_AMBIENT:
		type = Coo::Graphics::Model::TextureType::Ambient;
		break;
	case aiTextureType_EMISSIVE:
		type = Coo::Graphics::Model::TextureType::Emissive;
		break;
	case aiTextureType_HEIGHT:
		type = Coo::Graphics::Model::TextureType::Height;
		break;
	case aiTextureType_NORMALS:
		type = Coo::Graphics::Model::TextureType::Normals;
		break;
	case aiTextureType_SHININESS:
		type = Coo::Graphics::Model::TextureType::Shininess;
		break;
	case aiTextureType_OPACITY:
		type = Coo::Graphics::Model::TextureType::Opacity;
		break;
	case aiTextureType_DISPLACEMENT:
		type = Coo::Graphics::Model::TextureType::Displacement;
		break;
	case aiTextureType_LIGHTMAP:
		type = Coo::Graphics::Model::TextureType::Lightmap;
		break;
	case aiTextureType_REFLECTION:
		type = Coo::Graphics::Model::TextureType::Reflection;
		break;
	case aiTextureType_BASE_COLOR:
		type = Coo::Graphics::Model::TextureType::BaseColor;
		break;
	case aiTextureType_NORMAL_CAMERA:
		type = Coo::Graphics::Model::TextureType::NormalCamera;
		break;
	case aiTextureType_EMISSION_COLOR:
		type = Coo::Graphics::Model::TextureType::EmissionColor;
		break;
	case aiTextureType_METALNESS:
		type = Coo::Graphics::Model::TextureType::Metalness;
		break;
	case aiTextureType_DIFFUSE_ROUGHNESS:
		type = Coo::Graphics::Model::TextureType::DiffuseRoughness;
		break;
	case aiTextureType_AMBIENT_OCCLUSION:
		type = Coo::Graphics::Model::TextureType::AmbientOcclusion;
		break;
	default:
		type = Coo::Graphics::Model::TextureType::Unknown;
		break;
	}
	return static_cast<int>(type);
}