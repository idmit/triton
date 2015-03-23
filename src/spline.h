#ifndef SPLINE_H
#define SPLINE_H

#include <QPainter>
#include <QPointF>

class Spline {
public:
  explicit Spline();

  size_t addPoint(size_t idx, QPointF &point, bool rebuild = true);
  size_t addPoint(size_t idx, double x, double y);
  size_t addPointTo(size_t idx, QPointF &point, bool rebuild = true);
  QPointF atSup(size_t idx);
  QPointF &atVal(size_t idx);
  void build(size_t approxParam = approx);
  void removeAt(size_t idx, bool rebuild = true);
  void resetPoint(size_t idx, QPointF point);
  void resetPointX(size_t idx, double x);
  void resetPointY(size_t idx, double y);
  size_t supSize();
  size_t valSize();

private:
  class OrderedDoubleSet : public QList<double> {
  public:
    size_t append(double point);
    size_t insert(double point);
  };

  using QPointFList = QList<QPointF>;

  static constexpr size_t approx = 100;
  OrderedDoubleSet knots;
  QPointFList support;
  QPointFList values;

  static constexpr size_t degree = 3;

  double bSpline(int deg, int index, double t);
  QPointF generalSpline(double t);
  void generateKnots();

  friend QDataStream &operator<<(QDataStream &stream, const Spline &spline);
  friend QDataStream &operator>>(QDataStream &stream, Spline &spline);
};

#endif // SPLINE_H
