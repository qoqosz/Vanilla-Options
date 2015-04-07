#ifndef OBJECT_ORIENTED_HPP
#define OBJECT_ORIENTED_HPP
#include <cmath>
#include <numeric>
#include <memory>
#include <random>
#include <vector>
#include "parameters.hpp"

class Payoff {
public:
	Payoff() {};
	virtual ~Payoff() {};
	virtual double operator()(double S) = 0;
};

class PayoffCall : public Payoff {
	double K_;
public:
	PayoffCall(const double K);
	~PayoffCall() {};

	double operator()(double S);
};

class PayoffPut : public Payoff {
	double K_;
public:
	PayoffPut(const double K);
	~PayoffPut() {};

	double operator()(double S);	
};

class Statistics {
	double 		sum_;
	double 		sum2_;
	unsigned 	count_;
public:
	Statistics();

	void reset();
	void add(const double value);
	double mean() const;
	double stddev() const;
};

class GeometricBrownianMotion {
	double mu_;
	double vol_;
	std::shared_ptr<std::mt19937> mt_;
	std::shared_ptr<std::normal_distribution<>> dist_;
public:
	GeometricBrownianMotion(const double mu = 0.0, const double vol = 0.0);
	double St(const double S0, const double t) const;
	void seed(const unsigned seed);
};

class Option;

class MonteCarlo {
	GeometricBrownianMotion gbm_;
	unsigned n_;
public:
	MonteCarlo(const GeometricBrownianMotion& gbm, const unsigned n,
			   const unsigned seed);
	double execute(Option& p, double& error);
};

class Option {
	std::shared_ptr<Payoff>	payoff_;
	std::shared_ptr<MonteCarlo> mc_;
	Parameters 	params_;
	double 		NPV_;
	double 		error_;
	bool 		isNPV_;

	void calculate();
public:
	Option(const Parameters& p, const std::shared_ptr<Payoff>& payoff);

	double NPV();
	double error();
	void   setEngine(const std::shared_ptr<MonteCarlo>& mc);

	Parameters 				parameters() const;
	std::shared_ptr<Payoff> payoff() const;
};
#endif