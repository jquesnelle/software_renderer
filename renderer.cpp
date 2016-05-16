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
#include <cstdlib>
#include <utility>

struct Surface
{
	unsigned char* pixels;
	int width;
	int height; 
	int pitch;

	void inline SetPixel(int x, int y, int color)
	{
		auto offset = pixels + ((y * pitch) + (x * 4));
		*((int*)offset) = color;
	}

	void Line(int x0, int y0, int x1, int y1, int color)
	{
		bool steep = false;

		if (std::abs(x0 - x1) < std::abs(y0 - y1))
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
			steep = true;
		}

		if (x0 > x1)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
		}

		int dx = x1 - x0;
		int dy = y1 - y0;
		int derror2 = std::abs(dy) * 2;
		int error2 = 0;
		int y = y0;

		for (int x = x0; x < x1; ++x)
		{
			if (steep)
				SetPixel(y, x, color);
			else
				SetPixel(x, y, color);

			if ((error2 += derror2) > dx)
			{
				y += y1 > y0 ? 1 : -1;
				error2 -= dx * 2;
			}
		}

	}
};

void render(unsigned char* pixels, int width, int height, int pitch)
{
	auto surface = Surface{ pixels, width, height, pitch };

	surface.Line(13, 20, width/4, height/4, 0xFFFFFFFF);
		
}