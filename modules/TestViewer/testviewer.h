#ifndef TESTVIEWER_H
#define TESTVIEWER_H
#include "QVector"
#include "QVector2D"
#include "QVector3D"
#include "camera.h"
#include <QMainWindow>
#include "QPainter"
#include "lightsourse.h"
#include "graphicsobject.h"

using namespace  GraphicsObjectStruct;

class TestViewer
{
private:
    QVector<GraphicsObject*> objectsInScene;
    QVector<LightSourse*> lightsInScene;
    int modelIndex;
public:
    TestViewer();
    void addGraphicsObject (GraphicsObject* newObj);
    void addLight (LightSourse* light);
    void drawOn (QPainter* painter, const Camera cam, const int wid, const int hei) const;
    void drawOn (QPainter* painter, const Camera cam, const int wid, const int hei, bool allTogether) const;
    GraphicsObject* getModel () const;
    QVector<LightSourse*> getLights () const;

    int ModelCount () const;
    void SwapCurrentModelNext ();
    void SwapCurrentModelPrev ();
    void ClearGraphicsObjectsList ();
};

#endif // TESTVIEWER_H
