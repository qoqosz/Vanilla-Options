#ifndef BLACK_SCHOLES_HPP
#define BLACK_SCHOLES_HPP
#include <cmath>
#include "parameters.hpp"

double phi(double x);
double black_scholes(const Parameters& p);

#endif