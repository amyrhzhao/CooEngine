#include <Graphics/Inc/Graphics.h>
#include <Core/Inc/Core.h>
#include <Math/Inc/CooMath.h>

using namespace Coo::Graphics;
using namespace Coo::Math;
using namespace Coo::Core;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	const size_t planetCount{ 9 };

	// Set up application window
	Window myWindow;
	myWindow.Initialize(hInstance, "Hello Planets", 1280, 720);

	// Initialize graphic system
	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	// Define mesh data
	MeshPX sphereMesh{ MeshBuilder::CreateSpherePX() };
	MeshPX spaceMesh{ MeshBuilder::CreateSpherePX(400.0f) };

	// Initialize mesh buffer
	MeshBuffer planetsMeshBuffer;
	planetsMeshBuffer.Initialize(sphereMesh);
	MeshBuffer spaceMeshBuffer;
	spaceMeshBuffer.Initialize(spaceMesh);

	// Initialize vertex shader
	VertexShader vertexShader;
	vertexShader.Initialize(L"../../Assets/Shaders/Texturing.fx", VertexPX::Format);

	// Initialize pixel shader
	PixelShader pixelShader;
	pixelShader.Initialize(L"../../Assets/Shaders/Texturing.fx");

	// Initialize constant buffer
	ConstantBuffer constantBuffer;
	constantBuffer.Initialize(3, sizeof(Matrix4));

	// Texture
	std::array<Texture, planetCount> planetsTexture;
	Texture spaceTexture;

	// Initialize Sampler
	Sampler sampler;
	sampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);

	// Initialize Camera
	Camera camera;
	camera.SetPosition({ 0.0f,30.0f,-250.0f });
	camera.SetDirection({ 0.0f,0.0f,1.0f });

	// Transformation of Planets
	std::array<Matrix4, planetCount> planetsScaling;
	std::array<Matrix4, planetCount> planetsTranslation;
	std::array<float, planetCount> planetsRotY{ 0.0f };
	std::array<float, planetCount> planetsSelfRotY{ 0.0f };
	std::array<float, planetCount> planetsRotYSpeed{ 0.0f };
	std::array<float, planetCount> planetsSelfRotYSpeed{ 0.0f };
	std::array<Matrix4, planetCount> planetsRotation;
	std::array<Matrix4, planetCount> planetsSelfRotation;

	spaceTexture.Initialize(L"../../Assets/Images/StarUV.jpg");
	{
		// Sun
		planetsTexture[0].Initialize(L"../../Assets/Images/SunUV.jpg");
		planetsScaling[0] = Scale(20.0f);
		planetsTranslation[0] = Translate(Vector3{ 0.0f });
		planetsRotYSpeed[0] = 0.0f;
		planetsSelfRotYSpeed[0] = 0.10f;

		// Mercury
		planetsTexture[1].Initialize(L"../../Assets/Images/MercuryUV.jpg");
		planetsScaling[1] = Scale(2.0f);
		planetsTranslation[1] = Translate({ 25.0f,0.0f,0.0f });
		planetsRotYSpeed[1] = 0.04789f;
		planetsSelfRotYSpeed[1] = 0.00170503f;

		// Venus
		planetsTexture[2].Initialize(L"../../Assets/Images/VenusUV.jpg");
		planetsScaling[2] = Scale(2.4f);
		planetsTranslation[2] = Translate({ 35.2f,0.0f,0.0f });
		planetsRotYSpeed[2] = 0.03503f;
		planetsSelfRotYSpeed[2] = 0.000411523f;

		// Earth
		planetsTexture[3].Initialize(L"../../Assets/Images/EarthUV.jpg");
		planetsScaling[3] = Scale(2.8f);
		planetsTranslation[3] = Translate({ 45.4f,0.0f,0.0f });
		planetsRotYSpeed[3] = 0.03000f;
		planetsSelfRotYSpeed[3] = 0.00416667f;

		// Mars
		planetsTexture[4].Initialize(L"../../Assets/Images/MarsUV.jpg");
		planetsScaling[4] = Scale(2.6f);
		planetsTranslation[4] = Translate({ 55.3f,0.0f,0.0f });
		planetsRotYSpeed[4] = 0.02413f;
		planetsSelfRotYSpeed[4] = 0.004065041f;

		// Jupiter
		planetsTexture[5].Initialize(L"../../Assets/Images/JupiterUV.jpg");
		planetsScaling[5] = Scale(10.0f);
		planetsTranslation[5] = Translate({ 90.0f,0.0f,0.0f });
		planetsRotYSpeed[5] = 0.01306f;
		planetsSelfRotYSpeed[5] = 0.240038406f;

		// Saturn
		planetsTexture[6].Initialize(L"../../Assets/Images/SaturnUV.jpg");
		planetsScaling[6] = Scale(8.0f);
		planetsTranslation[6] = Translate({ 120.0f,0.0f,0.0f });
		planetsRotYSpeed[6] = 0.00964f;
		planetsSelfRotYSpeed[6] = 0.230000000f;

		// Uranus
		planetsTexture[7].Initialize(L"../../Assets/Images/UranusUV.jpg");
		planetsScaling[7] = Scale(7.0f);
		planetsTranslation[7] = Translate({ 145.0f,0.0f,0.0f });
		planetsRotYSpeed[7] = 0.00681f;
		planetsSelfRotYSpeed[7] = 0.141242938f;

		// Neptune
		planetsTexture[8].Initialize(L"../../Assets/Images/NeptuneUV.jpg");
		planetsScaling[8] = Scale(6.5f);
		planetsTranslation[8] = Translate({ 179.05f,0.0f,0.0f });
		planetsRotYSpeed[8] = 0.00543f;
		planetsSelfRotYSpeed[8] = 0.16f;
	}

	RasterizerState rasterizerState;
	rasterizerState.Initialize(RasterizerState::CullMode::Front, RasterizerState::FillMode::Solid);

	SimpleDraw::StaticInitialize();
	
	bool autoRun = true;
	bool done = false;
	while (!done)
	{
		done = myWindow.ProcessMessage();
		if (GetAsyncKeyState(VK_ESCAPE)) { done = true; }
		// Camera transation
		if (GetAsyncKeyState('U')) { camera.Walk(0.2f); }
		else if (GetAsyncKeyState('J')) { camera.Walk(-0.2f); }
		else if (GetAsyncKeyState('H')) { camera.Strafe(-0.2f); }
		else if (GetAsyncKeyState('K')) { camera.Strafe(0.2f); }
		else if (GetAsyncKeyState('Y')) { camera.Rise(0.2f); }
		else if (GetAsyncKeyState('I')) { camera.Rise(-0.2f); }
		else if (GetAsyncKeyState(VK_UP)) { camera.Pitch(0.02f); }
		else if (GetAsyncKeyState(VK_DOWN)) { camera.Pitch(-0.02f); }
		else if (GetAsyncKeyState(VK_LEFT)) { camera.Yaw(0.02f); }
		else if (GetAsyncKeyState(VK_RIGHT)) { camera.Yaw(-0.02f); }
		// Mesh Transation
		if (GetAsyncKeyState(VK_RETURN)) { autoRun = !autoRun; }
		else
		{
			if (autoRun)
			{
				for (int i = 0; i < planetCount; ++i)
				{
					planetsRotY[i] -= planetsRotYSpeed[i] * 0.5f;
					planetsSelfRotY[i] -= planetsSelfRotYSpeed[i] * 0.5f;
				}
			}
			else
			{
				if (GetAsyncKeyState('A'))
				{
					for (int i = 0; i < planetCount; ++i)
					{
						planetsRotY[i] += planetsRotYSpeed[i];
						planetsSelfRotY[i] += planetsSelfRotYSpeed[i];
					}
				}
				else if (GetAsyncKeyState('D'))
				{
					for (int i = 0; i < planetCount; ++i)
					{
						planetsRotY[i] -= planetsRotYSpeed[i];
						planetsSelfRotY[i] -= planetsSelfRotYSpeed[i];
					}
				}
			}
		}

		GraphicsSystem::Get()->BeginRender();
		sampler.BindPS();
		vertexShader.Bind();
		pixelShader.Bind();

		Matrix4 matrix[3];
		matrix[1] = Coo::Math::Transpose(camera.GetViewMatrix());
		matrix[2] = Coo::Math::Transpose(camera.GetPerspectiveMatrix());
		
		// Render Space Background
		{
			rasterizerState.Set();
			matrix[0] = Coo::Math::Transpose(Translate(camera.GetPosition()));
			constantBuffer.Set(matrix);
			constantBuffer.BindVS();
			spaceTexture.BindPS();
			spaceMeshBuffer.Render();
			rasterizerState.Clear();
		}

		// Render Planets
		for (int i = 0; i < planetCount; ++i)
		{
			// Apply planet rotation
			planetsRotation[i] = RotationY(planetsRotY[i]) ;
			planetsSelfRotation[i] = RotationY(planetsSelfRotY[i]);
			// Set tranformation matrix 
			matrix[0] = Coo::Math::Transpose(
				planetsSelfRotation[i] *
				planetsScaling[i] *
				planetsTranslation[i] *
				planetsRotation[i]);
			// Bind Constant Buffer
			constantBuffer.Set(matrix);
			constantBuffer.BindVS();
			// Bind Texture
			planetsTexture[i].BindPS();
			// Render
			planetsMeshBuffer.Render();
		}

		SimpleDraw::AddLine(Vector3{ 0.0f }, Vector3{ 100.0f,0.0f,0.0f }, Colors::Red);
		SimpleDraw::AddLine(Vector3{ 0.0f }, Vector3{ 0.0f,100.0f,0.0f }, Colors::Green);
		SimpleDraw::AddLine(Vector3{ 0.0f }, Vector3{ 0.0f,0.0f,100.0f }, Colors::Blue);
		SimpleDraw::Render(camera);

		GraphicsSystem::Get()->EndRender();
	}

	pixelShader.Terminate();
	vertexShader.Terminate();
	planetsMeshBuffer.Terminate();
	spaceMeshBuffer.Terminate();
	constantBuffer.Terminate();
	spaceTexture.Terminate();
	for (auto& texture : planetsTexture)
	{
		texture.Terminate();
	}
	sampler.Terminate();
	rasterizerState.Terminate();
	GraphicsSystem::StaticTerminate();
	myWindow.Terminate();
	SimpleDraw::StaticTerminate();
	return 0;
}
