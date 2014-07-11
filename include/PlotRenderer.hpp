/*
 * PlotRenderer.hpp
 *
 *  Created on: May 06, 2014
 *      Author: Thibaut Metivet
 */

#ifndef PLOT_RENDERER_HPP
#define PLOT_RENDERER_HPP

 #include "Globals.hpp"
 #include "PlotOptions.hpp"

 BEGIN_NAMESPACE(LQCDA)
 BEGIN_NAMESPACE(PLOT)

 class Plot;

 class Graph;

 class PlotFunction;
 class PlotData;
 class PlotXY;
 class PlotXYDX;
 class PlotXYDY;
 class PlotXYDXDY;
 class PlotXYDXDX;
 class PlotXYDYDY;
 class PlotXYDXDXDYDY;

 BEGIN_NAMESPACE(internal)

 class PlotRenderer
 {
 protected:
 	Plot * _Plot;

 public:
 	// Constructor
 	PlotRenderer(Plot * plot)
 	: _Plot(plot)
 	{}
 	// Destructor
 	virtual ~PlotRenderer() = default;

 	virtual void draw() =0;
 	virtual void render() =0;

 	virtual void render(const Graph& graph) =0;

 	// Visitor pattern visit
 	virtual void render(const PlotFunction&) =0;
 	virtual void render(const PlotData&) =0;
 	virtual void render(const PlotXY&) =0;
 	virtual void render(const PlotXYDX&) =0;
 	virtual void render(const PlotXYDY&) =0;
 	virtual void render(const PlotXYDXDY&) =0;
 	virtual void render(const PlotXYDXDX&) =0;
 	virtual void render(const PlotXYDYDY&) =0;
 	virtual void render(const PlotXYDXDXDYDY&) =0;

 private:
 	virtual void init(const Plot& p) =0;

 };

 class GracePlotRenderer
 : public PlotRenderer
 {
 private:
 	unsigned int _nGraphs;
 	// Current graph
 	unsigned int _CurrentGraph;
 	std::string _CurrentGraphStr;
 	// Sets per graph
 	std::vector<unsigned int> _nSets;


 public:
 	// Constructor
 	GracePlotRenderer(Plot * plot);
 	// Destructor
 	virtual ~GracePlotRenderer() noexcept;

 	virtual void draw() override;
 	virtual void render() override;

 	virtual void render(const Graph& graph) override;

 	// Visitor pattern visit
 	virtual void render(const PlotFunction&) override;
 	virtual void render(const PlotData&) override;
 	virtual void render(const PlotXY&) override;
 	virtual void render(const PlotXYDX&) override;
 	virtual void render(const PlotXYDY&) override;
 	virtual void render(const PlotXYDXDY&) override;
 	virtual void render(const PlotXYDXDX&) override;
 	virtual void render(const PlotXYDYDY&) override;
 	virtual void render(const PlotXYDXDXDYDY&) override;

 	void setCurrentGraph(unsigned int i);

 private:
 	virtual void init(const Plot& p) override;

 	virtual void setOptions(const PlotOptions& opts);
 	virtual void setOptions(const GraphOptions& opts, const std::string& graph);
 	virtual void setOptions(const SetOptions& opts, const std::string& graph, const std::string& set);
 	
 };

 END_NAMESPACE

 END_NAMESPACE
 END_NAMESPACE

#endif // PLOT_RENDERER_HPP