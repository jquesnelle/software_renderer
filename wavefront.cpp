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
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

Wavefront::Wavefront(const char* path)
{
	std::ifstream in;
	std::string line;

	in.open(path, std::ifstream::in);
	if (in.fail())
		throw std::runtime_error(std::string("Could not open ") + path);

	while (!in.eof())
	{
		std::getline(in, line);
		std::istringstream parse(line);

		char work;
		if (line.compare(0, 2, "v ") == 0)
		{
			float x, y, z;

			parse >> work;
			parse >> x;
			parse >> y;
			parse >> z;

			vertices.emplace_back(Vec3f{x, y, z});
		}
		else if (line.compare(0, 2, "f ") == 0)
		{
			size_t index[3], iwork, indexer = 0, in;
			
			parse >> work;

			while (parse >> in >> work >> iwork >> work >> iwork)
			{
				if (indexer < 3)
					index[indexer++] = in - 1;
			}

			faces.push_back(std::array<std::reference_wrapper<Vec3f>, 3>{Vertex(index[0]), Vertex(index[1]), Vertex(index[2])});
		}
	}
}

Wavefront::~Wavefront()
{

}