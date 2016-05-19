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
#include <vector>

class Wavefront
{
private:
	std::vector<Vec3f> vertices;
	std::vector<std::array<std::reference_wrapper<Vec3f>, 3>> faces;

	inline Vec3f& Vertex(size_t index)
	{
#ifndef NDEBUG
		if (index >= vertices.size())
			throw std::out_of_range("Vertex access on wavefront out of range");
#endif
		return vertices[index];
	}

public:
	Wavefront(const char* path);
	~Wavefront();

	size_t NumberOfVertices() { return vertices.size(); }
	size_t NumberOfFaces() { return faces.size(); }

	inline const Vec3f& Vertex(size_t index) const
	{
#ifndef NDEBUG
		if (index >= vertices.size())
			throw std::out_of_range("Vertex access on wavefront out of range");
#endif
		return vertices[index];
	}

	inline const std::array<std::reference_wrapper<Vec3f>, 3>& Face(size_t index) const
	{
#ifndef NDEBUG
		if (index > faces.size())
			throw std::out_of_range("Face access on wavefront out of range");
#endif
		return faces[index];
	}

};