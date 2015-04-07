#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <type_traits>

#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  initMenuBar();

  canvas = new Canvas(this);
  canvas->resize(maximumSize());
  //static_assert(false, "Specify path to default font.");
  openFont("demo.trft");
  ui->gridLayout->addWidget(canvas);
}

void MainWindow::initMenuBar() {
  QMenuBar *menuBar = this->menuBar();

  QMenu *menuFile = new QMenu("File", this);
  menuFile->addAction("Open Font", this, SLOT(openFont()),
                      QKeySequence(Qt::SHIFT + Qt::CTRL + Qt::Key_O));
  menuFile->addAction("Open File", this, SLOT(loadFile()),
                      QKeySequence(Qt::SHIFT + Qt::CTRL + Qt::Key_L));
  menuFile->addAction("Save File", this, SLOT(saveFile()),
                      QKeySequence(Qt::CTRL + Qt::Key_S));
  menuFile->addAction("Save As", this, SLOT(saveFileAs()),
                      QKeySequence(Qt::CTRL + Qt::Key_S));

  QMenu *menuEdit = new QMenu("Edit", this);
  menuEdit->addAction("Redo", this, SLOT(undoCmd()),
                      QKeySequence(Qt::CTRL + Qt::Key_Z));
  menuEdit->addAction("Undo", this, SLOT(redoCmd()),
                      QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Z));
  menuEdit->addAction("Increase font size", this, SLOT(incFont()),
                      QKeySequence(Qt::CTRL + Qt::Key_Equal));
  menuEdit->addAction("Decrease font size", this, SLOT(decFont()),
                      QKeySequence(Qt::CTRL + Qt::Key_Minus));

  QMenu *menuAbout = new QMenu("Help", this);
  menuAbout->addAction("About");

  menuBar->addMenu(menuFile);
  menuBar->addMenu(menuEdit);
  menuBar->addMenu(menuAbout);
}

void MainWindow::openFont(QString fontPath) {
  QString newFont = canvas->changeFont(ioController, fontPath);
  if (newFont.isEmpty()) {
    ui->statusBar->showMessage("No Font Set");
  } else {
    ui->statusBar->showMessage("Current Font: " + newFont);
  }
}

void MainWindow::loadFile() { canvas->loadFile(ioController); }

void MainWindow::saveFile() { canvas->saveFile(ioController); }

void MainWindow::saveFileAs() { canvas->saveFileAs(ioController); }

void MainWindow::redoCmd() { canvas->redoCmd(); }

void MainWindow::undoCmd() { canvas->undoCmd(); }

void MainWindow::incFont() { canvas->increaseFontSize(); }

void MainWindow::decFont() { canvas->decreaseFontSize(); }

MainWindow::~MainWindow() { delete ui; }
