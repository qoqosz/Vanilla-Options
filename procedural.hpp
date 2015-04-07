#ifndef PROCEDURAL_HPP
#define PROCEDURAL_HPP 
#include <cmath>
#include <numeric>
#include <random>
#include <vector>
#include "parameters.hpp"

double mc_sim(const Parameters& p, double& error, 
			  const unsigned N = 10000, const unsigned seed = 666);
double payoff(const double S, const double K, const Parameters::Type t);
#endif