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

#if defined (__SVR4) && defined (__sun)
#include <sys/sysinfo.h>
#include <kstat.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "csv.h"
#include "barry3.h"

#define MAX_CPUID 256        /* Maximum number of CPUIDs */
#define MENU_SPEED_INC 1     /* Menu speed control increase */
#define MENU_SPEED_DEC 2     /* Menu speed control decrease */
#define MENU_SWITCH_DATE 3   /* Menu date switch option */
#define MENU_SWITCH_CPUID 4  /* Menu cpuid switch option */
#define MENU_SWITCH_GRID 5   /* Menu coordinates switch option */
#define MENU_EXIT 10         /* Menu exit application option */

/* Prototypes */
static void print_header(int, int);
static void usage(void);
static void revision(void);
static void cpuid_sphere(int i);
static void switchDateFormat(void);
static double ratio(double v1, double v2, int i, int n);
static void glPuts(GLfloat, GLfloat, char*, void*, 
        GLubyte red, GLubyte green, GLubyte blue);


/* Variables */
static int hz;
static FILE *f;
static int popupMenu;

int levels[] = {25, 50, 75};

B3cart cB3;
B3bary bB3;
B3cart ca[MAX_CPUID][SAMPLE_MAX];
time_t ts[SAMPLE_MAX];
/* B3cart ca[SAMPLE_MAX]; */

/* time format for display */
char timeFormat[30], timeFormatCurr[30];

int showCpuid = 0;
int showGrid = 1;

GLfloat finalX[MAX_CPUID], finalY[MAX_CPUID];
/* GLfloat finalX, finalY; */
time_t finalTS = 0;

/* by default we have 1 CPU installed */
int no_cpuid = 1;

static int frameMs = 500;

void *font2 = GLUT_BITMAP_8_BY_13;
void *font1 = GLUT_BITMAP_9_BY_15;

/* how many samples we got from raw data */
static int numSamples = 0; 

int n;
FILE *fp;

unsigned char strTime[50];
unsigned char strCpuid[5];

double
ratio(double v1, double v2, int i, int n) 
{
    return v1 + (v2 - v1) / n * i;
}

