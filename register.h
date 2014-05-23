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

#define BIOME_NONE					0
#define BIOME_ICESHELF				1
#define BIOME_TUNDRA				2
#define BIOME_GLACIALCLIFF			3
#define BIOME_ICEBERG				4
#define BIOME_GLACIERSTREAM			5
#define BIOME_GLACIERRAVINE			6
#define BIOME_TUNDRAISLAND			7
#define BIOME_TUNDRAONTHEBAY		8
#define BIOME_ICECLIFF				9
#define BIOME_FROZENMEADOW			10
#define BIOME_FROZENROLLINGHILLS	11
#define BIOME_FROZENMOUNTAINTOP		12
#define BIOME_FROZENPOND			13
#define BIOME_FROZENROCKYSTREAM		14
#define BIOME_ROCKYRAVINE			15
#define BIOME_FROZENROCKYBEACH		16
#define BIOME_FROZENROCKYRIVER		17
#define BIOME_ROCKYICECLIFF			18
#define BIOME_CONIFEROUSFROZENVALLEY		19
#define BIOME_CONIFEROUSFROZENHILLS			20
#define BIOME_CONIFEROUSFROZENMOUNTAINTOP	21
#define BIOME_CONIFEROUSFROENPOND			22
#define BIOME_TAIGAPONDONROLLINGHILLS		23
#define BIOME_TAIGAPONDONAMOUNTAIN			24
#define BIOME_FROZENMOONBEACH				25
#define BIOME_FROZENMOONISLAND				26
#define BIOME_FROZENMOONISLANDCLIFF			27
#define BIOME_WASTELANDPLAINS		28
#define BIOME_SANDYHILLS			29
#define BIOME_SANDSTONECLIFFS		30
#define BIOME_WASTELANDRESERVOIR	31
#define BIOME_WASTELANDGUTTER		32
#define BIOME_WASTELANDFALLS		33
#define BIOME_SANDYISLAND			34
#define BIOME_SANDYHILLONTHEBAY		35
#define BIOME_SANDSTONEISLAND		36
#define BIOME_MEADOW				37
#define BIOME_ROLLINGHILLS			38
#define BIOME_MOUNTAINTOP			39
#define BIOME_POND					40
#define BIOME_STREAM				41
#define BIOME_RAVINE				42
#define BIOME_SHORTBEACH			43
#define BIOME_RIVER					44
#define BIOME_WATERFALLOASIS		45
#define BIOME_FLATFARMLAND			46
#define BIOME_FARMLANDHILLS			47
#define BIOME_FARMLANDTERRACE		48
#define BIOME_BOXWOODPOND			49
#define BIOME_BOXWOODSTREAM			50
#define BIOME_BOXWOODRAVINE			51
#define BIOME_DRAGONBEACH			52
#define BIOME_DRAGONISLAND			53
#define BIOME_DRAGONISLANDCLIFF		54
#define BIOME_DESERTFLATLAND		55
#define BIOME_DESERTHILLS			56
#define BIOME_DESETMOUNTAIN			57
#define BIOME_DESERTWELL			58
#define BIOME_DESERTOASIS			59
#define BIOME_DESERTRIVER			60
#define BIOME_DESERTISLAND			61
#define BIOME_WIDEBEACH				62
#define BIOME_BEACHCLIFF			63
#define BIOME_TROPICALMEADOW		64
#define BIOME_TROPICALROLLINGHILLS	65
#define BIOME_TROPICALMOUNTAINTOP	66
#define BIOME_TOPICALPOND			67
#define BIOME_TROPICALSTREAM		68
#define BIOME_TROPICALRAVINE		69
#define BIOME_TROPICALBEACH			70
#define BIOME_TROPICALOASIS			71
#define BIOME_TROPICALWATERFALL		72
#define BIOME_JUNGLEMEADOW			73
#define BIOME_JUNGLEHILLS			74
#define BIOME_JUNGLEMOUNTAIN		75
#define BIOME_JUNGLEPOND			76
#define BIOME_JUNGLESTREAM			77
#define BIOME_JUNGLERAVINE			78
#define BIOME_SUNBURSTBEACH			79
#define BIOME_SUNBURSTISLAND		80
#define BIOME_SUNBURSTISLANDCLIFF	81

#define TERRAIN_NULL		0
#define TERRAIN_SNOWY		1
#define TERRAIN_ICY			2
#define TERRAIN_GLACIER		3
#define TERRAIN_MEADOW		4
#define TERRAIN_STREAM		5
#define TERRAIN_BEACH		6
#define TERRAIN_CONIFEROUS	7
#define TERRAIN_JUNGLE		8


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