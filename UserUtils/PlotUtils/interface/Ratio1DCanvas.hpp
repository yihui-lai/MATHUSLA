/**
 * @file    Ratio1DCanvas.hpp
 * @brief   Canvas containing two Pad1Ds, with the top for main plotting
 *          while the bottom is designed for ratio plotting.
 * @author  [Yi-Mu "Enoch" Chen](https://github.com/yimuchen)
 * @details
 *
 */

#ifndef USERUTILS_PLOTUTILS_RATIO1DCANVAS_HPP
#define USERUTILS_PLOTUTILS_RATIO1DCANVAS_HPP

#ifdef CMSSW_GIT_HASH
#include "UserUtils/Common/interface/STLUtils/VectorUtils.hpp"
#include "UserUtils/PlotUtils/interface/Canvas.hpp"
#include "UserUtils/PlotUtils/interface/Pad1D.hpp"
#else
#include "UserUtils/Common/STLUtils/VectorUtils.hpp"
#include "UserUtils/PlotUtils/Canvas.hpp"
#include "UserUtils/PlotUtils/Pad1D.hpp"
#endif

#include <memory>
#include <vector>

namespace usr  {

namespace plt  {


/**
 * @brief Constructor container class for defining Pad dimension ratios
 */
struct PadRatio
{
  static float default_ratio;
  static float default_gap;
  PadRatio( const float _ratio = default_ratio,
            const float _gap   = default_gap ) :
    ratio( _ratio ),
    gap( _gap ){}
  const float ratio;
  const float gap;
};

/**
 * @brief Specialized Pad1D for additional axis/font settings.
 */
class Top1DPad : public Pad1D
{
public:
  // Constructors must be redeclared
  Top1DPad( const PadSize& x, const RangeByVar& y )
    : Pad1D( x, y ){}
  Top1DPad( const PadSize& x, const double y, const double z )
    : Pad1D( x, y, z ){}
  Top1DPad( const PadSize& x )
    : Pad1D( x ){}
  virtual
  ~Top1DPad (){}

  virtual void SetAxisFont();
};

/**
 * @brief Specialized Pad1D for additional axis/font settings.
 */
class Bottom1DPad : public Pad1D
{
public:
  // Constructors must be redeclared
  Bottom1DPad( const PadSize& x, const RangeByVar& y )
    : Pad1D( x, y ){}
  Bottom1DPad( const PadSize& x, const double y, const double z )
    : Pad1D( x, y, z ){}
  Bottom1DPad( const PadSize& x )
    : Pad1D( x ){}
  virtual
  ~Bottom1DPad(){}

  virtual void SetAxisFont();
};

/*-----------------------------------------------------------------------------
 *  Main class
   --------------------------------------------------------------------------*/
class Ratio1DCanvas : public Canvas
{
public:
  static TH1D* ScaleDivide(
    const TH1D*  num,
    const TH1D*  den,
    const double cen = 1. );

  static TH1D* ScaleDivide(
    const TH1D*   num,
    const TGraph* den,
    const double  cen         = 1.,
    const bool    extrapolate = false );

  static TGraphAsymmErrors* ScaleDivide(
    const TGraph* num,
    const TGraph* den,
    const double  cen         = 1.,
    const bool    extrapolate = false );

  static TGraphAsymmErrors* PullDivide(
    const TGraph* num,
    const TGraph* den,
    const double  cen = 0 );

  static length_t default_width;
  static length_t default_height;
  static FontSet default_font;

public:
  Ratio1DCanvas(
    const length_t width  = default_width,
    const length_t height = default_height,
    const PadRatio&       = PadRatio(),
    const FontSet&        = default_font
    );
  Ratio1DCanvas(
    const RangeByVar&,
    const length_t width  = default_width,
    const length_t height = default_height,
    const PadRatio&       = PadRatio(),
    const FontSet&        = default_font
    );

  virtual ~Ratio1DCanvas();

  /** @brief returning reference to top pad object */
  inline Top1DPad&
  TopPad(){ return GetPad<Top1DPad>( 0 ); }

  /** @brief returning reference to bottom pad object */
  inline Bottom1DPad&
  BottomPad(){ return GetPad<Bottom1DPad>( 1 ); }

  // Overloading the TopPad Pad1D functions to look like directly
  // 'plotting on this canvas' (Bottom canvases plot objects generated by the
  // static functions).
#define PASSTHROUGH_TOPPLOTFUNC( FUNC_NAME, PLOT_TYPE, RET_TYPE ) \
  template<typename ... Args>                                     \
  inline RET_TYPE& FUNC_NAME( PLOT_TYPE& obj, Args ... args )     \
  { return TopPad().FUNC_NAME( obj, args ... ); \
  }         \
  template<typename ... Args>                                     \
  inline RET_TYPE& FUNC_NAME( PLOT_TYPE* obj, Args ... args )     \
  { return TopPad().FUNC_NAME( obj, args ... );        }         \

