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

#define WITH_SSE2

#ifdef WITH_SSE2

	#include <xmmintrin.h>

	#if defined(_MSC_VER)
		#define ALIGNED _declspec(align(16))
	#elif defined(_GNUC__)
		#define ALIGNED __attribute__((aligned(16)))
	#endif

#else

	#define ALIGNED

#endif

template<size_t D, typename T> struct Vec
{
private:

	static constexpr size_t ELEMENTS()
	{
		return D <= 4 ? 4 : D;
	}

	ALIGNED T elements[ELEMENTS()];

public:

	Vec() {}
	Vec(const std::initializer_list<T>& il)
	{
		auto it = il.begin();
		for (size_t i = 0; i < D;  ++i)
			elements[i] = *it++;
		for (size_t i = D; i < ELEMENTS() ; ++i)
			elements[i] = 0;
	}

	inline T& operator[](size_t index)
	{
#ifndef NDEBUG
		return (index >= 0 && index < D) ? elements[index] : throw std::out_of_range("Invalid Vec element access"), elements[0];
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

	template<typename T> friend Vec<3, T> CrossProduct(const Vec<3, T>& lhs, const Vec<3, T>& rhs);

};

using Vec3f = Vec<3, float>;
using Vec2i = Vec<2, int>;

template<typename T> Vec<3, T> CrossProduct(const Vec<3, T>& lhs, const Vec<3, T>& rhs)
{
	return Vec<3, T>
	{
		lhs[1] * rhs[2] - lhs[2] * rhs[1],
		lhs[2] * rhs[0] - lhs[0] * rhs[2],
		lhs[0] * rhs[1] - lhs[1] * rhs[0]
	};
}

#ifdef WITH_SSE2
template<typename T> Vec<3, float> CrossProduct(const Vec<3, float>& lhs, const Vec<3, float>& rhs)
{
	Vec<3, float> ret;

	__m128* __restrict l = (__m128*)lhs.elements;
	__m128* __restrict r = (__m128*)rhs.elements;
	__m128* __restrict f = (__m128*)ret.elements;

	*f = _mm_sub_ps(
		_mm_mul_ps(_mm_shuffle_ps(*l, *l, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(*r, *r, _MM_SHUFFLE(3, 1, 0, 2))),
		_mm_mul_ps(_mm_shuffle_ps(*l, a, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(*r, *r, _MM_SHUFFLE(3, 0, 2, 1)))
	);

	return ret;

}
#endif

template<typename T> Vec<3, T> Barycentric(const std::array<Vec2i, 3>& vertices, const Vec2i& point)
{
	auto u = CrossProduct(
		Vec<3, T>
		{
			(T)(vertices[2][0] - vertices[0][0]),
			(T)(vertices[1][0] - vertices[0][0]),
			(T)(vertices[0][0] - point[0])
		},
		Vec<3, T>
		{
			(T)(vertices[2][1] - vertices[0][1]),
			(T)(vertices[1][1] - vertices[0][1]),
			(T)(vertices[0][1] - point[1])
		}
	);
	return std::abs(u[2]) < (T)1 ? Vec<3, T>{(T)-1, (T)1, (T)1} :
		Vec<3, T>{ ((T)1) - (u[0] + u[1]) / u[2], u[1] / u[2], u[0] / u[2] };
}