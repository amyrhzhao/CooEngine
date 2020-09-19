#include "PipeManager.h"

#include "Bird.h"

void PipeManager::Update(float deltaTime)
{
	mSpawnDelay -= deltaTime;
	if (mSpawnDelay <= 0.0f)
	{
		auto& pipe = mPipes.emplace_back();
		//pipe.Load();
		pipe.Spawn(104.0f);
		mSpawnDelay = 5.0f;
	}

	for (auto& pipe : mPipes)
		pipe.Update(deltaTime);

	auto IsOffScreen = [](const auto& pipe) { return pipe.GetTopRect().right < 0.0f; };
	mPipes.erase(std::remove_if(mPipes.begin(), mPipes.end(), IsOffScreen), mPipes.end());
}

void PipeManager::Render()
{
	for (auto& pipe : mPipes)
		pipe.Render();
}

void PipeManager::Reset()
{
	mPipes.clear();
	mSpawnDelay = 0.0f;
}

bool PipeManager::Intersect(const Bird& bird) const
{
	for (auto& pipe : mPipes)
	{
		if (Coo::Math::Intersect(pipe.GetTopRect(), bird.GetBound()) ||
			Coo::Math::Intersect(pipe.GetBottomRect(), bird.GetBound()))
		{
			return true;
		}
	}
	return false;
}

const Pipe* PipeManager::GetClosestPipe(const Bird& bird) const
{
	auto bound = bird.GetBound();
	auto birdLeft = bound.center.x - bound.radius;

	for (auto& pipe : mPipes)
	{
		if (!Coo::Math::Intersect(pipe.GetTopRect(), bird.GetBound()) &&
			!Coo::Math::Intersect(pipe.GetBottomRect(), bird.GetBound()) &&
			birdLeft < pipe.GetTopRect().right)
		{
			return &pipe;
		}
	}

	return nullptr;
}