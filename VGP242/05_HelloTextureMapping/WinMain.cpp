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
	myWindow.Initialize(hInstance, "Hello Texture Mapping", 1280, 720);

	// Initialize graphic system
	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	// Define vertex data
	std::vector<MeshPX> meshs;
	// Plane
	meshs.push_back(MeshBuilder::CreatePlanePX(1.0f, 1.0f, 2, 2));
	meshs.push_back(MeshBuilder::CreateConePX(1.0f, 3.0f, 108));
	meshs.push_back(MeshBuilder::CreateCylinderPX(1.0f, 3.0f, 32));
	meshs.push_back(MeshBuilder::CreateSpherePX(4.0f,32,32));

	// Initialize vertex buffers
	std::vector<MeshBuffer> meshBuffers;
	meshBuffers.resize(meshs.size());
	for (size_t i = 0; i < meshs.size(); ++i)
	{
		meshBuffers[i].Initialize(meshs[i]);
	}

	// Initialize vertex shader
	VertexShader vertexShader;
	uint32_t vertexFormat = VE_Position | VE_Texcoord;
	vertexShader.Initialize(L"../../Assets/Shaders/Texturing.fx", vertexFormat);

	// Initialize pixel shader
	PixelShader pixelShader;
	pixelShader.Initialize(L"../../Assets/Shaders/Texturing.fx");

	// Initialize constant buffer
	ConstantBuffer constantBuffer;
	constantBuffer.Initialize(3, sizeof(Matrix4));

	// Initialize Texture
	Texture texture;
	texture.Initialize(L"../../Assets/Images/EarthTexture.jpg");
	//texture.Initialize(L"../../Assets/Images/face.jpg");

	// Initialize Sampler
	Sampler sampler;
	sampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);

	// Initialize camera
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
		else if (GetAsyncKeyState('1')) { currentMeshIndex = 1; } // Plane
		else if (GetAsyncKeyState('2')) { currentMeshIndex = 2; } // Cone
		else if (GetAsyncKeyState('3')) { currentMeshIndex = 3; } // Cylinder
		else if (GetAsyncKeyState('4')) { currentMeshIndex = 4; } // Sphere

		// Calculate Matrixs Constant Buffer
		Matrix4 matRotX = RotationX(rotX);
		Matrix4 matRotY = RotationY(rotY);
		Matrix4 matRotZ = RotationZ(rotZ);
		Matrix4 matrix[3];
		matrix[0] = Coo::Math::Transpose(matRotX*matRotY*matRotZ);
		matrix[1] = Coo::Math::Transpose(camera.GetViewMatrix());
		matrix[2] = Coo::Math::Transpose(camera.GetPerspectiveMatrix());

		GraphicsSystem::Get()->BeginRender();
		texture.BindPS();
		sampler.BindPS();
		vertexShader.Bind();
		pixelShader.Bind();
		constantBuffer.Set(matrix);
		constantBuffer.BindVS();

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
	sampler.Terminate();
	texture.Terminate();

	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}
