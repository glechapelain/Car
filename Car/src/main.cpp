#include <algorithm>
#include <list>

#include <raylib.h>
#include <raymath.h>

#include "Car.h"
#include "Model.h"
#include "Particle.h"
#include "Track.h"

enum eStage {
    eStage_numPlayerSelect,
    eStage_CarSelect,
    eStage_Game,
    eStage_WrapUpScreen,
};

constexpr int NUM_CAR_MODELS = 8;
static float selectionMenuRotationSpeed = 50.f;// Degree / sec.

void
SetCameraForMenu(Camera& camera)
{
	camera.position = { 0.0f, 15.0f, 25.0f };  // Camera position
	camera.target = { 0.0f, -3.5f, 0.0f };     // Camera looking at point
	camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type
	UpdateCamera(&camera, CAMERA_FREE);
}

const int screenHeight = 450;
const int screenWidth = 800;

float carScale = 5.f;

void
SetCameraForGame(Camera& camera)
{
	camera.position = { screenWidth / 2.f, 15.0f, screenHeight / 2.f };   // Camera position
	camera.target = { screenWidth / 2.f, -3.5f, screenHeight / 2.f };     // Camera looking at point
	camera.up = { 0.0f, 0.0f, -1.0f };         // Camera up vector (rotation towards target)
	camera.projection = CAMERA_ORTHOGRAPHIC;             // Camera mode type
	camera.fovy = float(screenHeight);             // Camera field-of-view Y
	UpdateCamera(&camera, CAMERA_FREE);
}

template <typename Units>
bool
IsCarFinished(Car<Units> const &car)
{
	return car.isFinished();
}

