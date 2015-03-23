#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "canvas.h"
#include "iocontroller.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  Ui::MainWindow *ui;

  Canvas *canvas;
  IOController ioController;
  QString projectFileName;

  void initMenuBar();

public slots:
  void openFont(QString fontPath = "");
  void redoCmd() ;
  void undoCmd();
};

#endif // MAINWINDOW_H
