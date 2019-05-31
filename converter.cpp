#include "converter.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
using namespace std;
//Messy code but it works :#
class android_config{
public:
    bool analogx_enable = false;
    bool colorfulmusic_enable = false;
    bool convolver_enable = false;
    bool cure_enable = false;
    bool diffsurr_enable = false;
    bool enable = true;
    bool fetcompressor_autoattack = true;
    bool fetcompressor_autoknee = true;
    bool fetcompressor_autorelease = true;
    bool fetcompressor_autogain = true;
    bool fetcompressor_enable = false;
    bool fetcompressor_noclipenable = true;
    bool fidelity_bass_enable = false;
    bool fireq_enable = false;
    bool fidelity_clarity_enable = false;
    bool playbackgain_enable = false;
    bool reverb_enable = false;
    bool tube_enable = false;
    bool vhs_enable = false;
    bool vse_enable = false;
    QString analogx_mode = "0";
    QString channelpan = "0";
    QString colorfulmusic_coeffs = "120;200";
    QString colorfulmusic_midimage = "150";
    QString convolver_crosschannel = "0";
    QString cure_crossfeed = "0";
    QString diffsurr_delay = "500";
    QString fetcompressor_adapt = "50";
    QString fetcompressor_attack = "20";
    QString fetcompressor_crest = "20";
    QString fetcompressor_gain = "0";
    QString fetcompressor_knee = "0";
    QString fetcompressor_kneemulti = "0";
    QString fetcompressor_maxattack = "80";
    QString fetcompressor_maxrelease = "100";
    QString fetcompressor_ratio = "100";
    QString fetcompressor_release = "50";
    QString fetcompressor_threshold = "100";
    QString fidelity_bass_freq = "40";
    QString fidelity_bass_gain = "50";
    QString fidelity_bass_mode = "0";
    QString fidelity_clarity_gain = "50";
    QString fidelity_clarity_mode = "0";
    QString fireq_custom = "0.0;0.0;0.0;0.0;0.0;0.0;0.0;0.0;0.0;0.0;";
    QString limiter = "100";
    QString outvol = "100";
    QString playbackgain_maxscaler = "400";
    QString playbackgain_ratio = "50";
    QString playbackgain_volume = "80";
    QString reverb_damp = "0";
    QString reverb_dry = "50";
    QString reverb_roomsize = "0";
    QString reverb_roomwidth = "0";
    QString reverb_wet = "0";
    QString vhs_qual = "0";
    QString vse_value = "56";

    bool found_ddc = false;
    bool found_dynsys = false;
    bool found_irs = false;
    bool found_spkopt = false;
};
converter::converter()
{
}

