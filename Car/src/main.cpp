#include <algorithm>
#include <list>

#include <raylib.h>
#include <raymath.h>

#include "Car.h"
#include "Model.h"
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

template <typename Units>
bool
IsCarFinished(Car<Units> const &car)
{
	return car.isFinished();
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    eStage currentStage = eStage_numPlayerSelect;
	int currentPlayer = 0; // Player currently interacting (for selection & game screen.)

    Car<int> car; // Should be changed to `cars' ?
    Vector2d<float> v1(0.f, 0.f);
    Vector2d<float> v2(1.f, 1.f);
    Vector2d<float> v3(0.f, 1.f);
    Vector2d<float> v4(1.f, 0.f);

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

						car.initializPosition(circuit);

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
					car.graphics = std::make_unique<Graphics>(std::move(*selectedModel));
					selectedModel = models.erase(selectedModel);
					if (models.end() == selectedModel)
						selectedModel = models.begin();

					// if (cars.size() == currentPlayer)
					{
						currentPlayer = 1;
						currentStage = eStage_Game;
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
				for (int i = 1; i <= 9; ++i)
					if (IsKeyPressed(i + KEY_ZERO) || IsKeyPressed(i + KEY_KP_0))
					{
						car/*s[currentPlayer]*/.amendVelocity(i);
						if (car.getNextPosition() != car.getPosition()
							&& car.collidesWith(circuit))
						{
							car.setPositionToClosestPointOnCircuit(circuit);
							car.zeroSpeed();
						}
						car/*s[currentPlayer]*/.update(circuit);
						if (car.isFinished())
						{
							finishOrder.push_back(currentPlayer);
						}

						do {
						++currentPlayer;
						} while (car/*s[currentPlayer]*/.isFinished() && currentPlayer < 8 /* just to have an end condition with only one player */);
						//if (cars.size() == currentPlayer)
						{
							currentPlayer = 0;
						}
					}

				//if(!std::any_of(cars.begin(), cars.end(), !IsCarFinished))
				if (!!IsCarFinished(car))
				{
					currentStage = eStage_WrapUpScreen;
				}
			
				for (int i = 0; i <= screenWidth; i += gridUnit)
				{
					DrawLine(i, 0, i, screenHeight, BLACK);
				}

				for (int j = 0; j <= screenHeight; j += gridUnit)
				{
					DrawLine(0, j, screenWidth, j, BLACK);
				}
				circuit.render();
				//for (auto const& car : cars)
				{
					DrawLine(gridUnit * car.getPosition().x, gridUnit * car.getPosition().y, gridUnit * car.getNextPosition().x, gridUnit * car.getNextPosition().y, BLUE);

					DrawRectangle(gridUnit * car.getPosition().x - 10, gridUnit * car.getPosition().y - 10, gridUnit, gridUnit, RED);
				}
			}
			break;
			case eStage_WrapUpScreen:
			{
				if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
					SetCameraForMenu(camera);
					currentStage = eStage_numPlayerSelect;
					finishOrder.clear();
				}
			}
			break;
			}
        }
        EndDrawing();

    }
}
