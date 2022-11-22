/*
 *
 * Copyright (c) 2014 PerfDynamics. All rights reserved.
 * Copyright (c) 2014-2022 Stefan Parvu. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * 
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * https://github.com/sparvu/cpuplayer/blob/master/LICENSE
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

