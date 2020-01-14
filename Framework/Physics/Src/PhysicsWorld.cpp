#include "Precompiled.h"
#include "PhysicsWorld.h"

using namespace Coo::Physics;

void PhysicsWorld::Initialize(const Setting & settings)
{
	mSettings = settings;
}

void PhysicsWorld::Update(float deltaTime)
{
	mTimer += deltaTime;
	while (mTimer >= mSettings.timeStep)
	{
		mTimer -= mSettings.timeStep;

		AccumulateForces();
		Integrate();
		SatisfyConstraints();
	}
}

void PhysicsWorld::DebugDraw() const
{
	using namespace Coo::Graphics;
	for (auto p : mParticle)
	{
		SimpleDraw::AddSphere(p->position, p->radius, Colors::Cyan, 4, 2);
	}
	for (auto c : mConstraints) 
	{
		c->DebugDraw();
	}
}

void PhysicsWorld::AddParticle(Particle * p)
{
	mParticle.push_back(p); // TODO - this is bad, turn this into a pool
}

void PhysicsWorld::AddConstraint(Constraint* c)
{
	mConstraints.push_back(c);
}

void PhysicsWorld::AccumulateForces()
{
	for (auto p : mParticle)
	{
		p->acceleration = mSettings.gravity;
	}
}

void PhysicsWorld::Integrate()
{
	const float gamma = 1.0f - mSettings.drag;
	const float timeStepSqr = mSettings.timeStep * mSettings.timeStep;
	for (auto p : mParticle)
	{
		Math::Vector3 displacement = (p->position - p->lastPosition) * gamma + (p->acceleration * timeStepSqr);
		p->lastPosition = p->position;
		p->position += displacement;
	}
}

void PhysicsWorld::SatisfyConstraints()
{
	// for iterations
	// Apply all the constraints
	for (int i = 0; i < mSettings.iterations; ++i) 
	{
		for (auto& c : mConstraints) 
		{
			c->Apply();
		}
	}

}

void PhysicsWorld::ClearDynamic()
{
	for (auto p : mParticle) 
	{
		delete p;
	}
	mParticle.clear();
	for (auto c : mConstraints) 
	{
		delete c;
	}
	mConstraints.clear();
}
