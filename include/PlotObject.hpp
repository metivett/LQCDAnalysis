/*
 * PlotObject.hpp
 *
 *  Created on: May 06, 2014
 *      Author: Thibaut Metivet
 */

#ifndef PLOT_OBJECT_HPP
#define PLOT_OBJECT_HPP

 #include "Globals.hpp"
 #include "PlotRenderer.hpp"
 #include "PlotModifier.hpp"
 #include "Function.hpp"
 #include "XYData.hpp"

 BEGIN_NAMESPACE(LQCDA)
 BEGIN_NAMESPACE(PLOT)

 class PlotObject
 {
 private:
 	// Options
 	SetOptions _Opts;
 	// Line
 	class Line
 	{
 	private:
 		SetOptions::LineOptions& _Opts;
 		
 	public:
 		Line(SetOptions::LineOptions& opts)
 		: _Opts(opts)
 		{}
 		Line& operator<<(SetModifier::LineModifier&& m)
 		{
 			m(_Opts);
 			return *this;
 		}
 	};
 	friend struct Line;
 	// Symbol
 	class Symbol
 	{
 	private:
 		SetOptions::SymbolOptions& _Opts;
 	public:
 		Symbol(SetOptions::SymbolOptions& opts)
 		: _Opts(opts)
 		{}
 		Symbol& operator<<(SetModifier::SymbolModifier&& m)
 		{
 			m(_Opts);
 			return *this;
 		}
 	};
 	friend struct Symbol;

 public:
 	// Plot object type
 	enum class Type
 	{
 		Function,
 		Data,
 		XY,
 		XYDX,
 		XYDY,
 		XYDXDY,
 		XYDXDX,
 		XYDYDY,
 		XYDXDXDYDY
 	};
 	// Line
 	Line line;
 	// Symbol
 	Symbol symbol;

 public:
 	// Constructor
 	PlotObject()
 	: line(_Opts.line_options)
 	, symbol(_Opts.symbol_options)
 	{}
 	// Destructor
 	virtual ~PlotObject() = default;

 	virtual Type getType() const =0;
 	const SetOptions& options() const
 	{
 		return _Opts;
 	}

 	// Visitor pattern accept
 	virtual void accept(internal::PlotRenderer& renderer) =0;

 	// Modifier operator
 	PlotObject& operator<<(SetModifier&& m)
 	{
 		m(_Opts);
 		return *this;
 	}
 };

 class PlotFunction
 : public PlotObject
 {
 private:
 	typedef ScalarFunction<double> fcn_type;
 	const fcn_type& _F;
 	double _xMin, _xMax;

 public:
 	// Constructor
 	PlotFunction(const fcn_type& f, double xmin, double xmax)
 	: _F(f)
 	, _xMin{xmin}
 	, _xMax{xmax}
 	{}
 	// Destructor
 	virtual ~PlotFunction() noexcept = default;

 	virtual Type getType() const override { return Type::Function; }

 	// Visitor pattern accept
 	virtual void accept(internal::PlotRenderer& renderer) override
 	{
 		renderer.render(*this);
 	}

 	// Access
 	const fcn_type& getFunction() const { return _F; }
 	double getXmin() const { return _xMin; }
 	double getXmax() const { return _xMax; }
 };

 class PlotData
 : public PlotObject
 {
 private:
 	typedef XYData<double> data_type;
 	data_type _D;

 public:
 	// Constructor
 	PlotData(const data_type& d)
 	: _D(d)
 	{}
 	// Destructor
 	virtual ~PlotData() noexcept {}

 	virtual Type getType() const override { return Type::Data; }

 	// Visitor pattern accept
 	virtual void accept(internal::PlotRenderer& renderer) override
 	{
 		renderer.render(*this);
 	}

 	// Access
 	const data_type& getData() const { return _D; }
 };

 class PlotXY
 : public PlotObject
 {
 private:
 	typedef Array<double, Dynamic, 2> pts_type;
 	// Points
 	pts_type _Pts;

 public:
 	// Constructor
 	PlotXY(const pts_type& pts)
 	: _Pts(pts)
 	{}
 	// Destructor
 	virtual ~PlotXY() noexcept {}

 	virtual Type getType() const override { return Type::XY; }

 	// Visitor pattern accept
 	virtual void accept(internal::PlotRenderer& renderer) override
 	{
 		renderer.render(*this);
 	}

 	// Access
 	const pts_type& getPoints() const { return _Pts; }
 };

 class PlotXYDX
 : public PlotObject
 {
 private:
 	typedef Array<double, Dynamic, 3> pts_type;
 	// Points
 	pts_type _Pts;

 public:
 	// Constructor
 	PlotXYDX(const pts_type& pts)
 	: _Pts(pts)
 	{}
 	// Destructor
 	virtual ~PlotXYDX() noexcept {}

 	virtual Type getType() const override { return Type::XYDX; }

 	// Visitor pattern accept
 	virtual void accept(internal::PlotRenderer& renderer) override
 	{
 		renderer.render(*this);
 	}

 	// Access
 	const pts_type& getPoints() const { return _Pts; }
 };

 class PlotXYDY
 : public PlotObject
 {
 private:
 	typedef Array<double, Dynamic, 3> pts_type;
 	// Points
 	pts_type _Pts;

 public:
 	// Constructor
 	PlotXYDY(const pts_type& pts)
 	: _Pts(pts)
 	{}
 	// Destructor
 	virtual ~PlotXYDY() noexcept {}

 	virtual Type getType() const override { return Type::XYDY; }

 	// Visitor pattern accept
 	virtual void accept(internal::PlotRenderer& renderer) override
 	{
 		renderer.render(*this);
 	}

 	// Access
 	const pts_type& getPoints() const { return _Pts; }
 };

 class PlotXYDXDY
 : public PlotObject
 {
 private:
 	typedef Array<double, Dynamic, 4> pts_type;
 	// Points
 	pts_type _Pts;

 public:
 	// Constructor
 	PlotXYDXDY(const pts_type& pts)
 	: _Pts(pts)
 	{}
 	// Destructor
 	virtual ~PlotXYDXDY() noexcept {}

 	virtual Type getType() const override { return Type::XYDXDY; }

 	// Visitor pattern accept
 	virtual void accept(internal::PlotRenderer& renderer) override
 	{
 		renderer.render(*this);
 	}

 	// Access
 	const pts_type& getPoints() const { return _Pts; }
 };

 class PlotXYDXDX
 : public PlotObject
 {
 private:
 	typedef Array<double, Dynamic, 4> pts_type;
 	// Points
 	pts_type _Pts;

 public:
 	// Constructor
 	PlotXYDXDX(const pts_type& pts)
 	: _Pts(pts)
 	{}
 	// Destructor
 	virtual ~PlotXYDXDX() noexcept {}

 	virtual Type getType() const override { return Type::XYDXDX; }

 	// Visitor pattern accept
 	virtual void accept(internal::PlotRenderer& renderer) override
 	{
 		renderer.render(*this);
 	}

 	// Access
 	const pts_type& getPoints() const { return _Pts; }
 };

 class PlotXYDYDY
 : public PlotObject
 {
 private:
 	typedef Array<double, Dynamic, 4> pts_type;
 	// Points
 	pts_type _Pts;

 public:
 	// Constructor
 	PlotXYDYDY(const pts_type& pts)
 	: _Pts(pts)
 	{}
 	// Destructor
 	virtual ~PlotXYDYDY() noexcept {}

 	virtual Type getType() const override { return Type::XYDYDY; }

 	// Visitor pattern accept
 	virtual void accept(internal::PlotRenderer& renderer) override
 	{
 		renderer.render(*this);
 	}

 	// Access
 	const pts_type& getPoints() const { return _Pts; }
 };

 class PlotXYDXDXDYDY
 : public PlotObject
 {
 private:
 	typedef Array<double, Dynamic, 6> pts_type;
 	// Points
 	pts_type _Pts;

 public:
 	// Constructor
 	PlotXYDXDXDYDY(const pts_type& pts)
 	: _Pts(pts)
 	{}
 	// Destructor
 	virtual ~PlotXYDXDXDYDY() noexcept {}

 	virtual Type getType() const override { return Type::XYDXDXDYDY; }

 	// Visitor pattern accept
 	virtual void accept(internal::PlotRenderer& renderer) override
 	{
 		renderer.render(*this);
 	}

 	// Access
 	const pts_type& getPoints() const { return _Pts; }
 };

 END_NAMESPACE
 END_NAMESPACE

#endif // PLOT_OBJECT_HPP