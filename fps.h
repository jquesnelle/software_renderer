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

#include "SDL_timer.h"

class FPS
{
public:
	static const unsigned int NUM_MS_PER_FRAME = 1 << 3;

	void FrameStart()
	{
		unsigned int now = SDL_GetTicks();
		unsigned int time = now - last_frame_time;
		if (ms_valid == NUM_MS_PER_FRAME)
			ms_sum -= ms_per_frame[index];
		else
			++ms_valid;
		ms_per_frame[index = ((++index) & (NUM_MS_PER_FRAME - 1))] = time;
		ms_sum += time;
		last_frame_time = now;
	}

	float GetFPS() const
	{
		return 1000.0f / ((float)ms_sum / (float)ms_valid);
	}

private:
	unsigned int ms_per_frame[NUM_MS_PER_FRAME];
	unsigned int ms_valid = 0;
	unsigned int ms_sum = 0;
	unsigned int last_frame_time = 0;
	unsigned int index = 0;
	
};