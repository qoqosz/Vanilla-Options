#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

struct Parameters {
	double r;
	double S;
	double K;
	double T;
	double vol;
	enum Type {
		Put,
		Call
	} type;
};

Parameters make_params();
#endif