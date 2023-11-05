#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void ResetSmallGame();
	void ResetWholeGame();
	void ShowScore();

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* base1;
	SDL_Texture* base2;
	SDL_Texture* bone1;
	SDL_Texture* bone2;
	SDL_Texture* ball;
	SDL_Texture* bumper1;
	SDL_Texture* bumper2;
	SDL_Texture* bumper3;
	SDL_Texture* bg;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

public:

	PhysBody* pinball;
	PhysBody* Lflipper;
	PhysBody* Rflipper;
	PhysBody* pointLflipper;
	PhysBody* pointRflipper;
	PhysBody* boxStart;
	PhysBody* startSensor;
	float brush = 0.0f;

	PhysBody* ballPlayer;
	int ballradius;
	PhysBody* death;

	int lives = 3;
	bool startThrow = true;
	float powerThrow = 0;

	int score = 0;
	int highscore = 0;
};
