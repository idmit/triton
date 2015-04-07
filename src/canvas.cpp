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
  int h = fontSizeInPixels;
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
    if (text.at(t) == ' ') {
      ++cursorHorPos;
      continue;
    }
    if (!map.contains(text.at(t))) {
      continue;
    }
    SplineGroup d = map.value(text.at(t));
    SplineGroup *activeGroup = &d;

    QVector<QPainterPath> paths;
    int w = (int)(h / activeGroup->getHWR());
    int s = (int)(h / activeGroup->getHSR());
    double lBorder = activeGroup->getBorder("left");
    double bBorder = activeGroup->getBorder("bottom");
    double tBorder = activeGroup->getBorder("top");
    double rBorder = lBorder + (bBorder - tBorder) / activeGroup->getHWR();
    double sBorder = rBorder + (bBorder - tBorder) / activeGroup->getHSR();

    for (size_t k = 0; k < activeGroup->size(); ++k) {
      Spline spline = activeGroup->get(k);
      QPointF a = spline.atVal(0);
      paths.append(QPainterPath());
      double x = (a.x() - lBorder) / (rBorder - lBorder);
      double y = (a.y() - tBorder) / (bBorder - tBorder);
      paths[k].moveTo(cursorHorPos * (w + s) + x * w,
                      cursorVertPos * h + y * h);
      for (size_t i = 1; spline.valSize() > 0 && i < spline.valSize(); ++i) {
        QPointF b = spline.atVal(i);
        double x = (b.x() - lBorder) / (rBorder - lBorder);
        double y = (b.y() - tBorder) / (bBorder - tBorder);
        paths[k].lineTo(cursorHorPos * (w + s) + x * w,
                        cursorVertPos * h + y * h);
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
  if (text.size() > 0) {
    int t = text.size() - 1;
    SplineGroup d = map.value(text.at(t));
    SplineGroup *activeGroup = &d;
    int w = (int)(h / activeGroup->getHWR());
    int s = (int)(h / activeGroup->getHSR());
    painter.drawLine(QPointF(cursorHorPos * (w + s), cursorVertPos * h),
                     QPointF(cursorHorPos * (w + s), (cursorVertPos + 1) * h));
  } else {
    painter.drawLine(QPointF(0, cursorVertPos * h),
                     QPointF(0, (cursorVertPos + 1) * h));
  }
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
    if (event->key() == Qt::Key_Space) {
      typeLetter(" ");
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
    foreach (QString key, map.keys()) { map.find(key)->rebuildAll(); }
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

void Canvas::increaseFontSize() {
  fontSizeInPixels +=
      (fontSizeInPixels >= height() - minFontSizeInPixels) ? 0 : 5;
  repaint();
}

void Canvas::decreaseFontSize() {
  fontSizeInPixels -= (fontSizeInPixels <= minFontSizeInPixels) ? 0 : 5;
  repaint();
}
