#pragma once
#include <cstdlib>

using namespace std;

float random_float() {
    return float(rand())/float((RAND_MAX));
}