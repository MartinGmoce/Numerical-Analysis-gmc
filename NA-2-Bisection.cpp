#include <cmath>
#include <algorithm>
#include <iostream>

class Function {
public:
    virtual double operator () (double x) = 0;
};

class BisectionSolver {
protected:
    Function &F;
public:
    BisectionSolver(Function & F) : F(F) {}
    double solve(double l, double r) {
        // 在这里实现你的二分法，将答案返回
        const double f_tol = 1e-12;
        const double x_tol = 1e-9;

        int calls = 0;

        double fl = F(l); ++calls;
        double fr = F(r); ++calls;

        if (std::abs(fl) < f_tol) return l;
        if (std::abs(fr) < f_tol) return r;

        double best_x = l;
        double best_fx = std::abs(fl);
        if (std::abs(fr) < best_fx) { best_x = r; best_fx = std::abs(fr); }

        int remaining = 40 - calls;
        if (remaining <= 0) return best_x;

        bool signs_opposite = (fl * fr <= 0.0);

        for (int iter = 0; iter < remaining; ++iter) {
            double m = 0.5 * (l + r);
            double fm = F(m);
            ++calls;

            if (std::abs(fm) < best_fx) {
                best_x = m;
                best_fx = std::abs(fm);
            }

            if (std::abs(fm) < f_tol) return m;
            if ((r - l) * 0.5 < x_tol) return m;

            if (signs_opposite) {
                if (fl * fm <= 0.0) {
                    r = m;
                    fr = fm;
                } else {
                    l = m;
                    fl = fm;
                }
            } else {
                if (std::abs(fl) > std::abs(fr)) {
                    l = m;
                    fl = fm;
                } else {
                    r = m;
                    fr = fm;
                }
                if (fl * fr <= 0.0) signs_opposite = true;
            }

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
    BisectionSolver solver(func);
    double x = solver.solve(3.0, 4.7);
    std::cout << x << std::endl;
    return 0;
}