void
display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    int windowWidth = viewport[2];
    if (viewport[3] < windowWidth)
        windowWidth = viewport[3];

    /* plug in here the Barry Traingle */
    glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.8f, 0.0f);
	glVertex2f(b3Vertex[2][0], b3Vertex[2][1]);
	glColor3f(0.8f, 0.0f, 0.0f);
	glVertex2f(b3Vertex[0][0], b3Vertex[0][1]);
	glColor3f(0.0f, 0.0f, 0.8f);
	glVertex2f(b3Vertex[1][0], b3Vertex[1][1]);
    glEnd();


    /* Enable Anti-aliasing and width of Barry Triangle */
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /* glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE); */
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(1.0f);

    glColor3f(0.1, 0.1, 0.1);
    glBegin (GL_LINE_LOOP);
	glVertex2f(b3Vertex[1][0], b3Vertex[1][1]);
	glVertex2f(b3Vertex[0][0], b3Vertex[0][1]);
	glVertex2f(b3Vertex[2][0], b3Vertex[2][1]);
    glEnd();
    

    /* Reset the modelview matrix */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (showGrid) {

        /* draw coordinates of Barry */

        /* Enable Anti-aliasing and width of axis */
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glBegin(GL_TRIANGLES);
            /* usr arrow */
	    glColor3f(1.0f, 0.0f, 0.0f);
	    glVertex2f(b3Vertex[1][0], b3Vertex[1][1]);
	    glVertex2f(b3Vertex[1][0] - 0.07, b3Vertex[1][1] - 0.2);
	    glVertex2f(b3Vertex[1][0] + 0.07, b3Vertex[1][1] - 0.2);

            /* sys arrow */
	    glColor3f(1.0f, 1.0f, 0.0f);
	    glVertex2f(b3Vertex[0][0], b3Vertex[0][1]);
	    glVertex2f(b3Vertex[0][0] - 0.2, b3Vertex[0][1] + 0.2);
	    glVertex2f(b3Vertex[0][0] - 0.27, b3Vertex[0][1] + 0.05);

	    glColor3f(0.0f, 0.0f, 1.0f);
	    glVertex2f(b3Vertex[2][0], b3Vertex[2][1]);
	    glVertex2f(b3Vertex[2][0] + 0.27, b3Vertex[2][1] + 0.05);
	    glVertex2f(b3Vertex[2][0] + 0.2, b3Vertex[2][1] + 0.2);
        glEnd();

        B3cart op_usr, op_idl, op_sys;
        op_usr.x = (b3Vertex[2][0] + b3Vertex[0][0]) / 2;
        op_usr.y = (b3Vertex[2][1] + b3Vertex[0][1]) / 2;
        op_idl.x = (b3Vertex[1][0] + b3Vertex[0][0]) / 2;
        op_idl.y = (b3Vertex[1][1] + b3Vertex[0][1]) / 2;
        op_sys.x = (b3Vertex[2][0] + b3Vertex[1][0]) / 2;
        op_sys.y = (b3Vertex[2][1] + b3Vertex[1][1]) / 2;

        /* Enable Anti-aliasing and width of axis */
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        /* glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE); */
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glLineWidth(0.5);


        glBegin(GL_LINES);
	    
            /* draw axis */
            /* Usr Red */
	    glColor3f (1.0f, 0.0f, 0.0f);
	    glVertex2f(b3Vertex[1][0], b3Vertex[1][1]);
	    glVertex2f(op_usr.x, op_usr.y);

            /* Idle Blue */
	    glColor3f(0.0f, 0.0f, 1.0f);
	    glVertex2f(b3Vertex[2][0], b3Vertex[2][1]);
	    glVertex2f(op_idl.x, op_idl.y);

            /* Sys Yellow */
	    glColor3f(1.0f, 1.0f, 0.0f);
	    glVertex2f(b3Vertex[0][0], b3Vertex[0][1]);
	    glVertex2f(op_sys.x, op_sys.y);

	    /* draw levels (25/50/75) */
            int j, nr_levels = sizeof(levels) / sizeof(levels[0]);
	    for (j = 0; j < nr_levels; j++) {

                /* Blue axis */
                glColor3f(0.0f, 0.0f, 1.0f);
                glVertex2f(ratio(b3Vertex[0][0], b3Vertex[2][0], 
                               j + 1, nr_levels + 1), 
                           ratio(b3Vertex[0][1], b3Vertex[2][1], 
                               j + 1, nr_levels + 1)); 

		glVertex2f(ratio(b3Vertex[1][0], b3Vertex[2][0], 
                               j + 1, nr_levels + 1), 
                           ratio(b3Vertex[1][1], b3Vertex[2][1], 
                               j + 1, nr_levels + 1));

                /* Yellow Axis */
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex2f(ratio(b3Vertex[0][0], b3Vertex[2][0], 
                               j + 1, nr_levels + 1), 
                           ratio(b3Vertex[0][1], b3Vertex[2][1], 
                               j + 1, nr_levels + 1)); 

		glVertex2f(ratio(b3Vertex[0][0], b3Vertex[1][0], 
                               j + 1, nr_levels + 1), 
                           ratio(b3Vertex[0][1], b3Vertex[1][1], 
                               j + 1, nr_levels + 1));

                /* Red Axis */
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(ratio(b3Vertex[1][0], b3Vertex[2][0], 
                               j + 1, nr_levels + 1), 
                           ratio(b3Vertex[1][1], b3Vertex[2][1], 
                               j + 1, nr_levels + 1));

		glVertex2f(ratio(b3Vertex[1][0], b3Vertex[0][0], 
                               j + 1, nr_levels + 1), 
                           ratio(b3Vertex[1][1], b3Vertex[0][1], 
                               j + 1, nr_levels + 1)); 
	    }
        glEnd();

        int k, nlevels = sizeof(levels) / sizeof(levels[0]);
        for (k = 0; k < nlevels; k++) {

            /* Blue axis */
            if ( k == 0 ) 
                glPuts( ratio(b3Vertex[1][0], b3Vertex[2][0],
                              k + 1, nlevels + 1) - 0.4,
                        ratio(b3Vertex[1][1], b3Vertex[2][1],
                              k + 1, nlevels + 1),
                        "25%", GLUT_BITMAP_HELVETICA_10, 0, 0, 255);
            if ( k == 1 )
                glPuts( ratio(b3Vertex[1][0], b3Vertex[2][0],
                              k + 1, nlevels + 1) - 0.4,
                        ratio(b3Vertex[1][1], b3Vertex[2][1],
                              k + 1, nlevels + 1),
                        "50%", GLUT_BITMAP_HELVETICA_10, 0, 0, 255);

            if ( k == 2 )
                glPuts( ratio(b3Vertex[1][0], b3Vertex[2][0],
                              k + 1, nlevels + 1) - 0.4,
                        ratio(b3Vertex[1][1], b3Vertex[2][1],
                              k + 1, nlevels + 1),
                        "75%", GLUT_BITMAP_HELVETICA_10, 0, 0, 255);

            /* Yellow Axis */
            if ( k == 0 ) 
                glPuts( ratio(b3Vertex[0][0], b3Vertex[1][0],
                              k + 1, nlevels + 1) ,
                        ratio(b3Vertex[0][1], b3Vertex[1][1],
                              k + 1, nlevels + 1) + 0.2,
                        "75%", GLUT_BITMAP_HELVETICA_10, 255, 255, 0);
            if ( k == 1 )
                glPuts( ratio(b3Vertex[0][0], b3Vertex[1][0],
                              k + 1, nlevels + 1) ,
                        ratio(b3Vertex[0][1], b3Vertex[1][1],
                              k + 1, nlevels + 1) + 0.2 ,
                        "50%", GLUT_BITMAP_HELVETICA_10, 255, 255, 0);

            if ( k == 2 )
                glPuts( ratio(b3Vertex[0][0], b3Vertex[1][0],
                              k + 1, nlevels + 1) ,
                        ratio(b3Vertex[0][1], b3Vertex[1][1],
                              k + 1, nlevels + 1) + 0.2,
                        "25%", GLUT_BITMAP_HELVETICA_10, 255, 255, 0);
     
            /* Red Axis */
            if ( k == 0 )
                glPuts( ratio(b3Vertex[1][0], b3Vertex[0][0],
                              k + 1, nlevels + 1) + 0.3,
                        ratio(b3Vertex[1][1], b3Vertex[0][1],
                              k + 1, nlevels + 1) ,
                        "75%", GLUT_BITMAP_HELVETICA_10, 255, 0, 0);
            if ( k == 1 )
                glPuts( ratio(b3Vertex[1][0], b3Vertex[0][0],
                              k + 1, nlevels + 1) + 0.3,
                        ratio(b3Vertex[1][1], b3Vertex[0][1],
                              k + 1, nlevels + 1) ,
                        "50%", GLUT_BITMAP_HELVETICA_10, 255, 0, 0);

            if ( k == 2 )
                glPuts( ratio(b3Vertex[1][0], b3Vertex[0][0],
                              k + 1, nlevels + 1) + 0.3,
                        ratio(b3Vertex[1][1], b3Vertex[0][1],
                              k + 1, nlevels + 1) ,
                        "25%", GLUT_BITMAP_HELVETICA_10, 255, 0, 0);
 


        } /* for loop on all Blue axis for Grid marks: 25,50,75 */

    } /* end on grid check */

    int i;
    for (i = 0; i < no_cpuid; i++) {
        
        glPushMatrix();

        glTranslated(finalX[i], finalY[i], 0.0);

        /* draw the sphere */
	glCallList(i + 1);
	
	/* display the number of each cpu, if configured */
	if (showCpuid) {
	    snprintf(strCpuid, sizeof(strCpuid), "%d", i);
	    int pixLen = glutBitmapLength(GLUT_BITMAP_HELVETICA_12, strCpuid);
	    glPuts(-pixLen / 2.0 / windowWidth * 6, 
                   -17 / 2.0 / windowWidth * 6, 
                   strCpuid, GLUT_BITMAP_HELVETICA_12, 255, 255, 0);
	}

        glPopMatrix();
    }




    /* display time */
    strftime(strTime, sizeof(strTime), timeFormatCurr, localtime(&finalTS));
    int pixLen = glutBitmapLength(GLUT_BITMAP_8_BY_13, strTime);
    glPuts(-pixLen / 2.0 / windowWidth * 6, b3Vertex[0][1] / 2, 
           strTime, GLUT_BITMAP_8_BY_13, 255, 255, 255);
    
    /* put "usr" on the top vertex */
    pixLen = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, 
                              (unsigned char *)"usr");
    glPuts(-pixLen / 2.0 / windowWidth * 6, b3Vertex[1][1] + 0.1, 
           "usr", GLUT_BITMAP_HELVETICA_18, 0, 0, 0);
    
    /* put "sys" on the bottom-right vertex */
    glPuts(b3Vertex[0][0], b3Vertex[0][1] - 0.2, 
           "sys", GLUT_BITMAP_HELVETICA_18, 0, 0, 0);

    /* put "idl" on the bottom-left vertex */
    pixLen = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (unsigned char *)"idl");
    glPuts(b3Vertex[2][0] - pixLen / 1.0 / windowWidth * 6, 
           b3Vertex[2][1] - 0.2, 
           "idl", GLUT_BITMAP_HELVETICA_18, 0, 0, 0);
    
    glutSwapBuffers();
}

