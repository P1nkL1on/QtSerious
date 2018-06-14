#ifndef IOFBX_H
#define IOFBX_H

#include "QString"
#include "QVector"

namespace IOfbx {

static QVector<QString> parseBlockNames = {"Geometry Mesh Vertices", "Geometry Mesh Polygon Indexes", "Object Model Limb Node info", "???"};
enum class ParseType
{
    None = -1,
    FbxGeometryMeshVertices = 0,
    FbxGeometryMeshPolygonIndexes = 1,
    FbxObjectModelLimbNodeProperty = 2,
};

class FbxNode
{
public:
    virtual QString parse(QStringList S, const int param) = 0;
    template <typename T>
    static QVector<T> parseFbxArray(QStringList S, QString& error);
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
    const QVector<double> &getLocalTranslation() const;
    const QVector<double> &getLocalRotation() const;
    const QVector<double> &getLocalScaling() const;

private:
    QVector<double> localTranslation;
    QVector<double> localRotation;
    QVector<double> localScaling;
};

// _-_-_-_-_-_-_-_-_-_-PARSER-_-_-_-_-_-_-_-_-_-_-_-_


static QVector<QString> stackHeaders = QVector<QString>();
ParseType pushHeader(const QString fromLine);
int indexOfHeaders (const QVector<QString> tags);

// the method of loading
QString loadFromPath(const QString path);
void findIdAndNameInLine (const QString line, QString& id, QString& name);
}

#endif // IOFBX_H
