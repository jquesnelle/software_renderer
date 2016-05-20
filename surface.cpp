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


#include "surface.h"
#include <utility>

Surface::Surface() : pixels(nullptr), width(0), height(0), pitch(0)
{

}

void Surface::Prepare(unsigned char* pixels, int width, int height, int pitch)
{
	if (width != this->width || height != this->height || pitch != this->pitch)
	{
		z_buffer.resize(width * height);
		this->width = width;
		this->height = height;
		this->pitch = pitch;
	}

	std::fill(z_buffer.begin(), z_buffer.end(), -std::numeric_limits<float>::max());

	this->pixels = pixels;
}

void Surface::Line(int x0, int y0, int x1, int y1, int color)
{
	bool steep = false;

	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
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

void Surface::Triangle(const std::array<Vec2i, 3>& vertices, int color)
{
	Vec2i box_min{ width - 1, height - 1 };
	Vec2i clamp{ width - 1, height - 1 };
	Vec2i box_max{ 0, 0 };

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			box_min[j] = std::max(0,		std::min(box_min[j], vertices[i][j]));
			box_max[j] = std::min(clamp[j], std::max(box_max[j], vertices[i][j]));
		}
	}

	Vec2i point;
	for (point[0] = box_min[0] ; point[0] <= box_max[0] ; ++point[0])
	{
		for (point[1] = box_min[1]; point[1] <= box_max[1]; ++point[1])
		{
			auto bc = Barycentric<float>(vertices, point);
			if (bc[0] >= 0 && bc[1] >= 0 && bc[2] >= 0)
				SetPixel(point[0], point[1], color);
		}
	}
}

void Surface::Face(const std::array<Vec3f, 3>& vertices, int color)
{
	Vec2f box_min{ std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
	Vec2f box_max{ -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() };
	Vec2f clamp{ (float)(width - 1), (float)(height - 1) };

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			box_min[j] = std::max(0.0f, std::min(box_min[j], vertices[i][j]));
			box_max[j] = std::min(clamp[j], std::max(box_max[j], vertices[i][j]));
		}
	}

	Vec3f point;
	for (point[0] = box_min[0]; point[0] <= box_max[0]; ++point[0])
	{
		for (point[1] = box_min[1]; point[1] <= box_max[1]; ++point[1])
		{
			auto bc = Barycentric<float>(vertices, point);
			if (bc[0] >= 0 && bc[1] >= 0 && bc[2] >= 0)
			{
				point[2] = vertices[0][2] * bc[0] + vertices[1][2] * bc[1] + vertices[2][2] * bc[2]; //find the z value of this point
				int index = (int)(point[0] + point[1] * width);
				if (z_buffer[index] < point[2]) //if we're closer to the screen than the point already drawn, draw it
				{
					z_buffer[index] = point[2];
					SetPixel((int)point[0], (int)point[1], color);
				}
			}
		}
	}
}