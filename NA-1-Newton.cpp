#include <cmath>
#include <algorithm>
#include <iostream>

class Function {
public:
    virtual double operator () (double x) = 0;
    virtual double d(double x) = 0;
};

class NewtonSolver {
protected:
    Function &F;
public:
    NewtonSolver(Function & F) : F(F) {}
    double solve(double x0) {
        // 在这里实现你的牛顿迭代法，将答案返回
        const double abs_tol = 1e-12;  
        const double rel_tol = 1e-12;
        double x = x0;

        for (int iter = 0; iter < 5; ++iter) {
            double fx = F(x);       // 第一次调用 F
            double dfx = F.d(x);    // 第一次调用 F.d

            if (std::isnan(fx) || std::isnan(dfx) || std::isinf(fx) || std::isinf(dfx)) {
                return x;
            }
            if (std::abs(fx) <= abs_tol) return x;

            double dx = fx / dfx;
            x -= dx;

            if (std::abs(dx) <= std::max(abs_tol, rel_tol * std::abs(x))) return x;
        }

        return x; 
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
    NewtonSolver solver(func);
    double x = solver.solve(4.5);
    std::cout << x << std::endl;
    return 0;
}
