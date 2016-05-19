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

#include "vec.h"

#ifdef WITH_SSE2
Vec<3, float> CrossProduct(const Vec<3, float>& lhs, const Vec<3, float>& rhs)
{
	Vec<3, float> ret;

	__m128* __restrict l = (__m128*)lhs.elements;
	__m128* __restrict r = (__m128*)rhs.elements;
	__m128* __restrict f = (__m128*)ret.elements;

	*f = _mm_sub_ps(
		_mm_mul_ps(_mm_shuffle_ps(*l, *l, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(*r, *r, _MM_SHUFFLE(3, 1, 0, 2))),
		_mm_mul_ps(_mm_shuffle_ps(*l, *l, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(*r, *r, _MM_SHUFFLE(3, 0, 2, 1)))
	);

	return ret;

}
#endif