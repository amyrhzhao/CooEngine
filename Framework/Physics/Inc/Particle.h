#ifndef INCLUDED_COOENGINE_PHYSICS_PARTICLE_H
#define INCLUDED_COOENGINE_PHYSICS_PARTICLE_H

namespace Coo::Physics
{
	struct Particle 
	{
		Math::Vector3 position = Math::Vector3::Zero();
		Math::Vector3 lastPosition = Math::Vector3::Zero();
		Math::Vector3 acceleration = Math::Vector3::Zero();
		float radius = 1.0f;
		float invMass = 0.0f;
		float bounce = 0.0f;

		void SetPosition(const Math::Vector3& pos) 
		{
			// position = lastPosition ---> no motion
			position = pos;
			lastPosition = pos;
		}

		void SetVelocity(const Math::Vector3& velocity) 
		{
			lastPosition = position - velocity;
		}
	};

} // namespace Coo::Physics

#endif // !INCLUDED_COOENGINE_PHYSICS_PARTICLE_H
