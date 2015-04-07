#ifndef SPLINEGROUP_H
#define SPLINEGROUP_H

#include "spline.h"

#include <memory>
#include <QUndoStack>

class SplineGroup {
public:
  SplineGroup();
  void add(Spline spline);
  void insertAt(size_t idx, Spline spline);
  Spline &get(size_t idx);
  Spline &getActive();
  size_t getIdx();
  double getHWR();
  double getHSR();
  void pop();
  void rebuildAll();
  void removeAt(size_t idx);
  void resetAt(size_t idx, Spline spline);
  void setIdx(size_t idx);
  void setHWR(double ratio);
  void setHSR(double ratio);
  void setBorder(QString name, double val);
  double getBorder(QString name);
  size_t size();

  QUndoStack *undoStack = new QUndoStack();

private:
  size_t activeIdx = 0;
  QVector<Spline> splines;
  double hwr,hsr;
  QHash<QString,double> borders;
  friend QDataStream &operator<<(QDataStream &stream, const SplineGroup &group);
  friend QDataStream &operator>>(QDataStream &stream, SplineGroup &group);
};

#endif // SPLINEGROUP_H
