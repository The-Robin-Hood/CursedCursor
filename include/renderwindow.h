#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "entity.h"

class RenderWindow 
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
public:
	RenderWindow();
	void create(const char* p_title, int p_w, int p_h);
	SDL_Texture* loadTexture(const char* p_filePath);
	void clear();
	void render(Entity& p_entity);
	void render(float p_x, float p_y, SDL_Texture* p_tex);
	void render(SDL_Texture* p_tex);
	void render(float p_x, float p_y, SDL_Texture* p_tex, float healthPercent);
	void render(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor);
	void renderCenter(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor);
	void renderWithSrc(SDL_Rect src, SDL_Texture* p_tex, SDL_Rect dst);
	void display();
	void cleanUp();
};