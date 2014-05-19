/*
register.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 8, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#ifndef REGISTER_H
#define REGISTER_H

#define ID_NONE 0
#define ID_MARLBOROBUSH	1
#define ID_BOULDER		2
#define ID_MAHOGANYLOG	3
#define ID_SQUIRREL		4
#define ID_IRONGOLEM	5
#define ID_REDSEED		6
#define ID_REDPETAL		7
#define ID_BLUEPETAL	8
#define ID_BLUECHARM	9
#define ID_REDCHARM		10
#define ID_HEAVENSMIRRORSEED	11
#define ID_HEAVENSMIRROR		12
#define ID_BLUESEED		13
#define ID_REDROSE		14
#define ID_BLUEROSE		15
#define ID_HIBISCUSSEED	16
#define ID_HIBISCUS		17
#define ID_STUMP		18
#define ID_HOLE			19
#define ID_SCISSOR		20
#define ID_GREENGRASS	21
#define ID_EMERALDGRASS	22
#define ID_SAND			23
#define ID_DIRT			24
#define ID_MARSHGRASS	25
#define ID_WATER		26
#define ID_REDBRICKS	27
#define ID_MARBLE		28
#define ID_BLUEBRICKS	29
#define ID_GREYBRICKS	30
#define ID_PLANKS		31
#define ID_CLAY			32
#define ID_STONEWALL	33
#define ID_SNOW			34
#define ID_OBSIDIAN		35
#define ID_ICE			36
#define ID_WOLF			37
#define ID_DRYAD		38

#define SHEET_ENTITY	1
#define SHEET_TILES		2
#define SHEET_GUI		3

#define VARIANCE_NONE	0
#define VARIANCE_GREENGRASS	1
#define VARIANCE_EMERALDGRASS	2
#define VARIANCE_SAND	3
#define VARIANCE_DIRT	4
#define VARIANCE_MARSHGRASS	5
#define VARIANCE_WATER	6
#define VARIANCE_REDBRICKS	7
#define VARIANCE_MARBLE	8
#define VARIANCE_BLUEBRICKS	9
#define VARIANCE_GREYBRICKS	10
#define VARIANCE_PLANKS	11
#define VARIANCE_CLAY	12
#define VARIANCE_STONEWALL	13
#define VARIANCE_SNOW	14
#define VARIANCE_OBSIDIAN	15
#define VARIANCE_ICE	16

#define ET_NONE			0
#define ET_CREATURE		1
#define ET_DECO			2
#define ET_VEGETATION	3
#define ET_SEED			4
#define ET_INGREDIENT	5
#define ET_TOOL			6
#define ET_SUMMON		7

#define CPROTO_NONE		0

#define SHAPE_NULL			0
#define SHAPE_PARABOLIC		1
#define SHAPE_CONIC			2
#define SHAPE_CIRCLE		3
#define SHAPE_TRIANGLE		4
#define SHAPE_BOX			5

void registerValues();

#endif//REGISTER_H