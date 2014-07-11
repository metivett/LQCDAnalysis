/*
 * PlotModifier.cpp
 *
 *  Created on: May 16, 2014
 *      Author: Thibaut Metivet
 */

 #include "PlotModifier.hpp"

 using namespace LQCDA;
 using namespace PLOT;

/******************************************************************************
 *                                   Color                                    *
 ******************************************************************************/

 Color::Color(ColorType color)
 : _Color{color}
 {}

 Color::operator ColorType() const
 {
 	return _Color;
 }

 void Color::operator()(PlotOptions& opts) const
 {
 	opts.background_color = _Color;
 }

 void Color::operator()(SetOptions& opts) const
 {
 	opts.symbol_options.symbol_color = _Color;
 	opts.line_options.line_color = _Color;
 }

 void Color::operator()(SetOptions::SymbolOptions& opts) const
 {
 	opts.symbol_color = _Color;
 }

 void Color::operator()(SetOptions::LineOptions& opts) const
 {
 	opts.line_color = _Color;
 }

/******************************************************************************
 *                                   Title                                    *
 ******************************************************************************/

 Title::Title(const std::string& title)
 : _Title{title}
 {}

 void Title::operator()(GraphOptions& opts) const
 {
 	opts.title = _Title;
 }

/******************************************************************************
 *                                  Subtitle                                  *
 ******************************************************************************/

 Subtitle::Subtitle(const std::string& subtitle)
 : _Subtitle{subtitle}
 {}

 void Subtitle::operator()(GraphOptions& opts) const
 {
 	opts.subtitle = _Subtitle;
 }

/******************************************************************************
 *                                 Autoscale                                  *
 ******************************************************************************/

 Autoscale::Autoscale(bool autoscale)
 : _Autoscale{autoscale}
 {}

 void Autoscale::operator()(GraphOptions& opts) const
 {
 	opts.autoscale = _Autoscale;
 }

/******************************************************************************
 *                                   Legend                                   *
 ******************************************************************************/

 Legend::Legend(const std::string& legend)
 : _Legend{legend}
 {}

 void Legend::operator()(SetOptions& opts) const
 {
 	opts.legend = _Legend;
 }

/******************************************************************************
 *                                   Symbol                                   *
 ******************************************************************************/

 Symbol::Symbol(SymbolType symbol)
 : _Symbol{symbol}
 {}

 void Symbol::operator()(SetOptions::SymbolOptions& opts) const
 {
 	opts.symbol_type = _Symbol;
 }

/******************************************************************************
 *                                    Size                                    *
 ******************************************************************************/

 Size::Size(int size)
 : _Size(size)
 {}

 void Size::operator()(SetOptions::SymbolOptions& opts) const
 {
 	opts.symbol_size = _Size;
 }

/******************************************************************************
 *                                  Pattern                                   *
 ******************************************************************************/

 Pattern::Pattern(PatternType pattern)
 : _Pattern(pattern)
 {}

 void Pattern::operator()(SetOptions::SymbolOptions& opts) const
 {
 	opts.symbol_pattern = _Pattern;
 }

/******************************************************************************
 *                                    Line                                    *
 ******************************************************************************/

 Line::Line(LineStyle style, LineType type)
 : _Style(style)
 , _Type(type)
 {}

 void Line::operator()(SetOptions::LineOptions& opts) const
 {
 	opts.line_style = _Style;
 	opts.line_type = _Type;
 }

/******************************************************************************
 *                                   Width                                    *
 ******************************************************************************/

 Width::Width(float width)
 : _Width(width)
 {}

 void Width::operator()(SetOptions::LineOptions& opts) const
 {
 	opts.line_width = _Width;
 }

/******************************************************************************
 *                                   Label                                    *
 ******************************************************************************/

 Label::Label(const std::string& label)
 : _Label{label}
 {}

 void Label::operator()(GraphOptions::AxisOptions& opts) const
 {
 	opts.label = _Label;
 }

 /******************************************************************************
 *                                LabelLayout                                 *
 ******************************************************************************/

 LabelLayout::LabelLayout(AxisLabelLayout layout)
 : _LabelLayout{layout}
 {}

 void LabelLayout::operator()(GraphOptions::AxisOptions& opts) const
 {
 	opts.label_layout = _LabelLayout;
 }

/******************************************************************************
 *                                   Range                                    *
 ******************************************************************************/

 Range::Range(float start, float stop)
 : _Start(start)
 , _Stop(stop)
 {}

 void Range::operator()(GraphOptions::AxisOptions& opts) const
 {
 	opts.start = _Start;
 	opts.stop = _Stop;
 }

/******************************************************************************
 *                                TickSpacing                                 *
 ******************************************************************************/

 TickSpacing::TickSpacing(float spacing)
 : _Spacing(spacing)
 {}

 void TickSpacing::operator()(GraphOptions::AxisOptions& opts) const
 {
 	opts.tick_spacing = _Spacing;
 }