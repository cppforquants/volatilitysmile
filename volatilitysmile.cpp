#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

// Black-Scholes formula for a European call option
double black_scholes_call(double S, double K, double T, double r, double sigma) {
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);

    auto N = [](double x) {
        return 0.5 * std::erfc(-x / std::sqrt(2));
    };

    return S * N(d1) - K * std::exp(-r * T) * N(d2);
}

// Bisection method to compute implied volatility
double implied_volatility(double market_price, double S, double K, double T, double r,
                          double tol = 1e-5, int max_iter = 100) {
    double low = 0.0001;
    double high = 2.0;

    for (int i = 0; i < max_iter; ++i) {
        double mid = (low + high) / 2.0;
        double price = black_scholes_call(S, K, T, r, mid);

        if (std::abs(price - market_price) < tol) return mid;
        if (price > market_price) high = mid;
        else low = mid;
    }

    return (low + high) / 2.0; // best guess
}

// Synthetic volatility smile to simulate market prices
double synthetic_volatility(double K, double S) {
    double base_vol = 0.15;
    return base_vol + 0.0015 * std::pow(K - S, 2);
}

int main() {
    double S = 100.0;     // Spot price
    double T = 0.5;       // Time to maturity in years
    double r = 0.01;      // Risk-free rate

    std::cout << "Strike\tMarketPrice\tImpliedVol\n";

    for (double K = 60; K <= 140; K += 2.0) {
        double true_vol = synthetic_volatility(K, S);
        double market_price = black_scholes_call(S, K, T, r, true_vol);
        double iv = implied_volatility(market_price, S, K, T, r);

        std::cout << std::fixed << std::setprecision(2)
                  << K << "\t" << market_price << "\t\t" << std::setprecision(4) << iv << "\n";
    }

    return 0;
}