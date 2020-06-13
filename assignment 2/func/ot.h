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

    int run(std::vector<Vector>& points, std::vector<double>& weights, double f = 1) {
        lbfgsfloatval_t fx;
        int N = int(points.size());
        this->points = points;
        this->f = f;

        lbfgsfloatval_t *m_x = lbfgs_malloc(N);
        /* Initialize the weights. */
        for (int i = 0; i < N; i ++) {
            m_x[i] = weights[i];
        }

        /*
            Start the L-BFGS optimization; this will invoke the callback functions
            evaluate() and progress() when necessary.
         */
        int ret = lbfgs(N, m_x, &fx, _evaluate, NULL, this, NULL);

        /* Report the result. */
        printf("L-BFGS optimization terminated with status code = %d\n", ret);
        printf("  fx = %f, x[0] = %f, x[1] = %f\n", fx, m_x[0], m_x[1]);
        for (int i = 0; i < N; i ++) {
            printf("x[%d] = %f \n", i, m_x[i]);
        }
        
        return ret;
    }

protected:
    static lbfgsfloatval_t _evaluate(   void *instance, 
                                        const lbfgsfloatval_t *x, 
                                        lbfgsfloatval_t *g,
                                        const int n, 
                                        const lbfgsfloatval_t step) {
        return reinterpret_cast<objective_function*>(instance)->evaluate(x, g, n, step);
    }

    lbfgsfloatval_t evaluate(   const lbfgsfloatval_t *x, // weights
                                lbfgsfloatval_t *g, 
                                const int n, // number of weights
                                const lbfgsfloatval_t step) {
        lbfgsfloatval_t fx = 0.0;
        std::vector<double> ws;
        for (int i = 0; i < n; i ++) {
            ws.push_back(x[i]);
            printf("w[%d] = %f \n", i, x[i]);
        }

        for (int i = 0; i < n; i ++) {
            // printf("%d \n", i);
            g[i] = - g_grad(points, i, ws, f);
            printf("grad = %f \n", g[i]);
        }
        fx = - g_func(points, ws, f);
        printf("f = %f \n", fx);
        return fx;
    }
};

