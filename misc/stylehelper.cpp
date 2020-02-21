#include "stylehelper.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "config/appconfigwrapper.h"

#include <QTextStream>
#include <QApplication>

StyleHelper::StyleHelper(QObject* host){
    m_objhost = host;
}
void StyleHelper::SetStyle(){
    MainWindow* m_host = qobject_cast<MainWindow*>(m_objhost);
    AppConfigWrapper* m_appconf = m_host->getACWrapper();
    QApplication::setStyle(m_appconf->getTheme());
    QString style_sheet = m_appconf->getStylesheet();
    int theme_mode = m_appconf->getThememode();
    QString color_palette = m_appconf->getColorpalette();

    if(theme_mode==0){
        QApplication::setPalette(qApp->style()->standardPalette());
        QString stylepath = "";
        if (style_sheet=="blue")stylepath = ":darkblue/darkblue/darkblue.qss";
        else if (style_sheet=="amoled")stylepath = ":amoled/amoled/amoled.qss";
        else if (style_sheet=="aqua")stylepath = ":/aqua/aqua/aqua.qss";
        else if (style_sheet=="ubuntu")stylepath = ":/ubuntu/ubuntu/ubuntu.qss";
        else if (style_sheet=="vsdark")stylepath = ":/vsdark/vsdark/vsdark.qss";
        else if (style_sheet=="vslight")stylepath = ":/vslight/vslight/vslight.qss";
        else stylepath = ":/default.qss";
        m_host->ui->vb->setContentsMargins(4,4,4,4);
        QFile f(stylepath);
        if (!f.exists())printf("Unable to set stylesheet, file not found\n");
        else
        {
            f.open(QFile::ReadOnly | QFile::Text);
            QTextStream ts(&f);
            qApp->setStyleSheet(ts.readAll());
            if(style_sheet=="amoled" || style_sheet=="vsdark"){
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
    }else{
        loadIcons(false);
        if(color_palette=="dark"){
            QColor background = QColor(53,53,53);
            QColor foreground = Qt::white;
            QColor base = QColor(25,25,25);
            QColor selection = QColor(42, 130, 218);
            setPalette(base,background,foreground,selection,Qt::black);
        }else if(color_palette=="gray"){
            loadIcons(true);
            QColor background = QColor(49,49,74);
            QColor foreground = Qt::white;
            QColor base = QColor(83,83,125);
            QColor selection = QColor(85,85,127);
            setPalette(base,background,foreground,selection,Qt::black,QColor(144,144,179));
        }else if(color_palette=="white"){
            QColor background = Qt::white;
            QColor foreground = Qt::black;
            QColor base = Qt::white;
            QColor selection = QColor(56,161,227);
            setPalette(base,background,foreground,selection,Qt::black);
        }
        else if(color_palette=="blue"){
            loadIcons(true);
            QColor background = QColor(0,0,50);
            QColor foreground = Qt::white;
            QColor base = QColor(0,0,38);
            QColor selection = QColor(85,0,255);
            setPalette(base,background,foreground,selection,Qt::black);
        }
        else if(color_palette=="darkblue"){
            loadIcons(true);
            QColor background = QColor(19,25,38);
            QColor foreground = Qt::white;
            QColor base = QColor(14,19,29);
            QColor selection = QColor(70,79,89);
            setPalette(base,background,foreground,selection,Qt::black);
        }
        else if(color_palette=="honeycomb"){
            QColor background = QColor(212,215,208);
            QColor foreground = Qt::black;
            QColor base = QColor(185,188,182);
            QColor selection = QColor(243,193,41);
            setPalette(base,background,foreground,selection,Qt::white);
        }
        else if(color_palette=="black"){
            loadIcons(true);
            QColor background = QColor(16,16,16);
            QColor foreground = QColor(222,222,222);
            QColor base = Qt::black;
            QColor selection = QColor(132,132,132);
            setPalette(base,background,foreground,selection,Qt::black);
        }
        else if(color_palette=="darkgreen"){
            loadIcons(true);
            QColor background = QColor(27,34,36);
            QColor foreground = QColor(197,209,217);
            QColor base = QColor(30,30,30);
            QColor selection = QColor(21,67,58);
            setPalette(base,background,foreground,selection,Qt::black);
        }
        else if(color_palette=="green"){
            loadIcons(true);
            QColor background = QColor(0,12,0);
            QColor foreground = Qt::white;
            QColor base = QColor(6,29,12);
            QColor selection = QColor(86,191,121);
            setPalette(base,background,foreground,selection,Qt::black);
        }
        else if(color_palette=="stone"){
            loadIcons(true);
            QColor background = QColor(34,45,50);
            QColor foreground = Qt::white;
            QColor base = QColor(27,36,40);
            QColor selection = QColor(165,206,255);
            setPalette(base,background,foreground,selection,Qt::black);
        }
        else if(color_palette=="custom"){
            QColor base = QColor(loadColor(0,0),loadColor(0,1),loadColor(0,2));
            QColor background = QColor(loadColor(1,0),loadColor(1,1),loadColor(1,2));
            QColor foreground = QColor(loadColor(2,0),loadColor(2,1),loadColor(2,2));
            QColor selection = QColor(loadColor(3,0),loadColor(3,1),loadColor(3,2));
            QColor disabled = QColor(loadColor(4,0),loadColor(4,1),loadColor(4,2));
            QColor selectiontext = QColor(255-loadColor(3,0),255-loadColor(3,1),255-loadColor(3,2));

            setPalette(base,background,foreground,selection,selectiontext,disabled);
            loadIcons(m_appconf->getWhiteIcons());
        }
        else{
            QApplication::setPalette(m_host->style()->standardPalette());
            QFile f(":/default.qss");
            if (!f.exists())printf("Unable to set stylesheet, file not found\n");
            else
            {
                f.open(QFile::ReadOnly | QFile::Text);
                QTextStream ts(&f);
                qApp->setStyleSheet(ts.readAll());
            }
        }

    }
    emit styleChanged();
}
void StyleHelper::setPalette(const QColor& base,const QColor& background,const QColor& foreground,const QColor& selection,const QColor& selectiontext,const QColor& disabled){
    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Window, background);
    palette->setColor(QPalette::WindowText, foreground);
    palette->setColor(QPalette::Base, base);
    palette->setColor(QPalette::AlternateBase, background);
    palette->setColor(QPalette::ToolTipBase, background);
    palette->setColor(QPalette::ToolTipText, foreground);
    palette->setColor(QPalette::Text, foreground);
    palette->setColor(QPalette::Button, background);
    palette->setColor(QPalette::ButtonText, foreground);
    palette->setColor(QPalette::BrightText, Qt::red);
    palette->setColor(QPalette::Link, QColor(42, 130, 218));
    palette->setColor(QPalette::Highlight, selection);
    palette->setColor(QPalette::HighlightedText, selectiontext);
    qApp->setPalette(*palette);
    QString rgbdisabled = disabled.name();
    qApp->setStyleSheet("QFrame[frameShape=\"4\"], QFrame[frameShape=\"5\"]{ color: gray; }*::disabled { color: " + rgbdisabled +";}QToolButton::disabled { color: " + rgbdisabled +";}QComboBox::disabled { color: " + rgbdisabled +";}");
}
void StyleHelper::loadIcons(bool white){
    MainWindow* m_host = qobject_cast<MainWindow*>(m_objhost);
    if(white){
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
    MainWindow* m_host = qobject_cast<MainWindow*>(m_objhost);
    AppConfigWrapper* m_appconf = m_host->getACWrapper();
    QStringList elements = m_appconf->getCustompalette().split(';');
    if(elements.length()<5||elements[index].split(',').size()<3){
        if(index==0)return 25;
        else if(index==1)return 53;
        else if(index==2)return 255;
        else if(index==3){
            if(rgb_index==0)return 42;
            else if(rgb_index==1)return 130;
            else if(rgb_index==2)return 218;
        }
        else if(index==4) return 85;
    }
    QStringList rgb = elements[index].split(',');
    return rgb[rgb_index].toInt();
}
void StyleHelper::switchPalette(const QPalette& palette)
{
    MainWindow* m_host = qobject_cast<MainWindow*>(m_objhost);
    qApp->setPalette(palette);
    QList<QWidget*> widgets = m_host->findChildren<QWidget*>();
    foreach (QWidget* w, widgets)
        w->setPalette(palette);
}
