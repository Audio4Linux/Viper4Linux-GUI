#include "overlaymsgproxy.h"
#include "dialog/qmessageoverlay.h"

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>

OverlayMsgProxy::OverlayMsgProxy()
{

}

void OverlayMsgProxy::openError(QWidget* obj, QString title, QString desc){
    openBase(obj,title,desc,":/icons/error.svg");
}
void OverlayMsgProxy::openNormal(QWidget* obj, QString title, QString desc){
    openBase(obj,title,desc,"");
}
void OverlayMsgProxy::openBase(QWidget* obj, QString title, QString desc, QString icon){
    QMessageOverlay *lightBox = new QMessageOverlay(obj,false);

    QLabel* lbTitle = new QLabel(title);
    lbTitle->setStyleSheet("font-size: 24px; font-weight: bold; color: white");
    QLabel* lbIcon = new QLabel;
    lbIcon->setPixmap(QPixmap(icon));
    QLabel* lbDescription = new QLabel(desc);
    lbDescription->setStyleSheet("color: white");
    lbDescription->setWordWrap(true);
    QPushButton* lbClose = new QPushButton(tr("Close"));

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
    lbClose->setStyleSheet("QPushButton{ border-style: solid; border-top-color: transparent; border-right-color: transparent; border-left-color: transparent; border-bottom-color: transparent; border-width: 1px; border-style: solid; padding: 4 12 4 12; background-color: #000000; } QPushButton::default{ border-style: solid; border-top-color: transparent; border-right-color: transparent; border-left-color: transparent; border-bottom-color: #d72828; border-width: 1px; padding: 2px; background-color: #000000; } QPushButton:hover{ border-style: solid; border-top-color: transparent; border-right-color: transparent; border-left-color: transparent; border-bottom-color: #d72828; border-bottom-width: 1px; border-style: solid; color: #FFFFFF; padding-bottom: 2px; background-color: #000000; } QPushButton:pressed{ border-style: solid; border-top-color: transparent; border-right-color: transparent; border-left-color: transparent; border-bottom-color: #d72828; border-bottom-width: 2px; border-style: solid; color: #d72828; padding-bottom: 1px; background-color: #000000; } QPushButton:disabled{ border-style: solid; border-top-color: transparent; border-right-color: transparent; border-left-color: transparent; border-bottom-color: transparent; border-bottom-width: 2px; border-style: solid; color: #808086; padding-bottom: 1px; background-color: #000000; }");

    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect();
    lightBox->setGraphicsEffect(eff);
    lightBox->show();

    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(500);
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::InBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);

    connect(lbClose, &QPushButton::clicked, lightBox, [lightBox](){
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
