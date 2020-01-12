#ifndef STYLEHELPER_H
#define STYLEHELPER_H
#include <QObject>
#include <QColor>

class StyleHelper
{
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
};

#endif // STYLEHELPER_H
