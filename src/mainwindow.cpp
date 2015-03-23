#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  initMenuBar();

  canvas = new Canvas(this);
  canvas->resize(maximumSize());
  openFont("/Users/ivandmi/Downloads/font.trft");
  ui->gridLayout->addWidget(canvas);
}

void MainWindow::initMenuBar() {
  QMenuBar *menuBar = this->menuBar();

  QMenu *menuFile = new QMenu("File", this);
  menuFile->addAction("Create Project", this, SLOT(openFont()),
                      QKeySequence(Qt::SHIFT + Qt::CTRL + Qt::Key_O));

  QMenu *menuEdit = new QMenu("Edit", this);
  menuEdit->addAction("Redo", this, SLOT(undoCmd()),
                      QKeySequence(Qt::CTRL + Qt::Key_Z));
  menuEdit->addAction("Undo", this, SLOT(redoCmd()),
                      QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Z));

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

void MainWindow::redoCmd() { canvas->redoCmd(); }

void MainWindow::undoCmd() { canvas->undoCmd(); }

MainWindow::~MainWindow() { delete ui; }
