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
#ifndef PRESETEXTENSION_H
#define PRESETEXTENSION_H
#include <QString>
#include <QMap>
#include <QVector>
#include "initializableqmap.h"

#define FLOAT_LIST QVector<float>
#define INT_LIST QVector<int>

#define EQ_UNIT QString,FLOAT_LIST
#define DYNSYS_UNIT QString,INT_LIST
#define COLM_UNIT QString,INT_LIST

namespace PresetProvider {
class EQ {
public:
    static const FLOAT_LIST defaultPreset();
    static const FLOAT_LIST lookupPreset(QString preset);
    static const QMap<EQ_UNIT> EQ_LOOKUP_TABLE();
    static const QString reverseLookup(QVector<float> data);
};
class Dynsys{
    public:
    static const QMap<DYNSYS_UNIT> DYNSYS_LOOKUP_TABLE();
    static const INT_LIST lookupPreset(QString preset);
    static const QString reverseLookup(QVector<int> data);
};
class Colm{
    public:
    static const QMap<COLM_UNIT> COLM_LOOKUP_TABLE();
    static const INT_LIST lookupPreset(QString preset);
};
}
#endif // PRESETEXTENSION_H
