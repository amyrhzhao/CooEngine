#ifndef INCLUDED_COOENGINE_CAMERASERVICE_H
#define INCLUDED_COOENGINE_CAMERASERVICE_H

#include "Service.h"

namespace Coo 
{
	class CameraService : public Service 
	{
	public:
		META_CLASS_DECLARE;

		void DebugUI() override;

		Graphics::Camera* AddCamera(const char* name);
		Graphics::Camera* FindCamera(const char* name);

		void SetActiveCamera(const char* name);
		Graphics::Camera& GetActiveCamera();
		const Graphics::Camera& GetActiveCamera() const;

	private:
		struct Entry
		{
			explicit Entry(const char* mName)
				: name(mName)
			{
			}
			std::string name;
			Graphics::Camera camera;
		};

		std::vector<Entry> mCameraList;
		int mActiveCameraIndex = 0;
	};

} // namespace Coo

#endif // !INCLUDED_COOENGINE_CAMERASERVICE_H
