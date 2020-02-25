#include "animatedvipericon.h"

AnimatedViperIcon::AnimatedViperIcon(QWidget *parent)
    : QWidget(parent)
{
    svgItem->setGeometry(0,0,64,64);
    svgItemU->setGeometry(0,0,64,64);
    svgItemL->setGeometry(0,0,64,64);
    this->setGeometry(0,0,64,64);
    this->setFixedSize(64,64);

    svgItemU->hide();
    svgItemL->hide();

    group = new QParallelAnimationGroup(this);
    QPropertyAnimation *upper = new QPropertyAnimation(svgItemU, "geometry");
    upper->setDuration(1000);
    upper->setEasingCurve(QEasingCurve::Type::OutExpo);
    upper->setStartValue(QRect(0, -64, 64, 64));
    upper->setEndValue(QRect(0, 0, 64, 64));
    group->addAnimation(upper);
    QPropertyAnimation *lower = new QPropertyAnimation(svgItemL, "geometry");
    lower->setDuration(1000);
    lower->setEasingCurve(QEasingCurve::Type::OutExpo);
    lower->setStartValue(QRect(0, 128, 64, 64));
    lower->setEndValue(QRect(0, 0, 64, 64));
    group->addAnimation(lower);
}

AnimatedViperIcon::~AnimatedViperIcon()
{
    delete svgItem;
    delete svgItemU;
    delete svgItemL;
}

void AnimatedViperIcon::startAnimation()
{
    if(group->state() == QAbstractAnimation::Running)
        group->stop();
    group->start();
    svgItemU->show();
    svgItemL->show();
}

