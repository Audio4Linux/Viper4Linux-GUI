#include "stylehelper.h"
#include "ui_viper_window.h"
#include "viper_window.h"
#include "config/appconfigwrapper.h"

#include "dialog/settingsdlg.h"

#include <QTextStream>
#include <QApplication>
#include <QStyleFactory>

#ifndef VIPER_PLUGINMODE
#include "phantom/phantomstyle.h"
#define SET_STYLE(x) QApplication::setStyle(new x)
#define SET_STYLE_S(x) QApplication::setStyle(x)
#define SET_PALETTE(x) QApplication::setPalette(x)
#define SET_STYLESHEET(x) qApp->setStyleSheet(x)
#else
#define SET_STYLE(x) m_host->setStyle(new x)
#define SET_STYLE_S(x) m_host->setStyle(QStyleFactory::create(x))
#define SET_PALETTE(x) m_host->setPalette(x)
#define SET_STYLESHEET(x) m_host->setStyleSheet(x)
#endif

StyleHelper::StyleHelper(QWidget* host){
    m_objhost = host;
}
void StyleHelper::SetStyle(){
    ViperWindow* m_host = qobject_cast<ViperWindow*>(m_objhost);
    AppConfigWrapper* m_appconf = m_host->getACWrapper();
#ifndef VIPER_PLUGINMODE
    if(m_appconf->getTheme() == "Phantom")
        SET_STYLE(PhantomStyle);
    else
#endif
        SET_STYLE_S(m_appconf->getTheme());

    QString color_palette = m_appconf->getColorpalette();

    auto palettes = ColorStyleProvider::TABLE();
    if(color_palette=="custom"){
        QColor base = QColor(loadColor(0,0),loadColor(0,1),loadColor(0,2));
        QColor background = QColor(loadColor(1,0),loadColor(1,1),loadColor(1,2));
        QColor foreground = QColor(loadColor(2,0),loadColor(2,1),loadColor(2,2));
        QColor selection = QColor(loadColor(3,0),loadColor(3,1),loadColor(3,2));
        QColor disabled = QColor(loadColor(4,0),loadColor(4,1),loadColor(4,2));
        QColor selectiontext = QColor(255-loadColor(3,0),255-loadColor(3,1),255-loadColor(3,2));
        ColorStyle cs = ColorStyle(m_appconf->getWhiteIcons(),
                                   base,background,foreground,selection,selectiontext,disabled);

        setPalette(cs);
        loadIcons(m_appconf->getWhiteIcons());
    }
    else if(palettes.contains(color_palette)){
        ColorStyle currentColorStyle = palettes[color_palette];
        setPalette(currentColorStyle);
        loadIcons(currentColorStyle.useWhiteIcons);
    }
    else{
        loadIcons(false);
        SET_PALETTE(m_host->style()->standardPalette());
        QFile f(":/default.qss");
        if (!f.exists())printf("Unable to set stylesheet, file not found\n");
        else
        {
            f.open(QFile::ReadOnly | QFile::Text);
            QTextStream ts(&f);
            SET_STYLESHEET(ts.readAll());
        }
    }

    emit styleChanged();
}
void StyleHelper::setPalette(const ColorStyle& s){
    ViperWindow* m_host = qobject_cast<ViperWindow*>(m_objhost);

    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Window, s.background);
    palette->setColor(QPalette::WindowText, s.foreground);
    palette->setColor(QPalette::Base, s.base);
    palette->setColor(QPalette::AlternateBase, s.background);
    palette->setColor(QPalette::ToolTipBase, s.background);
    palette->setColor(QPalette::ToolTipText, s.foreground);
    palette->setColor(QPalette::Text, s.foreground);
    palette->setColor(QPalette::Button, s.background);
    palette->setColor(QPalette::ButtonText, s.foreground);
    palette->setColor(QPalette::BrightText, Qt::red);
    palette->setColor(QPalette::Link, QColor(42, 130, 218));
    palette->setColor(QPalette::Highlight, s.selection);
    palette->setColor(QPalette::HighlightedText, s.selectiontext);
    SET_PALETTE(*palette);
    SET_STYLESHEET(QString(R"(QFrame[frameShape="4"], QFrame[frameShape="5"]{
                                color: gray;
                                }
                                *::disabled {
                                color: %1;
                                }
                                QToolButton::disabled{
                                color: %1;

                                }
                                QComboBox::disabled{
                                color: %1;
                                })").arg(s.disabled.name()));
}
void StyleHelper::loadIcons(bool white){
    ViperWindow* m_host = qobject_cast<ViperWindow*>(m_objhost);
    if(white){
        m_host->settings_dlg->updateButtonStyle(true);
        QPixmap pix(":/icons/settings-white.svg");
        QIcon icon(pix);
        QPixmap pix2(":/icons/queue-white.svg");
        QIcon icon2(pix2);
        QPixmap pix3(":/icons/menu-white.svg");
        QIcon icon3(pix3);
        m_host->ui->set->setIcon(icon);
        m_host->ui->cpreset->setIcon(icon2);
        m_host->ui->toolButton->setIcon(icon3);
    }else{
        m_host->settings_dlg->updateButtonStyle(false);
        QPixmap pix(":/icons/settings.svg");
        QIcon icon(pix);
        QPixmap pix2(":/icons/queue.svg");
        QIcon icon2(pix2);
        QPixmap pix3(":/icons/menu.svg");
        QIcon icon3(pix3);
        m_host->ui->set->setIcon(icon);
        m_host->ui->cpreset->setIcon(icon2);
        m_host->ui->toolButton->setIcon(icon3);
    }
}
int StyleHelper::loadColor(int index,int rgb_index){
    ViperWindow* m_host = qobject_cast<ViperWindow*>(m_objhost);
    AppConfigWrapper* m_appconf = m_host->getACWrapper();
    QStringList elements = m_appconf->getCustompalette().split(';');
    if(elements.length()<5||elements[index].split(',').size()<3){
        if(index==0)return 40;
        else if(index==1)return 29;
        else if(index==2)return 255;
        else if(index==3)return 190;
        else if(index==4)return 90;
    }
    QStringList rgb = elements[index].split(',');
    return rgb[rgb_index].toInt();
}

#undef SET_STYLE
#undef SET_STYLE_S
#undef SET_PALETTE
#undef SET_STYLESHEET
