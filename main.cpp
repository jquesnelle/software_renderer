/*
*	(c) 2016 Jeffrey Quesnelle
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "fps.h"
#include "renderer.h"

#undef main
int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
		return 1;
	}

	if (TTF_Init() != 0)
	{
		std::cerr << "TTF_Init error: " << TTF_GetError() << std::endl;
		return 1;
	}

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture = nullptr;

	int window_width = 1280;
	int window_height = 720;

	if (SDL_CreateWindowAndRenderer(window_width, window_height, SDL_WINDOW_RESIZABLE, &window, &renderer) != 0)
	{
		std::cerr << "SDL_CreateWindowAndRenderer error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	TTF_Font* font = TTF_OpenFont("Roboto-Regular.ttf", 24);

	unsigned char* pixels;
	int pitch;
	SDL_Event e;
	bool keep_running = true;
	FPS fps;
	
	while (keep_running)
	{	
		if (texture == nullptr)
			texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

		if (SDL_LockTexture(texture, NULL, (void**)(&pixels), &pitch) == 0)
		{
			fps.FrameStart();

			render(pixels, window_width, window_height, pitch);

			SDL_UnlockTexture(texture);
			SDL_RenderCopy(renderer, texture, NULL, NULL);

			float the_fps = fps.GetFPS();
			char fps_txt[32];
			snprintf(fps_txt, 32, "%.1f", the_fps);
			auto fps_surface = TTF_RenderText_Solid(font, fps_txt, SDL_Color{ 0, 0, 255, 255 });
			auto fps_texture = SDL_CreateTextureFromSurface(renderer, fps_surface);
			SDL_Rect fps_dest = { window_width - 60, 10, 40, 24 };
			SDL_RenderCopy(renderer, fps_texture, NULL, &fps_dest);
			SDL_FreeSurface(fps_surface);
			SDL_DestroyTexture(fps_texture);
			
			SDL_RenderPresent(renderer);
		}


		if (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				keep_running = false;
				break;
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					if (texture)
					{
						SDL_DestroyTexture(texture);
						texture = nullptr;
					}
					window_width = e.window.data1;
					window_height = e.window.data2;
				}
				break;
			}
		}

		//SDL_Delay(500);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}