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
#ifndef STYLEHELPER_H
#define STYLEHELPER_H
#include <QObject>
#include <QColor>

class StyleHelper : public QObject
{
    Q_OBJECT
public:
    StyleHelper(QObject* host);
    void SetStyle();
    void loadIcons(bool white);
    int loadColor(int index,int rgb_index);
    void switchPalette(const QPalette& palette);
    void setPalette(const QColor& base,const QColor& background,
                                 const QColor& foreground,const QColor& selection = QColor(42,130,218),
                                 const QColor& selectiontext = Qt::black,const QColor& disabled = QColor(85,85,85));
private:
    QObject* m_objhost;
signals:
    void styleChanged();
};

#endif // STYLEHELPER_H
