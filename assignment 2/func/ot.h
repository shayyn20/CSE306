#include "g.h"
#include "../lib/lbfgs.c"

class objective_function {
protected:
    lbfgsfloatval_t *m_x;
    std::vector<Vector> points;
    double f;

public:
    objective_function() : m_x(NULL) {}

    virtual ~objective_function() {
        if (m_x != NULL) {
            lbfgs_free(m_x);
            m_x = NULL;
        }
    }

    std::vector<double> run(std::vector<Vector>& points, double f) {
        lbfgsfloatval_t fx;
        int N = int(points.size());
        this->points = points;
        this->f = f;

        lbfgsfloatval_t *m_x = lbfgs_malloc(N);
        /* Initialize the weights. */
        for (int i = 0; i < N; i ++) {
            m_x[i] = 1;
        }

        int ret = lbfgs(N, m_x, &fx, _evaluate, NULL, this, NULL);

        /* Report the result. */
        printf("L-BFGS optimization terminated with status code = %d\n", ret);
        printf("  fx = %f \n", fx);
        for (int i = 0; i < N; i ++) {
            printf("x[%d] = %f \n", i, m_x[i]);
        }
        std::vector<double> res;
        for (int i = 0; i < N; i ++) {
            res.push_back(m_x[i]);
        }
        
        return res;
    }

protected:
    static lbfgsfloatval_t _evaluate(void *instance, const lbfgsfloatval_t *x, lbfgsfloatval_t *g, const int n, const lbfgsfloatval_t step) {
        return reinterpret_cast<objective_function*>(instance)->evaluate(x, g, n, step);
    }

    lbfgsfloatval_t evaluate(const lbfgsfloatval_t *x, lbfgsfloatval_t *g, const int n, const lbfgsfloatval_t step) {
        lbfgsfloatval_t fx = 0.0;

        std::vector<double> weights;
        for (int i = 0; i < n; i ++) {
            weights.push_back(x[i]);
            //printf("w[%d] = %f \n", i, x[i]);
        }

        std::vector<Polygon> cells = powerDiagram(points, weights);

        for (int i = 0; i < n; i ++) {
            g[i] = - g_grad(points[i], cells[i], f);
            // printf("grad = %f \n", g[i]);
        }

        fx = - g_func(points, weights, f);
        printf("g = %f \n", fx);

        return fx;
    }
};

std::vector<double> optimalTransport(std::vector<Vector>& points, double f = 1) {
    objective_function obj;
    return obj.run(points, f);
}

