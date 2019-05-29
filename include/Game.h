//
// File: Game.h
// Created by: Alexander Oster - tensor@ultima-iris.de
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

#ifndef _GAME_H_
#define _GAME_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"
#include "renderer/Renderer.h"
#include "renderer/DynamicObjects.h"

class Game
{
private:
 Renderer * renderer;
 bool m_paused;
 int cursor3d [3];
 int cursorx, cursory;
 bool button_left;
 bool button_right;
 
 Uint32 drag_id;
 Uint16 drag_model;
 
 void GrabMousePosition(int x, int y);
 void MoveToMouse();
 cDynamicObject * GrabDynamicObject(int x, int y);

public:
    Game ();
   ~Game ();

   void LoadMuls(std::string mulpath);
   int Init(void);
   int DeInit(void);
   int RenderScene(void);
   int Handle(void);
   void Connect (void);
   void Disconnect (void);

   Renderer * GetRenderer(void);
   
   void SetPosition(int x, int y, int z);
   
   void OnKeyPress(SDL_keysym * key);
   void HandleMouseMotion(SDL_MouseMotionEvent * event);
   void HandleClick(int x, int y, unsigned int buttonstate, bool double_click);
   void HandleMouseDown(int x, int y, int button);
   void HandleMouseUp(int x, int y, int button);
   void HandleDrag(int x, int y);

   bool paused () { return m_paused; }
   void SetPause(bool pause) { m_paused = pause; }
   void Drag (Uint32 id, Uint16 model);
   void DragCancel ();

   void Walk (Uint8 direction);
   void Walk_Simple (Uint8 action);

protected:

};

extern	Game	pGame;

#endif //_GAME_H_
