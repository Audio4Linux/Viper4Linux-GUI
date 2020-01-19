#ifndef CONVERTER_H
#define CONVERTER_H
#include <QString>
#include <QDomElement>
typedef struct conversion_result_s{
    QString configuration;
    QString notices;
    bool error;
    bool found_ddc;
    bool found_irs;
    bool found_spkopt;
}conversion_result_t;
class converter
{
public:
    enum configtype{
        officialV4A,
        teamDeWittV4A
    };
    static conversion_result_t toLinux(const QString&,configtype);
    static conversion_result_t toAndroid(const QString&,configtype);
private:
    static QString boolToQString(bool b);
    static QDomElement generateXmlEntry(QDomDocument*,QString type,const QString& name,const QString& value);
};

#endif // CONVERTER_H
