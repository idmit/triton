#include "splinegroup.h"

SplineGroup::SplineGroup() { splines.append(Spline()); }

void SplineGroup::setIdx(size_t idx) { activeIdx = idx; }

void SplineGroup::setHWR(double ratio) { hwr = ratio; }

void SplineGroup::setHSR(double ratio) { hsr = ratio; }

size_t SplineGroup::getIdx() { return activeIdx; }

void SplineGroup::add(Spline spline) {
  splines.append(spline);
  setIdx(splines.size() - 1);
}

void SplineGroup::setBorder(QString name, double val) {
  if (borders.contains(name)) {
    borders.erase(borders.find(name));
  }
  borders.insert(name, val);
}

double SplineGroup::getBorder(QString name) { return borders.value(name); }

void SplineGroup::pop() {
  splines.pop_back();
  if (activeIdx == (size_t)splines.size()) {
    activeIdx -= 1;
  }
}

void SplineGroup::insertAt(size_t idx, Spline spline) {
  splines.insert(idx, spline);
}

void SplineGroup::resetAt(size_t idx, Spline spline) { splines[idx] = spline; }

void SplineGroup::rebuildAll() {
  for (int i = 0; i < splines.size(); ++i) {
    splines[i].build();
  }
}

void SplineGroup::removeAt(size_t idx) {
  splines.removeAt(idx);
  if (activeIdx == (size_t)splines.size()) {
    activeIdx -= 1;
  }
}

Spline &SplineGroup::get(size_t idx) { return splines[idx]; }

Spline &SplineGroup::getActive() { return splines[activeIdx]; }

double SplineGroup::getHWR() { return hwr; }

double SplineGroup::getHSR() { return hsr; }

size_t SplineGroup::size() { return splines.size(); }

QDataStream &operator<<(QDataStream &stream, const SplineGroup &group) {
  stream << group.hwr;
  stream << group.hsr;
  stream << group.splines;
  stream << group.borders;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, SplineGroup &group) {
  stream >> group.hwr;
  stream >> group.hsr;
  stream >> group.splines;
  stream >> group.borders;
  return stream;
}
