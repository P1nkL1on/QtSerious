#include "loaderfbx.h"

#include <QFile>
#include <QVector>
#include <qdebug.h>

// welcome to autorig
#include "Eigen/Core"
#include "Derivable/derivable.h"
#include "Derivable/dermatops.h"

using Eigen::Matrix;
using namespace DerOperations;
using namespace DerivableVectorMatrixes;


QString loaderFBX::loadModelFBXAdress(QString path, Rig &loadedRig)
{
    QString errMessage = "Can not load FBX from \"" + path + "\"";

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return errMessage;

    QTextStream stream(&file);
    QString err = loadModelFBX(stream, loadedRig);
    //return error with file adress | return emptystring == success
    if (err.isEmpty())
        qDebug() << "    @    " + path.remove(0,path.lastIndexOf('/') + 1) + " was loaded;";
    return ((!err.isEmpty())? errMessage + ": " + err : QString());
}

float QStringToFloat (QString str){
    return ::atof(str.toStdString().c_str());
}

int QStringToInt (QString str){
    return ::atof(str.toStdString().c_str());
}

QString loaderFBX::loadModelFBX (QTextStream &textStream, Rig &loadedRig){

    char isMoreDebug = 'n'; // if (isMoreDebug == 'y')
    Rigging = true;
    //check empty file
    if (textStream.atEnd())
        return "Empty file FBX loaded";

    // .... static counters ....
    int limbsConnected = 0, limbToClusterConnected = 0, bindMatrixesGeted = 0;


    // ..mesh..
    QVector<Matrix<Derivable,1,3>> loadedVertexes;
    QVector<int> loadedVertexIndexes;
    QVector<int> loadedPolygonStartIndexes;
    Mesh* resMesh = new Mesh();
    // ..bones..
    QVector<Joint*> loadedJoints;
    Joint* lastJointCreated;
    // ..clusters..
    QVector<QString> loadedClusterID;
    QVector<QVector<int>> loadedClusterVertIndexes;
    QVector<QVector<float>> loadedClusterVertWeightes;
    Skin* resSkin = new Skin();
    // ..bindposes..
    QVector<Matrix<Derivable,1,3>> bindTransformFromClusters;

    QString line = "", prevLine, currentDirectory = "", currentName, currentID = "", currentParse = "";
    QStringList currentParseSplited;
    int parseType = -1, nextJointUseInd = -1;

    // file reading
    while (!textStream.atEnd()){
        prevLine = line;
        line = textStream.readLine();


        // skip or remove tabs in begin of string
        while (line[0] == '\t')line.remove(0,1);
        if (line.length() == 0)
            continue;


        currentID = (line.indexOf("NodeAttribute") == 0 || line.indexOf("Model") == 0 || line.indexOf("Deformer") == 0)?
                    line.mid(line.indexOf(":") + 2, line.indexOf(",") - line.indexOf(":") - 2) : "";

        // ...
        // adding a directory to dir.stack
        if (line[line.length() - 1] == '{'){
            QStringList names = line.split('"');
            currentName = (names.length() > 2 && (names.length() -1) % 2 == 0)?
                names[names.length() - 2] : "";
            currentDirectory += "/" + line.mid(0, line.indexOf(':'));
            if (currentName.length() > 0 && isMoreDebug == 'y')
                qDebug() << currentDirectory + ((currentName.length() > 0)? "  @  " + currentName : "");
        }

        // ...
        // remoobing a directory
        if (line.lastIndexOf('}') >= 0){
            QVector<int> parsedIDs0;
            QVector<float> parsedWeight0;
            currentDirectory = currentDirectory.mid(0, currentDirectory.lastIndexOf('/'));
            // depend on parseType add a new struct from parsed data

            //...
            char prevWasNegative  = 'y';

            switch (parseType){
                case 0: // parse vertexes from giant array
                    currentParseSplited = currentParse.split(',');
                    for (int parsedPoint = 0; parsedPoint < currentParseSplited.length() / 3; parsedPoint ++)
                        loadedVertexes << Matrix<Derivable,1,3>(Derivable(QStringToFloat(currentParseSplited[parsedPoint * 3])),
                                                    Derivable(QStringToFloat(currentParseSplited[parsedPoint * 3 + 1])),
                                                    Derivable(QStringToFloat(currentParseSplited[parsedPoint * 3 + 2])));
                    //
                    break;
                case 1:
                    currentParseSplited = currentParse.split(',');
                    prevWasNegative = 'y';
                    for (int parseIndex = 0; parseIndex < currentParseSplited.length(); parseIndex ++)
                    {
                        int parsedIndex = QStringToInt(currentParseSplited[parseIndex]);
                        if (prevWasNegative == 'y')
                            loadedPolygonStartIndexes << parseIndex;
                        prevWasNegative = (parsedIndex < 0)? 'y' : 'n';
                        loadedVertexIndexes << ((parsedIndex < 0)? - parsedIndex - 1 : parsedIndex);
                    }
                    loadedPolygonStartIndexes << currentParseSplited.length() - 1;
                    break;
                case 5:
                    loadedJoints << lastJointCreated;
                    break;
                case 9: // parse vertexes from giant array int
                    currentParseSplited = currentParse.remove(0, currentParse.indexOf("{") + 1).split(',');
                    for (int parsedID = 0; parsedID < currentParseSplited.length(); parsedID ++)
                        parsedIDs0 << QStringToInt(currentParseSplited[parsedID]);
                    loadedClusterVertIndexes << parsedIDs0;
                    break;
                case 10:
                    currentParseSplited = currentParse.remove(0, currentParse.indexOf("{") + 1).split(',');
                    for (int parsedWei = 0; parsedWei < currentParseSplited.length(); parsedWei ++)
                        parsedWeight0 << QStringToFloat(currentParseSplited[parsedWei]);
                    loadedClusterVertWeightes << parsedWeight0;
                    break;
                default:
                    break;
            }

            //clear parse type
            parseType = -1;
            currentParse = "";
        }
        // ....
        // 0 == mesh parse vertexes
        // 1 == mesh parse polygonstarts

        // ....
        // 5 == limb main info parse
        // ...
        // 8 == cluster matrix trasform
        // 9 == cluster indexes
        // 10 == cluster weightes
        // ...
        // 11 == bindpose parse
        // ...
        // 15 == bone_to_bone_connection
        // 16 == cluster_to_bone_connection

        // CONDITION
        if (prevLine.indexOf("Vertices:")>=0 || parseType == 0)
            parseType = 0;
        if (prevLine.indexOf("PolygonVertexIndex") >= 0 || parseType == 1)
            parseType = 1;
        if ((prevLine.indexOf("Model") == 0 && prevLine.indexOf("LimbNode") >= 0) || parseType == 5)
            parseType = 5;
        if ((currentDirectory.indexOf("Deformer") >= 0 && line.indexOf("Transform:") >= 0)|| parseType == 8)
            parseType = 8;
        if ((currentDirectory.indexOf("Deformer") >= 0 && line.indexOf("Indexes:") >= 0)|| parseType == 9)
            parseType = 9;
        if ((currentDirectory.indexOf("Deformer") >= 0 && line.indexOf("Weights") >= 0)|| parseType == 10)
            parseType = 10;
        if (currentDirectory.indexOf("/Pose/PoseNode")>=0 || parseType == 11)
            parseType = 11;
        if (prevLine.indexOf("Model::") == 1 && prevLine.indexOf("Model::",8) > 0)
            parseType = 15;
        if (prevLine.indexOf("Model::") == 1 && prevLine.indexOf("SubDeformer::",8) > 0)
            parseType = 16;

        //PARSING
        // || currentDirectory.indexOf("/Weights") == currentDirectory.length() - 8)
        if (parseType == 0 || parseType == 1 ||
            parseType == 9 || parseType == 10 )
            // remember all to a currentParse
            currentParse += (line.indexOf("a: ") == 0)? line.remove(0, 3) : line;

        // limb parse
        if (line.indexOf("LimbNode") >= 0)
            lastJointCreated = new Joint(currentID, line.mid(line.indexOf("::") + 2, line.indexOf("\",") - line.indexOf("::") - 2));

        if (parseType == 5 && line.indexOf("Lcl") >= 0){
                currentParseSplited = line.split(',');
                Matrix<Derivable,1,3> parsedVect;
                if (line.indexOf("Translation") >= 0 || line.indexOf("Rotation") >= 0 || line.indexOf("Scaling") >= 0)
                    parsedVect = Matrix<Derivable,1,3>(Derivable(QStringToFloat(currentParseSplited[currentParseSplited.length() - 3])),
                                           Derivable(QStringToFloat(currentParseSplited[currentParseSplited.length() - 2])),
                                           Derivable(QStringToFloat(currentParseSplited[currentParseSplited.length() - 1])));
                // redirect it to new limbs
                if (line.indexOf("Translation") >= 0)
                    lastJointCreated->currentTranslation = parsedVect;
                if (line.indexOf("Rotation") >= 0)
                    lastJointCreated->currentRotation = parsedVect;
                if (line.indexOf("Scaling") >= 0);
                    // IGNORE THIS
                //qDebug() << lastJointCreated.ID << lastJointCreated.name;
            }
        if (line.indexOf("\"Cluster\"") > 0)
            loadedClusterID << currentID;
        // checking bindpose loler
        // now we know the index of joint, which will be bindmatrixed next
        if (parseType == 11 && line.indexOf("Node: ") == 0){
            QString jointID = line.remove(0,6);
            nextJointUseInd = -1;
            for (int curJoint = 0; curJoint < loadedJoints.length(); curJoint ++)
                if (loadedJoints[curJoint]->ID == jointID)
                { nextJointUseInd = curJoint; break; }
        }

        if (parseType == 8 && line.indexOf("a: ") == 0){
            currentParseSplited = line.remove(0,3).split(',');
            bindTransformFromClusters << Matrix<Derivable,1,3>(Derivable(QStringToFloat(currentParseSplited[12])),
                                                   Derivable(QStringToFloat(currentParseSplited[13])),
                                                   Derivable(QStringToFloat(currentParseSplited[14])));
            bindMatrixesGeted++;
        }

        if (parseType == 11 && prevLine.indexOf("Matrix: ")>=0 && nextJointUseInd >= 0){
            currentParseSplited = line.remove(0,3).split(',');
            // bindmatrix read
            QVector<float> temp = {};
            for (int cur = 0; cur < currentParseSplited.length(); cur++)
                temp << QStringToFloat(currentParseSplited[cur]);
            // we have 16 values of Matrix4x4
            // go and apply
            QMatrix4x4 lastBindMatrix =
            //loadedJoints[nextJointUseInd].bindMatrix =
                    QMatrix4x4(temp[0],temp[1],temp[2],temp[3],
                               temp[4],temp[5],temp[6],temp[7],
                               temp[8],temp[9],temp[10],temp[11],
                               temp[12],temp[13],temp[14],temp[15]);
            // ......kostil......
            QVector3D extractedScales = QVector3D(
                      lastBindMatrix.column(0).toVector3D().length(),
                      lastBindMatrix.column(1).toVector3D().length(),
                      lastBindMatrix.column(2).toVector3D().length());
            // out a scale
            QMatrix4x4 finmat = //loadedJoints[nextJointUseInd]->bindMatrix =
                   QMatrix4x4(temp[0]/extractedScales[0],temp[1]/extractedScales[1],temp[2]/extractedScales[2],temp[3],
                              temp[4]/extractedScales[0],temp[5]/extractedScales[1],temp[6]/extractedScales[2],temp[7],
                              temp[8]/extractedScales[0],temp[9]/extractedScales[1],temp[10]/extractedScales[2],temp[11],
                              0,0,0,temp[15]);
            loadedJoints[nextJointUseInd]->bindMatrix = SetDeriveMatrix(finmat);
            // ......kostil......
            // geted a binder in translation and rotate
        }
        // connect limb to limb
        if (parseType == 15)
        {
            currentParseSplited = line.split(',');
            int childInd = -1, parentInd = -1;
            for (int jointInd = 0; jointInd < loadedJoints.length(); jointInd++)
            {
                if (loadedJoints[jointInd]->ID == currentParseSplited[currentParseSplited.length() - 1])
                    parentInd = jointInd;
                if (loadedJoints[jointInd]->ID == currentParseSplited[currentParseSplited.length() - 2])
                    childInd = jointInd;
            }
            if (childInd >= 0 && parentInd >= 0 && childInd != parentInd)  // we have a pair
            {
                limbsConnected ++;
                if (isMoreDebug == 'y')
                    qDebug () << ("    V    Limbs connected child->parent " + QString::number(childInd) + " -> " + QString::number(parentInd));
                loadedJoints[childInd]->pater = (loadedJoints[parentInd]);
                loadedJoints[parentInd]->kids << (loadedJoints[childInd]);
            }else{
                if (isMoreDebug == 'y')
                    qDebug() << "    X    Limbs connection failed: " + line;
            }
            parseType = -1;
        }
        // connect limb to cluster
            if (parseType == 16)
            {
                currentParseSplited = line.split(',');
                int boneInd = -1, clusterInd = -1;
                for (int jointInd = 0; jointInd < loadedJoints.length(); jointInd++)
                    if (loadedJoints[jointInd]->ID == currentParseSplited[currentParseSplited.length() - 2])
                    { boneInd = jointInd; break; }
                for (int clustInd = 0; clustInd < loadedClusterID.length(); clustInd ++)
                    if (loadedClusterID[clustInd] == currentParseSplited[currentParseSplited.length() - 1])
                    { clusterInd = clustInd; break; }

                if (boneInd >= 0 && clusterInd >= 0)  // we have a pair
                {
                    limbToClusterConnected ++;
                    if (isMoreDebug == 'y')
                        qDebug () << ("    V    Limb connected with cluster " + QString::number(boneInd) + " <- " + QString::number(clusterInd));
                    loadedJoints[boneInd]->bindTransform = bindTransformFromClusters[clusterInd];
                    resSkin->addInfo(boneInd, loadedClusterVertIndexes[clusterInd], loadedClusterVertWeightes[clusterInd]);
                }else{
                    if (isMoreDebug == 'y')
                        qDebug() << "    X    Limb and cluster connection failed: " + line;
                }
                parseType = -1;
            }
    }

    // >.........................................................................
    // post work to apply bindmatrix data

    Skeleton* resSkeleton = new Skeleton(loadedJoints);
    QVector<Matrix<Derivable,1,3>> jointsGlobalCoords = QVector<Matrix<Derivable,1,3>>();
    Matrix<Derivable,1,3> par;
    //
    for (int curJoint = 0; curJoint < loadedJoints.length(); curJoint++)
        jointsGlobalCoords << resSkeleton->getJointCoordByIndex(curJoint, par);
    // binding
    for (int curJoint = 0; curJoint < loadedJoints.length(); curJoint++){
        Joint* last = (loadedJoints[curJoint]);
        //
        Matrix<Derivable,4,4> usemat = SetDeriveMatrix();
        if (last->pater != NULL){
            usemat = last->pater->bindMatrix/*.inverted()*/;
            usemat = usemat.reverse();
        }

        Matrix<Derivable,1,4> temp =
                Matrix<Derivable,1,4>(last->currentTranslation.x(), last->currentTranslation.y(), last->currentTranslation.z(), 1.0) * usemat;
        // kostil for djepa
        if (last->pater == NULL)
            temp = MakeVector4From3(-last->bindTransform, Derivable(0));
        loadedJoints[curJoint]->currentTranslation = Matrix<Derivable,1,3>(temp(0,0), temp(0,1), temp(0,2));
        loadedJoints[curJoint]->localTranslation = loadedJoints[curJoint]->currentTranslation;
    }
    // >.........................................................................


    // final mesh transforms
    resMesh->vertexes = loadedVertexes;
    resMesh->polygonIndexes = loadedVertexIndexes;
    resMesh->polygonStartIndexes = loadedPolygonStartIndexes;

    // final skeleton update
    //for (int curJoint = 0; curJoint < loadedJoints.length(); curJoint++)
    resSkeleton = new Skeleton(loadedJoints);
    // here is everything gut
    // success
    Q_ASSERT(loadedClusterID.length() == loadedClusterVertIndexes.length() && loadedClusterVertIndexes.length() == loadedClusterVertWeightes.length());
    qDebug() << "    V    " + QString::number(limbsConnected) + " from " + QString::number(loadedJoints.length())+" joints are connected;";
    qDebug() << "    V    " + QString::number(limbToClusterConnected) +" from "+QString::number(loadedClusterID.length())+ " clusters are connected to bones;";
    qDebug() << "    V    " + QString::number(bindMatrixesGeted) + " bind matrixes connected to bones;";


    resSkeleton->CalculateGlobalCoordForEachJointMatrix();
    resSkin->GenerateAttends(resMesh->vertexes, resSkeleton->getJointsGlobalTranslationsForSkin());

    loadedRig = Rig(resMesh, resSkeleton, resSkin);
    return QString();
}

