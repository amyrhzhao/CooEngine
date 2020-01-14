#include "Precompiled.h"
#include "SimpleDraw.h"

#include "ConstantBuffer.h"
#include "GraphicsSystem.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "VertexType.h"
#include "MeshBuffer.h"
#include "Font.h"

#pragma comment(lib, "FW1FontWrapper.lib")

using namespace Coo;
using namespace Coo::Graphics;

namespace
{
	struct TextCommand 
	{
		TextCommand(std::wstring inStr, float inSize, float inX, float inY, uint32_t inColor) 
			: str(std::move(inStr))
			, size(inSize)
			, x(inX)
			, y(inY)
			, color(inColor)
		{}
	
		std::wstring str;
		float size, x, y;
		uint32_t color;
	};

	constexpr uint32_t ToColor(const Color& color) 
	{
		uint8_t r = static_cast<uint8_t>(color.x * 255);
		uint8_t g = static_cast<uint8_t>(color.y * 255);
		uint8_t b = static_cast<uint8_t>(color.z * 255);
		return 0xff000000 | (b << 16) | (g << 8) | r;
	}

	// Private Implementation -> security
	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertexCount);
		void Terminate();

		void AddAABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ, const Color& color);
		void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
		void AddSphere(const Math::Vector3& center, float radius, const Color& color, uint32_t hCount = 8, uint32_t vCount = 8);

		void AddScreenRect(float minX, float maxX, float minY, float maxY, const Color& color);
		void AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color);
		void AddScreenCircle(const Math::Vector2& center, float radius, const Color& color);
		void AddScreenText(const char * str, float x, float y, float size, const Color& color);

		void Render(const Camera& camera);

	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		ConstantBuffer mConstantBuffer;
		MeshBuffer mMeshBuffer;
		std::vector<VertexPC> m2dLineVertices;
		float m2dLineVerticesDepth{1.0f};
		std::vector<VertexPC> m3dLineVertices;
		Font mFont;
		std::vector<TextCommand> mTextCommands;
		bool mInitialized{ false };
	};

	void SimpleDrawImpl::Initialize(uint32_t maxVertexCount)
	{
		ASSERT(!mInitialized, "[SimpleDraw] Already Initialized!");

		mVertexShader.Initialize(L"../../Assets/Shaders/SimpleDraw.fx", VertexPC::Format);
		mPixelShader.Initialize(L"../../Assets/Shaders/SimpleDraw.fx");
		mConstantBuffer.Initialize(sizeof(Math::Matrix4), 1);
		mMeshBuffer.Initialize(nullptr, sizeof(VertexPC), maxVertexCount, true);
		m3dLineVertices.reserve(maxVertexCount);
		m2dLineVertices.reserve(maxVertexCount);
		mFont.Initialize();
		
		mInitialized = true;
	}

	void SimpleDrawImpl::Terminate()
	{
		ASSERT(mInitialized, "[SimpleDraw] Already Terminated!");

		mConstantBuffer.Terminate();
		mVertexShader.Terminate();
		mPixelShader.Terminate();
		mMeshBuffer.Terminate();
		mFont.Terminate();

		mInitialized = false;

	}

	void SimpleDrawImpl::AddAABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ, const Color& color)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not Initialized!");
		AddLine({ minX,minY,minZ }, { minX,minY,maxZ }, color);
		AddLine({ minX,minY,maxZ }, { maxX,minY,maxZ }, color);
		AddLine({ maxX,minY,maxZ }, { maxX,minY,minZ }, color);
		AddLine({ maxX,minY,minZ }, { minX,minY,minZ }, color);
		AddLine({ minX,minY,minZ }, { minX,maxY,minZ }, color);
		AddLine({ minX,minY,maxZ }, { minX,maxY,maxZ }, color);
		AddLine({ maxX,minY,maxZ }, { maxX,maxY,maxZ }, color);
		AddLine({ maxX,minY,minZ }, { maxX,maxY,minZ }, color);
		AddLine({ minX,maxY,minZ }, { minX,maxY,maxZ }, color);
		AddLine({ minX,maxY,maxZ }, { maxX,maxY,maxZ }, color);
		AddLine({ maxX,maxY,maxZ }, { maxX,maxY,minZ }, color);
		AddLine({ maxX,maxY,minZ }, { minX,maxY,minZ }, color);
	}

	void SimpleDrawImpl::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not Initialized!");
		if (m3dLineVertices.size() + 2 <= m3dLineVertices.capacity())
		{
			m3dLineVertices.emplace_back(v0, color);
			m3dLineVertices.emplace_back(v1, color);
		}
	}

	void SimpleDrawImpl::AddSphere(const Math::Vector3& center, float radius, const Color& color, uint32_t hCount, uint32_t vCount)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not Initialized!");
		const float x = center.x;
		const float y = center.y;
		const float z = center.z;

		const uint32_t kSlices = hCount;
		const uint32_t kRings = vCount;
		const uint32_t kLines = (4 * kSlices * kRings) - (2 * kSlices);

		if (m3dLineVertices.size() + kLines <= m3dLineVertices.capacity())
		{
			const float kTheta = Math::Pi / (float)kRings;
			const float kPhi = 2 * Math::Pi / (float)kSlices;
			for (uint32_t j = 0; j < kSlices; ++j)
			{
				for (uint32_t i = 0; i < kRings; ++i)
				{
					const float a = i * kTheta;
					const float b = a + kTheta;
					const float ay = radius * cos(a);
					const float by = radius * cos(b);

					const float theta = j * kPhi;
					const float phi = theta + kPhi;

					const float ar = sqrt(radius * radius - ay * ay);
					const float br = sqrt(radius * radius - by * by);

					const float x0 = x + (ar * sin(theta));
					const float y0 = y + (ay);
					const float z0 = z + (ar * cos(theta));

					const float x1 = x + (br * sin(theta));
					const float y1 = y + (by);
					const float z1 = z + (br * cos(theta));

					const float x2 = x + (br * sin(phi));
					const float y2 = y + (by);
					const float z2 = z + (br * cos(phi));

					m3dLineVertices.push_back({ Math::Vector3(x0, y0, z0), color });
					m3dLineVertices.push_back({ Math::Vector3(x1, y1, z1), color });

					if (i < kRings - 1)
					{
						m3dLineVertices.push_back({ Math::Vector3(x1, y1, z1), color });
						m3dLineVertices.push_back({ Math::Vector3(x2, y2, z2), color });
					}
				}
			}
		}

	}

	void SimpleDrawImpl::AddScreenRect(float minX, float maxX, float minY, float maxY, const Color& color)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not Initialized!");
		AddScreenLine({minX,minY}, {minX,maxY}, color);
		AddScreenLine({minX,maxY}, {maxX,maxY}, color);
		AddScreenLine({maxX,maxY}, {maxX,minY}, color);
		AddScreenLine({maxX,minY}, {minX,minY}, color);

	}

	void SimpleDrawImpl::AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not Initialized!");
		if (m2dLineVertices.size() + 2 <= m2dLineVertices.capacity())
		{
			// todo 1.0f/maxVertexCount
			m2dLineVerticesDepth -= 0.000001f;
			m2dLineVertices.emplace_back(Math::Vector3{ v0.x, v0.y, m2dLineVerticesDepth }, color);
			m2dLineVertices.emplace_back(Math::Vector3{ v1.x, v1.y, m2dLineVerticesDepth }, color);
		}
	}

	void SimpleDrawImpl::AddScreenCircle(const Math::Vector2& center, float radius, const Color& color)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not Initialized!");
		uint32_t sides = 16;
		for (uint32_t i = 0, n = sides - 1; i <= sides; ++i)
		{
			float ratio0 = (float)i / n;
			float ratio1 = (float)(i + 1) / n;
			float r0 = ratio0 * (Math::Pi * 2.0f);
			float r1 = ratio1 * (Math::Pi * 2.0f);
			float x0 = cosf(r0) * radius;
			float x1 = cosf(r1) * radius;
			float y0 = sinf(r0) * radius;
			float y1 = sinf(r1) * radius;

			AddScreenLine({ center.x + x0, center.y + y0 }, { center.x + x1, center.y + y1 }, color);
		}

	}

	void SimpleDrawImpl::AddScreenText(const char* str, float x, float y, float size, const Color& color)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not Initialized!");
		static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		mTextCommands.emplace_back(converter.from_bytes(str), size, x, y, ToColor(color));
	}

	void SimpleDrawImpl::Render(const Camera& camera)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not Initialized!");

		mVertexShader.Bind();
		mPixelShader.Bind();

		// Draw 3d lines
		const Math::Matrix4& matView = camera.GetViewMatrix();
		const Math::Matrix4& matProj = camera.GetPerspectiveMatrix();
		auto transform = Math::Transpose(matView * matProj);
		mConstantBuffer.Set(&transform);
		mConstantBuffer.BindVS();

		mMeshBuffer.Update(m3dLineVertices.data(), static_cast<uint32_t>(m3dLineVertices.size()));
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
		mMeshBuffer.Render();

		// Draw 2d lines
		auto gs = GraphicsSystem::Get();
		const uint32_t w = gs->GetBackBufferWidth();
		const uint32_t h = gs->GetBackBufferHeight();
		Math::Matrix4 matInvScreen
		{
			2.0f / w, 0.0f, 0.0f, 0.0f,
			0.0f, -2.0f / h, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 1.0f
		};
		transform = Math::Transpose(matInvScreen);
		mConstantBuffer.Set(&transform);
		mConstantBuffer.BindVS();

		mMeshBuffer.Update(m2dLineVertices.data(), static_cast<uint32_t>(m2dLineVertices.size()));
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
		mMeshBuffer.Render();

		m3dLineVertices.clear();
		m2dLineVertices.clear();
		m2dLineVerticesDepth = 1.0f;

		// Text
		for (const auto& command : mTextCommands) 
		{
			mFont.Draw(command.str.c_str(), command.size, command.x, command.y, command.color);
		}
		mTextCommands.clear();
	}

	std::unique_ptr<SimpleDrawImpl> sSimpleDrawImpl;
}

