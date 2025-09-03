#ifndef CSVPROCESSOR_H
#define CSVPROCESSOR_H

#include <QString>
#include <QRegularExpression>
#include "../twodimvector.h"

class CSVProcessor
{
private:
    QString header; // contains column names if the are any
public:
    bool writeCSVFile(const QString& data, QString filename);
    QString readCSVFile(QString filename);
    TwoDimVector<double> parseFromCSV(QString data);
    QString parseToCSV(const TwoDimVector<double>& data);

    inline QString getHeader() { return header; }
};

#endif // CSVPROCESSOR_H