  /**
   * @{
   * @brief 'plotting data on canvas' is actually plotting to Top pad on canvas.
   */
  PASSTHROUGH_TOPPLOTFUNC( PlotHist,  TH1D,       TH1D );
  PASSTHROUGH_TOPPLOTFUNC( PlotGraph, TGraph,     TGraph );
  PASSTHROUGH_TOPPLOTFUNC( PlotFunc,  TF1,        TGraph );
  PASSTHROUGH_TOPPLOTFUNC( PlotData,  RooAbsData, TGraphAsymmErrors );
  PASSTHROUGH_TOPPLOTFUNC( PlotPdf,   RooAbsPdf,  TGraph );
  /** @} */
#undef PASSTHROUGH_TOPPLOTFUNC

#define PASSTHOUGH_TOPVOIDFUNC( FUNC_NAME ) \
  template<typename ... Args>               \
  inline void FUNC_NAME( Args ... args )    \
  { TopPad().FUNC_NAME( args ... ); }

  /**
   * @{
   * @brief Common plot styling functions are also setting top pad properties
   *        only.
   */
  PASSTHOUGH_TOPVOIDFUNC( DrawCMSLabel );
  PASSTHOUGH_TOPVOIDFUNC( DrawLuminosity );
  PASSTHOUGH_TOPVOIDFUNC( SetLogy );
  /** @} */
#undef PASSTHOUGH_TOPVOIDFUNC

#define DIVIDE_FUNCTION( FUNC_NAME, RET_TYPE, NUM_TYPE, DEN_TYPE )           \
  RET_TYPE& FUNC_NAME(                                                       \
    const NUM_TYPE&, const DEN_TYPE&, const std::vector<RooCmdArg>& );      \
  inline NUM_TYPE& FUNC_NAME( const NUM_TYPE & num, const DEN_TYPE & den ){  \
    return FUNC_NAME( num, den, {} ); }                                      \
  template<typename ... Args>                                                \
  inline RET_TYPE&                                                           \
  FUNC_NAME( const NUM_TYPE & num, const DEN_TYPE & den,                     \
    const RooCmdArg & arg1, Args ... args )                                  \
  { return FUNC_NAME( num, den, MakeVector<RooCmdArg>( arg1, args ... ) ); } \
  inline RET_TYPE& FUNC_NAME( const NUM_TYPE* num, const DEN_TYPE & den ){   \
    return FUNC_NAME( *num, den, {} ); }                                     \
  template<typename ... Args>                                                \
  inline RET_TYPE&                                                           \
  FUNC_NAME( const NUM_TYPE* num, const DEN_TYPE & den,                      \
    const RooCmdArg & arg1, Args ... args )                                  \
  { return FUNC_NAME( *num, den, MakeVector<RooCmdArg>( arg1, args ... ) ); } \
  inline RET_TYPE& FUNC_NAME( const NUM_TYPE & num, const DEN_TYPE* den ){   \
    return FUNC_NAME( num, *den, {} ); }                                     \
  template<typename ... Args>                                                \
  inline RET_TYPE&                                                           \
  FUNC_NAME( const NUM_TYPE & num, const DEN_TYPE* den,                      \
    const RooCmdArg & arg1, Args ... args )                                  \
  { return FUNC_NAME( num, *den, MakeVector<RooCmdArg>( arg1, args ... ) ); } \
  inline RET_TYPE& FUNC_NAME( const NUM_TYPE* num, const DEN_TYPE* den ){    \
    return FUNC_NAME( *num, *den, {} ); }                                    \
  template<typename ... Args>                                                \
  inline RET_TYPE&                                                           \
  FUNC_NAME( const NUM_TYPE* num, const DEN_TYPE* den,                       \
    const RooCmdArg & arg1, Args ... args )                                  \
  { return FUNC_NAME( *num, *den, MakeVector<RooCmdArg>( arg1, args ... ) ); }

  /**
   * @{
   * @brief plotting the scale division of two histogram onto bottom pad,
   * returning reference to the newly constructed histogram.
   */
  DIVIDE_FUNCTION( PlotScale, TH1D, TH1D, TH1D );
  /** @} */

  /**
   * @{
   * @brief plotting the scale division of a histogram and a graph onto bottom
   * pad, returning reference to the newly constructed histogram.
   */
  DIVIDE_FUNCTION( PlotScale, TH1D, TH1D, TGraph );
  /** @} */

  /**
   * @{
   * @brief plotting the scale division of two graphs onto bottom pad,
   * returning reference to the newly constructed histogram.
   */
  DIVIDE_FUNCTION( PlotScale, TGraphAsymmErrors, TGraph, TGraph );
  /** @} */

  DIVIDE_FUNCTION( PlotPull,  TGraphAsymmErrors, TGraph, TGraph );
#undef DIVIDE_FUNCTION



  void SetTopMargin( const float x );
  void SetLeftMargin( const float x );
  void SetRightMargin( const float x );
  void SetBottomMargin( const float x );

protected:
  float _splitNDC( const float ratio ) const;
  void  _init_margin( const float gap );

  void MakeBottomAxis();
};

}/* plt  */

}/* usr  */

#endif/* end of include guard: USERUTILS_PLOTUTILS_RATIO1DCANVAS_HPP  */
