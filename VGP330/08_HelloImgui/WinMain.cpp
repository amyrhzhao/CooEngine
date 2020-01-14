#include <Coo/Inc/Coo.h>

using namespace Coo::Graphics;
using namespace Coo::Math;
using namespace Coo::Core;
using namespace Coo::Input;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	const size_t planetCount{ 9 };

	// Set up application window
	Window myWindow;
	myWindow.Initialize(hInstance, "Hello ImGui", 1280, 720);

	// Initialize the input system
	InputSystem::StaticInitialize(myWindow.GetWindowHandle());

	// Initialize graphic system
	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	// Initialize Debug UI
	Dui::StaticInitialize(myWindow.GetWindowHandle(), true, false, Dui::Theme::Classic);

	// Initialize vertex shader
	VertexShader vertexShader;
	vertexShader.Initialize(L"../../Assets/Shaders/SimpleDraw.fx", VertexPC::Format);

	// Initialize pixel shader
	PixelShader pixelShader;
	pixelShader.Initialize(L"../../Assets/Shaders/SimpleDraw.fx");

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
	camera.SetPosition({ 0.0f,0.0f,-10.0f });
	camera.SetDirection({ 0.0f,0.0f,1.0f });

	float rotX{ 0.0f };
	float rotY{ 0.0f };
	float rotZ{ 0.0f };

	RasterizerState rasterizerState;
	rasterizerState.Initialize(RasterizerState::CullMode::Front, RasterizerState::FillMode::Solid);

	SimpleDraw::StaticInitialize();

	bool autoRun = true;
	bool done = false;
	while (!done)
	{
		done = myWindow.ProcessMessage();

		auto inputSystem = InputSystem::Get();
		inputSystem->Update();

		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE)) { done = true; }

		static auto lastTime = std::chrono::high_resolution_clock::now();
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
		lastTime = currentTime;

		const float moveSpeed = 10.0f;
		const float turnSpeed = 0.1f;

		// Camera transation
		if (inputSystem->IsKeyDown(KeyCode::W)) { camera.Walk(moveSpeed * deltaTime); }
		if (inputSystem->IsKeyDown(KeyCode::S)) { camera.Walk(-moveSpeed * deltaTime); }
		if (inputSystem->IsKeyDown(KeyCode::A)) { camera.Strafe(-moveSpeed * deltaTime); }
		if (inputSystem->IsKeyDown(KeyCode::D)) { camera.Strafe(moveSpeed * deltaTime); }
		if (inputSystem->IsKeyDown(KeyCode::Q)) { camera.Rise(moveSpeed * deltaTime); }
		if (inputSystem->IsKeyDown(KeyCode::E)) { camera.Rise(-moveSpeed * deltaTime); }
		if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
		{
			camera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
			// TODO: recalculate current X Axis to rotate
			camera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
		}


		GraphicsSystem::Get()->BeginRender();
		sampler.BindPS();
		vertexShader.Bind();
		pixelShader.Bind();

		Matrix4 matrix[3];
		matrix[1] = Coo::Math::Transpose(camera.GetViewMatrix());
		matrix[2] = Coo::Math::Transpose(camera.GetPerspectiveMatrix());

		// Test 3d Lines
		SimpleDraw::AddLine(Vector3{ 0.0f }, Vector3{ 1.0f,0.0f,0.0f }, Colors::Red);
		SimpleDraw::AddLine(Vector3{ 0.0f }, Vector3{ 0.0f,1.0f,0.0f }, Colors::Green);
		SimpleDraw::AddLine(Vector3{ 0.0f }, Vector3{ 0.0f,0.0f,1.0f }, Colors::Blue);

		// Test 3d AABB
		SimpleDraw::AddAABB(-2.0f, 2.0f, -3.0f, 3.0f, -4.0f, 4.0f, Colors::Khaki);

		// Test 3d Sphere
		SimpleDraw::AddSphere(Vector3{ 0.0f }, 2.0f, Colors::BurlyWood);

		// Test 2d Rect
		SimpleDraw::AddScreenRect(5.0f, 1275.0f, 5.0f, 715.0f, Colors::CadetBlue);

		// Test 2d Lines
		SimpleDraw::AddScreenLine({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, Colors::Cyan);

		// Test 2d Circle
		SimpleDraw::AddScreenCircle({ 640.0f, 360.0f }, 100.0f, Colors::BlueViolet);

		SimpleDraw::Render(camera);

		Dui::BeginRender();
		ImGui::ShowDemoWindow();
		Dui::EndRender();

		GraphicsSystem::Get()->EndRender();
	}

	Dui::StaticTerminate();
	pixelShader.Terminate();
	vertexShader.Terminate();
	constantBuffer.Terminate();
	spaceTexture.Terminate();
	for (auto& texture : planetsTexture)
	{
		texture.Terminate();
	}
	sampler.Terminate();
	rasterizerState.Terminate();

	GraphicsSystem::StaticTerminate();
	InputSystem::StaticTerminate();

	myWindow.Terminate();
	SimpleDraw::StaticTerminate();
	return 0;
}
