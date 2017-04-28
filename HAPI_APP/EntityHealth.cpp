#include "EntityHealth.h"



EntityHealth::EntityHealth()
{
	sprite_= new Sprite(HAPI_Sprites.MakeSurface("Data//HPHeartFull.png"));
	spriteHalf_ = new Sprite(HAPI_Sprites.MakeSurface("Data//HPHeartHalf.png"));
	spriteEmpty_ = new Sprite(HAPI_Sprites.MakeSurface("Data//HPHeartEmpty.png"));
	initialiseValues();
}


EntityHealth::~EntityHealth()
{
	delete spriteHalf_;
	delete spriteEmpty_;
}

void EntityHealth::initialiseValues()
{
	//push them onto vector?
	//set to alive?
	alive_ = true;
	pos_ = { 15,15 };

}

void EntityHealth::update(World& world)
{
	//Have checks to see how many full hearts are needed?
	playerHealth = world.getPlayerHealth();

	if (playerHealth >= 4)
	{
		//load 2 full hearts
		//is2 = true
	}
	if (playerHealth >= 2 && playerHealth < 4)
	{
		//1 full heart
		//is2 = false, is1 = true
	}
	if (playerHealth < 2)
	{
		//0 full hearts
		//is0 = true
	}

	if (is2 == true && playerHealth == 5)
	{
		//load third heart as half
	}

}

void EntityHealth::render()
{
	offsetHeart = 0;
	
	for (int i = 0; i < playerHealth / 2; ++i)
	{
		sprite_->Render(SCREEN_SURFACE, pos_ + Point{ 70 * offsetHeart,0 });
		offsetHeart++;

	}

	if (playerHealth % 2 == 1)
	{
		spriteHalf_->Render(SCREEN_SURFACE, pos_ + Point{ 70 * offsetHeart,0 });
		offsetHeart++;
	}


	for (int i = 0; i < (maxHealth - playerHealth) / 2; ++i)
	{
		spriteEmpty_->Render(SCREEN_SURFACE, pos_ + Point{ 70 * offsetHeart,0 });
		offsetHeart++;
	}


}
