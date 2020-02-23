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
#ifndef CONFIGCONTAINER_H
#define CONFIGCONTAINER_H

#include <QObject>
#include <QVariant>

class ConfigContainer
{
public:
    ConfigContainer();
    void setValue(QString key,QVariant value);
    QVariant getVariant(QString key);
    QString getString(QString key,bool setToDefaultIfMissing=true);
    int getInt(QString key);
    float getFloat(QString key);
    bool getBool(QString key, bool setToDefaultIfMissing=true);

    QVariantMap getConfigMap();
    void setConfigMap(QVariantMap newmap);

private:
    QVariantMap map = QVariantMap();

};

#endif // CONFIGCONTAINER_H
