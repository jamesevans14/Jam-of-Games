#pragma once
#include "HAPISprites_lib.h"
#include "EntityBullet.h"
#include "EntityPlayer.h"
#include "EntityMenu.h"
#include "EntityEnemy.h"
#include "EntityRangedEnemy.h"
#include "EntityEnemyMelee.h"
#include "EntityBruteEnemy.h"
#include "EntityEnemyBOSS.h"
#include "EntityHealth.h"
#include "EntityPickup.h"
#include "Room.h"
#include <random>

class CEntity;

class World
{
public:
	World();
	~World();

	enum EnemyType
	{
		eMelee, eRanged, eBrute, eBoss, eplayer	
	};

	enum menuStates
	{
		eMainMenu, ePlay, eGameOver, ePaused, eCreation, eWin, eControls, eCharacter
	};

	void Run();
	void Initialise();
	void Playing();
	void mainMenu();
	void charCreation();
	void endGame();
	void pause();
	void Create_Rooms(int Number_of_Rooms, int Texture_Size);
	void Connect_Rooms();
	int Generate_random_vector(int minimum_value, int maximum_values);
	std::vector<CEntityBullet*> getBullets() { return bulletVector; }
	Point getPlayerPos() { return player_->getPos(); }
	int getPlayerHealth() { return player_->getHealth(); }
	int getPlayerMaxHealth() { return player_->getMaxHealth(); }
	void spawnenemy(EntityEnemy* enemy_, Point tl, Rectangle room_size, std::string sprite, EnemyType type);
	void activatenemy(Point tl, Rectangle roomsize, EnemyType type);
	
private:
	menuStates currentState = eMainMenu;

	int deadzoneLeft = HK_GAMEPAD_LEFT_THUMB_DEADZONE;

	std::vector<CEntity*> entityVector;
	std::vector<CEntityBullet*> bulletVector;
	std::vector<CEntity*> healthVector;
	std::vector<Room> Rooms;

	CEntityPlayer* player_ = new CEntityPlayer;

	float currTime = 0;
	float updateTime = 0;

	Room* First_Room = new Room;
	Room* Second_Room = new Room;

	int optionSelected = 0;
	int currentHealth = 6;

	Point Position_To_Spawn{ 250,250 };
	Point Position_To_Spawn_second{ 2000, 450 };

	//Character Creation Variables
	unsigned int totalPoints = 8;
	unsigned int healthPoints = 1;
	unsigned int speedPoints = 1;
	unsigned int ratePoints = 1;
	unsigned int damagePoints = 1;
	unsigned int totalStats;
	bool isHealth = false;
	bool isSpeed = false;
	bool isRate = false;
	bool isDamage = false;

	CEntityMenu *bg = new CEntityMenu("Data//Background.jpg");
	CEntityMenu *sp = new CEntityMenu("Data//XboxRTLogo.png");

	std::default_random_engine rand_engine;
	int number_of_rooms;
};

