#ifndef IOFBX_FBXNODE_H
#define IOFBX_FBXNODE_H

#include <QString>
#include <QVector>

namespace IOfbx {

const bool showWarningsAndErrors = false;

void traceMessage(const QString mess);

class FbxNode
{
public:
    virtual QString parse(const QStringList &buffer, const int param) = 0;

    template<typename T>
    static QVector<T> parseFbxArray(QStringList S, QString &error);

    template<typename T>
    static QVector<T> parseLastXValues(const QString S, const int X, QString &error);

    void setNameAndID (const QString name, const QString ID);
    bool hasNameAndID () const;
    virtual void traceInfo() const;
    virtual bool isEmpty() const;
protected:
    QString id;
    QString name;
};

}








//IMPLEMENTATIONS

template<typename T>
QVector<T> IOfbx::FbxNode::parseFbxArray(QStringList S, QString &error)
{
    traceMessage (QString("o   Message: Called array parser"));
    error = QString();
    QVector<T> readenNumbers = QVector<T>();

    if (S[0].indexOf("a: ") < 0)
        traceMessage ("+   Warning: array parser is using for non-array value.");

    for (int lineIndex = 0; lineIndex < S.length(); lineIndex++){
        S[lineIndex] = S[lineIndex].trimmed();                                  // remove tabs
        if (S[lineIndex].indexOf("a: ") == 0) S[lineIndex] = S[lineIndex].remove(0,3);  // remove a:
        if (S[lineIndex][S[lineIndex].length() - 1] == ',') S[lineIndex].remove(S[lineIndex].length() - 2, 1) ;

        QVector<T> fromLine = parseLastXValues<T>(S[lineIndex], -1, error);
        if (!error.isEmpty())
        {return readenNumbers;}
        for (int valInd = 0; valInd < fromLine.length(); valInd++)
            readenNumbers << fromLine[valInd];
    }
    traceMessage ( "v   Success: array parser read "
                +QString::number( readenNumbers.length()) + " values;");
    return readenNumbers;
}

template<typename T>
QVector<T> IOfbx::FbxNode::parseLastXValues(const QString S, const int X, QString &error)
{
    QVector<T> readenNumbers = QVector<T>();
    error = "";
    if (S.indexOf(',') < 0)
    {error = "Line does not have any \",\""; return readenNumbers;}

    QStringList splitedNumbers = S.split(',');
    // case of incorrect format
    if (splitedNumbers.length() <= 0)
    {error = "Line " + S + " does not contain any T values!"; return readenNumbers;}
    if (splitedNumbers.length() < X)
    {error = "Line " + S + " does not contain at least "+X+" values! ( it only contains " + splitedNumbers.length() + " )"; return readenNumbers;}

    // okay, the line is splited, so it is time to parse
    bool numberParsedCorrectly = false;
    int count = X;
    if (count < 0) count = splitedNumbers.length();
    for (int numIndex = splitedNumbers.length() - count;
         numIndex < splitedNumbers.length();
         numIndex++)
    {
        // remove different tabs and spaces
        splitedNumbers[numIndex] = splitedNumbers[numIndex].trimmed();
        if (splitedNumbers[numIndex] != "")
        {
            const auto parsedValue = T(splitedNumbers[numIndex].toDouble(&numberParsedCorrectly));
            if (!numberParsedCorrectly)
            {error = "Can not parse a value from  \""
                     +splitedNumbers[numIndex] + "\"!" ; return readenNumbers;}
            readenNumbers << parsedValue;
        }
    }
    return readenNumbers;
}

#endif // IOFBX_FBXNODE_H
