#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "QFileDialog"
#include "iofbx.h"
#include "QDebug"

using namespace IOfbx;

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
FbxParsedContainer* parsed = nullptr;

void MainWindow::on_actionLoad_Rig_triggered()
{
    QString err = "";
    parsed = loadFromPath(QFileDialog::getOpenFileName(this, "Load Rig", "@/../Models/FBX/", "FBX files (*.FBX)"), err);
    if (!err.isEmpty())
        qDebug() << err;
    else
        parsed->traceInfo();
}

void MainWindow::on_actionLoad_Guad_triggered()
{
    QString err = "";
    parsed = loadFromPath("D:/QT-serious/Models/FBX/!guard yelling exported.FBX", err);
    if (!err.isEmpty())
        qDebug() << err;
    else
        parsed->traceInfo();
}
