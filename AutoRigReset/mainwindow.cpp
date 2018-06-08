#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "QFileDialog"
#include "iofbx.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_Rig_triggered()
{
    QString err = IOfbx::loadFromPath(QFileDialog::getOpenFileName(this, "Load Rig", "@/../Models/FBX/", "FBX files (*.FBX)"));
    if (!err.isEmpty())
        qDebug() << err;
}

void MainWindow::on_actionLoad_Guad_triggered()
{
    QString err = IOfbx::loadFromPath("D:/QT-serious/Models/FBX/!guard yelling exported.FBX");
    if (!err.isEmpty())
        qDebug() << err;
}
