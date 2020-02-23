#include "qanimatedslider.h"
#include <QEvent>
/*
 *  MIT License

    Copyright (c) 2020 Tim Schneeberger (ThePBone) <tim.schneeberger(at)outlook.de>

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 *
 */

QAnimatedSlider::QAnimatedSlider(QWidget *parent)
    : QSlider(parent)
{
    anim = new QPropertyAnimation(this,"value");
    connect(this,&QSlider::sliderMoved,[this]{
        cValue = value();
        emit valueChangedA(cValue);
    });
}

QAnimatedSlider::~QAnimatedSlider()
{
}

void QAnimatedSlider::setValueA(int val, bool animate)
{
    cValue = val;
    if(animate){
        anim->stop();
        anim->setDuration(mDuration);
        anim->setEasingCurve(mEasingCurve);
        anim->setStartValue(QSlider::value());
        anim->setEndValue(val);
        anim->start();
    }
    else
        QSlider::setValue(val);
}

int QAnimatedSlider::valueA() const
{
    return cValue;
}

QEasingCurve QAnimatedSlider::easingCurve() const
{
    return mEasingCurve;
}

void QAnimatedSlider::setEasingCurve(const QEasingCurve &easingCurve)
{
    mEasingCurve = easingCurve;
}

int QAnimatedSlider::duration() const
{
    return mDuration;
}

void QAnimatedSlider::setDuration(int duration)
{
    mDuration = duration;
}



