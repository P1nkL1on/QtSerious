#include "fbxposenode.h"

QString IOfbx::FbxPoseNode::parse(QStringList S, const int param)
{
    QStringList arrayContainer = QStringList();
    for (int i = 0; i < S.length(); i++){
        // retrieving of id
        if (S[i].indexOf("Node:") >= 0)
            setNameAndID("posenodeBindMatrix", S[i].trimmed().mid(6, S[i].trimmed().length() - 6));
        // the exactly 16 values
        if (S[i].indexOf("Matrix:") >= 0)
        {
            QString error = "";
            transformMatrixArray = parseFbxArray<double>(QStringList(S[i + 1]), error);
            if (!error.isEmpty())
                return "Error in parsing 16 values in matrix of posenode, error in line: " + S[i] + "; " + error;
        }
    }
    traceMessage (QString("v   Success parsed posenode " + this->id));
    return QString();
}


IOfbx::FbxPoseNode::FbxPoseNode()
{
    transformMatrixArray = QVector<double>();
}
