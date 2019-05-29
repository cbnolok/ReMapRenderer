//
// File: SDLEvent.h
// Created by: Gustav Nylander - blobba@ultima-iris.de
//

#include <iostream>
#include "SDLEvent.h"
#include "renderer/SDLScreen.h"
#include "renderer/Renderer.h"
#include "renderer/Camera.h"
#include "Debug.h"
#include <math.h>
#include "Config.h"
#include "Game.h"

using namespace std;

extern SDLScreen *SDLscreen;

SDLEvent::SDLEvent()
{
  lasttick = 0;
  last_click = 0;
  lastx = 0;
  lasty = 0;
  lastbutton = 0;
  quit = 0;
  clickdown_x = 0;
  clickdown_y = 0;
  dragging = false;
}

SDLEvent::~SDLEvent()
{
  // TODO: put destructor code here

}

void
  SDLEvent::PollEvent()
{

  /* handle the events in the queue */
  //SDL_WarpMouse( 320, 240 );

  unsigned int currenttime = SDL_GetTicks();

  if(last_click)
    if((currenttime - last_click) >= CLICK_TIME) {
		pGame.HandleClick(lastx, lasty, lastbutton, false);
//      printf("Single Click!\n");
      last_click = 0;
    }

  while (SDL_PollEvent(&event)) {
  	HandleEvent(event, currenttime);
  }
    HandleMovement();

}

void
  SDLEvent::WaitEvent()
{

  unsigned int currenttime = SDL_GetTicks();

  while (!quit && SDL_WaitEvent(&event)) {
  	HandleEvent(event, currenttime);
  }

}
void SDLEvent::HandleEvent(SDL_Event event, unsigned int currenttime)
{
    switch (event.type) {
    case SDL_ACTIVEEVENT:
      /* Something's happend with our focus
       * If we lost focus or we are iconified, we
       * shouldn't draw the screen
       */
      //if ( event.active.gain == 0 )
      //isActive = false;
      //else
      //isActive = TRUE;
      break;
    case SDL_VIDEORESIZE:
      /* handle resize event */


      break;
    case SDL_KEYDOWN:
      /* handle key presses */
      break;
    case SDL_MOUSEMOTION:
      /* handle mouse movements */
      break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    case SDL_QUIT:
      /* handle quit requests */
      quit = true;
      break;
    default:
      break;
    }
  }

/* function to handle key press events */
void SDLEvent::HandleKeyPress(SDL_keysym * keysym)
{
}

void SDLEvent::HandleMovement(void)
{

}

void SDLEvent::HandleMouseMotion(SDL_MouseMotionEvent * event)
{
}

