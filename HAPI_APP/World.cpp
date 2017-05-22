#include "World.h"


using namespace HAPISPACE;
World::World()
{
}


World::~World()
{
	for (auto p : entityVector)
		delete p;

	for (auto p : bulletVector)
		delete p;
}

void World::Run()
{
	
	Initialise();

	while (HAPI_Sprites.Update())
	{
		//
		HAPI_TMouseData mouse = HAPI_Sprites.GetMouseData();
		SCREEN_SURFACE.Clear();
		if (currentState == eMainMenu) //Main Menu goes here
		{
			mainMenu();

			if (mouse.rightButtonDown)
			{
				currentState = ePlay;
			}
		}

		if (currentState == ePlay)
		{
			Playing();
			//do playing function
			if (mouse.leftButtonDown)
			{
				currentState = eGameOver;
			}
		}

		if (currentState == eGameOver)
		{
			HAPI_Sprites.RenderText(660, 540, HAPI_TColour(255, 255, 0), "Game Over State", 70);
			//do gameover function
			if (mouse.middleButtonDown)
			{
				for (auto p : entityVector)
					p->initialiseValues();

				currentState = ePlay;
			}
		}

		if (currentState == eCharacter)
		{
			charCreation();
		}
	}
}

void World::Initialise()
{
	//healthVector.push_back()

	entityVector.push_back(player_);

	for (int i = 0; i < 500; i++)
	{
		CEntityBullet *bullet = new CEntityBullet;
		bulletVector.push_back(bullet);
		//max of 500 bullets
	}


	First_Room = new Room("Room_Floor_1.png", Position_To_Spawn, "Corners_And_Walls_Room_1.png", 32);


	First_Room->Create_Invidividual_Room();


	Second_Room = new Room("Room_Floor_1_.png", Position_To_Spawn_second, "Corners_And_Walls_Room_1.png", 32);


	auto test_texture = HAPI_Sprites.MakeSurface("Data\\Room_Floor_1.png");

	Second_Room->Create_Complex_Room(test_texture);


	EntityEnemy* enemy_;

	for (int i = 0; i < 20; i++)
	{


		int select = (rand() % 2);

		EnemyType type_select;

		switch (select)
		{
		case 0:
			type_select = eMelee;
			break;
		case 1:
			type_select = eRanged;
			break;
		case 2:
			type_select = eBrute;
			break;
		}

		spawnenemy(enemy_, First_Room->getPos(),First_Room->getsize(), "",type_select);
	}

	spawnenemy(enemy_, First_Room->getPos(), First_Room->getsize(),"",eBoss );

	//here we would add enemies to enemy vector to set a max number of enemies, all initally dead. then set however many we want to alive as you enter a room
	EntityHealth* health = new EntityHealth();
	entityVector.push_back(health);





}

void World::Playing()
{
	if (player_->isAlive() == false)
		currentState = eGameOver;

	currTime = HAPI_Sprites.GetTime();

	if (currTime >= updateTime)
	{
		for (auto p : entityVector)
			p->update(*this);


		for (auto p : bulletVector) //seperate bullet vector so i can pass them through
			p->update(*this);



		for (auto p : entityVector)
			for (auto r : entityVector)
			{
				if (p->isAlive() && r->isAlive() && p->getSide() != r->getSide() && p->isInvunerable() == false && r->isInvunerable() == false)
					if (p->getPntrToSprite()->CheckCollision(p->getPos(), r->getSprite(), r->getPos()) == true)
					{
						p->hasCollided(*r);
						r->hasCollided(*p);
					}
			}

		for (auto p : entityVector)
			for (auto r : bulletVector)
			{
				if (p->isAlive() == true && r->isAlive() == true && p->getSide() != r->getSide() && p->isInvunerable() == false)
					if (p->getPntrToSprite()->CheckCollision(p->getPos(), r->getSprite(), r->getPos()) == true)
					{
						p->hasCollided(*r);
						r->hasCollided(*p);
					}
			}
		updateTime = HAPI_Sprites.GetTime() + 30.0f;
	}

	First_Room->Render_Floor(getPlayerPos());

	Second_Room->Render_Floor(getPlayerPos());


	for (auto p : entityVector) //might be better to have a single vector instead of two and have the offset for where the bullets start
		p->render(getPlayerPos());

	for (auto p : bulletVector) //also the render is seperate to the update as update is every tick, render may be slowed down
		p->render(getPlayerPos());
}

