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

#pragma once

#include "vec.h"
struct Surface
{
	unsigned char* pixels;
	int width;
	int height;
	int pitch;

	void inline SetPixel(int x, int y, int color)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return;
		auto offset = pixels + ((y * pitch) + (x * 4));
		*((int*)offset) = color;
	}

	void Line(int x0, int y0, int x1, int y1, int color);
	void Triangle(const std::array<Vec2i, 3>& vertices, int color);

	static constexpr int RGB(int r, int g, int b)
	{
		return (r << 16) | (g << 8) | (b) | 0xff000000;
	}
};