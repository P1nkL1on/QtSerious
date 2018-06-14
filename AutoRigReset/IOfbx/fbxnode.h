#ifndef IOFBX_FBXNODE_H
#define IOFBX_FBXNODE_H

#include <QString>
#include <QVector>

namespace IOfbx {

const bool showWarningsAndErrors = false;

void traceMessage(const QString &mess);


template<typename Numerical>
Numerical fromString(const QString &string, bool &isOk);

template<>
int fromString<int>(const QString &string, bool &isOk);
template<>
float fromString<float>(const QString &string, bool &isOk);
template<>
double fromString<double>(const QString &string, bool &isOk);

class FbxNode
{
public:
    virtual QString parse(const QStringList &buffer) = 0;

    template<typename T>
    static QVector<T> parseFbxArray(const QStringList &buffer, QString &error);

    template<typename T>
    static QVector<T> parseLastXValues(const QString &line, const int X, QString &error);

    void setNameAndID(const QString &name, const QString &ID);
    bool hasNameAndID() const;
    virtual void traceInfo() const;
    virtual bool isEmpty() const;

protected:
    QString id;
    QString name;
};

}


//IMPLEMENTATIONS

template<typename T>
QVector<T> IOfbx::FbxNode::parseFbxArray(const QStringList &buffer, QString &error)
{
    traceMessage(QString("o   Message: Called array parser"));
    error = QString();
    QVector<T> readenNumbers;

    if (buffer[0].indexOf("a: ") < 0)
        traceMessage("+   Warning: array parser is using for non-array value.");

    for (int lineIndex = 0; lineIndex < buffer.length(); lineIndex++){
        QString line = buffer[lineIndex].trimmed();                                  // remove tabs
        if (line.indexOf("a: ") == 0) line = line.remove(0,3);  // remove a:
        if (line[line.length() - 1] == ',') line.remove(line.length() - 2, 1) ;

        QVector<T> fromLine = parseLastXValues<T>(line, -1, error);
        if (!error.isEmpty())
            return readenNumbers;

        for (int valInd = 0; valInd < fromLine.length(); ++valInd)
            readenNumbers << fromLine[valInd];
    }

    traceMessage ( "v   Success: array parser read "
                + QString::number( readenNumbers.length()) + " values;");
    return readenNumbers;
}

template<>
inline int IOfbx::fromString<int>(const QString &string, bool &isOk)
{
    return string.toInt(&isOk);
}

template<>
inline float IOfbx::fromString<float>(const QString &string, bool &isOk)
{
    return string.toFloat(&isOk);
}

template<>
inline double IOfbx::fromString<double>(const QString &string, bool &isOk)
{
    return string.toDouble(&isOk);
}

template<typename T>
QVector<T> IOfbx::FbxNode::parseLastXValues(const QString &line, const int X, QString &error)
{
    QVector<T> readenNumbers;
    error = "";
    if (line.indexOf(',') < 0){
        error = "Line does not have any \",\"";
        return readenNumbers;
    }

    QStringList splitedNumbers = line.split(',');
    // case of incorrect format
    if (splitedNumbers.length() <= 0){
        error = "Line " + line + " does not contain any T values!";
        return readenNumbers;
    }
    if (splitedNumbers.length() < X){
        error = QString("Line %1 does not contain at least %2 values! ( it only contains %3 )")
                .arg(line)
                .arg(X)
                .arg(splitedNumbers.length());
        return readenNumbers;
    }

    // okay, the line is splited, so it is time to parse
    bool isNumberParsedCorrectly = false;
    int count = X;
    if (count < 0)
        count = splitedNumbers.length();
    for (int numIndex = splitedNumbers.length() - count; numIndex < splitedNumbers.length(); ++numIndex){
        // remove different tabs and spaces
        splitedNumbers[numIndex] = splitedNumbers[numIndex].trimmed();
        if (splitedNumbers[numIndex] != ""){
            const auto parsedValue = fromString<T>(splitedNumbers[numIndex], isNumberParsedCorrectly);
            if (!isNumberParsedCorrectly){
                error = "Can not parse a value from  \""
                     +splitedNumbers[numIndex] + "\"!" ;
                return readenNumbers;
            }
            readenNumbers << parsedValue;
        }
    }
    return readenNumbers;
}

#endif // IOFBX_FBXNODE_H
