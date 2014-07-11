/*
 * Plot.hpp
 *
 *  Created on: May 06, 2014
 *      Author: Thibaut Metivet
 */

#ifndef PLOT_HPP
#define PLOT_HPP

 #include "Globals.hpp"
 #include "Graph.hpp"
 #include "PlotOptions.hpp"
 #include "PlotModifier.hpp"

 BEGIN_NAMESPACE(LQCDA)
 BEGIN_NAMESPACE(PLOT)

 class Plot
 {
 private:
 	// Plot options
 	PlotOptions _Opts;
 	// Graphs
 	std::vector<Graph> _Graphs;
 	std::vector<std::string> _GraphLabels;
 	// Plot renderer
 	std::unique_ptr<internal::PlotRenderer> _Renderer;

 protected:
 	// Constructor
 	Plot(internal::PlotRenderer* renderer, unsigned int nGraphs = 1);

 public:
 	// Destructor
 	virtual ~Plot() = default;

 	// Display
 	void display();

 	// Access
 	unsigned int nGraphs() const;
 	const std::vector<Graph>& graphs() const;
 	const PlotOptions& options() const;

 	void addGraph();
 	void addGraph(const std::string& label);

 	Graph& G(const unsigned int i);
 	const Graph& G(const unsigned int i) const;
 	Graph& G(const std::string& label);
 	const Graph& G(const std::string& label) const;

 	// Modifier operator
 	Plot& operator<<(PlotModifier&& m);

 	// Open/Save
 	// TODO

 };

 END_NAMESPACE
 END_NAMESPACE

#endif // PLOT_HPP