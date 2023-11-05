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

	PhysBody* ballPlayer;

	PhysBody* death;

	int lives = 3;
	bool startThrow = true;
	float powerThrow = 0;
};
