#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QFileDialog>
#include <QDebug>
#include "IOfbx/iofbxtests.h"
#include "IOfbx/fbxtorigconverter.h"

using namespace IOfbx;

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
FbxParsedContainer* parsed = nullptr;

void MainWindow::on_actionLoad_Rig_triggered()
{
    loadRigByAdress(QFileDialog::getOpenFileName(this, "Load Rig", "../Models/FBX/", "FBX files (*.FBX)"));
}

void MainWindow::on_actionLoad_Guad_triggered()
{
    loadRigGuardYelling();
}

void MainWindow::on_actionLoad_Ivan_Sergeich_triggered()
{
    loadRigIvanSergeich();
}

bool everDrawen = false;

void MainWindow::loadRigByAdress(QString path)
{
    QString err = "";
    parsed = loadFromPath(path, err);
    if (!err.isEmpty())
        qDebug() << err;
    else
    {
        parsed->traceInfo();
        everDrawen = false;
        this->repaint();
    }

}

void MainWindow::loadRigGuardYelling()
{
    loadRigByAdress("../Models/FBX/!guard yelling exported.FBX");
}

void MainWindow::loadRigIvanSergeich()
{
    loadRigByAdress("../Models/FBX/!IvanSergeich0.FBX");
}

void MainWindow::runTests()
{
    IOfbx::IOfbxTests fbxTests;
    QTest::qExec(&fbxTests);
}
void MainWindow::paintEvent(QPaintEvent *e)
{
    if (!everDrawen && parsed != nullptr){
        QPainter qp(this);
        FbxConverter::convertContainerToRig(parsed, &qp);
        qp.end();
        everDrawen = true;
    }
}
