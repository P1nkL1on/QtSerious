#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "autorigtests.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->runTests();
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::runTests()
{
    AutoRig::AutoRigTests arTests;

    QTest::qExec(&arTests);
}
