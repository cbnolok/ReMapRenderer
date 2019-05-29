//
// File: Geometry.h
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

#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"

extern void ExtractFrustum();
extern bool PointInFrustum( float x, float y, float z );
extern void NormalizeVector(float * vec);
extern void VectorAdd(float * Vector, float * result);
extern void CalcNormal(float * Vertex1, float * Vertex2, float * Vertex3, float * result);
extern bool QuadInFrustum( float * quad, float tx, float ty );
extern bool SphereInFrustum( float * sphere , float tx, float ty );
extern float CheckSphereNearPlane( float * sphere , float tx, float ty );
extern void InvertMatrix(float matrix[], float out[]);
extern void Vec3TransformCoord(float v[], float m[], float result[]);
extern float GetSquaredDistRayPoint(float vecOrigin[3], float vecDir[3], float vecPoint[3], float & lamda);
extern unsigned int polycounter;
extern float water_sin_table[32][9];
extern void CreateSinTable(void);
extern int flag_cullbackfaces;
bool IntersectTriangleWithRay(float vecOrigin[], float vecDir[], float vecP1[], float vecP2[], float vecP3[], float & lambda);
bool IntersectTriangleWithRay2(float vecOrigin[], float vecDir[], float vecP1[], float vecP2[], float vecP3[], float & lambda);

#endif //_GEOMETRY_H_
