/*
 * DifferentialEvolutionMinimizer.hpp
 *
 *  Created on: Nov 26, 2014
 *      Author: Thibaut Metivet
 */

#ifndef DIFFERENTIAL_EVOLUTION_MINIMIZER_HPP
#define DIFFERENTIAL_EVOLUTION_MINIMIZER_HPP

#include "Minimizer.hpp"
#include "Random.hpp"
#include "LinalgUtils.hpp"

BEGIN_NAMESPACE(LQCDA)
BEGIN_NAMESPACE(MIN)

class DifferentialEvolutionMinimizerOptions
: public MinimizerOptions
{
public:
    unsigned int population_size;
    double crossover_probability;
    double scaling_factor;

public:
    DifferentialEvolutionMinimizerOptions()
    {
        init();
    }
    DifferentialEvolutionMinimizerOptions(const MinimizerOptions &opts)
    : MinimizerOptions(opts)
    {
        init();
    }

    virtual ~DifferentialEvolutionMinimizerOptions() noexcept = default;

    virtual void print(std::ostream &os) const override
    {
        os << "DIFFERENTIAL_EVOLUTION options:\n"
           << "\tpopulation_size = " << population_size << std::endl
           << "\tcrossover_probability = " << crossover_probability << std::endl
           << "\tscaling_factor = " << scaling_factor << std::endl;
    }

private:
    void init()
    {
        population_size = 0;
        crossover_probability = 0.5;
        scaling_factor = 0.6;
    }
};

template<typename T>
class DifferentialEvolutionMinimizer
: public Minimizer<T>
{
public: // Constructors/Destructor
    DifferentialEvolutionMinimizer(unsigned int pop_size =0)
    : population_size(pop_size)
    , crossover_probability(0.9)
    , scaling_factor(0.8)
    , dither(false)
    , tolerance(1e-08)
    , max_iter(50000)
    {}

    virtual ~DifferentialEvolutionMinimizer() noexcept = default;

public: // Options
    virtual MinimizerOptions& options() override { return m_Options; }

public: // Minimize
    using Minimizer<T>::minimize;
    virtual typename Minimizer<T>::Result minimize(
        const ScalarFunction<T> &F,
        const std::vector<T> &x0,
        const std::vector<T> &e0,
        const std::vector<ScalarConstraint<T>> &c) override;

public: // Settings
    // INHERIT_MINIMIZER_SETTINGS
    MinimizerOptions m_Options;
    unsigned int population_size;
    double crossover_probability;
    double scaling_factor;
    bool dither;
    double tolerance;
    unsigned int max_iter;

};

template<typename T>
using DIFFERENTIAL_EVOLUTION = DifferentialEvolutionMinimizer<T>;

template<typename T>
typename Minimizer<T>::Result DifferentialEvolutionMinimizer<T>::minimize(
    const ScalarFunction<T> &F,
    const std::vector<T> &x0,
    const std::vector<T> &e0,
    const std::vector<ScalarConstraint<T>> &c)
{
    // Set default population size if not user provided
    unsigned int xDim = x0.size();
    if(!population_size)
    {
        population_size = 20 * xDim;
    }

    // Print info
    utils::vostream vout(std::cout, SILENT);
    vout(NORMAL) << "Minimizing with DIFFERENTIAL EVOLUTION minimizer\n";
    vout(NORMAL) << "population_size = " << population_size << std::endl;
    vout(NORMAL) << "crossover_probability = " << crossover_probability << std::endl;
    vout(NORMAL) << "scaling_factor = " << scaling_factor << std::endl;

    // Initialize random numbers generator
    RandGen rng;

    std::vector<T> * current_best_pt, * old_best_pt;
    std::vector<T> best_old_pts_diff(xDim);
    T current_min, old_min, min_buf;

    // Initialize population
    std::unique_ptr<std::vector<std::vector<T>>> x(new std::vector<std::vector<T>>(population_size));
    for(auto &xi: (*x))
    {
        xi.resize(xDim);
        // Randomly pick each coordinate in a gaussian N(x0[k], e0[k])
        for(int k=0; k<xDim; ++k)
        {
            xi[k] = rng.getNormal(x0[k], e0[k]);
        }
    }
    // Get best individual
    current_min = F((*x)[0]);
    current_best_pt = &(*x)[0];
    for(int i=1; i<population_size; ++i)
    {
        min_buf = F((*x)[i]);
        if(min_buf < current_min)
        {
            current_min = min_buf;
            current_best_pt = &(*x)[i];
        }
    }
    old_min = current_min;
    old_best_pt = current_best_pt;

    // Evolution
    unsigned int iter = 0;
    bool converged = false;
    std::unique_ptr<std::vector<std::vector<T>>> xnew(new std::vector<std::vector<T>>(*x));

    while(iter < max_iter && !converged)
    {
        for(int i=0; i<population_size; ++i)
        {
            for(int k=0; k<xDim; ++k)
            {
                if(rng.getUniform(0., 1.) < crossover_probability)
                {
                    std::set<unsigned int> uvw = rng.getUniformIntSample(population_size-1, 3);
                    auto uvw_it = uvw.begin();
                    unsigned int u = (*uvw_it)==i? population_size-1: (*uvw_it);++uvw_it;
                    unsigned int v = (*uvw_it)==i? population_size-1: (*uvw_it);++uvw_it;
                    unsigned int w = (*uvw_it)==i? population_size-1: (*uvw_it);
                    (*xnew)[i][k] = (*x)[u][k] + scaling_factor*((*x)[v][k] - (*x)[w][k]);
                }
            }
        }
        for(int i=0; i<population_size; ++i)
        {
            if(F((*xnew)[i]) < F((*x)[i]))
                (*x)[i] = (*xnew)[i];
        }

        // Find best individual
        current_min = F((*x)[0]);
        current_best_pt = &(*x)[0];
        for(int i=1; i<population_size; ++i)
        {
            min_buf = F((*x)[i]);
            if(min_buf < current_min)
            {
                current_min = min_buf;
                current_best_pt = &(*x)[i];
            }
        }

        // Check for convergence
        for(int k=0; k<xDim; ++k)
            best_old_pts_diff[k] = (*current_best_pt)[k] - (*old_best_pt)[k];
        if(current_min - old_min < tolerance && sqrt(DotSquare(best_old_pts_diff)) < tolerance)
            converged = true;

        // If dither, randomly pick next generation scaling factor
        if(dither)
            scaling_factor = rng.getUniform(0.5, 1.);

        iter++;
    }

    typename Minimizer<T>::Result result;
    result.final_cost = current_min;
    result.is_valid = converged;
    result.minimum = *current_best_pt;

    return result;
}

END_NAMESPACE
END_NAMESPACE

#endif // DIFFERENTIAL_EVOLUTION_MINIMIZER_HPP
