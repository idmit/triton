#include "iocontroller.h"
#include <QFile>

void IOController::writeToFile(QString &fileName,
                               const QHash<QString, SplineGroup> &map) {
  QFile file(fileName);
  file.open(QIODevice::WriteOnly);
  stream.setDevice(&file);
  stream << map;
  file.close();
}

QHash<QString, SplineGroup> IOController::readFromFile(QString fileName) {
  QFile file(fileName);
  QHash<QString, SplineGroup> map;
  file.open(QIODevice::ReadOnly);
  stream.setDevice(&file);
  stream >> map;
  file.close();
  return map;
}

void IOController::writeTextToFile(QString &fileName, const QString text)
{
  QFile file(fileName);
  file.open(QIODevice::WriteOnly | QIODevice::Text);
  textStream.setDevice(&file);
  textStream << text;
  file.close();
}

QString IOController::readTextFromFile(QString &fileName)
{
  QString text;
  QFile file(fileName);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  textStream.setDevice(&file);
  text = textStream.readAll();
  file.close();
  return text;
}
