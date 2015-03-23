#include <QApplication>
#include <QFileDialog>
#include <QKeyEvent>
#include <QPainter>

#include <cmath>

#include "canvas.h"
#include "canvas_actions.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent) {
  setFocusPolicy(Qt::StrongFocus);
}

void Canvas::drawGrid(QPainter &painter, size_t cellNum, size_t cellsInThick) {
  int w = width(), h = height();
  int grayTone = 240, lineWidth = 0;
  QColor color(grayTone, grayTone, grayTone);

  painter.fillRect(QRectF(0, 0, w, h), QBrush(QColor("white")));
  //  for (size_t i = 0; i < cellNum; ++i) {
  //    if ((i + 1) % cellsInThick == 0) {
  //      lineWidth = 1;
  //    } else {
  //      lineWidth = 2;
  //    }
  //    painter.setPen(QPen(color, lineWidth, Qt::SolidLine));
  //    painter.drawLine(0, i * h / cellNum, w, i * h / cellNum);
  //    painter.drawLine(i * w / cellNum, 0, i * w / cellNum, h);
  //  }
}

void Canvas::drawLetters(QPainter &painter) {
  int w = width(), h = height();

  for (size_t t = 0; t < text.size(); ++t) {
    SplineGroup d = map.value(text.at(t));
    SplineGroup *activeGroup = &d;

    QVector<QPainterPath> paths;

    for (size_t k = 0; k < activeGroup->size(); ++k) {
      Spline spline = activeGroup->get(k);
      QPointF a = spline.atVal(0);
      paths.append(QPainterPath());
      paths[k].moveTo(a.x() * w, a.y() * h);
      for (size_t i = 1; spline.valSize() > 0 && i < spline.valSize(); ++i) {
        QPointF b = spline.atVal(i);
        paths[k].lineTo(b.x() * w, b.y() * h);
      }
    }

    for (int i = 0; i < paths.size() - 1; ++i) {
      for (int k = i + 1; k < paths.size(); ++k) {
        if (paths[i].intersects(paths[k])) {
          QPainterPath interection = paths[i].intersected(paths[k]);
          paths[i] = paths[i].subtracted(interection);
          paths[k] = paths[k].subtracted(interection);
        }
      }
    }

    for (size_t i = 0; i < activeGroup->size(); ++i) {
      Spline spline = activeGroup->get(i);

      painter.fillPath(paths[i], QBrush(Qt::black, Qt::Dense5Pattern));

      painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
      if (spline.supSize() > 0) {
        for (size_t i = 0; i < spline.supSize() - 1; ++i) {
          QPointF a = spline.atSup(i), b = spline.atSup(i + 1);
          painter.drawLine(a.x() * w, a.y() * h, b.x() * w, b.y() * h);
        }
        QPointF a = spline.atSup(spline.supSize() - 1), b = spline.atSup(0);
        painter.drawLine(a.x() * w, a.y() * h, b.x() * w, b.y() * h);

        painter.setPen(QPen(QColor(0, 116, 217), 1, Qt::SolidLine));
        QColor brushColor = (i == activeGroup->getIdx())
                                ? QColor(255, 220, 0)
                                : QColor(221, 221, 221);
        painter.setBrush(QBrush(brushColor, Qt::SolidPattern));

        for (size_t i = 0; i < spline.supSize(); ++i) {
          QPointF a = spline.atSup(i);
          painter.drawEllipse(QPointF(a.x() * w, a.y() * h), 5, 5);
        }
      }
    }
  }
}

void Canvas::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  drawGrid(painter);
  drawLetters(painter);
}

void Canvas::keyPressEvent(QKeyEvent *event) {
  QString pressedKey = event->text();
  if (pressedKey.isEmpty()) {

  } else {
    if (event->key() == Qt::Key_Backspace) {
      removeLetter();
      return;
    }
    auto availableLetters = map.keys();
    if (availableLetters.contains(pressedKey)) {
      typeLetter(pressedKey);
    }
  }
}

QString Canvas::changeFont(IOController &ioController, QString fontPath) {
  if (fontPath.isEmpty()) {
    fontPath = QFileDialog::getOpenFileName(this, tr("Open Font"), "",
                                            tr("Trident font files (*.trft)"));
  }
  if (!fontPath.isEmpty()) {
    map = ioController.readFromFile(fontPath);
    repaint();
  }
  QFileInfo fileInfo(fontPath);
  return fileInfo.completeBaseName();
}

void Canvas::typeLetter(QString letter) {
  undoStack.push(new TypeLetterCmd(text, letter));
  repaint();
}
void Canvas::removeLetter() {
  undoStack.push(new RemoveLetterCmd(text));
  repaint();
}

void Canvas::redoCmd() {
  undoStack.redo();
  repaint();
}

void Canvas::undoCmd() {
  undoStack.undo();
  repaint();
}
