#include "globals.h"

//calcMagnitude(Vector2f):
//Purpose: returning the length/magnitude of a vector
//Input: sf::Vector2f v - 2D endpoint of a vector bound to the origin (0,0)
//Output: float - Magnitude (length/distance from origin) of vector v
float calcMagnitude(sf::Vector2f v)
{
	return sqrtf(((v.x)*(v.x))+((v.y)*(v.y)));
}

//calcDist(Vector2f, Vector2f):
//Purpose: returning the distance between two points in 2D space
//Input: sf::Vector2f a - point of a vector in 2D space
//		sf::Vector2f b - another point of a vector in 2D space with the same origin as vector a
//Output: float - Distance between point a and point b
//----------------------------------------------------
//Note: Pythagorean Theorem: a²+b²=c²
//solved for c(hypotenuse: c = √(a²+b²)
float calcDist(sf::Vector2f pta, sf::Vector2f ptb)
{
	return sqrtf(((pta.x-ptb.x)*(pta.x-ptb.x)) + ((pta.y-ptb.y)*(pta.y-ptb.y)));
}

//isCollision(Vector2f, IntRect):
//Purpose: testing bounding-box collision with a point
//Input: sf::Vector2f pt - point in 2D space
//		sf::IntRect box -  4-sided boundary box in the same space
//Output: bool - true if pt is inside the box, false otherwise
bool isCollision(sf::Vector2f pt, sf::IntRect box)
{
//	bool ret=false;
	//if(!(pt.x<box.Left || pt.x>box.Right || pt.y<box.Top || pt.y>box.Bottom)) ret=true;
	return box.contains(int(pt.x), int(pt.y));
}

//dot(Vector2f, Vector2f)
//Purpose: compute the dot product of vectors a & b
//Input: sf::Vector2f a - point of a vector in 2D space
//		sf::Vector2f b - another point of a vector in 2D space with the same origin as vector a
//Output: float - the dot product
//---------------------------------------
//Note: for vectors A(ax, ay) and B(bx, by),
//A • B = ax*bx + ay*by (= |A|*|B|*cos θ where θ is the angle between the vectors)
float dot(sf::Vector2f a, sf::Vector2f b)
{
	float ret=(a.x*b.x) + (a.y*b.y);
	return ret;
}

//normalVector(Vector2f):
//Purpose: compute the normalized vector (magnitude 1) from the given source vector
//Input: sf::Vector2f src - a vector on the 2D plane
//Output: sf::Vector2f - the normalized reculting vector
sf::Vector2f normalVector(sf::Vector2f src)
{
	float d=calcMagnitude(src);
	sf::Vector2f normal(src.x/d, src.y/d);
	return normal;
}

//angle(Vector2f):
//Purpose: to compute the angle of a vector from 2D space to a radial space
//Input: sf::Vector2f b - the vector in 2D space to be computed
//Output: float - (in radians) the angle θ around the radial unit circle
//-----------------------------------------------------------------------
//Note: in radians, the unit circle spans 0 ≤ θ ≤ 2π
//Other Note: if the y coordinate is below the x axis, the
//arccosine of x must be negated or subtracted from π to calculate the
//correct angle
float angle(sf::Vector2f b)
{
	sf::Vector2f v=normalVector(b);
	if(v.y<0)	return acosf(v.x);
	else return -acosf(v.x);

}

//unitVector(float):
//Purpose: to fabricate a vector on the 2D plane, of magnitude 1, given θ along the unit circle
//Input: float theta - in radians, θ will determine the angle to be traversed across the unit circle
//Output: sf::Vector2f - the fabricated vector, of length 1, on the 2D plane
sf::Vector2f unitVector(float theta)
{
	return sf::Vector2f(cos(theta),-sin(theta));
}

//scalar(float, Vector2f):
//Purpose: to compute the scalar multiple of a vector (multiply its magnitude)
//Input: float c - the scalar variable to be multiplied
//		sf::Vector2f v - the vector to be multiplied
//Output: sf::Vector2f - the output is a colinear vector to v, of magnitude c*|v|
sf::Vector2f scalar(float c, sf::Vector2f v)
{
	return sf::Vector2f(c*v.x, c*v.y);
}

//initRandom(void)
//Purpose: to begin a time-seeded random generation procedure
//Input: none
//Output: none
//------------
//Note: this function is more utility, for having an unpredictable random stream of numbers
void initRandom()
{
	srand((unsigned)time(NULL));
}

//initRandom(void)
//Purpose: to begin a seed for random generation procedure
//Input: unsigned long _seed - the seed, which startes a stream of pseudo-random numbers
//Output: none
//------------
//Note: this function is more utility, for having a partially controllable random stream of numbers
void initRandom(unsigned long _seed)
{
	srand(_seed);
}


float distortedCosine(float _theta, long seed)
{
	//output is 0<output<2
	srand(unsigned long(777*(seed+tan(_theta*(1+seed)))));
	float ret=((float(rand()%100)/100)+((1-cos(_theta))/2));
	srand((rand()%50)+(unsigned)time(NULL));
	return ret;
}

unsigned int anyRandom(long seed)
{
	float theta=float((unsigned)time(NULL))/PI;
	srand(unsigned long(777*(seed+tan(theta*(1+seed)))));
	unsigned int ret=unsigned int(float((rand()%100)/100.0f)*100.0f);
	srand((rand()%50)+(unsigned)time(NULL));
	return int(ret);
}

unsigned char newRandom(unsigned char low, unsigned char hi, float _theta, long seed)
{
	return unsigned char((distortedCosine(_theta, seed)/2)*(hi-low))+low;
}

//returns a specific format output for inserting into a sf::Color [either red, green, blue, or alpha]
unsigned char noiseyPixel(coord pos, unsigned char low, unsigned char range, int con, long seed)
{
	unsigned char pixel=0;
	unsigned char value=0;
	float theta = float((sin(float(pos.x))+cos(float(pos.y)))*PI);
	if(con>0)
		value=newRandom(low, range, theta/(con+1), seed);
	else
		value=newRandom(low, range, theta, seed);

	if(value>=0 && value<=255)
		pixel=value;
	else
		pixel=low;
	return pixel;
	//return newRandom(128, 196, (float((sin(float(x))+cos(float(y)))*PI))/(con+1), seed);
}

sf::Vector2f toVector(coord src)
{
	return sf::Vector2f(float(src.x), float(src.y));
}

bool operator==(const coord& a, const coord& b)
{
	return (a.x==b.x && a.y==b.y);
}
bool operator!=(const coord& a, const coord& b)
{
	return (!(a.x==b.x && a.y==b.y));
}
coord operator+(const coord& a, const coord& b)
{
	return coord(a.x+b.x, a.y+b.y);
}
coord operator-(const coord& a, const coord& b)
{
	return coord(a.x-b.x, a.y-b.y);
}

int max3(int a, int b, int c)
{
	int ret=0;
	if(a>b) ret=a; else ret=b;
	if(ret>c) return ret; else return c;
	return ret;
}