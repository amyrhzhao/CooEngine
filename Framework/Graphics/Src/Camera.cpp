#include "Precompiled.h"
#include "Camera.h"
#include "GraphicsSystem.h"
using namespace Coo;
using namespace Coo::Graphics;

Math::Matrix4 Graphics::ComputeViewMatrix(const Math::Vector3 & r, const Math::Vector3 & u, const Math::Vector3 & l, const Math::Vector3 & position)
{
	const float dx = -Math::Dot(r, position);
	const float dy = -Math::Dot(u, position);
	const float dz = -Math::Dot(l, position);

	return 
	{
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		 dx,  dy,  dz, 1.0f
	};

}

Math::Matrix4 Graphics::ComputePerspectiveMatrix(float n, float f, float fov, float aspect)
{
	const float h = 1.0f / tan(fov * 0.5f);
	const float w = h / aspect;
	const float zf = f;
	const float zn = n;
	const float d = zf / (zf - zn);

	return
	{
		   w, 0.0f,  0.0f, 0.0f,
		0.0f,    h,  0.0f, 0.0f,
		0.0f, 0.0f,     d, 1.0f,
		0.0f, 0.0f, -zn*d, 0.0f
	};
}

void Camera::SetPosition(const Math::Vector3 & position)
{
	mPosition = position;
}

void Camera::SetDirection(const Math::Vector3 & direction)
{
	mDirection = Math::Normalize(direction);
}

void Camera::SetLookAt(const Math::Vector3 & target)
{
	mDirection = Math::Normalize(target - mPosition);
}

void Camera::Walk(float distance)
{
	mPosition += mDirection * distance;
}

void Camera::Strafe(float distance)
{
	Math::Vector3 temp = Math::Cross(mDirection, { 0.0f,1.0f,0.0f });
	mPosition += Math::Normalize(temp) * distance;
}

void Camera::Rise(float distance)
{
	mPosition.y += distance;
}

void Camera::Yaw(float radian)
{
	Math::Vector3 rotate = Math::RotationY(mDirection, radian);
	mDirection = Math::Normalize(rotate);
}

void Camera::Pitch(float radian)
{
	const Math::Vector3 right = Math::Normalize(Math::Cross(Math::Vector3::YAxis(), mDirection));
	const Math::Matrix4 matRotate = Math::RotationAxis(right, radian);
	const Math::Vector3 newLook = Math::TransformNormal(mDirection, matRotate);
	mDirection = newLook;
}

Math::Matrix4 Camera::GetViewMatrix() const
{
	const Math::Vector3 l = mDirection;
	const Math::Vector3 r = Math::Normalize(Math::Cross({ 0.0f, 1.0f, 0.0f }, l));
	const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
	return ComputeViewMatrix(r, u, l, mPosition);
}

Math::Matrix4 Camera::GetPerspectiveMatrix() const
{
	float aspectRatio = mAspectRatio;
	if(aspectRatio == 0.0f)
	{
		auto width = GraphicsSystem::Get()->GetBackBufferWidth();
		auto height = GraphicsSystem::Get()->GetBackBufferHeight();
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	}
	return ComputePerspectiveMatrix(mNearPlane, mFarPlane, mFov, aspectRatio);
}

void Camera::SetFOV(float fov)
{
	mFov = fov;
}

void Camera::SetNearPlane(float nearPlane)
{
	mNearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	mFarPlane = farPlane;
}
