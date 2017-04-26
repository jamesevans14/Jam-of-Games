#pragma once
#include "HAPISprites_lib.h"
#include "EntityBullet.h"
#include "EntityPlayer.h"
#include "EntityMenu.h"
#include "EntityEnemy.h"
#include "Room.h"

class CEntity;

class World
{
public:
	World();
	~World();

	enum menuStates
	{
		eMainMenu, ePlay, eGameOver, ePaused, eCreation, eWin,eControls
	};

	void Run();
	void Initialise();
	void Playing();
	void mainMenu();
	std::vector<CEntityBullet*> getBullets() { return bulletVector; }
	Point getPlayerPos() { return player_->getPos(); }
	
private:
	menuStates currentState = eMainMenu;

	int deadzoneLeft = HK_GAMEPAD_LEFT_THUMB_DEADZONE;
	std::vector<CEntity*> entityVector;
	std::vector<CEntityBullet*> bulletVector;
	CEntityPlayer* player_ = new CEntityPlayer;
	float currTime = 0;
	float updateTime = 0;
	Room* First_Room = new Room;
	Room* Second_Room = new Room;
	int optionSelected = 0;

	Point Position_To_Spawn{ 250,250 };


	Point Position_To_Spawn_second{ 450,450 };
};

