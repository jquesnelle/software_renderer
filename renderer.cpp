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

	static Wavefront model("data/sample_models/african_head/african_head.obj");

	const float half_width = width / 2.0f;
	const float half_height = height / 2.0f;

	auto num_faces = model.NumberOfFaces();
	for (size_t i = 0; i < num_faces; ++i)
	{
		const auto world = model.Face(i);

		Vec2i screen;
		for (size_t j = 0; j < 3; ++j)
		{
			
		}
	}
		
}