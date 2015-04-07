#include "black_scholes.hpp"

double phi(double x) {
	// http://www.johndcook.com/blog/cpp_phi/
    double a1 =  0.254829592;
    double a2 = -0.284496736;
    double a3 =  1.421413741;
    double a4 = -1.453152027;
    double a5 =  1.061405429;
    double p  =  0.3275911;

    int sign = 1;
    if (x < 0)
        sign = -1;
    x = std::fabs(x) / std::sqrt(2.0);

    double t = 1.0 / (1.0 + p * x);
    double y = 1.0 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * 
    			std::exp(-x * x);
 
    return 0.5 * (1.0 + sign * y);
}

double black_scholes(const Parameters& p) {
	double d1, d2, call_value;
	d1 = (std::log(p.S / p.K) + (p.r + 0.5 * p.vol * p.vol) * p.T) / 
			(p.vol * std::sqrt(p.T));
	d2 = d1 - p.vol * std::sqrt(p.T);

	call_value = phi(d1) * p.S - phi(d2) * p.K * std::exp(-p.r * p.T);

	if (p.type == Parameters::Put) {
		return call_value - p.S + p.K * std::exp(-p.r * p.T);
	}
	return call_value;
}