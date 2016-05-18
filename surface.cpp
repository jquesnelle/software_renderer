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