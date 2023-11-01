#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	// 50 points circle
	CreateCircle(SCREEN_WIDTH / 2 - 20, 300, 30, 1.0f, true);

	// 25 points circle
	CreateCircle(450, 200, 20, 1.0f, true);
	
	// 100 points circle
	CreateCircle(550, 210, 25, 1.0f, true);

	//Scenary

	//Contorno
	int contorno[8] = {
		0, 0,
		500, 0,
		500, 750,
		0, 750,
	};
	CreateChain(255, 10, contorno, 8, 0.4f);

	//Paredes
	int rampDownLeft[10] = {
		0, 0,
		60, 0,
		180, 60,
		180, 125,
		0, 125,
	};
	CreateChain(255, 636, rampDownLeft, 10, 0.4f);

	int rampDownRight[10] = {
		180, 0,
		120, 0,
		0, 60,
		0, 125,
		180, 125,
	};
	CreateChain(525, 636, rampDownRight, 10, 0.4f);

	int triangleLeft[14] = {
		0, 0,
		5, 20,
		15, 40,
		25, 60,
		60, 110,
		30, 120,
		0, 150,
	};
	CreateChain(255, 300, triangleLeft, 14, 0.4f);

	int techoCirculo[28] = {
		0, 0,
		0, 200,
		15, 150,
		50, 100,
		110, 55,
		160, 35,
		220, 17,
		280, 17,
		340, 35,
		390, 55,
		450, 100,
		485, 150,
		500, 200,
		500, 0,

	};
	CreateChain(255, 10, techoCirculo, 28, 0.4f);

	int wallRight[44] = {
		0, 0,
		5, 10,
		10, 25,
		30, 45,
		50, 85,
		60, 135,
		60, 150,
		50, 190,
		35, 230,
		35, 235,
		40, 245,
		95, 290,
		95, 506,
		105, 506,
		105, 130,
		100, 110,
		90, 80,
		75, 50,
		60, 20,
		45, 0,
		20, -12,
		5, -10,

	};
	CreateChain(600, 130, wallRight, 44, 0.4f);

	int wallLeft[40] = {
		0, 0,
		5, 10,
		0, 20,
		-30, 40,
		-50, 70,
		-60, 120,
		-60, 170,
		-55, 200,
		-35, 230,
		-32, 240,
		-35, 245,
		-55, 235,
		-80, 200,
		-90, 170,
		-95, 120,
		-85, 70,
		-75, 40,
		-60, 20,
		-30, 0,
		-15, -5,


	};
	CreateChain(400, 130, wallLeft, 40, 0.4f);

	int LeftStick[20] = {
		0, 0,
		3, 1,
		6, 5,
		27, 40,
		27, 45,
		22, 48,
		15, 45,
		-9, 10,
		-9, 5,
		-6, 1,
	};
	CreateChain(390, 300, LeftStick, 20, 0.4f);

	int RightStick[20] = {
		0, 0,
		-3, 1,
		-6, 5,
		-27, 40,
		-27, 45,
		-22, 48,
		-15, 45,
		9, 10,
		9, 5,
		6, 1,
	};
	CreateChain(600, 300, RightStick, 20, 0.4f);

	int leftPad[22] = {
		0, 0,
		0, 45,
		5, 50,
		40, 70,
		45, 70,
		48, 67,
		51, 62,
		20, 0,
		15, -5,
		10, -8,
		5, -5,

	};
	CreateChain(340, 470, leftPad, 22, 0.7f);

	int rightPad[22] = {
		0, 0,
		0, 45,
		-5, 50,
		-40, 70,
		-45, 70,
		-48, 67,
		-51, 62,
		-20, 0,
		-15, -5,
		-10, -8,
		-5, -5,

	};
	CreateChain(620, 470, rightPad, 22, 0.7f);

	int CentrePad[24] = {
		0, 0,
		5, 0,
		30, 20,
		35, 25,
		30, 30,
		5, 45,
		0, 45,
		-5, 45,
		-30, 30,
		-35, 25,
		-30, 20,
		-5, 0,
	};
	CreateChain(SCREEN_WIDTH / 2 - 20, 390, CentrePad, 24, 0.7f);

	int leftArrow[14] = {
		0, 0,
		0, 20,
		5, 25,
		80, 65,
		90, 50,
		7, 5,
		3, -2,

	};
	CreateChain(300, 555, leftArrow, 14, 0.3f);

	int rightArrow[14] = {
		0, 0,
		0, 20,
		-5, 25,
		-80, 65,
		-90, 50,
		-7, 5,
		-3, -2,

	};
	CreateChain(660, 555, rightArrow, 14, 0.3f);
	

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, float res, bool isStatic)
{
	b2BodyDef body;
	if (isStatic == true) {
		body.type = b2_staticBody;
	}
	else {
		body.type = b2_dynamicBody;
	}
	
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = res;
	

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	//b->ApplyLinearImpulse()

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, float res)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.restitution = res;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {
			// test if the current body contains mouse position
				
				mouse_position = b2Vec2(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));
				if (f->GetShape()->TestPoint(b->GetTransform(), mouse_position) == true) {
					isInside = true;
					body_clicked = b;

					b2MouseJointDef def;
					def.bodyA = ground;
					def.bodyB = body_clicked;
					def.target = mouse_position;
					def.dampingRatio = 0.5f;
					def.frequencyHz = 2.0f;
					def.maxForce = 100.0f * body_clicked->GetMass();
					mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
					
				}


			}
			
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property


	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint != NULL) {
		b2Vec2 posInit = b2Vec2(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));
		mouse_joint->SetTarget(posInit);
		App->renderer->DrawLine(METERS_TO_PIXELS(mouse_joint->GetTarget().x), METERS_TO_PIXELS(mouse_joint->GetTarget().y), METERS_TO_PIXELS(body_clicked->GetPosition().x), METERS_TO_PIXELS(body_clicked->GetPosition().y), 255, 0, 0, 255);
		
	}

	// TODO 4: If the player releases the mouse button, destroy the joint
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP) {
		world->DestroyJoint(mouse_joint);
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);

	
}