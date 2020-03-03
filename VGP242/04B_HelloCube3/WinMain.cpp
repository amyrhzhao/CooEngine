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
	std::vector<MeshPC> meshs;
	// Cube 
	meshs.push_back(MeshBuilder::CreateCubePC(1.0f, 1.5f, 3.0f));
	// Cone
	meshs.push_back(MeshBuilder::CreateConePC(0.7f, 2.0f, 16));
	// Cylinder
	meshs.push_back(MeshBuilder::CreateCylinderPC(1.0f, 6.0f, 16));
	// Icosahedron
	meshs.push_back(MeshBuilder::CreateIcosahedronPC());
	// Sphere
	meshs.push_back(MeshBuilder::CreateSpherePC(2.0f, 108, 108));
	// Plane
	meshs.push_back(MeshBuilder::CreatePlanePC(5.0f, 5.0f));

	// Initialize vertex buffers
	std::vector <MeshBuffer> meshBuffers;
	meshBuffers.resize(meshs.size());
	for (size_t i = 0; i < meshs.size(); ++i)
	{
		meshBuffers[i].Initialize(
			meshs[i].vertices.data(),
			static_cast<uint32_t>(sizeof(VertexPC)),
			static_cast<uint32_t>(meshs[i].vertices.size()),
			meshs[i].indices.data(),
			static_cast<uint32_t>(meshs[i].indices.size()));
	}

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
	int currentMeshIndex = 1;
	while (!done)
	{
		done = myWindow.ProcessMessage();
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			done = true;
		}
		// Mesh transation
		else if (GetAsyncKeyState('W')) { rotX += 0.02f; }
		else if (GetAsyncKeyState('S')) { rotX -= 0.02f; }
		else if (GetAsyncKeyState('A')) { rotY += 0.02f; }
		else if (GetAsyncKeyState('D')) { rotY -= 0.02f; }
		else if (GetAsyncKeyState('Q')) { rotZ += 0.02f; }
		else if (GetAsyncKeyState('E')) { rotZ -= 0.02f; }
		// Camera transation
		else if (GetAsyncKeyState('U')) { camera.Walk(0.2f); }
		else if (GetAsyncKeyState('J')) { camera.Walk(-0.2f); }
		else if (GetAsyncKeyState('H')) { camera.Strafe(-0.2f); }
		else if (GetAsyncKeyState('K')) { camera.Strafe(0.2f); }
		else if (GetAsyncKeyState('Y')) { camera.Rise(0.2f); }
		else if (GetAsyncKeyState('I')) { camera.Rise(-0.2f); }
		else if (GetAsyncKeyState(VK_UP)) { camera.Pitch(0.02f); }
		else if (GetAsyncKeyState(VK_DOWN)) { camera.Pitch(-0.02f); }
		else if (GetAsyncKeyState(VK_LEFT)) { camera.Yaw(0.02f); }
		else if (GetAsyncKeyState(VK_RIGHT)) { camera.Yaw(-0.02f); }
		// Mesh selection
		else if (GetAsyncKeyState('1')) { currentMeshIndex = 1; } // Cube
		else if (GetAsyncKeyState('2')) { currentMeshIndex = 2; } // Cone
		else if (GetAsyncKeyState('3')) { currentMeshIndex = 3; } // Cylinder
		else if (GetAsyncKeyState('4')) { currentMeshIndex = 4; } // Icosahedron
		else if (GetAsyncKeyState('5')) { currentMeshIndex = 5; } // Sphere
		else if (GetAsyncKeyState('6')) { currentMeshIndex = 6; } // Plane

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
		meshBuffers[currentMeshIndex - 1].Render();

		GraphicsSystem::Get()->EndRender();
	}

	pixelShader.Terminate();
	vertexShader.Terminate();
	for (auto& meshBuffer : meshBuffers)
	{
		meshBuffer.Terminate();
	}
	constantBuffer.Terminate();

	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}