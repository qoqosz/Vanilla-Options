#include <iostream>
#include "parameters.hpp"

Parameters make_params() {
	Parameters p;
	char c;

	std::cout << "S: ";
	std::cin >> p.S;
	std::cout << "K: ";
	std::cin >> p.K;
	std::cout << "r: ";
	std::cin >> p.r;
	std::cout << "vol: ";
	std::cin >> p.vol;
	std::cout << "T: ";
	std::cin >> p.T;
	std::cout << "[P]ut / [C]all: ";
	std::cin >> c;
	if (c == 'P' || c == 'p') {
		p.type = Parameters::Put;
	}
	else {
		p.type = Parameters::Call;
	}

	return p;
}