#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height, type;
	bool open_chain;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, float res, bool isStatic, bool isSensor = false, int type = 999);
	PhysBody* CreateRectangle(int x, int y, int width, int height, bool isStatic);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, int type);
	PhysBody* CreateChain(int x, int y, int* points, int size, float res, bool open_chain = false);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

public:
	PhysBody* bounce25;
	PhysBody* bounce50;
	PhysBody* bounce100;
	PhysBody* rampDownLeft1;
	PhysBody* rampDownRight1;
	PhysBody* triangleLeft1;
	PhysBody* techoCirculo1;
	PhysBody* wallRight1;
	PhysBody* wallLeft1;
	PhysBody* LeftStick1;
	PhysBody* RightStick1;
	PhysBody* leftPad1;
	PhysBody* rightPad1;
	PhysBody* leftArrow1;
	PhysBody* rightArrow1;

	b2World* world;

	

private:

	bool debug;
	bool isInside;
	
	b2MouseJoint* mouse_joint;
	b2Body* ground;
	b2Body* body_clicked;
	b2Vec2 mouse_position;



};