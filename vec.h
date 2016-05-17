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

#include <stdexcept>
#include <array>

template<size_t D, typename T> struct Vec
{
private:
	std::array<T, D> elements;

public:

	constexpr Vec() : x(0), y(0) {}
	constexpr Vec(const Vec& rhs) : elements(rhs.elements) {}
	constexpr Vec(const std::array<T, D>& rhs) : elements(rhs) {}
	constexpr Vec(std::array<T, D>&& rhs) : elements(rhs) {}

	T& operator[](size_t index)
	{
#ifndef NDEBUG
		return (index >= 0 && index < D) ? elements[index] : throw std::out_of_range("Invalid Vec element access");
#else
		return elements[index];
#endif
	}

	constexpr const T& operator[](size_t index) const
	{
#ifndef NDEBUG
		return (index >= 0 && index < D) ? elements[index] : throw std::out_of_range("Invalid Vec element access");
#else
		return elements[index];
#endif
	}
};

using Vec3f = Vec<3, float>;