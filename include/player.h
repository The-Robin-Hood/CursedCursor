#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>

#include "entity.h"
#include "ground.h"

#define SAFE_FALL_HEIGHT 200
#define DAMAGE_MULTIPLIER 0.1

class Player : public Entity {
public:	
	Player(float p_x, float p_y, std::vector<SDL_Texture*> p_tex);
	float distanceFromCursor();
	bool jump();
	void update(Ground& ground);
	const char* getScore();
	const char* getHighscore();
	int getHealth();
	int getScoreInt();
	int isDead();
	void reset();
	int maxHealth = 100;
private:
	float velocityX, velocityY;
	bool grounded;
	void animEyes();
	void updateHealth(int currentHealth);
	float clamp(float p_value, float p_min, float p_max);
	int score = 0;
	int highscore = 0;
	int timer = 0;
	int dead = 0;
	int health = 100;
	bool doubleJump = true;
	float highestPointY = 320;  
	float fallHeight = 0; 
	bool isFalling = false;  
};