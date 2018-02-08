#include "testviewer.h"
#include "iostream"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qvector3d.h"
TestViewer::TestViewer()
{
    objectsInScene = {};
    lightsInScene = {};
    modelIndex = -1;
}


void TestViewer::addGraphicsObject(GraphicsObject *newObj)
{
   objectsInScene << newObj;
   modelIndex = objectsInScene.length() - 1;
}
void TestViewer::addLight(LightSourse *light)
{
    lightsInScene << light;
}

void TestViewer::drawOn(QPainter *painter, const Camera cam, const int wid, const int hei) const
{
    drawOn(painter, cam, wid, hei, true);
}
void TestViewer::drawOn(QPainter *painter, const Camera cam, const int wid, const int hei, bool allTogether) const{
    if (objectsInScene.length() == 0)
        qDebug() << "No objects to rasterise.";

    if (allTogether)
    {
        for (int currentModel = 0; currentModel < objectsInScene.length(); currentModel++)
            objectsInScene[currentModel]->ApplyDrawToCanvas(painter, cam.getViewingMatrix(), cam.getPerspectiveMatrix(), wid, hei);
    }
    else
        objectsInScene[modelIndex]->ApplyDrawToCanvas(painter, cam.getViewingMatrix(), cam.getPerspectiveMatrix(), wid, hei);

}

GraphicsObject *TestViewer::getModel() const
{
    if (objectsInScene.length() == 0)
        return NULL;
    return objectsInScene[0];
}

QVector<LightSourse *> TestViewer::getLights() const
{
    if (lightsInScene.length() == 0)
        return {};
    return lightsInScene;
}

int TestViewer::ModelCount() const
{
    return objectsInScene.length();
}

void TestViewer::SwapCurrentModelNext()
{
    modelIndex ++;
    if (modelIndex >= objectsInScene.length())
        modelIndex = 0;

    qDebug() << "Next model" << modelIndex;
}

void TestViewer::SwapCurrentModelPrev()
{
    modelIndex --;
    if (modelIndex < 0)
        modelIndex = objectsInScene.length() - 1;

    qDebug() << "Prev model" << modelIndex;
}

void TestViewer::ClearGraphicsObjectsList()
{
    objectsInScene = {};
    qDebug() << "Scene cleared!";
}

