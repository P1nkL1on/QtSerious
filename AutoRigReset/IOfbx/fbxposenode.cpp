#include "fbxposenode.h"

QString IOfbx::FbxPoseNode::parse(const QStringList &buffer)
{    
    for (int i = 0; i < buffer.length(); ++i){
        // retrieving of id
        if (buffer[i].indexOf("Node:") >= 0)
            setNameAndID("posenodeBindMatrix", buffer[i].trimmed().mid(6, buffer[i].trimmed().length() - 6));
        // the exactly 16 values
        if (buffer[i].indexOf("Matrix:") >= 0){
            QString error = "";
            transformMatrixArray = parseFbxArray<double>(QStringList(buffer[i + 1]), error);
            if (!error.isEmpty())
                return "Error in parsing 16 values in matrix of posenode, error in line: " + buffer[i] + "; " + error;
        }
    }
    traceMessage (QString("v   Success parsed posenode " + this->id));
    return QString();
}

const QVector<double> &IOfbx::FbxPoseNode::getTransformMatrixArray() const
{
    return transformMatrixArray;
}


