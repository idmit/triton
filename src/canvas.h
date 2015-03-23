#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

#include "iocontroller.h"
#include "splinegroup.h"

class Canvas : public QWidget {
  Q_OBJECT
public:
  explicit Canvas(QWidget *parent = 0);

protected:
  void paintEvent(QPaintEvent *);
  void keyPressEvent(QKeyEvent *event);

private:
  QHash<QString, SplineGroup> map;
  QString text;
  QUndoStack undoStack;
  unsigned  cursorHorPos = 0, cursorVertPos = 0;
  unsigned fontSizeInPixels = 30;
  void drawLetters(QPainter &painter);


public slots:
  QString changeFont(IOController &ioController, QString fontPath);
  void typeLetter(QString letter);
  void removeLetter();
  void redoCmd();
  void undoCmd();
};

#endif // CANVAS_H
