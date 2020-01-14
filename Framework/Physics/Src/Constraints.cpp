#include "Precompiled.h"
#include "Constraints.h"

using namespace Coo;
using namespace Coo::Physics;

Fixed::Fixed(Particle* p)
	: Fixed(p, p->position)
{
}

Fixed::Fixed(Particle* p, const Math::Vector3& position)
	: mParticle(p)
	, mPosition(position)
{
}

void Fixed::Apply() const
{
	mParticle->SetPosition(mPosition);
}

void Fixed::DebugDraw() const
{
	Graphics::SimpleDraw::AddAABB(mPosition, mParticle->radius, Graphics::Colors::Green);
}

Spring::Spring(Particle* a, Particle* b, float restLength)
	: mParticleA(a)
	, mParticleB(b)
	, mRestLength(restLength)
{
	if (mRestLength <= 0.0f)
		mRestLength = Math::Distance(a->position, b->position);
}

void Spring::Apply() const
{
	Math::Vector3 delta = mParticleB->position - mParticleA->position;
	float dist = Math::Magnitude(delta);
	float diff = (dist - mRestLength) / (dist * (mParticleA->invMass + mParticleB->invMass));
	mParticleA->position += delta * diff * mParticleA->invMass;
	mParticleB->position -= delta * diff * mParticleB->invMass;
}

void Spring::DebugDraw() const
{
	Graphics::SimpleDraw::AddLine(mParticleA->position, mParticleB->position, Graphics::Colors::Green);
}