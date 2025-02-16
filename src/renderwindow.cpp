#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "renderwindow.h"
#include "entity.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;

RenderWindow::RenderWindow()
{

}

void RenderWindow::create(const char* p_title, int p_w, int p_h)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

	if (window == NULL)
		std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}


SDL_Texture* RenderWindow::loadTexture(const char* p_filePath)
{
	SDL_Texture* texture = NULL; 
	texture = IMG_LoadTexture(renderer, p_filePath);

	if (texture == NULL)
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;

	return texture;
}

void RenderWindow::clear()
{
	SDL_SetRenderDrawColor(renderer, 202, 199, 195, 255);
	SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& p_entity)
{

	for (int i = p_entity.getSize() - 1; i >= 0; i--)
	{
		if(i==6) continue;
		SDL_Rect src;
		src.x = p_entity.getCurrentFrame().x;
		src.y = p_entity.getCurrentFrame().y;
		src.w = p_entity.getCurrentFrame().w;
		src.h = p_entity.getCurrentFrame().h;

		SDL_Rect dst;
		dst.x = p_entity.getX() + p_entity.getAnimOffsetX(i);
		dst.y = p_entity.getY() + p_entity.getAnimOffsetY(i);
		dst.w = p_entity.getCurrentFrame().w;
		dst.h = p_entity.getCurrentFrame().h;

		SDL_RenderCopy(renderer, p_entity.getTex(i), &src, &dst);
	}
}

void RenderWindow::render(float p_x, float p_y, SDL_Texture* p_tex)
{
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	SDL_QueryTexture(p_tex, NULL, NULL, &src.w, &src.h); 

	SDL_Rect dst;
	dst.x = p_x;
	dst.y = p_y;
	dst.w = src.w;
	dst.h = src.h;

	SDL_RenderCopy(renderer, p_tex, &src, &dst);
}

void RenderWindow::render(SDL_Texture* p_tex)
{
	render(0, 0, p_tex);
}


void RenderWindow::render(float p_x, float p_y, SDL_Texture* p_tex, float healthPercent)
{
    SDL_Rect src;
    src.x = 0;
    src.y = 0;

    // Get original texture width & height
    SDL_QueryTexture(p_tex, NULL, NULL, &src.w, &src.h); 

    // Crop width based on health percentage
    src.w = static_cast<int>(src.w * healthPercent); 

    SDL_Rect dst;
    dst.x = p_x;
    dst.y = p_y;
    dst.w = src.w;  // Scale the destination width too
    dst.h = src.h;

    SDL_RenderCopy(renderer, p_tex, &src, &dst);
}

void RenderWindow::renderWithSrc(SDL_Rect src, SDL_Texture* p_tex, SDL_Rect dst)
{
	SDL_RenderCopy(renderer, p_tex, &src, nullptr);
}

void RenderWindow::render(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor)
{
		SDL_Surface* surfaceMessage = TTF_RenderText_Blended( font, p_text, textColor);
		SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

		SDL_Rect src;
		src.x = 0;
		src.y = 0;
		src.w = surfaceMessage->w;
		src.h = surfaceMessage->h; 

		SDL_Rect dst;
		dst.x = p_x;
		dst.y = p_y;
		dst.w = src.w;
		dst.h = src.h;

		SDL_RenderCopy(renderer, message, &src, &dst);
		SDL_FreeSurface(surfaceMessage);
}

void RenderWindow::renderCenter(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor)
{
		SDL_Surface* surfaceMessage = TTF_RenderText_Blended( font, p_text, textColor);
		SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

		SDL_Rect src;
		src.x = 0;
		src.y = 0;
		src.w = surfaceMessage->w;
		src.h = surfaceMessage->h; 

		SDL_Rect dst;
		dst.x = SCREEN_WIDTH/2 - src.w/2 + p_x;
		dst.y = SCREEN_HEIGHT/2 - src.h/2 + p_y;
		dst.w = src.w;
		dst.h = src.h;

		SDL_RenderCopy(renderer, message, &src, &dst);
		SDL_FreeSurface(surfaceMessage);
}

void RenderWindow::display()
{
	SDL_RenderPresent(renderer);
}

void RenderWindow::cleanUp()
{
	SDL_DestroyWindow(window);
}