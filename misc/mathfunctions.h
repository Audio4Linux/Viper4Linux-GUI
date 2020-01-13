#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H
#include <cmath>
#include <QString>

class MathFunctions
{
private:
    static float CompMathA(float f, float f2, float f3) {
        return (float) exp(std::log((double) f2) + (((double) f) * (std::log((double) f3) - std::log((double) f2))));
    }
    static float CompMathB(float f, float f2, float f3) {
        return ((f3 - f2) * f) + f2;
    }
public:
    static QString buildCompressorUnitString(int mode, float f){
        //Mode: 0-Threshold, 1-Ratio, 2-Knee, 3-Gain, 4-Atk, 5-Rel, 6-Kneemulti, 7-MaxAtk, 8-MaxRel, 9-Crest, 10-Adapt
        float orig = f;
        long double in;
        f = f/100;
        switch (mode) {
        case 0:
            in = log10(pow(10.0L, ((double) (-60.0f * f)) / 20.0L)) * 20.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "dB (" + QString::number(orig) + ")";
        case 1:
            if(f>0.99f)return "\u221E:1";
            in = 1.0L / (1.0L - ((double) f));
            return QString::number(roundf((in)*100)/100,'f',2) + ":1 (" + QString::number(orig) + ")";
        case 2:
            in = log10(pow(10.0L, ((double) (60.0f * f)) / 20.0L)) * 20.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "dB (" + QString::number(orig) + ")";
        case 3:
            in = log10(pow(10.0L, ((double) (60.0f * f)) / 20.0L)) * 20.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "dB (" + QString::number(orig) + ")";
        case 4:
            in = ((double) CompMathA(f, 1.0E-4f, 0.2f)) * 1000.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "ms (" + QString::number(orig) + ")";
        case 5:
            in = ((double) CompMathA(f, 0.005f, 2.0f)) * 1000.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "ms (" + QString::number(orig) + ")";
        case 6:
            in = ((double) CompMathB(f, 0.0f, 4.0f));
            return QString::number(roundf((in)*100)/100,'f',2) + "x (" + QString::number(orig) + ")";
        case 7:
            in = ((double) CompMathA(f, 1.0E-4f, 0.2f)) * 1000.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "ms (" + QString::number(orig) + ")";
        case 8:
            in = ((double) CompMathA(f, 0.005f, 2.0f)) * 1000.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "ms (" + QString::number(orig) + ")";

        case 9:
            in = log10(pow(10.0L, ((double) (60.0f * f)) / 20.0L)) * 20.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "dB (" + QString::number(orig) + ")";
        case 10:
            in = ((double) CompMathA(f, 1.0f, 4.0f)) * 1000.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "ms (" + QString::number(orig) + ")";
        }
        return "E: Mode out of range";
    }
    static QString buildEqGainString(int f){
        QString pre("");
        if(f < 0 ) pre = "-";

        QString s;
        if(QString::number(abs(f)%100).length()==1)
        {
            char buffer[5];
            snprintf(buffer, sizeof(buffer), "%02d", abs(f)%100);
            s = pre + QString::number(abs(f)/100) + "."  + QString::fromUtf8(buffer) + "dB";
        }
        else
            s = pre + QString::number(abs(f)/100) + "."  + QString::number(abs(f%100)) + "dB";
        return s;
    }

};
#endif // MATHFUNCTIONS_H
