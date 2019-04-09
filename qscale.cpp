/*
 *   Copyright 2008-2012 Meinert Jordan <meinert@gmx.at>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "qscale.h"
#include <QtGui/QPainter>
#include <cmath>

QScale::QScale(QWidget *parent)
    : QWidget(parent)
{
    m_minimum = 0;
    m_maximum = 100;

    m_value = 0;

    m_labelsVisible = true;
    m_scaleVisible = true;

    m_borderWidth = 6;

    m_labelsFormat = 'g';
    m_labelsPrecision = -1;

    m_majorStepSize = 0;
    m_minorStepCount = -1;

    m_invertedAppearance = false;

    m_orientations = (Qt::Horizontal | Qt::Vertical);

    pi = acos(-1);

    setBackgroundRole(QPalette::Base);

    updateLabelSample();
    setMinimumSize(80,60);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QScale::~QScale()
= default;

void QScale::setMinimum(double min)
{
    if(min < INFINITY && min > -INFINITY)
        m_minimum = min;
    updateLabelSample();
    update();
}

double QScale::minimum() const
{
    return m_minimum;
}

void QScale::setMaximum(double max)
{
    if(max < INFINITY && max > -INFINITY)
        m_maximum = max;
    updateLabelSample();
    update();
}

double QScale::maximum() const
{
   return m_maximum;
}

void QScale::setRange(double min, double max)
{
    if(max < INFINITY && max > -INFINITY)
        m_maximum = max;
    if(min < INFINITY && min > -INFINITY)
        m_minimum = min;
    updateLabelSample();
    update();
}

void QScale::setValue(double value)
{
   m_value = value;
   update();
}

double QScale::value() const
{
   return m_value;
}

void QScale::setLabelsVisible(bool visible)
{
    m_labelsVisible = visible;
    update();
}

bool QScale::isLabelsVisible() const
{
    return m_labelsVisible;
}

void QScale::setScaleVisible(bool visible)
{
    m_scaleVisible = visible;
    update();
}

bool QScale::isScaleVisible() const
{
    return m_scaleVisible;
}

void QScale::setBorderWidth(int width)
{
    m_borderWidth = qMin(width, 0);
    update();
}

int QScale::borderWidth() const
{
    return m_borderWidth;
}

void QScale::setLabelsFormat(char format, int precision)
{
    m_labelsFormat = format;
    m_labelsPrecision = precision;
    updateLabelSample();
    update();
}

void QScale::setMajorStepSize(double stepSize)
{
    m_majorStepSize = stepSize;
    update();
}

double QScale::majorStepSize() const
{
    return m_majorStepSize;
}

void QScale::setMinorStepCount(int stepCount)
{
    m_minorStepCount = stepCount;
    update();
}

int QScale::minorStepCount() const
{
    return m_minorStepCount;
}

void QScale::setInvertedAppearance(bool invert)
{
    m_invertedAppearance = invert;
    update();
}

bool QScale::invertedAppearance() const
{
    return m_invertedAppearance;
}

void QScale::setOrientations(Qt::Orientations orientations)
{
    m_orientations = orientations;
    update();
}

Qt::Orientations QScale::orientations() const
{
    return m_orientations;
}

void QScale::resizeEvent(QResizeEvent *re)
{
    QWidget::resizeEvent(re);
}

void QScale::paintEvent(QPaintEvent*)
{
    painter = new QPainter(this);

    bool vertical;
    int wWidget, hWidget;
    double wLabel, hLabel;
    double wScale, hScale;
    double radius;
    double angleSpan;
    double angleStart;
    double valueSpan;
    double majorStep;
    int minorSteps;
    int order;
    QRectF boundingRect;
    QPointF center;
    QPolygon polygon;

    double u;

    if(!(m_orientations & Qt::Vertical) ^ !(m_orientations & Qt::Horizontal))
        vertical = (bool)(m_orientations & Qt::Vertical);
    else
        vertical = (height() > width());

    wWidget = width();
    hWidget = height();

    painter->setRenderHint(QPainter::Antialiasing, true);

    boundingRect = painter->boundingRect(QRectF(0, 0, width(), height()),
                               Qt::AlignBottom | Qt::AlignHCenter, labelSample);
    wLabel = (m_labelsVisible)? boundingRect.width() : 0;
    hLabel = (m_labelsVisible)? boundingRect.height() : 0;

    if(vertical){
        qSwap(wWidget, hWidget);
        qSwap(wLabel, hLabel);
    }

    wScale = wWidget - wLabel - 2. * m_borderWidth;
    hScale = .5 * hWidget - hLabel - m_borderWidth;

    radius = .125 * pow( wScale, 2) / hScale + .5 * hScale;
    if(radius < hScale + .5* hWidget - m_borderWidth){
        radius = (4. * (hLabel + m_borderWidth) +
                  sqrt(4. * pow(hLabel + m_borderWidth, 2) +
                       3. * pow(wScale, 2))
                  ) / 3. - hLabel - 2. * m_borderWidth;
        center = QPointF(.5 * wWidget, hWidget - m_borderWidth);
    }
    else
        center = QPointF(.5 * wWidget, radius + hLabel + m_borderWidth);

    angleSpan = - 360 / pi * asin(wScale/(2. * radius));
    angleStart = 90. - .5 * angleSpan;

    valueSpan = m_maximum - m_minimum;
    majorStep = fabs(valueSpan) * qMax(wLabel, 1.5 * boundingRect.height()) /
                                                                         wScale;
    order = 0;
    while(majorStep < 1)
        majorStep *= 10, order--;
    while(majorStep >= 10)
        majorStep /= 10, order++;
    if(majorStep > 5)
        majorStep = 10 * pow(10, order), minorSteps = 5;
    else if(majorStep > 2)
        majorStep = 5 * pow(10, order), minorSteps = 5;
    else
        majorStep = 2 * pow(10, order), minorSteps = 4;
    if(m_majorStepSize > 0)
        majorStep = m_majorStepSize;
    if(m_minorStepCount > 0)
        minorSteps = m_minorStepCount;

    // draw scale
    painter->setPen(QPen(palette().color(QPalette::Text), 1));
    if(m_scaleVisible && majorStep != 0){
        if(vertical){
            painter->rotate(90);
            painter->translate(0, -hWidget + wLabel / 4);
        }
        painter->translate(center);
        painter->rotate(fmod(m_minimum, majorStep / minorSteps) /
                        valueSpan * angleSpan - angleStart);
        int offsetCount = (minorSteps - (int)ceil(fmod(m_minimum, majorStep) /
                                          majorStep * minorSteps)) % minorSteps;
        double scaleWidth = qMin(qMin(.25 * (hWidget - m_borderWidth),
                                    .25 * radius), 2.5 * boundingRect.height());
        for(int i = 0; i <= (int)(minorSteps * valueSpan / majorStep); i++){
            if(i % minorSteps == offsetCount)
                painter->drawLine(QLineF(radius - scaleWidth, 0, radius, 0));
            else
                painter->drawLine(QLineF(radius - scaleWidth, 0,
                                         radius - scaleWidth * 0.4, 0));
            painter->rotate(majorStep * angleSpan / (-valueSpan * minorSteps));
        }
        painter->resetMatrix();
    }

    // draw labels
    if(m_labelsVisible && majorStep != 0)
        for(int i = (int)ceil(m_minimum / majorStep);
            i <= (int)(m_maximum / majorStep); i++){
            u = pi/180 * ((majorStep * i - m_minimum) / valueSpan *
                                     angleSpan + angleStart);
            QRect position;
            Qt::Alignment align;
            if(vertical){
                align = Qt::AlignLeft | Qt::AlignVCenter;
                position = QRect(width() - center.y() + radius * sin(u), 0,
                                   width(), height() + 2 * radius * cos(u));
            }
            else{
                align = Qt::AlignHCenter | Qt::AlignBottom;
                position = QRect(0, 0, 2. * (center.x() + radius * cos(u)),
                                              center.y() - radius * sin(u));
            }
            painter->resetMatrix();
            painter->drawText(position, align, QString::number(i * majorStep,
                                            m_labelsFormat, m_labelsPrecision));
       }

    // draw needle
    if(vertical){
        painter->rotate(90);
        painter->translate(0, -hWidget + wLabel / 4);
    }
    painter->translate(center);
    painter->rotate((m_minimum - m_value) / valueSpan * angleSpan - angleStart);
    painter->setPen(Qt::NoPen);
    painter->setBrush(palette().color(QPalette::Text));
    polygon.setPoints(5, 0, -2, (int)radius - 10, -2, (int)radius, 0,
                      (int)radius - 10, 2, 0, 2);
    painter->drawConvexPolygon(polygon);
    painter->setPen(QPen(palette().color(QPalette::Base), 2));
    painter->drawLine(0, 0, radius - 15, 0);
    painter->resetMatrix();

    // draw cover
    painter->setPen(Qt::NoPen);
    painter->setBrush(palette().color(QPalette::Mid));

    if(vertical){
        painter->drawRect(QRect(0, 0, m_borderWidth, height()));
        center = QPointF(width() - center.y() - wLabel / 4, .5 * height());
        u = .25 * (hWidget - wLabel) - center.x() - m_borderWidth;
    }
    else{
        painter->drawRect(QRect(0, hWidget, wWidget, -m_borderWidth));
        u = center.y() - m_borderWidth - .75 * hWidget;
    }
    u = qMax(u, .25 * radius);
    painter->drawEllipse(center, u, u);

    delete painter;
}

void QScale::updateLabelSample()
{
    double margin = qMax(fabs(m_minimum), fabs(m_maximum));
    double wildcard = (qMin(m_minimum, m_maximum) < 0)? -8 : 8;
    while(margin < 1){
        margin *= 10;
        wildcard /= 10;
    }
    while(margin >= 10){
        margin /= 10;
        wildcard *= 10;
    }
    labelSample = QString::number(wildcard, m_labelsFormat, m_labelsPrecision);
}
