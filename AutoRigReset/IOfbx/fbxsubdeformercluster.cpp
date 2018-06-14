#include "fbxsubdeformercluster.h"

QString IOfbx::FbxSubDeformerCluster::parse(const QStringList &buffer)
{
    QString error = "";
    for (int i = 0; i < buffer.length(); ++i){
        // retrieving of inds
        if (buffer[i].indexOf("Indexes:") >= 0){
            indexes = parseFbxArray<int>(QStringList(buffer[i + 1]), error);
            if (!error.isEmpty())
                error = "Error in parsing point indexes values, error in line: " + buffer[i] + "; " + error;
        }
        if (buffer[i].indexOf("Weights:") >= 0
            || buffer[i].indexOf("Transform:") >= 0
            || buffer[i].indexOf("TransformLink:") >= 0){

            QVector<double> parsed = parseFbxArray<double>(QStringList(buffer[i + 1]), error);
            if (!error.isEmpty())
                error = "Error in parsing point double values, error in line: " + buffer[i] + "; " + error;
            if (buffer[i].indexOf("Weights:") >= 0)
                weights = parsed;
            if (buffer[i].indexOf("Transform:") >= 0)
                transformMatrix = parsed;
            if (buffer[i].indexOf("TransformLink:") >= 0)
                transformLinkMatrix = parsed;
        }
    }
    if (transformMatrix.length() > 0 && transformMatrix.length() < 16)
        error = "Incorrrect transform matrix vector size: " + QString::number(transformMatrix.length()) + ", not 16;";

    if (transformLinkMatrix.length() > 0 && transformLinkMatrix.length() < 16)
        error = "Incorrrect link transform matrix vector size: " + QString::number(transformLinkMatrix.length()) + ", not 16;";

    if (!error.isEmpty())
        return "Error in parsing cluster: " + error;

    traceMessage (QString("v   Success parsed subdeformer " + this->id));
    return QString();
}

bool IOfbx::FbxSubDeformerCluster::isEmpty() const
{
    return (indexes.length() == 0);
}

