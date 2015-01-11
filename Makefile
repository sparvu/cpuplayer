
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