void SimpleDraw::StaticInitialize(uint32_t maxVertexCount)
{
	sSimpleDrawImpl = std::make_unique<SimpleDrawImpl>();
	sSimpleDrawImpl->Initialize(maxVertexCount);
}

void SimpleDraw::StaticTerminate()
{
	sSimpleDrawImpl->Terminate();
	sSimpleDrawImpl.reset();
}

void SimpleDraw::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
{
	sSimpleDrawImpl->AddLine(v0, v1, color);
}

void SimpleDraw::AddSphere(const Math::Vector3& center, float radius, const Color& color, uint32_t hCount, uint32_t vCount)
{
	sSimpleDrawImpl->AddSphere(center, radius, color, hCount, vCount);
}

void SimpleDraw::AddAABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ, const Color& color)
{
	sSimpleDrawImpl->AddAABB(minX, maxX, minY, maxY, minZ, maxZ, color);
}

void Coo::Graphics::SimpleDraw::AddAABB(const Math::Vector3 & pos, float radius, const Color & color)
{
	sSimpleDrawImpl->AddAABB(pos.x - radius, pos.x + radius, pos.y - radius, pos.y + radius, pos.z - radius, pos.z + radius, color);
}

void SimpleDraw::AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color)
{
	sSimpleDrawImpl->AddScreenLine(v0, v1, color);
}

void SimpleDraw::AddScreenCircle(const Math::Vector2& center, float radius, const Color& color)
{
	sSimpleDrawImpl->AddScreenCircle(center, radius, color);
}

void SimpleDraw::AddScreenRect(float minX, float maxX, float minY, float maxY, const Color& color)
{
	sSimpleDrawImpl->AddScreenRect(minX, maxX, minY, maxY, color);
}

void SimpleDraw::AddScreenText(const char* str, float x, float y, float size, const Color& color)
{
	sSimpleDrawImpl->AddScreenText(str, x, y, size, color);
}

void SimpleDraw::Render(const Camera& camera)
{
	sSimpleDrawImpl->Render(camera);
}
