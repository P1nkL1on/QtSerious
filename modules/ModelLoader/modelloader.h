#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include "QImage"
#include <model.h>
#include "QImageReader"

namespace ModelLoader
{

    QString loadModel(QTextStream& textStream, ModelStructs::Model& loadedModel);
    QString loadModelByAdress (QString path, ModelStructs::Model& ls);

}

#endif // MODELLOADER_H