QString loaderFBX::loadMeshOBJAdress(QString path, Mesh &loadedMesh)
{
    QString errMessage = "Can not load OBJ from \"" + path + "\"";

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return errMessage;

    QTextStream stream(&file);
    QString err = loadMeshOBJ(stream, loadedMesh);
    //return error with file adress | return emptystring == success
    if (err.isEmpty())
        qDebug() << "    @    " + path.remove(0,path.lastIndexOf('/') + 1) + " was loaded;";
    return ((!err.isEmpty())? errMessage + ": " + err : QString());
}

QString loaderFBX::loadMeshOBJ(QTextStream &textStream, Mesh &loadedMesh)
{
    if (textStream.atEnd())
        return "Empty file FBX loaded";

    QString line;
    QStringList currentParseSplited;
    QVector<Matrix<Derivable,1,3>> loadedVertexes;
    QVector<int> loadedPolygonIndexes;
    QVector<int> loadedPolygonStartIndexes;

    while (!textStream.atEnd()){
        line = textStream.readLine();
        if (line.indexOf("v ") == 0){
            currentParseSplited = line.remove(0, 2).split(' ');
            Q_ASSERT(currentParseSplited.length() == 3);
            loadedVertexes << Matrix<Derivable,1,3>(QStringToFloat(currentParseSplited[0]),
                                        QStringToFloat(currentParseSplited[1]),
                                        QStringToFloat(currentParseSplited[2]));
        }
        if (line.indexOf("f ") == 0){
            currentParseSplited = line.remove(0, 2).split(' ');
            for (int curInd = 0; curInd < currentParseSplited.length(); curInd++){
                int polygonInd = QStringToInt(currentParseSplited[curInd].split('/')[0]) - 1;
                loadedPolygonIndexes << polygonInd;
                if (curInd == 0)
                    loadedPolygonStartIndexes << loadedPolygonIndexes.length() - 1;
            }
        }
    }
    QVector<Matrix<Derivable,1,3>> loadedDerV3s = QVector<Matrix<Derivable,1,3>>();

    Derivable scale = 1.2;
    Matrix<Derivable,1,3> trans = Matrix<Derivable,1,3>(10,20,-30);
    for (int i = 0; i < loadedVertexes.length(); i++)
        loadedDerV3s << Matrix<Derivable,1,3>(
                    scale * Derivable(loadedVertexes[i].x()) + trans(0,0),
                    scale * Derivable(loadedVertexes[i].y()) + trans(0,1),
                    scale * Derivable(loadedVertexes[i].z()) + trans(0,2));
    loadedMesh.vertexes = loadedDerV3s;
    loadedMesh.polygonIndexes = loadedPolygonIndexes;
    loadedMesh.polygonStartIndexes = loadedPolygonStartIndexes;
    return QString("");
}
