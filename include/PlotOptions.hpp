/*
 * PlotOptions.hpp
 *
 *  Created on: May 15, 2014
 *      Author: Thibaut Metivet
 */

#ifndef PLOT_OPTIONS_HPP
#define PLOT_OPTIONS_HPP

 #include "Globals.hpp"

 BEGIN_NAMESPACE(LQCDA)
 BEGIN_NAMESPACE(PLOT)

 enum class ColorType : int
 {
 	white = 0,
 	black,
 	red,
 	green,
 	blue,
 	yellow,
 	brown,
 	grey,
 	violet,
 	cyan,
 	magenta,
 	orange,
 	indigo,
 	maroon,
 	turquoise,
 	green4
 };

 enum class SymbolType
 {
 	none,
 	circle,
 	square,
 	diamond,
 	triangle_up,
 	triangle_left,
 	triangle_down,
 	triangle_right,
 	plus,
 	X,
 	star,
 	custom
 };

 enum class PatternType
 {
 	none,
 	plain
 };

 enum class LineType
 {
 	none,
 	straight,
 	left_stairs,
 	right_stairs,
 	segments
 };

 enum class LineStyle
 {
 	none,
 	plain,
 	dashes
 };

 enum class AxisLabelLayout
 {
 	parallel,
 	perpendicular
 };


 struct PlotOptions
 {
 	ColorType background_color {ColorType::white};
 };

 struct GraphOptions
 {
 	std::string title {};
 	std::string subtitle {};
 	bool autoscale {true};
 	// double xmin {}, xmax {}, ymin {}, ymax {};
 	// Axis
 	struct AxisOptions
 	{
 		// label
 		std::string label {};
 		AxisLabelLayout label_layout {AxisLabelLayout::parallel};
 		// range
 		float start {0.}, stop {1.};
 		// ticks
 		float tick_spacing {0.2};
 	} x_axis_options, y_axis_options;
 };

 struct SetOptions
 {
 	// symbol
 	struct SymbolOptions
 	{
 		SymbolType symbol_type {SymbolType::none};
 		float symbol_size {0.5};
 		ColorType symbol_color {ColorType::white};
 		PatternType symbol_pattern {PatternType::none};
 	} symbol_options;
 	// line style
 	struct LineOptions
 	{
 		LineType line_type {LineType::none};
 		LineStyle line_style {LineStyle::none};
 		ColorType line_color {ColorType::white};
 		float line_width {1.0};
 	} line_options;
 	// legend
 	std::string legend {};
 };

 END_NAMESPACE
 END_NAMESPACE

#endif // PLOT_OPTIONS_HPP