int main(void)
{

    eStage currentStage = eStage_numPlayerSelect;
	int currentPlayer = 0; // Player currently interacting (for selection & game screen.)

    Car<int> car; // Should be changed to `cars' ?
	std::list<Particle> particles;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");
    SetTargetFPS(60);						  // Set our game to run at 60 frames-per-second

	std::list<Graphics> models;
	// loadCars
	for (int i = 1; i <= NUM_CAR_MODELS; ++i) {
		std::string fileName = "res/Car.obj";
		std::string textureName = "res/Text"; 
		textureName += std::to_string(i);
		textureName += ".png";
		models.emplace_back(fileName, textureName);
	}

	Camera camera = { 0 };
	SetCameraForMenu(camera);

	auto selectedModel = models.begin();
	std::list<int> finishOrder;

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        BeginDrawing();
        {
			ClearBackground(RAYWHITE);
			switch (currentStage) {
			case eStage_numPlayerSelect:
			{
				DrawText("SELECT THE NUMBER OF PLAYERS", 10, 10, 20, BLUE);
				for (int i = 1; i <= 9; ++i)
					if (IsKeyPressed(i + KEY_ZERO) || IsKeyPressed(i + KEY_KP_0))
					{
						//cars.resize(i);

						//for(int cari = 0; cari < cars.size(); ++cari)
							car/*s[cari]*/.initializPosition(circuit/*, cari, cars.size() Would be nice having that working*/);

						currentStage = eStage_CarSelect;
						currentPlayer = 1;
						break;
					}

				break;
			}
			case eStage_CarSelect:
			{

				if (IsKeyPressed(KEY_LEFT)) {
					if (selectedModel == models.begin())
						selectedModel = models.end();
					--selectedModel;
				}
				else if (IsKeyPressed(KEY_RIGHT)) {
					selectedModel++;
					if (selectedModel == models.end())
						selectedModel = models.begin();
				}
				else if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
					car/*s[currentPlayer-1]*/.graphics = std::make_unique<Graphics>(std::move(*selectedModel));
					selectedModel = models.erase(selectedModel);
					if (models.end() == selectedModel)
						selectedModel = models.begin();

					// if (cars.size() == currentPlayer)
					{
						currentPlayer = 1;
						currentStage = eStage_Game;
						SetCameraForGame(camera);
					}
					++currentPlayer;
				}

				DrawText(TextFormat("PLAYER %i SELECT YOUR CAR", currentPlayer), 10, 10, 20, BLUE);

				BeginMode3D(camera);
				{
					int i = 0;
					static float angle = 10.f;
					angle += GetFrameTime() * selectionMenuRotationSpeed;
					if (angle > 360.f)
						angle -= 360.f;
					auto modelToDisplay = selectedModel;
					while (models.end() != modelToDisplay) {
						float angleToPutTheModelAt = PI * 2.f * float(i) / float(models.size());

						modelToDisplay->Render(Vector3Transform(Vector3{ 0.f, 0.f, 12.f }, MatrixRotateY(angleToPutTheModelAt)), angle);
						++i;
						++modelToDisplay;
					}
					if (models.begin() != selectedModel)
					{
						i = -1;

						modelToDisplay = std::prev(selectedModel);
						do
						{
							float angleToPutTheModelAt = PI * 2.f * float(i) / float(models.size());

							modelToDisplay->Render(Vector3Transform(Vector3{ 0.f, 0.f, 12.f }, MatrixRotateY(angleToPutTheModelAt)), angle);
							if (models.begin() == modelToDisplay)
								break;
							--i;
							--modelToDisplay;
						} while(true);

					}

					DrawGrid(20, 10.0f);         // Draw a grid
				}
				EndMode3D();
			}
			break;
			case eStage_Game:
			{
				// Car<int>& car = cars[currentPlayer-1];
				for (int i = 1; i <= 9; ++i)
					if (IsKeyPressed(i + KEY_ZERO) || IsKeyPressed(i + KEY_KP_0))
					{
						Vector2d<float> positionOfImpact;
						car.amendVelocity(i);
						if (car.getNextPosition() != car.getPosition()
							&& car.collidesWith(circuit, positionOfImpact))
						{
							car.setPositionToClosestPointOnCircuit(circuit);
							car.zeroSpeed();

							int numOfParticlesToGenerate = GetRandomValue(60, 80);
							while (numOfParticlesToGenerate > 0)
							{
								Vector3d<float> variation(float(GetRandomValue(60, 80))/70.f,
									float(GetRandomValue(60, 80)) / 70.f,
									float(GetRandomValue(60, 80)) / 70.f);
								Vector3d<float> initialVelocity(car.getVelocity().x, car.getVelocity().y, 1.f);
								initialVelocity *= variation;
								Vector3d<float> positionOfImpact3D(positionOfImpact.x, positionOfImpact.y, 1.f * carScale);
								particles.emplace_back(positionOfImpact3D, initialVelocity * float(gridUnit), float(GetRandomValue(32, 55)) / 10.f);

								--numOfParticlesToGenerate;
							}
						}
						car.update(circuit);
						if (car.isFinished())
						{
							finishOrder.push_back(currentPlayer);

							//if (!std::any_of(cars.begin(), cars.end(), [](auto const& car) {return !IsCarFinished(car); }))
							if (!!IsCarFinished(car))
							{
								currentStage = eStage_WrapUpScreen;
								SetCameraForMenu(camera);
							}
						}

						while(finishOrder.end() != std::find(finishOrder.begin(), finishOrder.end(), currentPlayer))
							++currentPlayer;
					}
			
				BeginMode3D(camera);
				{
					DrawGrid(80, gridUnit);         // Draw a grid
				}
				EndMode3D();

				circuit.render();

				BeginMode3D(camera);
				{
					// render particle shadows
					Vector3d/* just 3 for the maths. */<float> lightDirection = {.4f, .3f, .0f};
					for (Particle const& particle : particles)
					{
						Vector3d<float> shadowPosition = particle.getPosition() + lightDirection * particle.getPosition().z;
						DrawCircle(shadowPosition.x, shadowPosition.y, 1.f, DARKGRAY);
					}

					//for (auto const& car : cars)
					{
						car.graphics->Render(Vector3{ float(car.getPosition().x * gridUnit), .5f, float(car.getPosition().y * gridUnit)}, car.getOrientation(), carScale);
					}

					for (Particle const& particle : particles)
					{
						DrawCircle(particle.getPosition().x, particle.getPosition().y, 1.f, ORANGE);
					}
				}
				EndMode3D();

				//for (auto const& car : cars)
				{
					DrawLine(gridUnit * car.getPosition().x, gridUnit * car.getPosition().y, gridUnit * car.getNextPosition().x, gridUnit * car.getNextPosition().y, BLUE);
				}
			}
			break;
			case eStage_WrapUpScreen:
			{
				if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
					currentStage = eStage_numPlayerSelect;
					finishOrder.clear();
				}
				DrawText("RANKING", 10, 10, 20, BLUE);

				BeginMode3D(camera);
				{
					static float angle = 10.f;
					angle += GetFrameTime() * selectionMenuRotationSpeed;

					int i = 0;
					for (int winner : finishOrder)
					{
						car/*s[winner]*/.graphics->Render(Vector3{ 0.f, 5.f - 1.2f * float(i), 0.f}, angle);
						++i;
					}
				}
				EndMode3D();
			}
			break;
			}
        }
        EndDrawing();

    }
}
