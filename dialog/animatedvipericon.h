#ifndef WIDGET_H
#define WIDGET_H

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSvgWidget>

class AnimatedViperIcon : public QWidget
{
    Q_OBJECT

public:
    AnimatedViperIcon(QWidget *parent = nullptr);
    ~AnimatedViperIcon();
    void startAnimation();
private:
    QSvgWidget* svgItem = new QSvgWidget(":/icons/parts/viper.svg",this);
    QSvgWidget* svgItemU = new QSvgWidget(":/icons/parts/upper.svg",this);
    QSvgWidget* svgItemL = new QSvgWidget(":/icons/parts/lower.svg",this);
    QParallelAnimationGroup *group;
};
#endif // WIDGET_H
