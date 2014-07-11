/*
 * GracePlotRenderer.cpp
 *
 *  Created on: May 07, 2014
 *      Author: Thibaut Metivet
 */

 #include <grace_np.h>

 #include "PlotRenderer.hpp"
 #include "Plot.hpp"
 #include "Utilities.hpp"
 #include "Exceptions.hpp"

 using namespace LQCDA;
 using namespace PLOT;
 using namespace internal;

 GracePlotRenderer::GracePlotRenderer(Plot * plot)
 : PlotRenderer(plot)
 {
 	// Start Grace with buffer size of 2048
 	GraceOpen(2048);
 }

 GracePlotRenderer::~GracePlotRenderer() noexcept
 {
 	GracePrintf("saveall \"test.agr\"");
 	GraceClose();
 }

 void GracePlotRenderer::draw()
 {
 	// Redraw
 	GracePrintf("redraw");
 }

 void GracePlotRenderer::render()
 {
 	init(*_Plot);
 	setOptions(_Plot->options());

 	const std::vector<Graph>& graphs = _Plot->graphs();
 	_nGraphs = graphs.size();

 	for(unsigned int gno = 0; gno < _nGraphs; ++gno)
 	{
 		setCurrentGraph(gno);
 		render(graphs[gno]);
 	}
 }

 void GracePlotRenderer::render(const Graph& graph)
 {
 	const std::vector<std::unique_ptr<PlotObject>>& gobj = graph.objects();

 	setOptions(graph.options(), _CurrentGraphStr);

 	for(unsigned int objno = 0; objno < gobj.size(); ++objno)
 	{
 		gobj[objno]->accept(*this);
 	}
 }

 void GracePlotRenderer::render(const PlotFunction& f)
 {
 	ERROR(IMPLEMENTATION, "Not implemented yet");
 }

 void GracePlotRenderer::render(const PlotData& d)
 {
 	ERROR(IMPLEMENTATION, "Not implemented yet");
 }

 void GracePlotRenderer::render(const PlotXY& d)
 {
 	const Array<double, Dynamic, 2>& pts = d.getPoints();

 	std::string CurrentSetStr = "s" + utils::strFrom(_nSets[_CurrentGraph]);
 	std::string CmdHead = _CurrentGraphStr + "." + CurrentSetStr + " ";
 	std::string cmd;

 	setOptions(d.options(), _CurrentGraphStr, CurrentSetStr);

 	cmd = CmdHead + " type xy";
 	GracePrintf(cmd.c_str());

 	for(index_t i = 0; i < pts.rows(); ++i)
 	{
 		cmd = CmdHead + " point %f, %f";
 		GracePrintf(cmd.c_str(), pts(i, 0), pts(i, 1));
 	}

 	_nSets[_CurrentGraph]++;
 }

 void GracePlotRenderer::render(const PlotXYDX& d)
 {
 	const Array<double, Dynamic, 3>& pts = d.getPoints();

 	std::string CurrentSetStr = "s" + utils::strFrom(_nSets[_CurrentGraph]);
 	std::string CmdHead = _CurrentGraphStr + "." + CurrentSetStr + " ";
 	std::string cmd;

 	setOptions(d.options(), _CurrentGraphStr, CurrentSetStr);

 	cmd = CmdHead + " type xydx";
 	GracePrintf(cmd.c_str());

 	for(index_t i = 0; i < pts.rows(); ++i)
 	{
 		cmd = CmdHead + " point %f, %f";
 		GracePrintf(cmd.c_str(), pts(i, 0), pts(i, 1));
 		cmd = CmdHead + ".Y1[" + CurrentSetStr + ".LENGTH-1]=%f";
 		GracePrintf(cmd.c_str(), pts(i, 2));
 	}

 	_nSets[_CurrentGraph]++;
 }

 void GracePlotRenderer::render(const PlotXYDY& d)
 {
 	const Array<double, Dynamic, 3>& pts = d.getPoints();

 	std::string CurrentSetStr = "s" + utils::strFrom(_nSets[_CurrentGraph]);
 	std::string CmdHead = _CurrentGraphStr + "." + CurrentSetStr;
 	std::string cmd;

 	setOptions(d.options(), _CurrentGraphStr, CurrentSetStr);

 	cmd = CmdHead + " type xydy";
 	GracePrintf(cmd.c_str());

 	for(index_t i = 0; i < pts.rows(); ++i)
 	{
 		cmd = CmdHead + " point %f, %f";
 		GracePrintf(cmd.c_str(), pts(i, 0), pts(i, 1));
 		cmd = CmdHead + ".Y1[" + CurrentSetStr + ".LENGTH-1]=%f";
 		GracePrintf(cmd.c_str(), pts(i, 2));
 	}

 	_nSets[_CurrentGraph]++;
 }

 void GracePlotRenderer::render(const PlotXYDXDY& d)
 {
 	const Array<double, Dynamic, 4>& pts = d.getPoints();

 	std::string CurrentSetStr = "s" + utils::strFrom(_nSets[_CurrentGraph]);
 	std::string CmdHead = _CurrentGraphStr + "." + CurrentSetStr + " ";
 	std::string cmd;

 	setOptions(d.options(), _CurrentGraphStr, CurrentSetStr);

 	cmd = CmdHead + " type xydxdy";
 	GracePrintf(cmd.c_str());

 	for(index_t i = 0; i < pts.rows(); ++i)
 	{
 		cmd = CmdHead + " point %f, %f";
 		GracePrintf(cmd.c_str(), pts(i, 0), pts(i, 1));
 		cmd = CmdHead + ".Y1[" + CurrentSetStr + ".LENGTH-1]=%f";
 		GracePrintf(cmd.c_str(), pts(i, 2));
 		cmd = CmdHead + ".Y2[" + CurrentSetStr + ".LENGTH-1]=%f";
 		GracePrintf(cmd.c_str(), pts(i, 3));
 	}

 	_nSets[_CurrentGraph]++;
 }

 void GracePlotRenderer::render(const PlotXYDXDX& d)
 {
 	const Array<double, Dynamic, 4>& pts = d.getPoints();

 	std::string CurrentSetStr = "s" + utils::strFrom(_nSets[_CurrentGraph]);
 	std::string CmdHead = _CurrentGraphStr + "." + CurrentSetStr + " ";
 	std::string cmd;

 	setOptions(d.options(), _CurrentGraphStr, CurrentSetStr);

 	cmd = CmdHead + " type xydxdx";
 	GracePrintf(cmd.c_str());

 	for(index_t i = 0; i < pts.rows(); ++i)
 	{
 		cmd = CmdHead + " point %f, %f";
 		GracePrintf(cmd.c_str(), pts(i, 0), pts(i, 1));
 		cmd = CmdHead + ".Y1[" + CurrentSetStr + ".LENGTH-1]=%f";
 		GracePrintf(cmd.c_str(), pts(i, 2));
 		cmd = CmdHead + ".Y2[" + CurrentSetStr + ".LENGTH-1]=%f";
 		GracePrintf(cmd.c_str(), pts(i, 3));
 	}

 	_nSets[_CurrentGraph]++;
 }

 void GracePlotRenderer::render(const PlotXYDYDY& d)
 {
 	const Array<double, Dynamic, 4>& pts = d.getPoints();

 	std::string CurrentSetStr = "s" + utils::strFrom(_nSets[_CurrentGraph]);
 	std::string CmdHead = _CurrentGraphStr + "." + CurrentSetStr + " ";
 	std::string cmd;

 	setOptions(d.options(), _CurrentGraphStr, CurrentSetStr);

 	cmd = CmdHead + " type xydydy";
 	GracePrintf(cmd.c_str());

 	for(index_t i = 0; i < pts.rows(); ++i)
 	{
 		cmd = CmdHead + " point %f, %f";
 		GracePrintf(cmd.c_str(), pts(i, 0), pts(i, 1));
 		cmd = CmdHead + ".Y1[" + CurrentSetStr + ".LENGTH-1]=%f";
 		GracePrintf(cmd.c_str(), pts(i, 2));
 		cmd = CmdHead + ".Y2[" + CurrentSetStr + ".LENGTH-1]=%f";
 		GracePrintf(cmd.c_str(), pts(i, 3));
 	}

 	_nSets[_CurrentGraph]++;
 }

 void GracePlotRenderer::render(const PlotXYDXDXDYDY& d)
 {
 	const Array<double, Dynamic, 6>& pts = d.getPoints();

 	std::string CurrentSetStr = "s" + utils::strFrom(_nSets[_CurrentGraph]);
 	std::string CmdHead = _CurrentGraphStr + "." + CurrentSetStr + " ";
 	std::string cmd;

 	setOptions(d.options(), _CurrentGraphStr, CurrentSetStr);

 	cmd = CmdHead + " type xydxdxdydy";
 	GracePrintf(cmd.c_str());

 	for(index_t i = 0; i < pts.rows(); ++i)
 	{
 		cmd = CmdHead + " point %f, %f";
 		GracePrintf(cmd.c_str(), pts(i, 0), pts(i, 1));
 		cmd = CmdHead + ".Y1[" + CurrentSetStr + ".LENGTH-1]=%f";
 		GracePrintf(cmd.c_str(), pts(i, 2));
 		cmd = CmdHead + ".Y2[" + CurrentSetStr + ".LENGTH-1]=%f";
 		GracePrintf(cmd.c_str(), pts(i, 3));
 		cmd = CmdHead + ".Y3[" + CurrentSetStr + ".LENGTH-1]=%f";
 		GracePrintf(cmd.c_str(), pts(i, 4));
 		cmd = CmdHead + ".Y4[" + CurrentSetStr + ".LENGTH-1]=%f";
 		GracePrintf(cmd.c_str(), pts(i, 5));
 	}

 	_nSets[_CurrentGraph]++;
 }

 void GracePlotRenderer::setCurrentGraph(unsigned int i)
 {
 	if(i >= _nGraphs)
 		ERROR(SIZE, "no graph " + utils::strFrom(i) + " registered");
 	_CurrentGraph = i;
 	_CurrentGraphStr = "g" + utils::strFrom(i);
 }

 void GracePlotRenderer::init(const Plot& p)
 {
 	_nGraphs = p.nGraphs();
 	_nSets.resize(_nGraphs, 0);
 	setCurrentGraph(0);
 }

 std::string grace_cmd(const ColorType& type)
 {
 	return utils::strFrom(static_cast<int>(type));
 }
 std::string grace_cmd(const SymbolType& type)
 {
 	return utils::strFrom(static_cast<int>(type));
 }
 std::string grace_cmd(const PatternType& type)
 {
 	return utils::strFrom(static_cast<int>(type));
 }
 std::string grace_cmd(const LineType& type)
 {
 	return utils::strFrom(static_cast<int>(type));
 }
 std::string grace_cmd(const LineStyle& type)
 {
 	return utils::strFrom(static_cast<int>(type));
 }
 std::string grace_cmd(const AxisLabelLayout& layout)
 {
 	std::string res;
 	switch(layout)
 	{
 		case AxisLabelLayout::parallel:
 		res = "para";
 		case AxisLabelLayout::perpendicular:
 		res = "perp";
 	}
 	return res;
 }

 void GracePlotRenderer::setOptions(const PlotOptions& opts)
 {
 	std::string cmd = "background color 0";// + opts.background_color;
 	GracePrintf(cmd.c_str());
 }

 void GracePlotRenderer::setOptions(const GraphOptions& opts, const std::string& graph)
 {
 	std::string cmd = "with " + graph;
 	GracePrintf(cmd.c_str());

 	cmd = "title \"" + opts.title + "\"";
 	GracePrintf(cmd.c_str());
 	cmd = "subtitle \"" + opts.subtitle + "\"";
 	GracePrintf(cmd.c_str());
 	cmd = "world xmin " + utils::strFrom(opts.x_axis_options.start);
 	GracePrintf(cmd.c_str());
 	cmd = "world xmax " + utils::strFrom(opts.x_axis_options.stop);
 	std::cout << opts.x_axis_options.stop << std::endl;
 	GracePrintf(cmd.c_str());
 	cmd = "world ymin " + utils::strFrom(opts.y_axis_options.start);
 	GracePrintf(cmd.c_str());
 	cmd = "world ymax " + utils::strFrom(opts.y_axis_options.stop);
 	GracePrintf(cmd.c_str());
 	cmd = "xaxis label \"" + opts.x_axis_options.label + "\"";
 	GracePrintf(cmd.c_str());
 	cmd = "yaxis label \"" + opts.y_axis_options.label + "\"";
 	GracePrintf(cmd.c_str());
 	cmd = "xaxis label layout " + grace_cmd(opts.x_axis_options.label_layout);
 	GracePrintf(cmd.c_str());
 	cmd = "yaxis label layout " + grace_cmd(opts.y_axis_options.label_layout);
 	GracePrintf(cmd.c_str());
 	cmd = "xaxis tick major " + utils::strFrom(opts.x_axis_options.tick_spacing);
 	GracePrintf(cmd.c_str());
 	cmd = "yaxis tick major " + utils::strFrom(opts.y_axis_options.tick_spacing);
 	GracePrintf(cmd.c_str());
 }

 void GracePlotRenderer::setOptions(const SetOptions& opts, const std::string& graph, const std::string& set)
 {
 	std::string g_s = graph + "." + set + " ";
 	std::string cmd;
 	cmd = g_s + "symbol " + grace_cmd(opts.symbol_options.symbol_type);
 	GracePrintf(cmd.c_str());
 	cmd = g_s + "symbol size " + utils::strFrom(opts.symbol_options.symbol_size);
 	GracePrintf(cmd.c_str());
 	cmd = g_s + "symbol color " + grace_cmd(opts.symbol_options.symbol_color);
 	GracePrintf(cmd.c_str());
 	cmd = g_s + "symbol pattern " + grace_cmd(opts.symbol_options.symbol_pattern);
 	GracePrintf(cmd.c_str());
 	cmd = g_s + "line type " + grace_cmd(opts.line_options.line_type);
 	GracePrintf(cmd.c_str());
 	cmd = g_s + "line linestyle " + grace_cmd(opts.line_options.line_style);
 	GracePrintf(cmd.c_str());
 	cmd = g_s + "line color " + grace_cmd(opts.line_options.line_color);
 	GracePrintf(cmd.c_str());
 	cmd = g_s + "line linewidth " + utils::strFrom(opts.line_options.line_width);
 	GracePrintf(cmd.c_str());
 }
