
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <functional>
#include <limits>
#include <algorithm>
#include <chrono>
#include <random>
#include "InputData.h"
#include "Random.h"

using namespace std;

void Random::SeedInput(InputData &data)
{
    // Mersenne Twiester Engine
    // 1.1 Uses a seed defined in the "InputData" class 
    // 1.2 Generates a seed with high resolution clock
    long long longseed;
    (data.use_random_seed == 0 ) ? longseed = data.seed : longseed = 0.0000001*std::chrono::high_resolution_clock::now().time_since_epoch().count();

    // 2. Initializes the random number generator with a twister engine object of the class MT19937
    twisterengine.seed (longseed);
}

void Random::SeedInput(InputData *pdata)
{
    // Mersenne Twiester Engine
    // 1.1 Uses a seed defined in the "InputData" class 
    // 1.2 Generates a seed with high resolution clock
    long long longseed;
    (pdata->use_random_seed == 0 ) ? longseed = pdata->seed : longseed = 0.0000001*std::chrono::high_resolution_clock::now().time_since_epoch().count();

    
    // 2. Initializes the random number generator with a twister engine object of the class MT19937
    twisterengine.seed(longseed);
}

void Random::SeedInput()
{
    // Mersenne Twiester Engine
    // Generates a seed with high resolution clock
    long long longseed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    longseed = longseed * (00000000.1);
    
    // 2. Initializes the random number generator with a twister engine object of the class MT19937
    twisterengine.seed(longseed);
}

double Random::dRN(double min, double max)
{
    // Generates an uniform distribution in range of [double min, double max]. 
    std::uniform_real_distribution<double> dist_real_uniform(min, max);
    // Returns random number within [min, max].
    return dist_real_uniform(twisterengine);
}

double Random::iRN(int min, int max)
{
    // Generates an uniform distribution in range of [int min, int max]. 
    std::uniform_int_distribution<int> dist_int_uniform(min, max - 1);
    // Returns random number within [min, max].
    return dist_int_uniform(twisterengine);
}
