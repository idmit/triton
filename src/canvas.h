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
  QString projectFileName;
  QUndoStack undoStack;
  unsigned cursorHorPos = 0, cursorVertPos = 0;
  int fontSizeInPixels = 30;
  void drawLetters(QPainter &painter);
  const int minFontSizeInPixels = 15;

public slots:
  QString changeFont(IOController &ioController, QString fontPath);
  void loadFile(IOController &ioController);
  void saveFile(IOController &ioController);
  void saveFileAs(IOController &ioController);
  void typeLetter(QString letter);
  void removeLetter();
  void redoCmd();
  void undoCmd();
  void increaseFontSize();
  void decreaseFontSize();
};

#endif // CANVAS_H
