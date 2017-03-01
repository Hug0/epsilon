#ifndef SHARED_CURVE_VIEW_H
#define SHARED_CURVE_VIEW_H

#include <escher.h>
#include <poincare.h>
#include "curve_view_range.h"
#include "curve_view_cursor.h"
#include "banner_view.h"
#include <math.h>

namespace Shared {

class CurveView : public View {
public:
  typedef void Model;
  enum class Axis {
    Horizontal = 0,
    Vertical = 1
  };
  CurveView(CurveViewRange * curveViewRange = nullptr, CurveViewCursor * curveViewCursor = nullptr,
    BannerView * bannerView = nullptr, View * cursorView = nullptr, View * okView = nullptr);
  virtual void reload();
  // When the main view is selected, the banner view is visible
  bool isMainViewSelected() const;
  void selectMainView(bool mainViewSelected);
  void setCursorView(View * cursorView);
  void setBannerView(BannerView * bannerView);
  void setOkView(View * okView);
protected:
  void setCurveViewRange(CurveViewRange * curveViewRange);
  // Drawing methods
  constexpr static KDCoordinate k_labelMargin =  4;
  constexpr static KDCoordinate k_okMargin =  10;
  constexpr static KDCoordinate k_labelGraduationLength =  6;
  constexpr static int k_maxNumberOfXLabels = CurveViewRange::k_maxNumberOfXGridUnits;
  constexpr static int k_maxNumberOfYLabels =  CurveViewRange::k_maxNumberOfYGridUnits;
  constexpr static KDCoordinate k_cursorSize = 25;
  constexpr static int k_externRectMargin = 1;
  float pixelToFloat(Axis axis, KDCoordinate p) const;
  float floatToPixel(Axis axis, float f) const;
  void drawLine(KDContext * ctx, KDRect rect, Axis axis,
      float coordinate, KDColor color, KDCoordinate thickness = 1) const;
  void drawSegment(KDContext * ctx, KDRect rect, Axis axis,
      float coordinate, float lowerBound, float upperBound,
      KDColor color, KDCoordinate thickness = 1) const;
  void drawDot(KDContext * ctx, KDRect rect, float x, float y, KDColor color) const;
  void drawGridLines(KDContext * ctx, KDRect rect, Axis axis, float step, KDColor color) const;
  void drawGrid(KDContext * ctx, KDRect rect) const;
  void drawAxes(KDContext * ctx, KDRect rect, Axis axis) const;
  void drawCurve(KDContext * ctx, KDRect rect, Model * curve, KDColor color, bool colorUnderCurve = false, float colorLowerBound = 0.0f, float colorUpperBound = 0.0f, bool continuously = false) const;
  void drawHistogram(KDContext * ctx, KDRect rect, Model * model,  float firstBarAbscissa, float barWidth,
    bool fillBar, KDColor defaultColor, KDColor highlightColor,  float highlightLowerBound = INFINITY, float highlightUpperBound = -INFINITY) const;
  void computeLabels(Axis axis);
  void drawLabels(KDContext * ctx, KDRect rect, Axis axis, bool shiftOrigin) const;
  virtual KDSize cursorSize();
private:
  /* The window bounds are deduced from the model bounds but also take into
  account a margin (computed with k_marginFactor) */
  float min(Axis axis) const;
  float max(Axis axis) const;
  float gridUnit(Axis axis) const;
  KDCoordinate pixelLength(Axis axis) const;
  virtual char * label(Axis axis, int index) const = 0;
  int numberOfLabels(Axis axis) const;
  virtual float evaluateModelWithParameter(Model * curve, float t) const;
  /* Recursively join two dots (dichotomy). The method stops when the
   * maxNumberOfRecursion in reached. */
  void jointDots(KDContext * ctx, KDRect rect, Model * curve, float x, float y, float u, float v, KDColor color, int maxNumberOfRecursion) const;
  /* Join two dots with a straight line. */
  void straightJoinDots(KDContext * ctx, KDRect rect, float pxf, float pyf, float puf, float pvf, KDColor color) const;
  /* Stamp centered around (pxf, pyf). If pxf and pyf are not round number, the
   * function shifts the stamp (by blending adjacent pixel colors) to draw with
   * anti alising. */
  void stampAtLocation(KDContext * ctx, KDRect rect, float pxf, float pyf, KDColor color) const;
  void layoutSubviews() override;
  int numberOfSubviews() const override;
  View * subviewAtIndex(int index) override;
  /* m_curveViewRange has to be non null but the cursor model, the banner and
   * cursor views may be nullptr if not needed. */
  CurveViewRange * m_curveViewRange;
  CurveViewCursor * m_curveViewCursor;
  BannerView * m_bannerView;
  View * m_cursorView;
  View * m_okView;
  bool m_mainViewSelected;
  uint32_t m_drawnRangeVersion;
};

}

#endif