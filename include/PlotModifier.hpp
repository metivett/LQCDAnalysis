/*
 * PlotModifier.hpp
 *
 *  Created on: May 15, 2014
 *      Author: Thibaut Metivet
 */

#ifndef PLOT_MODIFIER_HPP
#define PLOT_MODIFIER_HPP

 #include "Globals.hpp"
 #include "PlotOptions.hpp"

 BEGIN_NAMESPACE(LQCDA)
 BEGIN_NAMESPACE(PLOT)

 class PlotModifier
 {
 public:
 	virtual ~PlotModifier() noexcept = default;

 	virtual void operator()(PlotOptions& opts) const =0;
 };

 class GraphModifier
 {
 public:
 	class AxisModifier;

 public:
 	virtual ~GraphModifier() noexcept = default;

 	virtual void operator()(GraphOptions& opts) const =0;
 };

 class SetModifier
 {
 public:
 	class SymbolModifier;
 	class LineModifier;

 public:
 	virtual ~SetModifier() noexcept = default;

 	virtual void operator()(SetOptions& opts) const =0;
 };

 class SetModifier::SymbolModifier
 : public virtual SetModifier
 {
 public:
 	virtual ~SymbolModifier() noexcept = default;

 	virtual void operator()(SetOptions& opts) const
 	{
 		this->operator()(opts.symbol_options);
 	}
 	virtual void operator()(SetOptions::SymbolOptions& opts) const =0;
 };

 class SetModifier::LineModifier
 : public virtual SetModifier
 {
 public:
 	virtual ~LineModifier() noexcept = default;

 	virtual void operator()(SetOptions& opts) const
 	{
 		this->operator()(opts.line_options);
 	}
 	virtual void operator()(SetOptions::LineOptions& opts) const =0;
 };

 class GraphModifier::AxisModifier
 {
 public:
 	virtual ~AxisModifier() noexcept = default;

 	virtual void operator()(GraphOptions::AxisOptions& opts) const =0;
 };

 class Color
 : public virtual PlotModifier
 , public virtual SetModifier::SymbolModifier
 , public virtual SetModifier::LineModifier
 {
 private:
 	ColorType _Color;

 public:
 	// Constructor
 	Color(ColorType color);
 	// Destructor
 	virtual ~Color() noexcept = default;

 	// Conversion operator
 	operator ColorType() const;

 	// Modifiers
 	virtual void operator()(PlotOptions& opts) const override;
 	virtual void operator()(SetOptions& opts) const override;
 	virtual void operator()(SetOptions::SymbolOptions& opts) const override;
 	virtual void operator()(SetOptions::LineOptions& opts) const override;

 private:
 	template<typename T>
 	operator T() const;
 };

 class Title
 : public GraphModifier
 {
 private:
 	std::string _Title;

 public:
 	// Constructor
 	explicit Title(const std::string& title);
 	// Destructor
 	virtual ~Title() noexcept = default;

 	// Modifiers
 	virtual void operator()(GraphOptions& opts) const override;
 };

 class Subtitle
 : public GraphModifier
 {
 private:
 	std::string _Subtitle;

 public:
 	// Constructor
 	explicit Subtitle(const std::string& subtitle);
 	// Destructor
 	virtual ~Subtitle() noexcept = default;

 	// Modifiers
 	virtual void operator()(GraphOptions& opts) const override;
 };

 class Autoscale
 : public GraphModifier
 {
 private:
 	bool _Autoscale;

 public:
 	// Constructor
 	explicit Autoscale(bool autoscale);
 	// Destructor
 	virtual ~Autoscale() noexcept = default;

 	// Modifiers
 	virtual void operator()(GraphOptions& opts) const override;
 };

 class Legend
 : public SetModifier
 {
 private:
 	std::string _Legend;

 public:
 	// Constructor
 	explicit Legend(const std::string& legend);
 	// Destructor
 	virtual ~Legend() noexcept = default;

 	// Modifiers
 	virtual void operator()(SetOptions& opts) const override;
 };

 class Symbol
 : public virtual SetModifier
 , public virtual SetModifier::SymbolModifier
 {
 private:
 	SymbolType _Symbol;

 public:
 	// Constructor
 	Symbol(SymbolType symbol);
 	// Destructor
 	virtual ~Symbol() noexcept = default;

 	// Modifiers
 	virtual void operator()(SetOptions::SymbolOptions& opts) const override;
 };

 class Size
 : public virtual SetModifier::SymbolModifier
 {
 private:
 	int _Size;

 public:
 	// Constructor
 	Size(int size);
 	// Destructor
 	virtual ~Size() noexcept = default;

 	// Modifiers
 	virtual void operator()(SetOptions::SymbolOptions& opts) const override;
 };

 class Pattern
 : public virtual SetModifier::SymbolModifier
 {
 private:
 	PatternType _Pattern;
 public:
 	// Constructor
 	Pattern(PatternType pattern);
 	// Destructor
 	virtual ~Pattern() noexcept = default;

 	// Modifiers
 	virtual void operator()(SetOptions::SymbolOptions& opts) const override;
 };

 class Line
 : public virtual SetModifier::LineModifier
 {
 private:
 	LineStyle _Style;
 	LineType _Type;
 public:
 	// Constructor
 	Line(LineStyle style, LineType type = LineType::straight);
 	// Destructor
 	virtual ~Line() noexcept = default;

 	// Modifiers
 	virtual void operator()(SetOptions::LineOptions& opts) const override;
 };

 class Width
 : public virtual SetModifier::LineModifier
 {
 private:
 	float _Width;

 public:
 	// Constructor
 	Width(float width);
 	// Destructor
 	virtual ~Width() noexcept = default;

 	// Modifiers
 	void operator()(SetOptions::LineOptions& opts) const override;
 };

 class Label
 : public virtual GraphModifier::AxisModifier
 {
 private:
 	std::string _Label;

 public:
 	// Constructor
 	Label(const std::string& label);
 	// Destructor
 	virtual ~Label() noexcept = default;

 	// Modifiers
 	void operator()(GraphOptions::AxisOptions& opts) const override;
 };

 class LabelLayout
 : public virtual GraphModifier::AxisModifier
 {
 private:
 	AxisLabelLayout _LabelLayout;

 public:
 	// Constructor
 	LabelLayout(AxisLabelLayout layout);
 	// Destructor
 	virtual ~LabelLayout() noexcept = default;

 	// Modifiers
 	void operator()(GraphOptions::AxisOptions& opts) const override;
 };

 class Range
 : public virtual GraphModifier::AxisModifier
 {
 private:
 	float _Start, _Stop;

 public:
 	// Constructor
 	Range(float start, float stop);
 	// Destructor
 	virtual ~Range() noexcept = default;

 	// Modifiers
 	void operator()(GraphOptions::AxisOptions& opts) const override;
 };

 class TickSpacing
 : public virtual GraphModifier::AxisModifier
 {
 private:
 	float _Spacing;

 public:
 	// Constructor
 	TickSpacing(float spacing);
 	// Destructor
 	virtual ~TickSpacing() noexcept = default;

 	// Modifiers
 	void operator()(GraphOptions::AxisOptions& opts) const override;
 };


 END_NAMESPACE
 END_NAMESPACE

#endif // PLOT_MODIFIER_HPP
