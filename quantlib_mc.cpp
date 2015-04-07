#include "quantlib_mc.hpp"
#include <chrono>

void ql_mc(const Parameters& p) {
	using namespace QuantLib;

	Calendar cal = TARGET();
	Date today(7, April, 2015);
	Settings::instance().evaluationDate() = today;

	DayCounter counter = ActualActual();

	int d_days = static_cast<int>(p.T * 365.25);
	Date maturity = today + d_days * Days;

	std::cout << counter.yearFraction(today, maturity) << std::endl;

	boost::shared_ptr<Exercise> europeanEX(
		new EuropeanExercise(maturity));

	Handle<Quote> underlying_H(
		boost::shared_ptr<Quote>(new SimpleQuote(p.S)));

	Handle<YieldTermStructure> rTS(
		boost::shared_ptr<YieldTermStructure>(
			new FlatForward(today, p.r, counter)));

	Handle<YieldTermStructure> dividendTS(
		boost::shared_ptr<YieldTermStructure>(
			new FlatForward(today, 0.0, counter)));

	Handle<BlackVolTermStructure> volTS(
		boost::shared_ptr<BlackVolTermStructure>(
			new BlackConstantVol(today, cal, p.vol, counter)));

	Option::Type type;

	if (p.type == Parameters::Put) {
		type = Option::Put;
	}
	else {
		type = Option::Call;
	}

	boost::shared_ptr<StrikedTypePayoff> payoff(
		new PlainVanillaPayoff(type, p.K));

	boost::shared_ptr<BlackScholesMertonProcess> bsmProcess(
		new BlackScholesMertonProcess(underlying_H, dividendTS, rTS, volTS));

	VanillaOption option(payoff, europeanEX);

	option.setPricingEngine(boost::shared_ptr<PricingEngine>(
		new AnalyticEuropeanEngine(bsmProcess)));

	auto start = std::chrono::high_resolution_clock::now();

	std::cout << "QL (analytic): " << option.NPV() << std::endl;

	auto end = std::chrono::high_resolution_clock::now();
	auto diff = end - start;
	std::cout << "\t\ttook: " << 
		std::chrono::duration<double, std::milli> (diff).count() << " ms\n";


	start = std::chrono::high_resolution_clock::now();
	
	boost::shared_ptr<PricingEngine> mc_engine;
	mc_engine = MakeMCEuropeanEngine<PseudoRandom>(bsmProcess)
				.withSteps(1)
				.withAbsoluteTolerance(0.02)
				.withSeed(666);
	option.setPricingEngine(mc_engine);

	std::cout << "QL (MC): " << option.NPV() << std::endl;

	end = std::chrono::high_resolution_clock::now();
	diff = end - start;
	std::cout << "\t\ttook: " << 
		std::chrono::duration<double, std::milli> (diff).count() << " ms\n";

}