void World::mainMenu()
{
	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0);
	static int trans1 = 255;
	static int trans2 = 70;
	static float timelimit = 0;

	menuStates selectedState = eCharacter;
	

	sp->setPosition(Point{ 1725,980 });
	HAPI_Sprites.RenderText(1650, 990, HAPI_TColour(255, 255, 255, 255), "Press		 to select", 24);

	HAPI_Sprites.RenderText(0, 750, HAPI_TColour(255, 255, 255, trans1), "Play", 84);
	HAPI_Sprites.RenderText(0, 870, HAPI_TColour(255, 255, 255, trans2), "Controls", 84);
	bg->render(getPlayerPos());
	sp->render(getPlayerPos());
	
	if (conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER]) //selection
	{
		currentState = selectedState;
	}
	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -deadzoneLeft && HAPI_Sprites.GetTime() > timelimit) //changing selection
	{
		timelimit = HAPI_Sprites.GetTime() + 200;
		optionSelected += 1;
		if (optionSelected >= 2)
			optionSelected = 0;

		if (optionSelected == 0)
		{
			trans1 = 255;
			trans2 = 70;
			selectedState = eCharacter;

		}
		else
		{
			trans1 = 70;
			trans2 = 255;
			selectedState = eControls;

		}
	}
	else if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > deadzoneLeft && HAPI_Sprites.GetTime() > timelimit) //changing selection
	{

		optionSelected += 1;
		if (optionSelected >= 2)
			optionSelected = 0;

		if (optionSelected == 0)
		{
			trans1 = 255;
			trans2 = 70;
			selectedState = eCharacter;

		}
		else
		{
			trans1 = 70;
			trans2 = 255;
			selectedState = eControls;

		}
	}
}

void World::charCreation()
{
	//Menu
	totalStats = healthPoints + speedPoints + ratePoints + damagePoints;
	totalPoints - totalStats;
	static float timelimit = 0;
	static int trans1 = 255;
	static int trans2 = 70;
	static int trans3 = 70;
	static int trans4 = 70;

	HAPI_Sprites.RenderText(650, 200, HAPI_TColour(255, 255, 255, 255), 0, 0, "Choose Your Stats!", 60);
	HAPI_Sprites.RenderText(500, 350, HAPI_TColour(255, 255, 255, trans1), 0, 0, "Health:", 34);
	HAPI_Sprites.RenderText(500, 450, HAPI_TColour(255, 255, 255, trans2), 0, 0, "Speed:", 34);
	HAPI_Sprites.RenderText(500, 550, HAPI_TColour(255, 255, 255, trans3), 0, 0, "Fire Rate:", 34);
	HAPI_Sprites.RenderText(500, 650, HAPI_TColour(255, 255, 255, trans4), 0, 0, "Damage:", 34);

	HAPI_Sprites.RenderText(900, 350, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(healthPoints), 34);
	HAPI_Sprites.RenderText(900, 450, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(speedPoints), 34);
	HAPI_Sprites.RenderText(900, 550, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(ratePoints), 34);
	HAPI_Sprites.RenderText(900, 650, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(damagePoints), 34);

	HAPI_Sprites.RenderText(1000, 270, HAPI_TColour(255, 255, 255, 255), 0, 0, "Points Remaining:", 34);
	HAPI_Sprites.RenderText(1400, 272, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(totalPoints), 34);

	sp->setPosition(Point{ 1725,980 });
	HAPI_Sprites.RenderText(1650, 990, HAPI_TColour(255, 255, 255, 255), "Press		 to select", 24);

	bg->render(getPlayerPos());
	sp->render(getPlayerPos());
	

	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0);

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -deadzoneLeft && HAPI_Sprites.GetTime() > timelimit) //changing selection
	{
		timelimit = HAPI_Sprites.GetTime() + 300;
		std::cout << "Down Press" << std::endl;
		optionSelected += 1;
	}
	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > deadzoneLeft && HAPI_Sprites.GetTime() > timelimit) //changing selection
	{
		timelimit = HAPI_Sprites.GetTime() + 300;
		std::cout << "Down Press" << std::endl;
		optionSelected -= 1;
	}



		if (optionSelected >= 4)
			optionSelected = 0;

		if (optionSelected < 0)
			optionSelected = 3;

		isHealth = false;
		isSpeed = false;
		isRate = false;
		isDamage = false;

		if (optionSelected == 0)
		{
			trans1 = 255;
			trans2 = 70;
			trans3 = 70; //health
			trans4 = 70;
			isHealth = true;
		}
		if (optionSelected == 1)
		{
			trans1 = 70;
			trans2 = 255;
			trans3 = 70; //speed
			trans4 = 70;
			isSpeed = true;
		}
		if (optionSelected == 2)
		{
			trans1 = 70;
			trans2 = 70; //firerate
			trans3 = 255;
			trans4 = 70;
			isRate = true;
		}
		if (optionSelected == 3)
		{
			trans1 = 70;
			trans2 = 70; //damage
			trans3 = 70;
			trans4 = 255;
			isDamage = true;
		}
		

