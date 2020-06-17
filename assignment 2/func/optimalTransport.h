#include "optimalTransport_aux.h"
#include "../lib/lbfgs.c"

class objective_function {
protected:
    lbfgsfloatval_t *m_x;
    std::vector<Vector> points;
    std::vector<double> lambdas;
    lbfgs_parameter_t para;
    double f;


public:
    objective_function() : m_x(NULL) {}

    virtual ~objective_function() {
        if (m_x != NULL) {
            lbfgs_free(m_x);
            m_x = NULL;
        }
    }

    std::vector<double> run(const std::vector<Vector>& points, std::vector<double> lambdas, const double f) {
        lbfgsfloatval_t fx;
        int N = int(lambdas.size());
        this->points = points;
        this->f = f;
        lbfgsfloatval_t *m_x = lbfgs_malloc(N);
        this->lambdas = lambdas;

        /* Initialize the weights. */
        for (int i = 0; i < N; i ++) {
            m_x[i] = 0.001;
        }

        lbfgs_parameter_init(&this->para);
        this->para.max_iterations = 100;

        int ret = lbfgs(N, m_x, &fx, _evaluate, NULL, this, &para);

        /* Report the result. */
        printf("L-BFGS optimization terminated with status code = %d\n", ret);
        printf("  fx = %f \n", fx);

        std::vector<double> res(N, 0);
        for (int i = 0; i < N; i ++) {
            res[i] = m_x[i];
        }
        
        return res;
    }

protected:
    static lbfgsfloatval_t _evaluate(void *instance, const lbfgsfloatval_t *x, lbfgsfloatval_t *g, const int n, const lbfgsfloatval_t step) {
        return reinterpret_cast<objective_function*>(instance)->evaluate(x, g, n, step);
    }

    lbfgsfloatval_t evaluate(const lbfgsfloatval_t *x, lbfgsfloatval_t *g, const int n, const lbfgsfloatval_t step) {
        
        lbfgsfloatval_t fx = 0.0;

        std::vector<double> weights(n, 0);
        
        for (int i = 0; i < n; i ++) {
            weights[i] = x[i];
        }
        
        double areaSum = 0;
        std::vector<Polygon> cells = powerDiagram(points, weights);
        for (int i = 0; i < n-1; i ++) {
            areaSum += weights[i];
        }
        
        for (int i = 0; i < n - 1; i ++) {
            g[i] = - g_grad(points[i], cells[i], lambdas[i], f);
        }
        g[n-1] = lambdas[n - 1] - areaSum;

        fx = - g_func(points, weights, lambdas, f);
        // printf("g = %f \n", fx);

        return fx;
    }
};

std::vector<double> optimalTransport(const std::vector<Vector>& points, const double f = 1, std::string l_type = "uniform") {
    objective_function obj;
    return obj.run(points, uniform_lambda(points), f);
}

struct ParticleWeight {
    std::vector<double> fluid;
    double air;
};

ParticleWeight optimalTransport_fluid(std::vector<Vector>& points, std::vector<double> lambdas, const double f = 1) {
    // lambdas is of size len(points) + 1
    ParticleWeight res;
    objective_function obj;
    std::vector<double> weights = obj.run(points, lambdas, f); // the last weight is for the air cells.
    res.air = weights[int(weights.size()) - 1];
    weights.erase(weights.begin() + (int(weights.size()) - 1));
    res.fluid = weights;
    return res;
}