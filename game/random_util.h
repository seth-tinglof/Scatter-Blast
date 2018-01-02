#pragma once

/**
Returns a random double between 0 and 1. Not all values are possible or equally likely due to rounding.
*/
double randDouble();

/**
Returns a random integer between the values of the two arguments inclusive of the first and exclusive of the second. First argument should
be the lower bound.
*/
int randRange(int i, int f);