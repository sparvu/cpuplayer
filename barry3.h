/*
 * Copyright (c) 2014 PerfDynamics. All rights reserved.
 * Copyright (c) 2014 Stefan Parvu. All rights reserved.
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

#pragma ident   "%Z%%M% %I%     %E% sp"

#define SAMPLE_MAX 32676
double b3Vertex[3][2];

typedef struct {
    double a;
    double b;
    double c;
} B3bary;

typedef struct {
    double x;
    double y;
} B3cart;

//extern B3cart ca[SAMPLE_MAX];
extern void SetBarry3Vertices();
extern B3cart GetXYFromBarry3(B3bary b3coords);
