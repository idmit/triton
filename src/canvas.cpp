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

void Canvas::drawLetters(QPainter &painter) {
  int w = fontSizeInPixels, h = fontSizeInPixels;
  cursorHorPos = 0, cursorVertPos = 0;

  painter.fillRect(QRectF(0, 0, width(), height()), QBrush(QColor("white")));

  for (int t = 0; t < text.size(); ++t) {
    if (text.at(t) == '\n') {
      ++cursorVertPos;
      cursorHorPos = 0;
      continue;
    }
    if (text.at(t) == '\r') {
      continue;
    }
    SplineGroup d = map.value(text.at(t));
    SplineGroup *activeGroup = &d;

    QVector<QPainterPath> paths;

    for (size_t k = 0; k < activeGroup->size(); ++k) {
      Spline spline = activeGroup->get(k);
      QPointF a = spline.atVal(0);
      paths.append(QPainterPath());
      paths[k].moveTo(cursorHorPos * w + a.x() * w,
                      cursorVertPos * h + a.y() * h);
      for (size_t i = 1; spline.valSize() > 0 && i < spline.valSize(); ++i) {
        QPointF b = spline.atVal(i);
        paths[k].lineTo(cursorHorPos * w + b.x() * w,
                        cursorVertPos * h + b.y() * h);
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
      painter.fillPath(paths[i], QBrush(Qt::black, Qt::SolidPattern));
    }
    ++cursorHorPos;
  }
  painter.drawLine(QPointF(cursorHorPos * w, cursorVertPos * h),
                   QPointF(cursorHorPos * w, (cursorVertPos + 1) * h));
}

void Canvas::paintEvent(QPaintEvent *) {
  QPainter painter(this);
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
  if ((event->key() == Qt::Key_Enter) || (event->text() == "\r")) {
    typeLetter("\n");
  }
}

QString Canvas::changeFont(IOController &ioController, QString fontPath) {
  if (fontPath.isEmpty()) {
    fontPath = QFileDialog::getOpenFileName(this, tr("Open Font"), "",
                                            tr("Trident font files (*.trft)"));
  }
  if (!fontPath.isEmpty()) {
    map = ioController.readFromFile(fontPath);
    foreach (QString key, map.keys()) {
      map.find(key)->rebuildAll();
    }
    repaint();
  }
  QFileInfo fileInfo(fontPath);
  return fileInfo.completeBaseName();
}

void Canvas::loadFile(IOController &ioController) {
  projectFileName = QFileDialog::getOpenFileName(this, tr("Open file"), "");
  if (!projectFileName.isEmpty()) {
    text = ioController.readTextFromFile(projectFileName);
    repaint();
  }
}

void Canvas::saveFile(IOController &ioController) {
  if (projectFileName.isEmpty()) {
    projectFileName = QFileDialog::getSaveFileName(this, tr("Save File"), "");
  }
  ioController.writeTextToFile(projectFileName, text);
}

void Canvas::saveFileAs(IOController &ioController) {
  projectFileName = QFileDialog::getSaveFileName(this, tr("Save File"), "");
  ioController.writeTextToFile(projectFileName, text);
}

void Canvas::typeLetter(QString letter) {
  undoStack.push(new TypeLetterCmd(text, letter));
  repaint();
}
void Canvas::removeLetter() {
  if (!text.isEmpty()) {
    undoStack.push(new RemoveLetterCmd(text));
  }
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
