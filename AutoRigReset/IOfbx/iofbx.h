#ifndef IOFBX_H
#define IOFBX_H

#include "QString"
#include "QVector"

namespace IOfbx {

static bool showWarningsAndErrors = false;
void turnOnWarnings();

static QVector<QString> parseBlockNames = {"Geometry Mesh Vertices",
                                           "Geometry Mesh Polygon Indexes",
                                           "Object Model Limb Node info",
                                           "Object Pose Node Id",
                                           "Object Pose Node Bind Matrix",
                                           "Cluster info",
//                                           "Cluster Indexes",
//                                           "Cluster Weightes",
//                                           "Cluster Transform",
//                                           "Cluster Link Transform",
                                           "Connection",
                                           "???"};
enum class ParseType
{
    None = -1,
    FbxGeometryMeshVertices = 0,
    FbxGeometryMeshPolygonIndexes = 1,
    FbxObjectModelLimbNodeProperty = 2,
    FbxObjectPoseNodeID = 3,
    FbxObjectPoseNodeMatrix = 4,
    FbxObjectDeformerCluster = 5,
//    FbxObjectDeformerClusterIndexes = 6,
//    FbxObjectDeformerClusterWeightes = 7,
//    FbxObjectDeformerClusterTransform = 8,
//    FbxObjectDeformerClusterLinkTransform = 9
    FbxConnection = 6,

};
enum class ConnectionType
{
    None = -1,
    BoneToBone = 0,
    ModelToSubDeformer = 1,
    Other = 2
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
    virtual bool isEmpty() const;
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

class FbxSubDeformerCluster : public FbxNode
{
public:
    FbxSubDeformerCluster();
    QString parse(QStringList S, const int param) override;
    bool isEmpty() const override;
private:
    QVector<int> indexes;
    QVector<double> weights;
    QVector<double> transformMatrix;
    QVector<double> transformLinkMatrix;
};

class FbxConnection : public FbxNode
{
public:
    FbxConnection();
    QString parse(QStringList S, const int param) override;
    QString getIdLeft() const;
    QString getIdRight() const;
    ConnectionType getType() const;
    bool setIds (const QString left, const QString right);
    void setType(const ConnectionType &value);
private:
    QString idLeft;
    QString idRight;
    ConnectionType type;
};

class FbxParsedContainer
{
public:
    FbxParsedContainer();
    FbxParsedContainer(FbxGeometryMesh* mesh,
                       QVector<FbxModelJoint>* joints,
                       QVector<FbxPoseNode>* posenodes,
                       QVector<FbxSubDeformerCluster>* clusters,
                       QVector<FbxConnection>* connections);
    ~FbxParsedContainer();
    FbxGeometryMesh *getMesh() const;
    QVector<FbxModelJoint> *getJoints() const;
    QVector<FbxPoseNode> *getPosenodes() const;
    QVector<FbxSubDeformerCluster> *getClusters() const;
    QVector<FbxConnection> *getConnections() const;
    void traceInfo () const;
private:
    FbxGeometryMesh* mesh;
    QVector<FbxModelJoint>* joints;
    QVector<FbxPoseNode>* posenodes;
    QVector<FbxSubDeformerCluster>* clusters;
    QVector<FbxConnection>* connections;
};

// _-_-_-_-_-_-_-_-_-_-PARSER-_-_-_-_-_-_-_-_-_-_-_-_


static QVector<QString> stackHeaders = QVector<QString>();
ParseType pushHeader(const QString fromLine);
int indexOfHeaders (const QVector<QString> tags);

// the method of loading
FbxParsedContainer *loadFromPath(const QString path, QString &error);
void findIdAndNameInLine (const QString line, QString& id, QString& name, QString &subName);
void traceMessage (const QString mess);
}

#endif // IOFBX_H
