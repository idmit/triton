#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

#include <QDataStream>
#include <QTextStream>
#include "splinegroup.h"

class IOController {
public:
  void writeToFile(QString &fileName, const QHash<QString, SplineGroup> &map);
  QHash<QString, SplineGroup> readFromFile(QString fileName);

  void writeTextToFile(QString &fileName, const QString text);
  QString readTextFromFile(QString &fileName);

private:
  QDataStream stream;
  QTextStream textStream;
};

#endif // IOCONTROLLER_H
