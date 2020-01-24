/*
 *  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  ThePBone <tim.schneeberger(at)outlook.de> (c) 2020
 */
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
