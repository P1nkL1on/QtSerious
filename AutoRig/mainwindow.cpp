#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "LoaderFBXlight/loaderfbx.h"
#include "testviewer.h"
#include "testautorig.h"

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

Camera cam = Camera (0, 0, 0);
TestViewer tv = TestViewer();
QVector<Rig*> rgs;
//QVector<Mesh*> mshs;
QVector<QString> names;
TestAutoRig tar;

// constrols
bool mouseStillPressed = false;
void MainWindow::mouseMoveEvent(QMouseEvent *m)
{
    //QApplication::processEvents();
    unsigned short mod = 3;
    if (m->modifiers() & Qt::ShiftModifier) mod = 2;
    if (m->modifiers() & Qt::AltModifier) mod = 1;
    if (m->modifiers() & Qt::ControlModifier) mod = 0;

    if (mouseStillPressed){
        cam.transformByMouseMoving(QVector2D(m->x(), m->y()), mod);
        this->repaint();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *m)
{
    mouseStillPressed = true;
    cam.prevMousePlace = QVector2D(m->x(), m->y());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *m)
{
    mouseStillPressed = false;
}
// !constrols

QVector3D camCenter; int iteration = 0;
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_R){
        iteration = 0;
        tar.SetCustomLowModel();
        this->repaint();
    }

    if (e->key() == Qt::Key_H){
        iteration = 0;
        tar.MiscBugHunt();
        this->repaint();
    }
    if (e->key() == Qt::Key_Up){
        iteration = 0;
        tar.ChangeTargetMeshInd(1);
        this->repaint();
    }
    if (e->key() == Qt::Key_Down){
        iteration = 0;
        tar.ChangeTargetMeshInd(-1);
        this->repaint();
    }
    if (e->key() == Qt::Key_Left){
        tv.SwapCurrentModelNext();
        this->repaint();
    }
    if (e->key() == Qt::Key_Right){
        tv.SwapCurrentModelPrev();
        this->repaint();
    }
    if (e->key() == Qt::Key_E){
        for (int i = 0; i < 1000; i++){
            tar.SetCustomLowModel();
            this->repaint();
            tar.GausNewtone();
            this->repaint();
        }
    }
//    if (e->key() == Qt::Key_J){
//        qDebug() << "Distance is " << tar.JacobianStep() << " on iteration " << ++iteration;
//        this->repaint();
//    }
    if (e->key() == Qt::Key_O){
        tar.GausNewtone();
        this->repaint();
    }
    if (e->key() == Qt::Key_T){
        tar.TestSkinBending();
        this->repaint();
    }


    if (tv.ModelCount() == 0 && e->key() == Qt::Key_Space){
        qDebug() << "Autorug stanalobe (ha-ha) activated;";
        names = {
            //"!bboy 2 exported",
            //"joints_only2,"
            //"joints_only2t",
            //"!pucn export",
//            "!Samba exported",
//            "!bboy exported",
           "!guard yelling exported"
//            "plane"
        };

        int loadedModel = 0;
        for (int currentModel = 0; currentModel < names.length(); currentModel++){
            Rig* rg = new Rig();
            QString err =
                    loaderFBX::loadModelFBXAdress(modelsAdress + "FBX/"+names[currentModel]+".FBX", *rg);
            if (!err.isEmpty())
                qDebug() << err;
            else
            {
                // add a loaded morel to test viewer
                rg->cameraCenter = &camCenter;
                rgs << rg;
                loadedModel++;
            }
        }
        // also load a OBJ poses
        QVector<QString> meshNames = {"plane2", "head-left","fabulos", "fabulos-zad","liying", "bind", "handforwardy90"};
        for (int i = 0; i < meshNames.length(); i++){
            Mesh* loadMesh = new Mesh();
            QString errMes = loaderFBX::loadMeshOBJAdress(modelsAdress + "GuardPosesOBJ/" + meshNames[i]+".OBJ", *loadMesh);
            if (errMes.isEmpty()){
                Rig* rgMs = new Rig(loadMesh, NULL, NULL);
                rgMs->cameraCenter = &camCenter;
                rgMs->modelColor = QColor(0, 200, 0, 20);
                rgMs->conturColor = QColor(0,0,0,0);
                rgs << rgMs;
                loadedModel ++;
            }else
                qDebug() << errMes;
        }
        tar = TestAutoRig((rgs[0]), rgs);
        tar.window = this;
        tar.camCenter = &camCenter;
        tv.addGraphicsObject(&tar);
    }
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    camCenter = cam.GetCamInfo()[0] * 100.0;
    if (tv.ModelCount() > 0)
        tv.drawOn(&qp, cam, (width() > height())? height() : width(), (width() > height())? height() : width(), false);
    qp.end();
}
