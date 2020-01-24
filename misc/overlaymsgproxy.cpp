#include "overlaymsgproxy.h"
#include "dialog/qmessageoverlay.h"

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QGridLayout>
#include <QPushButton>

OverlayMsgProxy::OverlayMsgProxy()
{

}

void OverlayMsgProxy::openError(QWidget* obj, QString title, QString desc, QString close){
    openBase(obj,title,desc,":/icons/error.svg",close,"#d72828");
}
void OverlayMsgProxy::openNormal(QWidget* obj, QString title, QString desc, QString color){
    openBase(obj,title,desc,"",tr("Close"),color);
}
void OverlayMsgProxy::openBase(QWidget* obj, QString title, QString desc, QString icon, QString close, QString color){
    QMessageOverlay *lightBox = new QMessageOverlay(obj,false);

    QLabel* lbTitle = new QLabel(title);
    lbTitle->setStyleSheet("font-size: 24px; font-weight: bold; color: white");
    QLabel* lbIcon = new QLabel;
    lbIcon->setPixmap(QPixmap(icon));
    QLabel* lbDescription = new QLabel(desc);
    lbDescription->setStyleSheet("color: white");
    lbDescription->setWordWrap(true);
    QPushButton* lbClose = new QPushButton(close);

    QGridLayout* lbLayout = new QGridLayout;
    lbLayout->setRowStretch(0, 1);
    lbLayout->setColumnStretch(0, 1);
    lbLayout->addWidget(lbTitle, 1, 1);
    lbLayout->addWidget(lbTitle, 1, 1);
    lbLayout->addWidget(lbIcon, 1, 2, Qt::AlignRight);
    lbLayout->setColumnStretch(3, 1);
    lbLayout->addWidget(lbDescription, 2, 1, 1, 2);
    lbLayout->addWidget(lbClose, 3, 2);
    lbLayout->setRowStretch(4, 1);

    QFile f(":/overlay.qss");
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream ts(&f);
    lbClose->setStyleSheet(ts.readAll().replace("#e67e22",color));

    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect();
    lightBox->setGraphicsEffect(eff);
    lightBox->show();

    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(500);
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::InBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);

    connect(lbClose, &QPushButton::clicked, lightBox, [lightBox,lbClose](){
        lbClose->setEnabled(false);
        QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect();
        lightBox->setGraphicsEffect(eff);
        QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
        a->setDuration(500);
        a->setStartValue(1);
        a->setEndValue(0);
        a->setEasingCurve(QEasingCurve::OutBack);
        a->start(QPropertyAnimation::DeleteWhenStopped);
        connect(a,&QAbstractAnimation::finished, [lightBox](){
            lightBox->hide();
        });
    });
    lightBox->setLayout(lbLayout);
}