void
cpuid_sphere(int i)
{
    glColor3f(1.0 - (i / 1.2 / no_cpuid), 0.0, 1.0 - (i / 1.5 / no_cpuid));
    glEnable(GL_LIGHTING);
    glutSolidSphere(0.10, 50, 50);
    glDisable(GL_LIGHTING);
}


void 
myinit(void)
{

    float light_position[]  = {150.0, 150.0, 250.0, 1.0};
    int k=0;

    float light_ambient[]  = {0.05, 0.05, 0.05, 1.00};
    float light_diffuse[]  = {0.65, 0.65, 0.65, 1.00};
    float light_specular[] = {1.00, 1.00, 1.00, 1.00};

    float material_specular[] = {0.6, 0.6, 0.6, 1.0};
    float material_specular_exponent[] = {80.0};
   
    /* Grey Background  
    glClearColor(0.9, 0.9, 0.9, 1.0); */
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
    glShadeModel(GL_SMOOTH);
 
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_specular_exponent);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);

    int i;
    
    for (i = 0; i < no_cpuid; i++) {
        glNewList(i + 1, GL_COMPILE);
            cpuid_sphere(i);
        glEndList();
    }
}


void 
reshape (int w, int h)
{
    GLfloat nRange = 3.0f;

    if (h == 0)
        h=1;

    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    if (w <= h)
        glOrtho (-nRange, nRange, -nRange*(GLfloat)h/(GLfloat)w,
                nRange*(GLfloat)h/(GLfloat)w, -nRange*2, nRange*2);
    else
        glOrtho (-nRange*(GLfloat)w/(GLfloat)h,
                nRange*(GLfloat)w/(GLfloat)h, -nRange, nRange, 
                -nRange*2, nRange*2);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


static void
keyboard(unsigned char k, int x, int y)
{
    switch (k) {
	case 27:  /* Escape */
		exit(0);
		break;
	case 'd':
		switchDateFormat();
		break;
	case 'c':
		showCpuid = 1 - showCpuid;
		break;
	case 'g':
		showGrid = 1 - showGrid;
		break;
	default:
		return;
    }
    glutPostRedisplay();
}

static 
void keyboardSpecial(int key, int x, int y) 
{
    switch(key) {
	case GLUT_KEY_UP:
		frameMs += 50;
		break;
	case GLUT_KEY_DOWN:
		if (frameMs > 50)
			frameMs -= 50;
		break;
    }
}

/*
 * Draw a string
 */
void 
glPuts(GLfloat x, GLfloat y, char *str, void *font, 
       GLubyte red, GLubyte green, GLubyte blue)
{
    int len, i;
    glColor3ub(red, green, blue);
    glRasterPos2f(x, y);
    while (*str)
    	glutBitmapCharacter(font, *str++);
}


void
animateTimer(int val)
{

    static int currSample = 1;

    int i;
    for (i = 0; i < no_cpuid; i++) {
        finalX[i] = ca[i][currSample - 1].x;
        finalY[i] = ca[i][currSample - 1].y;
    }
    finalTS = ts[currSample - 1];

    currSample++;
    if ( currSample > SAMPLE_MAX || currSample > numSamples)
        currSample = 1;

    glutPostRedisplay();
    glutTimerFunc(frameMs, animateTimer, 1);
}


void 
menu(int value) 
{
    switch (value) {
	case MENU_SWITCH_DATE:
	    switchDateFormat();
	    break;
	case MENU_SWITCH_CPUID:
	    showCpuid = 1 - showCpuid;
	    break;
	case MENU_SWITCH_GRID:
	    showGrid = 1 - showGrid;
	    break;
	case MENU_EXIT:
	    exit(0);
	    break;
	default:
	    return;
	}		
}


static void
usage(void)
{
    (void) fprintf(stderr, "Usage: cpuplayer [-hv] filename... \n"
        "Options:\n"
        " -h display help information\n"
        " -v display revision information\n"
        "Keys:\n"
        " Up/Down Arrow - decrease/increase speed\n"
        " d - switch between date formats\n"
	" c - show/hide cpuid tag\n"
        " g - show/hide grid\n"
	" Esc - exit\n");
    exit(1);
}


static void
revision(void)
{
    (void) fprintf(stderr,
            "cpuplayer: beta version: 0.52\n");
    exit(1);
}


void 
processDataLine(time_t* times_prev) 
{
    time_t times;
    int cpuid;
	
    /* fetch User, Sys, Idle */
    int i;
    for (i = 0; i < csvnfield(); i++) {
        /* printf("field_1 Long: %.2f\n", atof(csvfield(1))); */
	times = atoi(csvfield(0));
	cpuid = atoi(csvfield(1));
	bB3.a = atof(csvfield(2));
	bB3.b = atof(csvfield(3));
	bB3.c = atof(csvfield(4));
			
    }
    cB3 = GetXYFromBarry3(bB3);

    printf("%s cpuid: %d (u: %4.2lf, s: %4.2lf, i: %4.2lf):"
           "(x: %4.2lf, y: %4.2lf)\n", \
	   ctime(&times), cpuid, bB3.a, bB3.b, bB3.c, cB3.x, cB3.y);

    if (cpuid >= no_cpuid)
        no_cpuid = cpuid + 1;

 
    /* we save first cpuid, we have 1 cpu
    if (no_cpuid == 1 && fcpuid == 0 )
        fcpuid=cpuid;
    else
        no_cpuid++; */
 
    printf("\nDEBUG: no_cpuid = %d\n", no_cpuid);

    if (times != *times_prev) {
        *times_prev = times;
	numSamples++;
    }

    ca[cpuid][numSamples].x = cB3.x;
    ca[cpuid][numSamples].y = cB3.y;

    ts[numSamples] = times;
}



void 
switchDateFormat(void) 
{
    if (strcmp(timeFormat, timeFormatCurr) != 0)
        strcpy(timeFormatCurr, timeFormat);
    else
        strcpy(timeFormatCurr, "%H:%M:%S");
}

/* 
 * Main Here
 * Declare initial window size, position, and display mode
 * (single buffer and RGBA).  Open window with "barry"
 * in its title bar.  Call initialization routines.
 * Register callback function to display graphics.
 * Enter main loop and process events.
 */
int 
main(int argc, char** argv)
{
    extern int optind;

    /* hz = sysconf(_SC_CLK_TCK); */

    int c, i, kstat_flag=0;
    
    char *line;
    time_t times_prev = 0;

    strncpy(timeFormatCurr, "%H:%M:%S", sizeof(timeFormatCurr));
    timeFormatCurr[sizeof(timeFormatCurr) - 1] = '\0';
    strncpy(timeFormat, "%Y-%m-%d %H:%M:%S", sizeof(timeFormat));
    timeFormat[sizeof(timeFormat) - 1] = '\0';
    
    while ((c = getopt(argc, argv, "khvd:s:")) != (int)EOF)
    switch (c) {
	case 'k':
		kstat_flag = 1;
		break;

        case 's':
                frameMs = atoi(optarg);
                break;

        case 'h':
                usage();
                break;


	case 'v':
		revision();
		break;

	case 'd':
		strncpy(timeFormat, optarg, sizeof(timeFormat));
		timeFormat[sizeof(timeFormat) - 1] = '\0';
		strncpy(timeFormatCurr, timeFormat, sizeof(timeFormatCurr));
		timeFormatCurr[sizeof(timeFormatCurr) - 1] = '\0';
		break;

	case '?':
		usage();
		break;
    }

    glutInit(&argc, argv);

    SetBarry3Vertices();
    
    numSamples = -1;
    if (argc == optind || argc == 1) {
	/* we have only the command line name */
	while ((line = csvgetline(stdin)) != NULL)
		processDataLine(&times_prev);
    } else {
	for (i = optind; i < argc; i++) {
		f = fopen(argv[i], "r");
		if (f == NULL) {
		    fprintf(stderr, 
                            "Cannot open file: %s, skipping\n", argv[i]);
	            continue;
		}
			
		/* we do here the process each file for data */
		while ((line = csvgetline(f)) != NULL)
			processDataLine(&times_prev);
		fclose(f);
	}
    }
    numSamples++;

    glutInitDisplayMode (GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (410, 440);
    glutInitWindowPosition (150, 150);
    glutCreateWindow ("cpuplayer");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc (keyboard);
    glutSpecialFunc(keyboardSpecial);
    glutTimerFunc(50, animateTimer, 1);

    popupMenu = glutCreateMenu(menu); 
//    glutAddMenuEntry("Increase speed (BtnUp)", MENU_SPEED_INC);
//    glutAddMenuEntry("Decrease speed (BtnDown)", MENU_SPEED_DEC);
    glutAddMenuEntry("Cpuid on/off (c)", MENU_SWITCH_CPUID);
    glutAddMenuEntry("Grid on/off (g)", MENU_SWITCH_GRID);
    glutAddMenuEntry("Switch date format (d)", MENU_SWITCH_DATE);
    glutAddMenuEntry("Quit (Esc)", MENU_EXIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    myinit();
    glutMainLoop();

    return 0;
}
