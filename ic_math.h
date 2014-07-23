//ic_math.h
//Iclea Project Advanced Math Functions:
//Oct 2012: Linear Algebra and Trigonometry
#ifndef IC_MATH_H
#define IC_MATH_H

struct coord
{
	int x;
	int y;
	coord() {x=0; y=0;}
	coord(int _x, int _y) {x=_x; y=_y;}
	coord(const coord& src) {x=src.x; y=src.y;}
};

#define PI 3.14159265359f
sf::Vector2f toVector(coord src);
float calcMagnitude(sf::Vector2f v);
float calcDist(sf::Vector2f pta, sf::Vector2f ptb);
bool isCollision(sf::Vector2f pt, sf::IntRect box);
float dot(sf::Vector2f a, sf::Vector2f b);
sf::Vector2f normalVector(sf::Vector2f src);
float angle(sf::Vector2f b);
sf::Vector2f unitVector(float theta);
sf::Vector2f scalar(float c, sf::Vector2f v);
void initRandom();
void initRandom(unsigned long _seed);
float distortedCosine(float _theta, long seed);
unsigned int anyRandom(long seed);
unsigned char newRandom(unsigned char low, unsigned char hi, float _theta, long seed);
unsigned char noiseyPixel(coord pos, unsigned char low, unsigned char range, int con, long seed);
bool operator==(const coord& a, const coord& b);
bool operator!=(const coord& a, const coord& b);
bool operator<(const coord& a, const coord& b);
bool operator>(const coord& a, const coord& b);
bool operator<=(const coord& a, const coord& b);
bool operator>=(const coord& a, const coord& b);

coord operator+(const coord& a, const coord& b);
coord operator-(const coord& a, const coord& b);
int max3(int a, int b, int c);
#endif //IC_MATH_H ends
