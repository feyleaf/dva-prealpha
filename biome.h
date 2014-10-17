//biome.h

#ifndef BIOME_H
#define BIOME_H

class BiomeClass
{
public:
	char name[40];
	int maxVolume;
	int maxReach;
	std::vector<coord> points;

	BiomeClass();
	~BiomeClass();

	bool addPoint(coord _pt);
};

#endif//BIOME_H