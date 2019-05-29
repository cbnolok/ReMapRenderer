//
// File: SDLEvent.h
// Created by: Gustav Nylander - blobba@ultima-iris.de
//
/*****
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *****/

#ifndef _SDLEVENT_H_
#define _SDLEVENT_H_

#include "SDL/SDL.h"

class SDLEvent
{
private:
  void HandleKeyPress (SDL_keysym * keysym);
  void HandleMouseMotion (SDL_MouseMotionEvent * event);
  void HandleMouseDown (SDL_MouseButtonEvent * event);
  void HandleEvent (SDL_Event event,unsigned int currenttime);
  void HandleMovement (void);
  SDL_Event event;
  int lasttick;
  int lastx, lasty, lastbutton;
  unsigned int last_click;
  int clickdown_x, clickdown_y;
  bool dragging;

public:
    SDLEvent ();
   ~SDLEvent ();

  void PollEvent ();
  void WaitEvent ();
  int quit;

protected:

};

#endif //_SDLEVENT_H_
