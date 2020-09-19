#include "Bird.h"
#include "PipeManager.h"

void Bird::Update(float deltaTime, const PipeManager& pm)
{
	if (IsAlive())
	{
		if (pm.Intersect(*this))
		{
			Kill();
		}
		else
		{
			if (brain)
			{
				auto closetPipe = pm.GetClosestPipe(*this);
				if (closetPipe == nullptr)
				{
					closetPipe = new Pipe();
				}
				auto result = brain->Evaluate({ mPosition.y, closetPipe->GetTopRect().left, closetPipe->GetTopRect().bottom, closetPipe->GetBottomRect().top });
				if (result[0] > 0.5)
					Flap();
				fitness += deltaTime;
			}

			//const float numFrames = static_cast<float>(std::size(mTextureIds) - 1);
			//mAnimationFrame += deltaTime * numFrames;
			//if (mAnimationFrame >= numFrames)
			//	mAnimationFrame -= numFrames;

			const float gravity = 3000.0f;
			mVelocity.y += gravity * deltaTime;
			mPosition += mVelocity * deltaTime;
		}
	}
	else
	{
		//mAnimationFrame = static_cast<float>(std::size(mTextureIds) - 1);

		const float gravity = 3000.0f;
		mVelocity.y += gravity * deltaTime;
		mVelocity.x = -100.0f;
		mPosition += mVelocity * deltaTime;
	}
	//const float gravity = 3000.0f;
	//mVelocity.y += gravity * deltaTime;
	//mPosition += mVelocity * deltaTime;
}

void Bird::Render()
{
	Coo::Graphics::SimpleDraw::AddScreenCircle(mPosition, mRadius, Coo::Graphics::Colors::Yellow);
}

void Bird::Flap()
{
	if (IsAlive())
		mVelocity.y = -500.0f;
}

void Bird::Spawn(const Coo::Math::Vector2& pos)
{
	mPosition = pos;
	mVelocity = Coo::Math::Vector2();
	mAlive = true;
}

void Bird::Kill()
{
	mAlive = false;
}
