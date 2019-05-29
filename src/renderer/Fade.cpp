//
// File: Fade.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "renderer/Fade.h"
#include "include.h"

using namespace std;

cFader::cFader (float from, float to, Uint32 time, Uint32 id)
{
	fade_start = 0;
	fade_from = from;
	fade_to = to;
	fade_time = time;
	m_id = id;
}

void cFader::Start ()
{
	fade_start = SDL_GetTicks();
	m_value = fade_from;
	m_finished = false;
}

void cFader::Handle ()
{
	if (fade_start) {
  		Uint32 currenttime = SDL_GetTicks();
		if (fade_start + fade_time < currenttime) {
			m_value = fade_to;
			fade_start = 0;
			m_finished = true;
		} else 
			m_value = (float) (fade_to - fade_from) / (float) fade_time * (float) (currenttime - fade_start) + fade_from;
	}
}
