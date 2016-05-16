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

#include "renderer.h"
#include <string.h>

struct Surface
{
	unsigned char* pixels;
	int width;
	int height; 
	int pitch;

	void SetPixel(int x, int y, int color)
	{
		auto offset = pixels + ((y * pitch) + (x * 4));
		*((int*)pixels) = color;
	}
};

void line(int x0, int y0, int x1, int y1, int color, Surface& surface)
{
	for (float t = 0.f; t<1.f; t += .01f) {
		int x = (int)(x0*(1.f - t) + x1*t);
		int y = (int)(y0*(1.f - t) + y1*t);
		
		surface.SetPixel(x, y, color);

	}
}

void render(unsigned char* pixels, int width, int height, int pitch)
{
	auto surface = Surface{ pixels, width, height, pitch };

	line(13, 20, 80, 40, 0xFFFFFFFF, surface);
		
}