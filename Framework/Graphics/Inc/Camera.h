#ifndef INCLUDED_COOENGINE_GRAPHICS_CAMERA_H
#define INCLUDED_COOENGINE_GRAPHICS_CAMERA_H

namespace Coo::Graphics {

Math::Matrix4 ComputeViewMatrix(
	const Math::Vector3& right,
	const Math::Vector3& up,
	const Math::Vector3& look, 
	const Math::Vector3& position);

Math::Matrix4 ComputePerspectiveMatrix(float n, float f, float fov, float aspect);

// This camera assumes YAxis as up direction always
class Camera
{
public:
	void SetPosition(const Math::Vector3& position);
	void SetDirection(const Math::Vector3& direction);
	void SetLookAt(const Math::Vector3& target);
	void SetAspectRatio(float aspectRatio) { mAspectRatio = aspectRatio; }

	// 3 degrees of freedom for translations
	void Walk(float distance);
	void Strafe(float distance);
	void Rise(float distance);

	// 2 degrees of freedom for rotations
	void Yaw(float radian);
	void Pitch(float radian);

	void SetFOV(float fov);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);

	const Math::Vector3& GetPosition() const { return mPosition; }
	const Math::Vector3& GetDirection() const { return mDirection; }
	float GetFOV() const { return mFov; }
	float GetNearPlane() const { return mNearPlane; }
	float GetFarPlane() const { return mFarPlane; }

	Math::Matrix4 GetViewMatrix() const;
	Math::Matrix4 GetPerspectiveMatrix() const;

private:
	Math::Vector3 mPosition{ 0.0f };
	Math::Vector3 mDirection{ 0.0f,0.0f,1.0f };

	float mFov{ 60.0f * Math::DegToRad };
	float mNearPlane{ 1.0f };
	float mFarPlane{ 1000.0f };
	float mAspectRatio = 0.0f;
};

} // namespace Coo::Graphics


#endif // !INCLUDED_COOENGINE_GRAPHICS_CAMERA_H