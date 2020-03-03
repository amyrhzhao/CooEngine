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
	myWindow.Initialize(hInstance, "Hello Cube", 1280, 720);

	// Initialize graphic system
	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	// Define vertex data
	// Cube 
	MeshBase<VertexPC> mesh = MeshBuilder::CreateCubePC();

	// Initialize vertex buffer
	MeshBuffer meshBuffer;
	meshBuffer.Initialize(
		mesh.vertices.data(),
		static_cast<uint32_t>(sizeof(VertexPC)), 
		static_cast<uint32_t>(mesh.vertices.size()), 
		mesh.indices.data(), 
		static_cast<uint32_t>(mesh.indices.size()));

	// Initialize vertex shader
	VertexShader vertexShader;
	uint32_t vertexFormat = VE_Position | VE_Color;
	vertexShader.Initialize(L"../../Assets/Shaders/DoTransform.fx", vertexFormat);

	// Initialize pixel shader
	PixelShader pixelShader;
	pixelShader.Initialize(L"../../Assets/Shaders/DoTransform.fx");

	ConstantBuffer constantBuffer;
	constantBuffer.Initialize(3, sizeof(Matrix4));

	Camera camera;
	camera.SetPosition({ 0.0f,0.0f,-10.0f });
	camera.SetDirection({ 0.0f,0.0f,1.0f });

	float rotX{ 0.0f };
	float rotY{ 0.0f };
	float rotZ{ 0.0f };

	bool done = false;
	while (!done)
	{
		done = myWindow.ProcessMessage();
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			done = true;
		}
		else if (GetAsyncKeyState(0x57)) // W
		{
			rotX += 0.02f;
		}
		else if (GetAsyncKeyState(0x53)) // S
		{
			rotX -= 0.02f;
		}
		else if (GetAsyncKeyState(0x41)) // A
		{
			rotY += 0.02f;
		}
		else if (GetAsyncKeyState(0x44)) // D
		{
			rotY -= 0.02f;
		}
		else if (GetAsyncKeyState(0x51)) // Q
		{
			rotZ += 0.02f;
		}
		else if (GetAsyncKeyState(0x45)) // E
		{
			rotZ -= 0.02f;
		}
		else if (GetAsyncKeyState(0x55)) // U
		{
			camera.Walk(0.2f);
		}
		else if (GetAsyncKeyState(0x4A)) // J
		{
			camera.Walk(-0.2f);
		}
		else if (GetAsyncKeyState(0x48)) // H
		{
			camera.Strafe(-0.2f);
		}
		else if (GetAsyncKeyState(0x4B)) // K
		{
			camera.Strafe(0.2f);
		}
		else if (GetAsyncKeyState(0x59)) // Y
		{
			camera.Rise(0.2f);
		}
		else if (GetAsyncKeyState(0x49)) // I
		{
			camera.Rise(-0.2f);
		}
		else if (GetAsyncKeyState(VK_UP)) // UP
		{
			camera.Pitch(0.02f);
		}
		else if (GetAsyncKeyState(VK_DOWN)) // DOWN
		{
			camera.Pitch(-0.02f);
		}
		else if (GetAsyncKeyState(VK_LEFT)) // LEFT
		{
			camera.Yaw(0.02f);
		}
		else if (GetAsyncKeyState(VK_RIGHT)) // RIGHT
		{
			camera.Yaw(-0.02f);
		}


		// Run game logic
		Matrix4 matRotX = RotationX(rotX);
		Matrix4 matRotY = RotationY(rotY);
		Matrix4 matRotZ = RotationZ(rotZ);
		// NOTE! Constant buffer takes the matrices tranposed!
		Matrix4 matrix[3];
		matrix[0] = Coo::Math::Transpose(matRotX*matRotY*matRotZ);
		matrix[1] = Coo::Math::Transpose(camera.GetViewMatrix());
		matrix[2] = Coo::Math::Transpose(camera.GetPerspectiveMatrix());

		GraphicsSystem::Get()->BeginRender();
		vertexShader.Bind();
		pixelShader.Bind();
		constantBuffer.Set(matrix);
		constantBuffer.BindVS();

		// Draw stuff
		meshBuffer.Render();

		GraphicsSystem::Get()->EndRender();
	}

	pixelShader.Terminate();
	vertexShader.Terminate();
	meshBuffer.Terminate();
	constantBuffer.Terminate();

	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}