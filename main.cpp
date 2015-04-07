#include <iostream>
#include <memory>
#include <chrono>
#include "parameters.hpp"
#include "black_scholes.hpp"
#include "procedural.hpp"
#include "object_oriented.hpp"
#include "quantlib_mc.hpp"

int main() {
	Parameters p = make_params();
	double err;
	const int N = 10000;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	auto start = std::chrono::high_resolution_clock::now();

	std::cout << "B-S:\t\t" << black_scholes(p) << std::endl;

	auto end = std::chrono::high_resolution_clock::now();
	auto diff = end - start;
	std::cout << "\t\ttook: " << 
		std::chrono::duration<double, std::milli> (diff).count() << " ms\n";

	start = std::chrono::high_resolution_clock::now();
	std::cout << "MC:\t\t" << mc_sim(p, err, N, seed) << "(" << err << ")\n";

	end = std::chrono::high_resolution_clock::now();
	diff = end - start;
	std::cout << "\t\ttook: " << 
		std::chrono::duration<double, std::milli> (diff).count() << " ms\n";

	seed = std::chrono::system_clock::now().time_since_epoch().count();

	start = std::chrono::high_resolution_clock::now();
	std::shared_ptr<Payoff> payoff;
	if (p.type == Parameters::Call) {
		payoff = std::make_shared<PayoffCall>(p.K);
	}
	else {
		payoff = std::make_shared<PayoffPut>(p.K);
	}
	GeometricBrownianMotion gbm(p.r, p.vol);
	std::shared_ptr<MonteCarlo> mc = std::make_shared<MonteCarlo>(gbm, N, seed);
	Option option(p, payoff);
	option.setEngine(mc);

	std::cout << "(OO) MC:\t" << option.NPV() << "(" << option.error() << ")\n";
	end = std::chrono::high_resolution_clock::now();
	diff = end - start;
	std::cout << "\t\ttook: " << 
		std::chrono::duration<double, std::milli> (diff).count() << " ms\n";

	std::cout << "\n\n\n";

	ql_mc(p);

	return 0;
}
