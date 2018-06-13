#include "fbxsubdeformercluster.h"

QString IOfbx::FbxSubDeformerCluster::parse(QStringList S, const int param)
{
    QString error = "";
    for (int i = 0; i < S.length(); i++){
        // retrieving of inds
        if (S[i].indexOf("Indexes:") >= 0){
            indexes = parseFbxArray<int>(QStringList(S[i + 1]), error);
            if (!error.isEmpty())
                error = "Error in parsing point indexes values, error in line: " + S[i] + "; " + error;
        }
        if (S[i].indexOf("Weights:") >= 0
            || S[i].indexOf("Transform:") >= 0
            ||S[i].indexOf("TransformLink:") >= 0){
            QVector<double> parsed = parseFbxArray<double>(QStringList(S[i + 1]), error);
            if (!error.isEmpty())
                error = "Error in parsing point double values, error in line: " + S[i] + "; " + error;
            if (S[i].indexOf("Weights:") >= 0)
                weights = parsed;
            if (S[i].indexOf("Transform:") >= 0)
                transformMatrix = parsed;
            if (S[i].indexOf("TransformLink:") >= 0)
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
