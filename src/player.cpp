#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>	

#include "player.h"
#include "entity.h"
#include "ground.h"

const float GRAVITY = 0.09f;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;
const int ALIVE = 0;
const int CURSOR_DEATH = 1;
const int HOLE_DEATH = 2;
const int NO_HEALTH_DEATH = 3;

Player::Player(float p_x, float p_y, std::vector<SDL_Texture*> p_tex)
	: Entity{ p_x, p_y, p_tex}
{
	setX(SCREEN_WIDTH/2 - getWidth()/2);
	setY(SCREEN_HEIGHT - getHeight() - 64);
}

float Player::distanceFromCursor()
{
	int mouseX = 0;
	int mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);

	return sqrt(pow((getX() + getWidth()/2) - mouseX, 2) + pow((getY() + getHeight()/2) - mouseY, 2));
}
bool Player::jump()
{
	if (distanceFromCursor() < 100)
	{
		if (grounded) {
    		doubleJump = true;  
		}

		if (grounded || doubleJump) {
			velocityY = -(1 / distanceFromCursor() * 200);
				
			if (!grounded) {
				highestPointY = getY();
				doubleJump = false; 
			}
			grounded = false;  
			return true;
		}

	}
	return false;
}

float Player::clamp(float p_value, float p_min, float p_max)
{
	if (p_value > p_max)
		return p_max;
	if (p_value < p_min)
		return p_min;
	return p_value;
}

void Player::animEyes()
{
	int mouseX = 0;
	int mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);

	setAnimOffsetX(0, clamp(mouseX - getX() - getWidth()/2, -2.5, 2.5));
	setAnimOffsetY(0, clamp(mouseY - getY() - getHeight()/2 + 15, -2.5, 2.5));
}

void Player::updateHealth(int currentHealth){
	health = currentHealth;
}

int Player::getHealth(){
	std::cout << "HEALTH: " << health << std::endl;
	return health;
}

void Player::update(Ground& ground)
{
	timer++;
	score = timer/50; 
	if (score > highscore)
	{
		highscore = score;
	}

	setX(getX() - 1); //autoscroll
	int mouseX = 0;
	int mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);

	animEyes();
	setAnimOffsetY(3, 0);
	setAnimOffsetY(4, 0);

	if(!grounded){
		if(velocityY < 0){
			highestPointY = std::min(getY(), highestPointY);
			std::cout << "HIGHEST POINT: " << highestPointY << std::endl;
			std::cout << "CURRENT Y: " << getY() << std::endl;
			isFalling = true;
		}
	}

	if(grounded && isFalling){
		fallHeight = abs(highestPointY - getY());
		std::cout << "CURRENT Y: " << getY() << std::endl;
		std::cout << "FALL HEIGHT: " << fallHeight << std::endl;
		if (fallHeight > SAFE_FALL_HEIGHT) {  
			int damage = (fallHeight - SAFE_FALL_HEIGHT) * DAMAGE_MULTIPLIER;
			std::cout<< "TAKING DAMAGE" << std::endl;
			updateHealth(getHealth()-damage);
   		}

    	isFalling = false;  
		highestPointY = getY();
	}

	if(getHealth() < 0){
		dead = NO_HEALTH_DEATH;
	}

	if (distanceFromCursor() < 100)
	{
		setAnimOffsetY(3, sin(SDL_GetTicks()/50) * velocityX - 2);
		setAnimOffsetY(4, -sin(SDL_GetTicks()/50) * velocityX - 2 );
		if (mouseX < getX())
		{
			velocityX = 1/distanceFromCursor() * 100;
		}
		else if (mouseX > getX() + getWidth())
		{
			velocityX = 1/distanceFromCursor() * -100;
		}
		else 
		{
			velocityX = 0;
			if (mouseY > getY() && mouseY < getY() + getHeight())
			{
				//kill player
				dead = CURSOR_DEATH;
			}
		}
	}
	else
	{
		velocityX = 0;
	}
	setX(getX() + velocityX);

	setY(getY() + velocityY);
	if (ground.isTileBelow(getX(), getWidth()))
	{
		if (getY() + getHeight() < SCREEN_HEIGHT - 64)
		{
			velocityY += GRAVITY;
			grounded = false;
		}
		else 
		{
			if (getY() < SCREEN_HEIGHT - getHeight() - 64 + 20)
			{
				setY(SCREEN_HEIGHT - getHeight() - 64);
				grounded = true;
			}
		}
	}
	else
	{
			velocityY += GRAVITY;
			grounded = false;
			if (getY() > SCREEN_HEIGHT)
			{
				dead = HOLE_DEATH;
			}
	}	
}

const char* Player::getScore()
{
	std::string s = std::to_string(score);
	s = "DISTANCE: " + s;
	return s.c_str();
}

const char* Player::getHighscore()
{
	std::string s = std::to_string(highscore);
	s = "BEST: " + s;
	return s.c_str();
}

int Player::getScoreInt()
{
	return score;
}

int Player::isDead()
{
	return dead;
}

void Player::reset()
{
	setX(SCREEN_WIDTH/2 - getWidth()/2);
	setY(SCREEN_HEIGHT - getHeight() - 64);
	score = 0;
	timer = 0;
	velocityX = 0;
	velocityY = 0;
	dead = 0;
	health = 100;
}
