#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleWindow.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	
	base1 = App->textures->Load("pinball/Sprites pinball/Finished/skull_pile 1.png");
	base2 = App->textures->Load("pinball/Sprites pinball/Finished/skull_pile 2.png");
	ball = App->textures->Load("pinball/Sprites pinball/Finished/skull_ball.png");
	bone1 = App->textures->Load("pinball/Sprites pinball/Finished/bone1.png");
	bone2 = App->textures->Load("pinball/Sprites pinball/Finished/bone2.png");
	bumper1 = App->textures->Load("pinball/Sprites pinball/Finished/skull bumper 1.png");
	bumper2 = App->textures->Load("pinball/Sprites pinball/Finished/skull bumper 2.png");
	bumper3 = App->textures->Load("pinball/Sprites pinball/Finished/skull bumper 3.png");
	rightwall = App->textures->Load("pinball/Sprites pinball/Finished/pder.png");
	leftwall = App->textures->Load("pinball/Sprites pinball/Finished/pizq.png");
	trizq = App->textures->Load("pinball/Sprites pinball/Finished/trizq.png");
	bg = App->textures->Load("pinball/Sprites pinball/Finished/bg.png");
	techo = App->textures->Load("pinball/Sprites pinball/Finished/techo.png");
	leftstick = App->textures->Load("pinball/Sprites pinball/Finished/leftstick.png");
	rightstick = App->textures->Load("pinball/Sprites pinball/Finished/rightstick.png");
	leftpad = App->textures->Load("pinball/Sprites pinball/Finished/leftpad.png");
	rightpad = App->textures->Load("pinball/Sprites pinball/Finished/rightpad.png");
	centerpad = App->textures->Load("pinball/Sprites pinball/Finished/centerpad.png");
	leftarrow = App->textures->Load("pinball/Sprites pinball/Finished/leftarrow.png");
	rightarrow = App->textures->Load("pinball/Sprites pinball/Finished/rightarrow.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50, 0);
	sensor->listener = this;

	//Contorno
	int contorno[8] = {
		0, 0,
		500, 0,
		500, 750,
		0, 750,
	};
	pinball = App->physics->CreateChain(255, 10, contorno, 8, 0.4f, false);

	// Box Start position
	boxStart = App->physics->CreateRectangle(730, 680, 50, 60, true);

	// Start Position Sensor
	startSensor = App->physics->CreateRectangleSensor(730, 660, 50, 60, 1);
	startSensor->listener = this;

	// Flippers
	Lflipper = App->physics->CreateRectangle(415, 605, 77, 15, false);
	Rflipper = App->physics->CreateRectangle(545, 605, 77, 15, false);

	pointLflipper = App->physics->CreateCircle(383, 605, 1, 0, true);
	pointRflipper = App->physics->CreateCircle(577, 605, 1, 0, true);

	// Set up Joints
	b2RevoluteJointDef first_joint;
	b2RevoluteJointDef second_joint;

	first_joint.Initialize(Lflipper->body, pointLflipper->body, pointLflipper->body->GetWorldCenter());
	first_joint.collideConnected = false;
	first_joint.enableLimit = true;
	first_joint.lowerAngle = -30 * DEGTORAD;
	first_joint.upperAngle = 30 * DEGTORAD;

	second_joint.Initialize(Rflipper->body, pointRflipper->body, pointRflipper->body->GetWorldCenter());
	second_joint.collideConnected = false;
	second_joint.enableLimit = true;
	second_joint.lowerAngle = -30 * DEGTORAD;
	second_joint.upperAngle = 30 * DEGTORAD;

	App->physics->world->CreateJoint(&first_joint);
	App->physics->world->CreateJoint(&second_joint);

	// Create Ball player
	ballradius = 12;
	ballPlayer = App->physics->CreateCircle(720, 600, ballradius, 0, false);
	ballPlayer->listener = this;



	ResetWholeGame();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	//Pallets controller
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		Lflipper->body->ApplyAngularImpulse(-10, true);
	else
		Lflipper->body->ApplyAngularImpulse(1, true);

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		Rflipper->body->ApplyAngularImpulse(10, true);
	else
		Rflipper->body->ApplyAngularImpulse(-1, true);

	if (startThrow && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		if (powerThrow < 500) {
			powerThrow += 10;
		}
		else {
			powerThrow = 500;
		}
		
	}

	if (startThrow && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
		ballPlayer->body->ApplyForceToCenter(b2Vec2(0, -powerThrow), true);
		startThrow = false;
	}

	ShowScore();


	int x, y;
	float a;


	App->renderer->Blit(bg, 255, 10);

	Lflipper->GetPosition(x, y);
	a = Lflipper->GetRotation();
	App->renderer->Blit(bone1, x, y, NULL, 1.0f, a, 32, 7);

	Rflipper->GetPosition(x, y);
	a = Rflipper->GetRotation();
	App->renderer->Blit(bone2, x, y, NULL, 1.0f, a, 45, 7);

	ballPlayer->GetPosition(x, y);

	a = ballPlayer->GetRotation();

	App->renderer->Blit(ball, x, y, NULL, 1.0f, a, ballradius, ballradius);


	
	App->physics->rampDownLeft1->GetPosition(x, y);
	App->renderer->Blit(base1, x, y);

	App->physics->rampDownRight1->GetPosition(x, y);
	App->renderer->Blit(base2, x, y);

	App->physics->bounce25->GetPosition(x, y);
	App->renderer->Blit(bumper1, x, y);
	
	App->physics->bounce100->GetPosition(x, y);
	App->renderer->Blit(bumper2, x, y);
	App->physics->bounce50->GetPosition(x, y);
	App->renderer->Blit(bumper3, x, y);

	App->physics->techoCirculo1->GetPosition(x, y);
	App->renderer->Blit(techo, x, y);
	
	App->physics->wallRight1->GetPosition(x, y);
	App->renderer->Blit(rightwall, x, y-12);

	App->physics->wallLeft1->GetPosition(x, y);
	App->renderer->Blit(leftwall, x-95, y - 5);

	App->physics->triangleLeft1->GetPosition(x, y);
	App->renderer->Blit(trizq, x , y);
	
	App->physics->LeftStick1->GetPosition(x, y);
	App->renderer->Blit(leftstick, x-9, y);

	App->physics->RightStick1->GetPosition(x, y);
	App->renderer->Blit(rightstick, x - 27, y);

	App->physics->leftPad1->GetPosition(x, y);
	App->renderer->Blit(leftpad, x, y-8);

	App->physics->rightPad1->GetPosition(x, y);
	App->renderer->Blit(rightpad, x-51, y - 8);

	App->physics->CentrePad1->GetPosition(x, y);
	App->renderer->Blit(centerpad, x - 35, y);

	App->physics->leftArrow1->GetPosition(x, y);
	App->renderer->Blit(leftarrow, x, y - 2);

	App->physics->rightArrow1->GetPosition(x, y);
	App->renderer->Blit(rightarrow, x - 90, y - 2);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::ShowScore()
{
	static char title[256];
	sprintf_s(title, 256, "Current Score: %d Highscore: %d Lifes: %d", score, highscore, lives);

	App->window->SetTitle(title);
}

void ModuleSceneIntro::ResetSmallGame()
{
	ballPlayer->body->SetTransform(b2Vec2(PIXEL_TO_METERS(720), PIXEL_TO_METERS(600)), 0);
	ballPlayer->body->SetLinearVelocity(b2Vec2(0, 0));
	lives--;
	startThrow = true;
	powerThrow = 0;
}

void ModuleSceneIntro::ResetWholeGame()
{
	ballPlayer->body->SetTransform(b2Vec2(PIXEL_TO_METERS(720), PIXEL_TO_METERS(600)), 0);
	ballPlayer->body->SetLinearVelocity(b2Vec2(0, 0));
	lives = 3;
	startThrow = true;
	powerThrow = 0;
	highscore = score;
	score = 0;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

	if (bodyB == App->physics->bounce25) {
		score += 25;
		App->audio->PlayFx(bonus_fx);
	}
	else if (bodyB == App->physics->bounce50) {
		score += 50;
		App->audio->PlayFx(bonus_fx);
	}
	else if (bodyB == App->physics->bounce100) {
		score += 100;
		App->audio->PlayFx(bonus_fx);
	}

	if (bodyA->body->GetFixtureList()->IsSensor()) // Sensor manage
	{
		if (bodyA->type == 0) // Death Sensor
		{
			if (lives != 0) {
				ResetSmallGame();
			}
			else {
				ResetWholeGame();
			}
			
		}
		else if (bodyA->type == 1) // Start Position Sensor
		{
			startThrow = true;
		}
	}
}
