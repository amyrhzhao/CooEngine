#include "Precompiled.h"
#include "CameraService.h"

using namespace Coo;

META_DERIVED_BEGIN(CameraService, Service)
	META_NO_FIELD
META_CLASS_END

void Coo::CameraService::DebugUI()
{

}

Graphics::Camera* Coo::CameraService::AddCamera(const char* name)
{
	auto camera = FindCamera(name);
	if (camera == nullptr) 
	{
		auto& entry = mCameraList.emplace_back(name);
		camera = &entry.camera;
	}
	return camera;
}

Graphics::Camera* Coo::CameraService::FindCamera(const char* name)
{
	auto iter = std::find_if(mCameraList.begin(), mCameraList.end(), [name](auto& entry) 
	{
		return entry.name == name;
	});
	return (iter == mCameraList.end()) ? nullptr : &iter->camera;
}

void Coo::CameraService::SetActiveCamera(const char* name)
{
	for (size_t i = 0; i < mCameraList.size(); ++i) 
	{
		auto& entry = mCameraList[i];
		if (entry.name == name) 
		{
			mActiveCameraIndex = static_cast<int>(i);
			break;
		}
	}
}

Graphics::Camera & Coo::CameraService::GetActiveCamera()
{
	ASSERT(static_cast<size_t>(mActiveCameraIndex) < mCameraList.size(), "[CameraService] No active camera!");
	return mCameraList[mActiveCameraIndex].camera;
}

const Graphics::Camera& Coo::CameraService::GetActiveCamera() const
{
	ASSERT(static_cast<size_t>(mActiveCameraIndex) < mCameraList.size(), "[CameraService] No active camera!");
	return mCameraList[mActiveCameraIndex].camera;
}
