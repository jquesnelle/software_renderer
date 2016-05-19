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
#include <cassert>

#define WITH_SSE2
#define WITH_SSE4

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
#if defined(WITH_SSE2) || defined(WITH_SSE4)
		return D <= 4 ? 4 : D;
#else
		return D;
#endif
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
		assert(index >= 0 && index < D);
		return elements[index];
	}

	inline const T& operator[](size_t index) const
	{
		assert(index >= 0 && index < D);
		return elements[index];
	}

	T norm() const
	{
		T ret = 0;
		for (size_t i = 0; i < D; i++)
			ret += elements[i] * elements[i];
		return std::sqrt(ret);
	}

	void normalize()
	{
		auto n = norm();
		for (size_t i = 0; i < D; ++i)
			elements[i] /= n;
	}

	template<typename T> friend Vec<3, T> CrossProduct(const Vec<3, T>& lhs, const Vec<3, T>& rhs);

#ifdef WITH_SSE2
	friend Vec<3, float> CrossProduct(const Vec<3, float>& lhs, const Vec<3, float>& rhs);
	friend Vec<3, float> operator-(Vec<3, float> lhs, const Vec<3, float>& rhs);
#endif

#ifdef WITH_SSE4
	friend float operator*(const Vec<3, float>& lhs, const Vec<3, float>& rhs);
#endif

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

template<size_t D, typename T> Vec<D, T> operator-(Vec<D, T> lhs, const Vec<D, T>& rhs)
{
	for (size_t i = 0; i < D; ++i)
		lhs[i] -= rhs[i];
	return lhs;
}

//dot product
template<size_t D, typename T> T operator*(const Vec<D, T>& lhs, const Vec<D, T>& rhs)
{
	T ret = 0;
	for (size_t i = 0; i < D; ++i)
		ret += lhs[i] * rhs[i];
	return ret;
}


#ifdef WITH_SSE4
template<> float Vec<3, float>::norm() const
{
	ALIGNED float ret;
	__m128 v = *(const __m128*)elements;
	_mm_store_ss(&ret, _mm_dp_ps(v, v, 0x71)); //7 -- multiply top 3 elements, 1 -- write result to first element
	//google-fu seems to suggest it's faster to just pull out the sum and do a regular sqrt
	return std::sqrt(ret);
}

template<> void Vec<3, float>::normalize()
{
	__m128* e = (__m128*)elements;
	__m128 v = *e;
	__m128 inv_norm = _mm_rsqrt_ps(_mm_dp_ps(v, v, 0x77)); //7 -- multiply top 3, 7 -- write result to top 3
	*e = _mm_mul_ps(v, inv_norm);
}

#endif