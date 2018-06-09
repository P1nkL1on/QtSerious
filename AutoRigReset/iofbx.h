#ifndef IOFBX_H
#define IOFBX_H

#include "QString"
#include "QVector"

namespace IOfbx {

static QVector<QString> parseBlockNames = {"Geometry Mesh Vertices",
                                           "Geometry Mesh Polygon Indexes",
                                           "Object Model Limb Node info",
                                           "Object Pose Node Id",
                                           "Object Pose Node Bind Matrix",
                                           "???"};
enum class ParseType
{
    None = -1,
    FbxGeometryMeshVertices = 0,
    FbxGeometryMeshPolygonIndexes = 1,
    FbxObjectModelLimbNodeProperty = 2,
    FbxObjectPoseNodeID = 3,
    FbxObjectPoseNodeMatrix = 4,
};

class FbxNode
{
public:
    virtual QString parse(QStringList S, const int param) = 0;
    template <typename T>
    static QVector<T> parseFbxArray(QStringList S, QString& error);
    template <typename T>
    static QVector<T> parseLastXValues (const QString S, const int X, QString& error);
    void setNameAndID (const QString name, const QString ID);
    bool hasNameAndID () const;
    virtual void traceInfo() const;
protected:
    QString id;
    QString name;
};

class FbxGeometryMesh : public FbxNode
{
public:
    FbxGeometryMesh();
    QString parse(QStringList S, const  int param) override;
private:
    QVector<double> vertices;
    QVector<int> polygonVertexIndex;
};

class FbxModelJoint : public FbxNode
{
public:
    FbxModelJoint ();
    void addSubName (QString subName);
    QString parse(QStringList S, const  int param) override;
private:
    QString subName;
    QVector<double> localTranslation;
    QVector<double> localRotation;
    QVector<double> localScaling;
};

class FbxPoseNode : public FbxNode
{
public:
    FbxPoseNode();
    QString parse(QStringList S, const int param) override;
private:
    QVector<double> transformMatrixArray;
};

// _-_-_-_-_-_-_-_-_-_-PARSER-_-_-_-_-_-_-_-_-_-_-_-_


static QVector<QString> stackHeaders = QVector<QString>();
ParseType pushHeader(const QString fromLine);
int indexOfHeaders (const QVector<QString> tags);

// the method of loading
QString loadFromPath(const QString path);
void findIdAndNameInLine (const QString line, QString& id, QString& name, QString &subName);
}

#endif // IOFBX_H
