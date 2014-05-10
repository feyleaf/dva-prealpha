/*
globals.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 7, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/
#ifndef GLOBALS_H
#define GLOBALS_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "ic_math.h"
#include "templates.h"
#include "register.h"
#include "tempRegistry.h"
#include "gameobject.h"
#include "game.h"


void saveSizesTextfile(const char*);

#endif//GLOBALS_H