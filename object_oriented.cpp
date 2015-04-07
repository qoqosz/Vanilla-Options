#include "object_oriented.hpp"

PayoffCall::PayoffCall(const double K) : K_{K} {
}
double PayoffCall::operator()(const double S) {
	return std::max(S - K_, 0.0);
}

PayoffPut::PayoffPut(const double K) : K_{K} {
}
double PayoffPut::operator()(const double S) {
	return std::max(K_ - S, 0.0);
}

Statistics::Statistics()
	: sum_{0.0}, sum2_{0.0}, count_{0} {
}
void Statistics::reset() {
	sum_ 	= 0.0;
	sum2_ 	= 0.0;
	count_ 	= 0;
}
void Statistics::add(const double value) {
	sum_ 	+= value;
	sum2_ 	+= value * value;
	++count_;
}
double Statistics::mean() const {
	return sum_ / static_cast<double>(count_);
}
double Statistics::stddev() const {
	double N = static_cast<double>(count_);
	return std::sqrt( (sum2_ - sum_ * sum_ / N) / N / (N + 1.0) );
}

GeometricBrownianMotion::GeometricBrownianMotion(const double mu, 
	const double vol)
	: mu_{mu}, vol_{vol} {
	mt_ 	= std::make_shared<std::mt19937>(666);
	dist_ 	= std::make_shared<std::normal_distribution<>>(0.0, 1.0);
}
double GeometricBrownianMotion::St(const double S0, const double t) const {
	double dummy = (mu_ - 0.5 * vol_ * vol_) * t;
	dummy += std::sqrt(t) * vol_ * dist_->operator()(*mt_);

	return S0 * std::exp(dummy);
}
void GeometricBrownianMotion::seed(const unsigned seed) {
	mt_->seed(seed);
}

MonteCarlo::MonteCarlo(const GeometricBrownianMotion& gbm, const unsigned n,
	const unsigned seed) {
	gbm_ = gbm;
	n_ = n;
	gbm_.seed(seed);
}

double MonteCarlo::execute(Option& op, double& error) {
	Statistics stat;
	Parameters p = op.parameters();
	std::shared_ptr<Payoff> poff = op.payoff();

	for (unsigned i = 0; i < n_; ++i) {
		double dummy = gbm_.St(p.S, p.T);
		dummy = poff->operator()(dummy) * std::exp(-p.r * p.T);

		stat.add(dummy);
	}
	error = stat.stddev();
	return stat.mean();
}

Option::Option(const Parameters& p, const std::shared_ptr<Payoff>& payoff)
	: payoff_{payoff}, NPV_{0.0}, error_{0.0}, isNPV_{false} {
	params_ = p;
}
double Option::NPV() {
	if (!isNPV_) {
		calculate();
	}
	return NPV_;
}
double Option::error() {
	if (!isNPV_) {
		calculate();
	}
	return error_;
}
void Option::setEngine(const std::shared_ptr<MonteCarlo>& mc) {
	mc_ = mc;
}
void Option::calculate() {
	NPV_ = mc_->execute(*this, error_);
}
Parameters Option::parameters() const {
	return params_;
}
std::shared_ptr<Payoff> Option::payoff() const {
	return payoff_;
}