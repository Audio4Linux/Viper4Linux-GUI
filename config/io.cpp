#include "io.h"

#include <QMessageBox>
#include <QRegularExpression>
#include <fstream>
#include <string>

void ConfigIO::writeFile(QString path,QVariantMap map){
    std::ofstream myfile(path.toUtf8().constData());
    if (myfile.is_open())
    {
        for(auto e : map.keys())
          myfile << e.toStdString() << "=" << map.value(e).toString().toStdString() << std::endl;
        myfile.close();
    }

}
QVariantMap ConfigIO::readFile(QString path,bool allowWarning){
    QVariantMap map;

    std::ifstream cFile(path.toUtf8().constData());
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)){
            if(QString::fromStdString(line).trimmed()[0] == '#' || line.empty()) continue; //Skip commented lines
            auto delimiterInlineComment = line.find('#'); //Look for config properties mixed up with comments
            auto extractedProperty = line.substr(0, delimiterInlineComment);
            auto delimiterPos = extractedProperty.find('=');
            auto name = extractedProperty.substr(0, delimiterPos);
            auto value = extractedProperty.substr(delimiterPos + 1);
            QString qname = QString::fromStdString(name).trimmed();
            QString qvalue = QString::fromStdString(value).trimmed();

            QRegExp re("\\d*");
            QRegExp re_float("[+-]?([0-9]*[.])?[0-9]+");
            if(qvalue=="true")
                map[qname] = QVariant(true);
            else if(qvalue=="false")
                map[qname] = QVariant(false);
            else if (re.exactMatch(qvalue))
                map[qname] = QVariant(qvalue.toInt());
            else if (re_float.exactMatch(qvalue))
                map[qname] = QVariant(qvalue.toFloat());
            else
                map[qname] = QVariant(qvalue);
        }
        cFile.close();
    }
    else if(allowWarning){
        QMessageBox msgBox;
        msgBox.setText(QString("Configuration file not found at \n%1").arg(path));
        msgBox.setInformativeText("You can change the path in the settings.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Icon::Critical);
        msgBox.exec();
    }

    return map;
}
