#include <cmath>
#include <algorithm>
#include <iostream>

class Function {
public:
    virtual double operator () (double x) = 0;
};

class SecantSolver {
protected:
    Function &F;
public:
    SecantSolver(Function & F) : F(F) {}
    double solve(double x0, double x1) {
        const double f_tol = 1e-12;
        const double x_tol = 1e-9;
        const int max_calls = 30;

        int calls = 0;
        double f0 = F(x0); ++calls;
        double f1 = F(x1); ++calls;

        if (std::abs(f0) < f_tol) return x0;
        if (std::abs(f1) < f_tol) return x1;

        double best_x = x1;
        double best_fx = std::abs(f1);
        if (std::abs(f0) < best_fx) { best_x = x0; best_fx = std::abs(f0); }

        while (calls < max_calls) {
            double denom = (f1 - f0);
            double x2;
            if (std::abs(denom) < 1e-16) {
                x2 = 0.5 * (x0 + x1);
            } else {
                x2 = x1 - f1 * (x1 - x0) / denom;
            }

            if (std::abs(x2 - x1) < 1e-16) {
                x2 = 0.5 * (x0 + x1);
            }

            if (calls >= max_calls) break;

            double f2 = F(x2); ++calls;

            if (std::abs(f2) < best_fx) {
                best_fx = std::abs(f2);
                best_x = x2;
            }

            if (std::abs(f2) < f_tol) return x2;
            if (std::abs(x2 - x1) < x_tol) return x2;

            x0 = x1; f0 = f1;
            x1 = x2; f1 = f2;
        }

        return best_x;
    }
};

//测试代码的运行
class Func1 : public Function {
public:
    double operator () (double x) {
        return x - tan(x);
    }
    double d(double x) {
        return 1 - 1 / pow(cos(x), 2);
    }
};

int main() {
    Func1 func;
    SecantSolver solver(func);
    double x = solver.solve(4.5, 5.0);
    std::cout << x << std::endl;
    return 0;
}
