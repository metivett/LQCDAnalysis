/*
 * Graph.hpp
 *
 *  Created on: May 06, 2014
 *      Author: Thibaut Metivet
 */

#ifndef GRAPH_HPP
#define GRAPH_HPP

 #include "Globals.hpp"
 #include "PlotObject.hpp"
 #include "PlotModifier.hpp"

 BEGIN_NAMESPACE(LQCDA)
 BEGIN_NAMESPACE(PLOT)

 class Graph
 {
 public:
 	struct RenderParameters
 	{
 		float Xmin, Xmax;
 		float Ymin, Ymax;
 	};

 private:
 	// Options
 	GraphOptions _Opts;
 	// Plot objects
 	std::vector<std::unique_ptr<PlotObject>> _Objects;
 	// Axis options
 	struct Axis
 	{
 		// Constructor
 		Axis(GraphOptions::AxisOptions& opts)
 		: _Opts(opts)
 		{}
 		// Options
 		GraphOptions::AxisOptions& _Opts;
 		// Modifier operator
 		Axis& operator<<(GraphModifier::AxisModifier&& m);
 	};

 public:
 	// Axes
 	Axis xAxis, yAxis;

 public:
 	// Constructor
 	Graph()
 	: _Opts()
 	, _Objects()
 	, xAxis(_Opts.x_axis_options)
 	, yAxis(_Opts.y_axis_options)
 	{}
 	Graph(Graph&& g)
 	: _Opts(std::move(g._Opts))
 	, _Objects(std::move(g._Objects))
 	, xAxis(_Opts.x_axis_options)
 	, yAxis(_Opts.y_axis_options)
 	{}
 	// Destructor
 	~Graph() = default;

 	// operators
 	template
 	<
 	class T,
 	class = typename std::enable_if<std::is_base_of<PlotObject, T>::value>::type
 	>
 	Graph& operator<<(T&& o);

 	// Access
 	const std::vector<std::unique_ptr<PlotObject>>& objects() const { return _Objects; }
 	PlotObject& S(unsigned int i);
 	const PlotObject& S(unsigned int i) const;
 	const GraphOptions& options() const;

 	// Modifier operator
 	Graph& operator<<(GraphModifier&& m);

 };

 template
 <
 class T,
 class
 >
 Graph& Graph::operator<<(T&& o)
 {
 	_Objects.push_back(utils::make_unique<T>(o));
 	return *this;
 }

 END_NAMESPACE
 END_NAMESPACE

#endif // GRAPH_HPP