#include "fbxnode.h"
#include <QDebug>

void IOfbx::FbxNode::setNameAndID(const QString &name, const QString &ID)
{
    this->name = name;
    this->id = ID;
}

bool IOfbx::FbxNode::hasNameAndID() const
{
    return (!name.isEmpty() && !id.isEmpty());
}

void IOfbx::FbxNode::traceInfo() const
{
    traceMessage ( "Name: " + name + ";   ID: " + id + ";");
}

bool IOfbx::FbxNode::isEmpty() const
{
    return !hasNameAndID();
}

void IOfbx::traceMessage(const QString &mess)
{
    if (IOfbx::showWarningsAndErrors || mess.indexOf('!') == 0)
        qDebug() << mess;
}
