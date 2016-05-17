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

#include "wavefront.h"

Wavefront::Wavefront(const char* path)
{

}

Wavefront::~Wavefront()
{

}

std::vector<std::reference_wrapper<const Vec3f>> Wavefront::Face(size_t index) const
{
	std::vector<std::reference_wrapper<const Vec3f>> ret;
	std::for_each(vertices.begin(), vertices.end(), [&](const Vec3f& vertex) {
		ret.push_back(vertex);
	});
	return ret;
}