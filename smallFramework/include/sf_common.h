#pragma once

#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// std. Includes

#include <cstdio>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <random>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#define NOMINMAX
using namespace std;

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ONE_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

#define PI 3.141592654f
#define FLOAT_MAX  99999999999999999.f
#define FLOAT_MIN -99999999999999999.f

typedef unsigned char ubyte;

// math quickies
inline float cotf(float a) { return cosf(a) / sinf(a); };
inline float fractf(float a) { return a - floorf(a); };
inline float sqrf(float a) { return a*a; };
inline int sqri(int a) { return a*a; };
inline int powi(int base, int exp) { return int(powf((float)base, (float)exp)); };
inline float logf(float base, float x) { return logf(x) / logf(base); };
inline int logi(int base, int x) { return int(logf((float)x) / logf((float)base)); };
inline float signf(float x) { if (x > 0) return 1; else if (x < 0) return -1; else return 0; };
inline float maxf(float a, float b) { if (a < b) return b; else return a; };
inline float minf(float a, float b) { if (a < b) return a; else return b; };
inline int maxi(int a, int b) { if (a < b) return b; else return a; };
inline int mini(int a, int b) { if (a < b) return a; else return b; };
inline int mini(int a, int b, int c) { if (a < b) { if (a < c) return a; else return c; } else { if (b < c) return b; else return c; } };
inline int absi(int a) { if (a < 0) return -a; else return a; };
inline short abss(short a) { if (a < 0) return -a; else return a; };
inline void swapi(int & a, int & b) { int t = a; a = b; b = t; };
inline int floori(float a) { return int(floor(a)); };
inline int ceili(float a) { return int(ceil(a)); };
inline float clampf(float x, float min, float max)
{
	if (x < min) x = min;
	if (x > max) x = max;
	return x;
}
inline int clampi(int x, int min, int max)
{
	if (x < min) x = min;
	if (x > max) x = max;
	return x;
}
