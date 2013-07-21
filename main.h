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

#include <iostream>
#include <fstream>

//project includes
#include "misc.h"
#include "palringoClient.h"

//project variables
misc engine;
palringoClient *palClient;

int main(int argc, char* argv[]);				//the main

void spinUp(string botName);                    //sets console title and such
void spinning(map<string, string> botSettings); //main application loop
void spinDown(void);	                        //ending application function

#endif // MAIN_H_INCLUDED