//Returns an error description or the converted config
string converter::read(string path){
    android_config *conf = new android_config();
    int errorLine, errorColumn;
    QString errorMsg;
    QFile modelFile(QString::fromStdString(path));
    QDomDocument document;
    if (!document.setContent(&modelFile, &errorMsg, &errorLine, &errorColumn))
    {
        QString error("Syntax error line %1, column %2:\n%3");
        error = error
                .arg(errorLine)
                .arg(errorColumn)
                .arg(errorMsg);
        qDebug() << error;
        return error.toUtf8().constData();
    }

    QString prefix = "viper4android.headphonefx";
    QDomElement root = document.documentElement();
    QDomNodeList key = root.elementsByTagName("boolean");
    for(int i = 0; i < key.count(); i++)
    {

        QDomNode keynode = key.at(i);
        if(keynode.isElement())
        {
            QDomElement att = keynode.toElement();

            if (att.attribute("name") == prefix + ".analogx.enable")conf->analogx_enable = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".colorfulmusic.enable")conf->colorfulmusic_enable = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".cure.enable")conf->cure_enable = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".diffsurr.enable")conf->diffsurr_enable = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".enable")conf->enable = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".fetcompressor.autoattack")conf->fetcompressor_autoattack = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".fetcompressor.autogain")conf->fetcompressor_autogain= att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".fetcompressor.autoknee")conf->fetcompressor_autoknee = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".fetcompressor.autorelease")conf->fetcompressor_autorelease = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".fetcompressor.enable")conf->fetcompressor_enable = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".fetcompressor.noclipenable")conf->fetcompressor_noclipenable = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".fidelity.bass.enable")conf->fidelity_bass_enable = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".fidelity.clarity.enable")conf->fidelity_clarity_enable = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".fireq.enable")conf->fireq_enable = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".playbackgain.enable")conf->playbackgain_enable = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".reverb.enable")conf->reverb_enable = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".tube.enable")conf->tube_enable = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".vhs.enable")conf->vhs_enable = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".vse.enable")conf->vse_enable = att.attribute("value") == "true";
            else if (att.attribute("name") == prefix + ".convolver.enable"){
                conf->convolver_enable = att.attribute("value") == "true";
                conf->found_irs = true;
            }
            else if (att.attribute("name") == prefix + ".dynamicsystem.enable")conf->found_dynsys = true;
            else if (att.attribute("name") == prefix + ".viperddc.enable")conf->found_ddc = true;
            else if (att.attribute("name") == "viper4android.speakerfx.spkopt.enable")conf->found_spkopt = true;
        }
    }

    QDomNodeList key_str = root.elementsByTagName("string");
    for(int i = 0; i < key_str.count(); i++)
    {
        QDomNode keynode = key_str.at(i);
        if(keynode.isElement())
        {
            QDomElement att = keynode.toElement();
            if (att.attribute("name") == prefix + ".analogx.mode")conf->analogx_mode = att.text();
            else if (att.attribute("name") == prefix + ".channelpan")conf->channelpan = att.text();
            else if (att.attribute("name") == prefix + ".colorfulmusic.coeffs")conf->colorfulmusic_coeffs = att.text();
            else if (att.attribute("name") == prefix + ".colorfulmusic.midimage")conf->colorfulmusic_midimage = att.text();
            else if (att.attribute("name") == prefix + ".convolver.crosschannel")conf->convolver_crosschannel = att.text();
            else if (att.attribute("name") == prefix + ".cure.crossfeed")conf->cure_crossfeed = att.text();
            else if (att.attribute("name") == prefix + ".diffsurr.delay")conf->diffsurr_delay = att.text();
            else if (att.attribute("name") == prefix + ".fetcompressor.adapt")conf->fetcompressor_adapt = att.text();
            else if (att.attribute("name") == prefix + ".fetcompressor.attack")conf->fetcompressor_attack = att.text();
            else if (att.attribute("name") == prefix + ".fetcompressor.crest")conf->fetcompressor_crest = att.text();
            else if (att.attribute("name") == prefix + ".fetcompressor.gain")conf->fetcompressor_gain = att.text();
            else if (att.attribute("name") == prefix + ".fetcompressor.knee")conf->fetcompressor_knee = att.text();
            else if (att.attribute("name") == prefix + ".fetcompressor.kneemulti")conf->fetcompressor_kneemulti = att.text();
            else if (att.attribute("name") == prefix + ".fetcompressor.maxattack")conf->fetcompressor_maxattack = att.text();
            else if (att.attribute("name") == prefix + ".fetcompressor.maxrelease")conf->fetcompressor_maxrelease = att.text();
            else if (att.attribute("name") == prefix + ".fetcompressor.ratio")conf->fetcompressor_ratio = att.text();
            else if (att.attribute("name") == prefix + ".fetcompressor.release")conf->fetcompressor_release = att.text();
            else if (att.attribute("name") == prefix + ".fetcompressor.threshold")conf->fetcompressor_threshold = att.text();
            else if (att.attribute("name") == prefix + ".fidelity.bass.freq")conf->fidelity_bass_freq = att.text();
            else if (att.attribute("name") == prefix + ".fidelity.bass.gain")conf->fidelity_bass_gain = att.text();
            else if (att.attribute("name") == prefix + ".fidelity.bass.mode")conf->fidelity_bass_mode = att.text();
            else if (att.attribute("name") == prefix + ".fidelity.clarity.gain")conf->fidelity_clarity_gain = att.text();
            else if (att.attribute("name") == prefix + ".fidelity.clarity.mode")conf->fidelity_clarity_mode = att.text();
            else if (att.attribute("name") == prefix + ".fireq.custom")conf->fireq_custom = att.text();
            else if (att.attribute("name") == prefix + ".limiter")conf->limiter = att.text();
            else if (att.attribute("name") == prefix + ".outvol")conf->outvol = att.text();
            else if (att.attribute("name") == prefix + ".playbackgain.maxscaler")conf->playbackgain_maxscaler = att.text();
            else if (att.attribute("name") == prefix + ".playbackgain.ratio")conf->playbackgain_ratio = att.text();
            else if (att.attribute("name") == prefix + ".playbackgain.volume")conf->playbackgain_volume = att.text();
            else if (att.attribute("name") == prefix + ".reverb.damp")conf->reverb_damp = att.text();
            else if (att.attribute("name") == prefix + ".reverb.dry")conf->reverb_dry = att.text();
            else if (att.attribute("name") == prefix + ".reverb.roomsize")conf->reverb_roomsize = att.text();
            else if (att.attribute("name") == prefix + ".reverb.roomwidth")conf->reverb_roomwidth = att.text();
            else if (att.attribute("name") == prefix + ".reverb.wet")conf->reverb_wet = att.text();
            else if (att.attribute("name") == prefix + ".vhs.qual")conf->vhs_qual = att.text();
            else if (att.attribute("name") == prefix + ".vse.value")conf->vse_value = att.text();

        }

    }
    string out = "";
    string n = "\n";

    out += "cure_enable=";
    if(conf->cure_enable)out += "true" + n;
    else out += "false" + n;
    out += "cure_level=";
    out += conf->cure_crossfeed.toUtf8().constData() + n;

    out += "ax_enable=";
    if(conf->analogx_enable)out += "true" + n;
    else out += "false" + n;
    out += "ax_mode=";
    out += conf->analogx_mode.toUtf8().constData() + n;

    out += "vse_enable=";
    if(conf->vse_enable)out += "true" + n;
    else out += "false" + n;

    out += "vse_ref_bark=7600\n"; //Hardcoded on android
    out += "vse_bark_cons=";
    out += to_string((int)round(conf->vse_value.toFloat() * 5.6f * 100.0f)) + n; //Used same calculation as in the android app
    out += "conv_enable=";
    if(conf->convolver_enable)out += "true" + n;
    else out += "false" + n;
    out += "conv_cc_level=";
    out += conf->convolver_crosschannel.toUtf8().constData() + n;

    out += "fetcomp_enable=";
    if(conf->fetcompressor_enable)out += "true" + n;
    else out += "false" + n;

    out += "fetcomp_autogain=";
    if(conf->fetcompressor_autogain)out += "true" + n;
    else out += "false" + n;
    out += "fetcomp_autoknee=";
    if(conf->fetcompressor_autoknee)out += "true" + n;
    else out += "false" + n;
    out += "fetcomp_autoattack=";
    if(conf->fetcompressor_autoattack)out += "true" + n;
    else out += "false" + n;
    out += "fetcomp_autorelease=";
    if(conf->fetcompressor_autorelease)out += "true" + n;
    else out += "false" + n;

    out += "fetcomp_noclip=";
    if(conf->fetcompressor_noclipenable)out += "true" + n;
    else out += "false" + n;

    out += "fetcomp_threshold=";
    out += conf->fetcompressor_threshold.toUtf8().constData() + n;
    out += "fetcomp_gain=";
    out += conf->fetcompressor_gain.toUtf8().constData() + n;
    out += "fetcomp_kneewidth=";
    out += conf->fetcompressor_knee.toUtf8().constData() + n;
    out += "fetcomp_ratio=";
    QString fc_r(conf->fetcompressor_ratio);
    int fc_ri = fc_r.toInt();
    out += to_string((int)(fc_ri / 10)) + n;

    out += "fetcomp_attack=";
    out += conf->fetcompressor_attack.toUtf8().constData() + n;
    out += "fetcomp_release=";
    out += conf->fetcompressor_release.toUtf8().constData() + n;
    out += "fetcomp_meta_adapt=";
    out += conf->fetcompressor_adapt.toUtf8().constData() + n;
    out += "fetcomp_meta_crest=";
    out += conf->fetcompressor_crest.toUtf8().constData() + n;
    out += "fetcomp_meta_maxattack=";
    out += conf->fetcompressor_maxattack.toUtf8().constData() + n;
    out += "fetcomp_meta_maxrelease=";
    out += conf->fetcompressor_maxrelease.toUtf8().constData() + n;
    out += "fetcomp_meta_kneemulti=";
    out += conf->fetcompressor_kneemulti.toUtf8().constData() + n;


    out += "eq_enable=";
    if(conf->fireq_enable)out += "true" + n;
    else out += "false" + n;

    char* eq_str = strtok(conf->fireq_custom.toLocal8Bit().data(), ";");
    int count = 0;
    while (eq_str != nullptr) { //EQ Values are split by a semicolon on android
        if (count == 0)out += "eq_band1=";
        else if (count == 1)out += "eq_band2=";
        else if (count == 2)out += "eq_band3=";
        else if (count == 3)out += "eq_band4=";
        else if (count == 4)out += "eq_band5=";
        else if (count == 5)out += "eq_band6=";
        else if (count == 6)out += "eq_band7=";
        else if (count == 7)out += "eq_band8=";
        else if (count == 8)out += "eq_band9=";
        else if (count == 9)out += "eq_band10=";
        else break;

        QString qEq(eq_str);
        double temp = qEq.toDouble();
        out += to_string((int)(temp * 100)) + n;

        eq_str = strtok (nullptr, ";");
        count++;
    }

    //BASS
    out += "vb_enable=";
    if(conf->fidelity_bass_enable)out += "true" + n;
    else out += "false" + n;
    out += "vb_mode=";
    out += conf->fidelity_bass_mode.toUtf8().constData() + n;
    out += "vb_freq=";
    out += conf->fidelity_bass_freq.toUtf8().constData() + n;
    out += "vb_gain=";
    out += conf->fidelity_bass_gain.toUtf8().constData() + n;

    //HEADSET ENGINE
    out += "vhe_enable=";
    if(conf->vhs_enable)out += "true" + n;
    else out += "false" + n;
    out += "vhe_level=";
    out += conf->vhs_qual.toUtf8().constData() + n;

    //DIFFERENTIAL SOUND
    out += "ds_enable=";
    if(conf->diffsurr_enable)out += "true" + n;
    else out += "false" + n;
    out += "ds_level=";
    QString ds(conf->diffsurr_delay);
    int dsl = ds.toInt();
    out += to_string((int)(dsl / 20)) + n;

    //REVERB
    out += "reverb_enable=";
    if(conf->reverb_enable)out += "true" + n;
    else out += "false" + n;
    out += "reverb_roomsize=";
    out += conf->reverb_roomsize.toUtf8().constData() + n;
    out += "reverb_width=";
    out += conf->reverb_roomwidth.toUtf8().constData() + n;
    out += "reverb_damp=";
    out += conf->reverb_damp.toUtf8().constData() + n;
    out += "reverb_wet=";
    out += conf->reverb_wet.toUtf8().constData() + n;
    out += "reverb_dry=";
    out += conf->reverb_dry.toUtf8().constData() + n;

    //TUBE SIMULATOR
    out += "tube_enable=";
    if(conf->tube_enable)out += "true" + n;
    else out += "false" + n;

    //COLORFUL
    out += "colm_enable=";
    if(conf->colorfulmusic_enable)out += "true" + n;
    else out += "false" + n;
    out += "colm_midimage=";
    out += conf->colorfulmusic_midimage.toUtf8().constData() + n;

    char* colm_wd = strtok(conf->colorfulmusic_coeffs.toLocal8Bit().data(), ";");
    int count_colm = 0;
    while (colm_wd != nullptr) { //Widening and Depth is split by a semicolon on android
        if (count_colm == 0)out += "colm_widening=";
        else if (count_colm == 1)out += "colm_depth=";
        else break;
        out +=  colm_wd + n;
        colm_wd = strtok (nullptr, ";");
        count_colm++;
    }

    //CLARITY
    out += "vc_enable=";
    if(conf->fidelity_clarity_enable)out += "true" + n;
    else out += "false" + n;
    out += "vc_mode=";
    out += conf->fidelity_clarity_mode.toUtf8().constData() + n;
    out += "vc_level=";
    out += conf->fidelity_clarity_gain.toUtf8().constData() + n;

    //GAIN CONTROL
    out += "agc_enable=";
    if(conf->playbackgain_enable)out += "true" + n;
    else out += "false" + n;
    out += "agc_ratio=";
    out += conf->playbackgain_ratio.toUtf8().constData() + n;
    out += "agc_maxgain=";
    out += conf->playbackgain_maxscaler.toUtf8().constData() + n;
    out += "agc_volume=";
    out += conf->playbackgain_volume.toUtf8().constData() + n;

    //MASTER
    out += "lim_threshold=";
    QString lim(conf->limiter);
    int liml = lim.toInt();
    out += to_string((int)(liml / 10)) + n;
    out += "out_pan=";
    out += conf->channelpan.toUtf8().constData() + n;
    out += "out_volume=";
    QString vol(conf->outvol);
    int voll = vol.toInt();
    out += to_string((int)(voll / 2)) + n;


    string info = "";
    if(conf->found_ddc) info += "Viper DDC not supported\n";
    if(conf->found_dynsys) info += "Dynamic System not supported\n";
    if(conf->found_spkopt) info += "Speaker Optimization not supported\n";
    if(conf->found_irs) info += "IRS must be manually reselected\n";
    string resp = out + "|" + info;
    return resp;
}
string converter::intToString(int i){
    stringstream ss;
    ss << i;
    return ss.str();
}
