#include "spline.h"

Spline::Spline() {
  QPointFList initialPoints = {
    { 0.4, 0.4 }, { 0.4, 0.6 }, { 0.5, 0.6 }, { 0.6, 0.6 }, { 0.6, 0.4 }
  };
  support.append(initialPoints);
  build(approx);
}

size_t Spline::addPoint(size_t idx, QPointF &point, bool rebuild) {
  support.insert(idx, point);
  if (rebuild) {
    build(approx);
  }
  return idx;
}

size_t Spline::addPoint(size_t idx, double x, double y) {
  QPointF p(x, y);
  return addPoint(idx, p);
}

size_t Spline::addPointTo(size_t idx, QPointF &point, bool rebuild) {
  support.insert(idx, point);
  if (rebuild) {
    build(approx);
  }
  return idx;
}

QPointF Spline::atSup(size_t idx) { return support[idx]; }

double Spline::bSpline(int deg, int index, double t) {
  if (deg == 0) {
    return (knots[index] <= t && t < knots[index + 1]) ? 1 : 0;
  }

  double s1 = 0, s2 = 0;
  if (knots[index + deg] - knots[index] > 1e-9) {
    s1 = bSpline(deg - 1, index, t) * (t - knots[index]) /
         (knots[index + deg] - knots[index]);
  }
  if (knots[index + deg + 1] - knots[index + 1] > 1e-9) {
    s2 = bSpline(deg - 1, index + 1, t) * (knots[index + deg + 1] - t) /
         (knots[index + deg + 1] - knots[index + 1]);
  }
  return s1 + s2;
}

QPointF Spline::generalSpline(double t) {
  QPointF sum(0, 0);
  size_t n = supSize() - 1;
  for (size_t i = 0; i < n + 1; ++i) {
    sum += support[i] * bSpline(degree, i, t);
  }
  return sum;
}

void Spline::generateKnots() {
  knots.clear();

  for (size_t i = 0; i < degree + 1; ++i) {
    support.append(support[i]);
  }

  size_t n = supSize() - 1;

  knots.append(0);
  for (size_t i = 1; i < n + degree + 1; i++) {
    knots.append((double)i / (n + degree + 1));
  }
  knots.append(1);
}

void Spline::build(size_t approxParam) {
  double arg = 0;
  QPointFList vals;

  if (supSize() > 4) {
    generateKnots();
    size_t n = supSize() - 1;
    arg = knots[degree];
    double step = (knots[n] - knots[degree]) / (double)approxParam;
    for (size_t num = 0; num < approxParam; ++num) {
      arg += step;
      vals.push_back(generalSpline(arg));
    }
    values = vals;
    for (size_t i = 0; i < degree + 1; ++i) {
      support.pop_back();
    }
  } else {
    values.clear();
  }
}

size_t Spline::OrderedDoubleSet::append(double point) {
  QList<double>::push_back(point);
  return QList<double>::size() - 1;
}

size_t Spline::OrderedDoubleSet::insert(double point) {
  size_t idx = 0;
  for (const auto &p : (*this)) {
    if (p >= point) {
      break;
    }
    ++idx;
  }
  QList<double>::insert(idx, point);
  return idx;
}

QPointF &Spline::atVal(size_t idx) { return values[idx]; }

void Spline::removeAt(size_t idx, bool rebuild) {
  support.removeAt(idx);
  if (rebuild) {
    build(approx);
  }
}

void Spline::resetPoint(size_t idx, QPointF point) {
  support[idx] = point;
  build(approx);
}

void Spline::resetPointX(size_t idx, double x) {
  support[idx].setX(x);
  build(approx);
}

void Spline::resetPointY(size_t idx, double y) {
  support[idx].setY(y);
  build(approx);
}

size_t Spline::supSize() { return support.size(); }

size_t Spline::valSize() { return values.size(); }

QDataStream &operator<<(QDataStream &stream, const Spline &spline) {
  stream << spline.support;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, Spline &spline) {
  stream >> spline.support;
  return stream;
}
