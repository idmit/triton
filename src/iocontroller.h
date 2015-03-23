#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

#include <QDataStream>
#include "splinegroup.h"

class IOController {
public:
  void writeToFile(QString &fileName, const QHash<QString, SplineGroup> &map);
  QHash<QString, SplineGroup> readFromFile(QString fileName);

private:
  QDataStream stream;
};

#endif // IOCONTROLLER_H