#pragma region Stats Selection

	if (totalPoints > 0)
	{
		if (healthPoints < 1 || healthPoints > 8)
			healthPoints = 1;

		if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > deadzoneLeft && isHealth == true && HAPI_Sprites.GetTime() > timelimit)
		{
			timelimit = HAPI_Sprites.GetTime() + 300;
			healthPoints++;
		}

		if (ratePoints < 1 || ratePoints > 8)
			ratePoints = 1;

		if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > deadzoneLeft && isRate == true && HAPI_Sprites.GetTime() > timelimit)
		{
			timelimit = HAPI_Sprites.GetTime() + 300;
			ratePoints++;
		}
		
		if (speedPoints < 1 || speedPoints > 8)
			speedPoints = 1;

		if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > deadzoneLeft && isSpeed == true && HAPI_Sprites.GetTime() > timelimit)
		{
			timelimit = HAPI_Sprites.GetTime() + 300;
			speedPoints++;
		}

		if (damagePoints < 1 || damagePoints > 8)
			damagePoints = 1;

		if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > deadzoneLeft && isDamage == true && HAPI_Sprites.GetTime() > timelimit)
		{
			timelimit = HAPI_Sprites.GetTime() + 300;
			damagePoints++;
		}
	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -deadzoneLeft && isHealth == true && HAPI_Sprites.GetTime() > timelimit)
	{
		timelimit = HAPI_Sprites.GetTime() + 300;
		if (healthPoints == 1)
		{
			healthPoints = 1;
		}
		healthPoints--;
	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -deadzoneLeft && isRate == true && HAPI_Sprites.GetTime() > timelimit)
	{
		timelimit = HAPI_Sprites.GetTime() + 300;
		if (ratePoints == 1)
		{
			ratePoints = 1;
		}
		ratePoints--;
	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -deadzoneLeft && isSpeed == true && HAPI_Sprites.GetTime() > timelimit)
	{
		timelimit = HAPI_Sprites.GetTime() + 300;
		if (speedPoints == 1)
		{
			speedPoints = 1;
		}
		speedPoints--;
	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -deadzoneLeft && isDamage == true && HAPI_Sprites.GetTime() > timelimit)
	{
		timelimit = HAPI_Sprites.GetTime() + 300;
		if (damagePoints == 1)
		{
			damagePoints = 1;
		}
		damagePoints--;
	}
#pragma endregion

	totalPoints = 12 - (healthPoints + ratePoints + damagePoints + speedPoints);

	if (conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER] && totalPoints == 0)
	{
		player_->initialiseValues(healthPoints, speedPoints, ratePoints, damagePoints);
		currentState = ePlay;
	}


}

void World::spawnenemy(EntityEnemy* enemy_, Point tl, int room_size, std::string sprite, EnemyType type)
{
	int posX = rand() % tl.x + room_size;
	int posY = rand() % tl.y + room_size;
	
	switch (type)
	{
		case eMelee:
			enemy_ = new CEntityEnemyMelee("Data//fireBall.png");
			break;
		case eRanged:
			enemy_ = new CEntityRangedEnemy("Data//rocketUp.png");
			break;
		case eBrute:
			enemy_ = new CEntityBruteEnemy("Data//HPHeartEmpty.png");
			break;
		case eBoss:
			enemy_ = new CEntityEnemyBOSS("Data//HAPI Sprites Logo.png");
			break;
	}	


	Point pos = { posX,posY };

	enemy_->setpos(pos);

	entityVector.push_back(enemy_);
}

