#include "fbxmodeljoint.h"

QString IOfbx::FbxModelJoint::parse(QStringList S, const int param)
{
    QString error = QString();
    for (int lineInd = 0; lineInd < S.length(); lineInd++){
        QString line = S[lineInd];
        if (line.indexOf("\"Lcl") > 0){
            QVector<double> values = parseLastXValues<double>(line, 3, error);
            if (!error.isEmpty())
                return "Error in parsing 3 last values, error in line: " + line + "; " + error;
            if (line.indexOf("Lcl Translation") >= 0)
                this->localTranslation = values;
            if (line.indexOf("Lcl Rotation") >= 0)
                this->localRotation = values;
            if (line.indexOf("Lcl Scaling") >= 0)
                this->localScaling = values;
        }
    }
    traceMessage (QString("v   Success parsed joint " + this->name));
    return QString();
}

void IOfbx::FbxModelJoint::addSubName(const QString &subName)
{
    this->subName = subName;
}


IOfbx::FbxModelJoint::FbxModelJoint()
{
    localTranslation = QVector<double>();
    localRotation = QVector<double>();
    localScaling = QVector<double>();
}

