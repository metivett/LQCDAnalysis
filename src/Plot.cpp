/*
 * Plot.cpp
 *
 *  Created on: May 07, 2014
 *      Author: Thibaut Metivet
 */

 #include "Plot.hpp"
 #include "Exceptions.hpp"

 using namespace LQCDA;
 using namespace PLOT;
 using namespace internal;

 Plot::Plot(PlotRenderer* renderer, unsigned int nGraphs)
 : _Graphs(nGraphs)
 , _Renderer(renderer)
 {}

 void Plot::display()
 {
 	// g.display(_Renderer, Graph::RenderParameters{0.15, 0.15, 1.15, 1.15});
 	_Renderer->render();
 	_Renderer->draw();
 }

 unsigned int Plot::nGraphs() const 
 {
 	return _Graphs.size();
 }

 const std::vector<Graph>& Plot::graphs() const
 {
 	return _Graphs;
 }

 const PlotOptions& Plot::options() const
 {
 	return _Opts;
 }

 void Plot::addGraph()
 {
 	_GraphLabels.emplace_back("G" + utils::strFrom(_Graphs.size()));
 	_Graphs.emplace_back();
 }

 void Plot::addGraph(const std::string& label)
 {
 	_GraphLabels.emplace_back(label);
 	_Graphs.emplace_back();
 }

 Graph& Plot::G(unsigned int i)
 {
 	if(i >= _Graphs.size())
 		ERROR(SIZE, "no graph " + utils::strFrom(i) + " in the plot");

 	return _Graphs[i];
 }

 const Graph& Plot::G(unsigned int i) const
 {
 	if(i >= _Graphs.size())
 		ERROR(SIZE, "no graph " + utils::strFrom(i) + " in the plot");
 	
 	return _Graphs[i];
 }

 Graph& Plot::G(const std::string& label)
 {
 	unsigned int i = distance(find(_GraphLabels.begin(), _GraphLabels.end(), label), _GraphLabels.begin());
 	if(i >= _Graphs.size())
 		ERROR(SIZE, "no graph with label " + label + " in the plot");

 	return _Graphs[i];
 }

 const Graph& Plot::G(const std::string& label) const
 {
 	unsigned int i = distance(find(_GraphLabels.begin(), _GraphLabels.end(), label), _GraphLabels.begin());
 	if(i >= _Graphs.size())
 		ERROR(SIZE, "no graph with label " + label + " in the plot");

 	return _Graphs[i];
 }

 Plot& Plot::operator<<(PlotModifier&& m)
 {
 	m(_Opts);
 	return *this;
 }