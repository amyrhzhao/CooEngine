#include <Graphics/Inc/Graphics.h>
#include <Core/Inc/Core.h>
#include <Math/Inc/CooMath.h>

using namespace Coo::Graphics;
using namespace Coo::Math;
using namespace Coo::Core;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// Set up application window
	Window myWindow;
	myWindow.Initialize(hInstance, "Hello Shapes", 1280, 720);

	// Initialize graphic system
	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	// Define vertex data
	// triangle 
	VertexPC vertices1[]{
		{ {  0.0f, 0.5f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f }},
		{ {  0.5f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f }},
		{ { -0.5f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f }}
	};
	// triforce 
	VertexPC vertices2[]{
		{ {  0.00f,  0.70f, 0.0f }, {1.0f, 0.7f, 0.0f, 1.0f }},
		{ {  0.29f,  0.00f, 0.0f }, {1.0f, 0.7f, 0.0f, 1.0f }},
		{ { -0.29f,  0.00f, 0.0f }, {1.0f, 0.7f, 0.0f, 1.0f }},
		{ {  0.29f,  0.00f, 0.0f }, {1.0f, 0.7f, 0.0f, 1.0f }},
		{ {  0.58f, -0.70f, 0.0f }, {1.0f, 0.7f, 0.0f, 1.0f }},
		{ {  0.00f, -0.70f, 0.0f }, {1.0f, 0.7f, 0.0f, 1.0f }},
		{ { -0.29f,  0.00f, 0.0f }, {1.0f, 0.7f, 0.0f, 1.0f }},
		{ {  0.00f, -0.70f, 0.0f }, {1.0f, 0.7f, 0.0f, 1.0f }},
		{ { -0.58f, -0.70f, 0.0f }, {1.0f, 0.7f, 0.0f, 1.0f }}
	};
	// heart 
	VertexPC vertices3[]{
		{ {  0.00f,  0.30f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ { -0.30f,  0.52f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ { -0.12f,  0.52f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ {  0.00f,  0.30f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ {  0.12f,  0.52f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ {  0.30f,  0.52f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ {  0.00f,  0.30f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ { -0.40f,  0.30f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ { -0.30f,  0.52f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ {  0.00f,  0.30f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ {  0.30f,  0.52f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ {  0.40f,  0.30f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ { -0.40f,  0.30f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ {  0.40f,  0.30f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ {  0.00f, -0.50f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }}
	};
	// something 
	VertexPC vertices4[]{
		{ {  0.00f,  0.80f, 0.0f },{ 0.0f, 0.45f, 0.0f, 1.0f }},
		{ {  0.20f,  0.55f, 0.0f },{ 0.0f, 0.45f, 0.0f, 1.0f }},
		{ { -0.20f,  0.55f, 0.0f },{ 0.0f, 0.45f, 0.0f, 1.0f }},
		{ {  0.00f,  0.65f, 0.0f },{ 0.0f, 0.40f, 0.0f, 1.0f }},
		{ {  0.27f,  0.30f, 0.0f },{ 0.0f, 0.40f, 0.0f, 1.0f }},
		{ { -0.27f,  0.30f, 0.0f },{ 0.0f, 0.40f, 0.0f, 1.0f }},
		{ {  0.00f,  0.40f, 0.0f },{ 0.0f, 0.35f, 0.0f, 1.0f }},
		{ {  0.35f, -0.00f, 0.0f },{ 0.0f, 0.35f, 0.0f, 1.0f }},
		{ { -0.35f, -0.00f, 0.0f },{ 0.0f, 0.35f, 0.0f, 1.0f }},
		{ { -0.05f,  0.00f, 0.0f },{ 0.3f, 0.2f, 0.1f, 1.0f} },
		{ {  0.05f,  0.00f, 0.0f },{ 0.3f, 0.2f, 0.1f, 1.0f} },
		{ { -0.05f, -0.45f, 0.0f },{ 0.3f, 0.2f, 0.1f, 1.0f} },
		{ {  0.05f,  0.00f, 0.0f },{ 0.3f, 0.2f, 0.1f, 1.0f} },
		{ {  0.05f, -0.45f, 0.0f },{ 0.3f, 0.2f, 0.1f, 1.0f} },
		{ { -0.05f, -0.45f, 0.0f },{ 0.3f, 0.2f, 0.1f, 1.0f} }
	};
	// another heart
	VertexPC vertices5[]{
		{ {  0.00f,  0.30f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ { -0.30f,  0.52f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ { -0.12f,  0.52f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ {  0.12f,  0.52f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ {  0.30f,  0.52f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ { -0.40f,  0.30f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ {  0.40f,  0.30f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }},
		{ {  0.00f, -0.50f, 0.0f }, {1.0f, 0.0f, 0.2f, 1.0f }}
	};
	uint32_t indices5[]
	{
		0,1,2,
		0,3,4,
		0,5,1,
		0,4,6,
		5,6,7
	};

	// Initialize vertex buffer
	MeshBuffer meshBuffer;
	meshBuffer.Initialize(vertices1, static_cast<uint32_t>(sizeof(VertexPC)), static_cast<uint32_t>(std::size(vertices1)));

	// Initialize vertex shader
	VertexShader vertexShader;
	uint32_t vertexFormat = VE_Position | VE_Color;
	vertexShader.Initialize(L"../../Assets/Shaders/DoSomething.fx", vertexFormat);

	// Initialize pixel shader
	PixelShader pixelShader;
	pixelShader.Initialize(L"../../Assets/Shaders/DoSomething.fx");

	bool done = false;
	while (!done)
	{
		done = myWindow.ProcessMessage();
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			done = true;
		}
		else if (GetAsyncKeyState('1'))
		{
			// key 1 triangle
			meshBuffer.Terminate();
			meshBuffer.Initialize(vertices1, static_cast<uint32_t>(sizeof(VertexPC)), static_cast<uint32_t>(std::size(vertices1)));
		}
		else if (GetAsyncKeyState('2'))
		{
			// key 2 triforce
			meshBuffer.Terminate();
			meshBuffer.Initialize(vertices2, static_cast<uint32_t>(sizeof(VertexPC)), static_cast<uint32_t>(std::size(vertices2)));
		}
		else if (GetAsyncKeyState('3'))
		{
			// key 3 heart
			meshBuffer.Terminate();
			meshBuffer.Initialize(vertices3, static_cast<uint32_t>(sizeof(VertexPC)), static_cast<uint32_t>(std::size(vertices3)));
		}
		else if (GetAsyncKeyState('4'))
		{
			// key 4 something
			meshBuffer.Terminate();
			meshBuffer.Initialize(vertices4, static_cast<uint32_t>(sizeof(VertexPC)), static_cast<uint32_t>(std::size(vertices4)));
		}
		else if (GetAsyncKeyState('5'))
		{
			// key 5 another heart
			meshBuffer.Terminate();
			meshBuffer.Initialize(vertices5, static_cast<uint32_t>(sizeof(VertexPC)), static_cast<uint32_t>(std::size(vertices5)), indices5, static_cast<uint32_t>(std::size(indices5)));
		}


		// Run game logic
		GraphicsSystem::Get()->BeginRender();

		vertexShader.Bind();
		pixelShader.Bind();

		// Draw stuff
		meshBuffer.Render();

		GraphicsSystem::Get()->EndRender();
	}

	pixelShader.Terminate();
	vertexShader.Terminate();
	meshBuffer.Terminate();

	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}