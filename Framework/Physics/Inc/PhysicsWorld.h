#ifndef INCLUDED_COOENGINE_PHYSICS_PHYSICSWORLD_H
#define INCLUDED_COOENGINE_PHYSICS_PHYSICSWORLD_H

#include "Particle.h"
#include "Constraints.h"

namespace Coo::Physics
{
	struct Setting
	{
		Math::Vector3 gravity{ 0.0f, -9.8f, 0.0f };
		float timeStep = 1.0f / 60.0f;
		float drag = 0.0f;
		int iterations = 1;
	};

	class PhysicsWorld
	{
	public:
		void Initialize(const Setting& settings);
		
		void Update(float deltaTime);
		void DebugDraw() const;

		void AddParticle(Particle* p);
		void AddConstraint(Constraint* c);
		void ClearDynamic();

	private:
		void AccumulateForces();
		void Integrate();
		void SatisfyConstraints();

		std::vector<Particle*> mParticle;
		std::vector<Constraint*> mConstraints;

		Setting mSettings;
		float mTimer = 0.0f;
	};

} // namespace Coo::Physics

#endif // !INCLUDED_COOENGINE_PHYSICS_PHYSICSWORLD_H
