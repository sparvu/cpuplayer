/*
 * Copyright (c) 2014 PerfDynamics. All rights reserved.
 * Copyright (c) 2014 Stefan Parvu (www.systemdatarecorder.org)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 * (http://www.gnu.org/copyleft/gpl.html)
 *
 */

#include <math.h>

#include "barry3.h"

B3cart
GetXYFromBarry3(B3bary b3coords)
{
	B3cart cartesianB3;
	double x, y;
	double sum = b3coords.a + b3coords.b + b3coords.c;

	x = (b3Vertex[0][0] * b3coords.b) + 
            (b3Vertex[1][0] * b3coords.a) + 
            (b3Vertex[2][0] * b3coords.c);
	
        y = (b3Vertex[0][1] * b3coords.b) + 
            (b3Vertex[1][1] * b3coords.a) + 
            (b3Vertex[2][1] * b3coords.c);
	
        cartesianB3.x = x/sum;
	cartesianB3.y = y/sum;

	return(cartesianB3);
}

void SetBarry3Vertices()
{
	b3Vertex[1][0] = 0;
	b3Vertex[1][1] = 2.6;
	b3Vertex[0][0] = 2.6;
	b3Vertex[0][1] = -1.9;
	b3Vertex[2][0] = -2.6;
	b3Vertex[2][1] = -1.9;
}

