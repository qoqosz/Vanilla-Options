#include "procedural.hpp"

double mc_sim(const Parameters& p, double& error, 
			  const unsigned N, const unsigned seed) {
	std::mt19937 mt(seed);
	std::normal_distribution<> dist(0.0, 1.0);

	double sum = 0.0, sum_sqd = 0.0, NN = static_cast<double>(N);

	for (auto i = 0; i < N; ++i) {
		double St = p.S, discounted_St;

		St *= std::exp((p.r - 0.5 * p.vol * p.vol) * p.T + 
				p.vol * std::sqrt(p.T) * dist(mt));

		discounted_St = payoff(St, p.K, p.type) * std::exp(-p.r * p.T);
		sum 	+= discounted_St;
		sum_sqd += discounted_St * discounted_St;
	}
	// standard error of the mean
	error = std::sqrt( (sum_sqd - sum * sum / NN) / NN / (NN + 1.0));
	return sum / NN;
}

double payoff(const double S, const double K, const Parameters::Type t) {
	if (t == Parameters::Call) {
		return std::max(S - K, 0.0);
	}
	return std::max(K - S, 0.0);
}
