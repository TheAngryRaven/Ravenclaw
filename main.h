/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*	Class: main
 *	the main entry point for the application
 */

#pragma once
#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <windows.h>

using namespace std;

//project includes
#include "misc.h"
#include "palringoClient.h"

//project variables
misc engine;
palringoClient *palClient = new palringoClient;

//main loops/functions
int main();				//the main

void spinUp(void);		//application startup
void spinning(void);	//main application loop
void spinDown(void);	//ending application function

#endif // MAIN_H_INCLUDED
