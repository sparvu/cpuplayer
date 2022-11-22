# 
#  Copyright (c) 2014-2022 Stefan Parvu. All rights reserved.
#  
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#  
#  * Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
# 
#  * Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#  
#  * Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#  
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
#  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
#  https://github.com/sparvu/cpuplayer/blob/master/LICENSE
# 
# ######################### #
# CPUPLAYER MAKEFILE ENGINE #
# ######################### #


# Include Platform Defintion
include $(OSTYPE).config

INCDIRS = -I/usr/local/include
#LIBDIRS = -L/usr/local/lib -R/usr/local/lib
LIBDIRS = -L/usr/local/lib
CFLAGS  = -g $(INCDIRS)
OBJECTS = csv.o barry3.o cpuplayer.o


all:	cpuplayer

cpuplayer:	$(OBJECTS)
		$(CC) -o cpuplayer $(OBJECTS) $(LIBDIRS) $(LIBS)


barry3.o:	barry3.c
		$(CC) $(CFLAGS) -c barry3.c

csv.o:		csv.c
		$(CC) $(CFLAGS) -c csv.c

cpuplayer.o:	cpuplayer.c
		$(CC) $(CFLAGS) -c cpuplayer.c

clean:
		rm -f a.out cpuplayer $(OBJECTS)

# End of makefile
