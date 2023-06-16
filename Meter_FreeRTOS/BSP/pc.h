/*
   *********************************************************************************************************
   *                                          PC SUPPORT FUNCTIONS for WIN32
   *
   *                          (c) Copyright 2004-2005, Werner.Zimmermann@hs-esslingen.de
   *                                           All Rights Reserved
   *
   * File : PC.H
   * By   : Werner Zimmermann
   *********************************************************************************************************
 */

#ifndef PC_H
#define PC_H
//#define _CRT_SECURE_NO_WARNINGS

#define PC_CHECK_RECURSIVE_CALLS             FALSE          //Set to TRUE, if you want to check whether the keyboard
                                                            //and display functions are called recursively, i.e.
                                                            //the previous call could not be finished before the
                                                            //functions are called the next time

//#define DEBUG_PC                                          //Uncomment, if you want to debug the display functions

/*
   *********************************************************************************************************
   *                                               CONSTANTS
   *                                    COLOR ATTRIBUTES FOR VGA MONITOR
   *
   * Description: These #defines are used in the PC_Disp???() functions.  The 'color' argument in these
   *              function MUST specify a 'foreground' color and a 'background'.
   *              If you don't specify a background color, BLACK is assumed.  You would
   *              specify a color combination as follows:
   *
   *              PC_DispChar(0, 0, 'A', DISP_FGND_WHITE + DISP_BGND_BLUE);
   *
   *              To have the ASCII character 'A' with a white letter on a blue background.
   *********************************************************************************************************
 */
#include <windows.h>
#include <stdint.h>

#define DISP_BLINK                0x80
#define DISP_FGND_BLACK           0x00
#define DISP_FGND_BLUE            FOREGROUND_BLUE
#define DISP_FGND_GREEN           FOREGROUND_GREEN
#define DISP_FGND_RED             FOREGROUND_RED
#define DISP_FGND_CYAN            (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define DISP_FGND_YELLOW          (FOREGROUND_RED  | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define DISP_FGND_WHITE           (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY)
#define DISP_FGND_GRAY            (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED)

#define DISP_BGND_BLACK           0x00
#define DISP_BGND_BLUE            BACKGROUND_BLUE
#define DISP_BGND_GREEN           BACKGROUND_GREEN
#define DISP_BGND_RED             BACKGROUND_RED
#define DISP_BGND_CYAN            (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY)
#define DISP_BGND_YELLOW          (BACKGROUND_RED  | BACKGROUND_GREEN | BACKGROUND_INTENSITY)
#define DISP_BGND_WHITE           (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY)
#define DISP_BGND_GRAY            (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)
#define DISP_BGND_LIGHT_GRAY      (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)         //???

/*$PAGE */
/*
   *********************************************************************************************************
   *                                           FUNCTION PROTOTYPES
   *********************************************************************************************************
 */

void PC_DispChar(uint8_t x, uint8_t y, uint8_t c, uint8_t color);
void PC_DispClrScr(uint8_t bgnd_color);
void PC_DispStr(uint8_t x, uint8_t y, uint8_t* s, uint8_t color);

void PC_ElapsedInit(void);
void PC_ElapsedStart(uint8_t n);
uint32_t PC_ElapsedStop(uint8_t n);

void PC_GetDateTime(char *s);
BOOLEAN PC_GetKey(int16_t* c);

#endif

