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
#include "surface.h"
#include "wavefront.h"


void render(unsigned char* pixels, int width, int height, int pitch)
{
	auto surface = Surface{ pixels, width, height, pitch };
#if 0
	static Wavefront model("data/sample_models/african_head/african_head.obj");

	const float half_width = width / 2.0f;
	const float half_height = height / 2.0f;

	auto num_faces = model.NumberOfFaces();
	for (size_t i = 0; i < num_faces; ++i)
	{
		const auto face = model.Face(i);

		for (size_t v = 0; v < 3; ++v)
		{
			const auto& v0 = *face[v];
			const auto& v1 = *face[(v + 1) % 3];

			int x0 = (int)((v0[0] + 1.0f) * half_width);
			int y0 = height - (int)((v0[1] + 1.0f) * half_height);
			int x1 = (int)((v1[0] + 1.0f) * half_width);
			int y1 = height - (int)((v1[1] + 1.0f) * half_height);

			surface.Line(x0, y0, x1, y1, 0xFFFFFFFF);
		}
	}
#endif

	std::array<Vec2i, 3> points{ Vec2i{10,10}, Vec2i{100, 30}, Vec2i{190, 160} };

	surface.Triangle(points, 0xFFFFFFFF);
		
}