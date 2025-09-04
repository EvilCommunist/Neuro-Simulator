#include "csvprocessor.h"
#include <QFile>

QString CSVProcessor::readCSVFile(QString filename){
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "\0";

    QString fileData = "";
    QTextStream in(&file);
    // __trying to initialize header_______________________
    QString firstLine = in.readLine();
    QRegularExpression headerRegex("[X,D,E][0-9]+");
    QRegularExpressionMatch match = headerRegex.match(firstLine);
    if(match.hasMatch()){
        header = firstLine;
    }else{
        fileData += firstLine;
        fileData+="\n";
        header = "\0";
    }
    // __trying to initialize header_______________________

    while (!in.atEnd()) {
        fileData += in.readLine();
        fileData += "\n";
    }
    return fileData;
}

bool CSVProcessor::writeCSVFile(const QString& data, QString filename){
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out << data;
    return true;
}


TwoDimVector<double> CSVProcessor::parseFromCSV(QString data){
    data = data.trimmed();
    QStringList linesOfData = data.split("\n");
    TwoDimVector<double> res((linesOfData[0].size()+1)/2, linesOfData.size(), 0); // line = 8;8;8
    size_t cur_i = 0, cur_j = 0;
    for(auto line : linesOfData){
        if (line[line.size()-1]==";"){
            line.append("\n");
            line.remove(QRegularExpression("[;]{1}\n"));
        }
        auto symbols = line.split(";");
        for(auto symbol : symbols){
            res.setValue(cur_j++, cur_i, symbol.toDouble());
        }
        cur_i ++;
        cur_j = 0;
    }

    return res;
}

QString CSVProcessor::parseToCSV(const TwoDimVector<double>& data){
    QString res = "";
    for(size_t i = 0; i < data.getHeight(); i++){
        for(size_t j = 0; j < data.getWidth(); j++){
            res.append(QString::number(data.getValue(j, i)));
            if(j < data.getHeight()-1)
                res.append(";");
        }
        res.append("\n");
    }
    return res;
}
