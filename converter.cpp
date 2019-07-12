#include "converter.h"
#include "configlist.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <fstream>
#include "main.h"
using namespace std;
//Messy code but it works :S
//Returns an error description or the converted linux config
string converter::toLinux(string path,configtype cmode){
    configmodel *conf = new configmodel();
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
            QString key = att.attribute("name");
            if (key == "65584" || key == prefix + ".analogx.enable")conf->analogx_enable = att.attribute("value") == "true";
            else if (key == "65553" || key == prefix + ".colorfulmusic.enable")conf->colorfulmusic_enable = att.attribute("value") == "true";
            else if (key == "65581" || key == prefix + ".cure.enable")conf->cure_enable = att.attribute("value") == "true";
            else if (key == "65557" || key == prefix + ".diffsurr.enable")conf->diffsurr_enable = att.attribute("value") == "true";
            else if (key == prefix + ".enable")conf->enable = att.attribute("value") == "true";
            else if (key == "65618" || key == prefix + ".fetcompressor.autoattack")conf->fetcompressor_autoattack = att.attribute("value") == "true";
            else if (key == "65616" || key == prefix + ".fetcompressor.autogain")conf->fetcompressor_autogain= att.attribute("value") == "true";
            else if (key == "65614" || key == prefix + ".fetcompressor.autoknee")conf->fetcompressor_autoknee = att.attribute("value") == "true";
            else if (key == "65620" || key == prefix + ".fetcompressor.autorelease")conf->fetcompressor_autorelease = att.attribute("value") == "true";
            else if (key == "65610" || key == prefix + ".fetcompressor.enable")conf->fetcompressor_enable = att.attribute("value") == "true";
            else if (key == "65626" || key == prefix + ".fetcompressor.noclipenable")conf->fetcompressor_noclipenable = att.attribute("value") == "true";
            else if (key == "65574" || key == prefix + ".fidelity.bass.enable")conf->fidelity_bass_enable = att.attribute("value") == "true";
            else if (key == "65578" || key == prefix + ".fidelity.clarity.enable")conf->fidelity_clarity_enable = att.attribute("value") == "true";
            else if (key == "65551" || key == prefix + ".fireq.enable")conf->fireq_enable = att.attribute("value") == "true";
            else if (key == "65565" || key == prefix + ".playbackgain.enable")conf->playbackgain_enable = att.attribute("value") == "true";
            else if (key == "65559" || key == prefix + ".reverb.enable")conf->reverb_enable = att.attribute("value") == "true";
            else if (key == "65583" || key == prefix + ".tube.enable")conf->tube_enable = att.attribute("value") == "true";
            else if (key == "65544" || key == prefix + ".vhs.enable")conf->vhs_enable = att.attribute("value") == "true";
            else if (key == "65548" || key == prefix + ".vse.enable")conf->vse_enable = att.attribute("value") == "true";
            else if (key == "65538" || key == prefix + ".convolver.enable"){
                conf->convolver_enable = att.attribute("value") == "true";
                conf->found_irs = true;
            }
            else if (key == "65569" || key == prefix + ".dynamicsystem.enable")conf->dynsys_enable = att.attribute("value") == "true";
            else if ((key == "65646" || key == prefix + ".viperddc.enable")&&att.attribute("value") == "true")conf->found_ddc = true;
            else if (key == "viper4android.speakerfx.spkopt.enable")conf->found_spkopt = true;
        }
    }

    QDomNodeList key_str = root.elementsByTagName("string");
    for(int i = 0; i < key_str.count(); i++)
    {
        QDomNode keynode = key_str.at(i);
        if(keynode.isElement())
        {
            QDomElement att = keynode.toElement();
            QString key = att.attribute("name");

            if(cmode==officialV4A){
                if (key == prefix + ".analogx.mode")conf->analogx_mode = att.text();
                else if (key == prefix + ".channelpan")conf->channelpan = att.text();
                else if (key == prefix + ".colorfulmusic.coeffs")conf->colorfulmusic_coeffs = att.text();
                else if (key == prefix + ".colorfulmusic.midimage")conf->colorfulmusic_midimage = att.text();
                else if (key == prefix + ".convolver.crosschannel")conf->convolver_crosschannel = att.text();
                else if (key == prefix + ".cure.crossfeed")conf->cure_crossfeed = att.text();
                else if (key == prefix + ".diffsurr.delay")conf->diffsurr_delay = att.text();
                else if (key == prefix + ".dynamicsystem.bass")conf->dynsys_bassgain = att.text();
                else if (key == prefix + ".dynamicsystem.coeffs")conf->dynsys_coeffs = att.text();
                else if (key == prefix + ".fetcompressor.adapt")conf->fetcompressor_adapt = att.text();
                else if (key == prefix + ".fetcompressor.attack")conf->fetcompressor_attack = att.text();
                else if (key == prefix + ".fetcompressor.crest")conf->fetcompressor_crest = att.text();
                else if (key == prefix + ".fetcompressor.gain")conf->fetcompressor_gain = att.text();
                else if (key == prefix + ".fetcompressor.knee")conf->fetcompressor_knee = att.text();
                else if (key == prefix + ".fetcompressor.kneemulti")conf->fetcompressor_kneemulti = att.text();
                else if (key == prefix + ".fetcompressor.maxattack")conf->fetcompressor_maxattack = att.text();
                else if (key == prefix + ".fetcompressor.maxrelease")conf->fetcompressor_maxrelease = att.text();
                else if (key == prefix + ".fetcompressor.ratio")conf->fetcompressor_ratio = att.text();
                else if (key == prefix + ".fetcompressor.release")conf->fetcompressor_release = att.text();
                else if (key == prefix + ".fetcompressor.threshold")conf->fetcompressor_threshold = att.text();
                else if (key == prefix + ".fidelity.bass.freq")conf->fidelity_bass_freq = att.text();
                else if (key == prefix + ".fidelity.bass.gain")conf->fidelity_bass_gain = att.text();
                else if (key == prefix + ".fidelity.bass.mode")conf->fidelity_bass_mode = att.text();
                else if (key == prefix + ".fidelity.clarity.gain")conf->fidelity_clarity_gain = att.text();
                else if (key == prefix + ".fidelity.clarity.mode")conf->fidelity_clarity_mode = att.text();
                else if (key == prefix + ".fireq.custom")conf->fireq_custom = att.text();
                else if (key == prefix + ".limiter")conf->limiter = att.text();
                else if (key == prefix + ".outvol")conf->outvol = att.text();
                else if (key == prefix + ".playbackgain.maxscaler")conf->playbackgain_maxscaler = att.text();
                else if (key == prefix + ".playbackgain.ratio")conf->playbackgain_ratio = att.text();
                else if (key == prefix + ".playbackgain.volume")conf->playbackgain_volume = att.text();
                else if (key == prefix + ".reverb.damp")conf->reverb_damp = att.text();
                else if (key == prefix + ".reverb.dry")conf->reverb_dry = att.text();
                else if (key == prefix + ".reverb.roomsize")conf->reverb_roomsize = att.text();
                else if (key == prefix + ".reverb.roomwidth")conf->reverb_roomwidth = att.text();
                else if (key == prefix + ".reverb.wet")conf->reverb_wet = att.text();
                else if (key == prefix + ".vhs.qual")conf->vhs_qual = att.text();
                else if (key == prefix + ".vse.value")conf->vse_value = att.text();
            }
            else if(cmode==teamDeWittV4A){
                QString value = att.attribute("value");
                if (key == "65575")conf->fidelity_bass_mode = att.text();
                else if (key == "65579")conf->fidelity_clarity_mode = att.text();
                else if (key == "65552")conf->fireq_custom = att.text();
                else if (key == "65570;65571;65572")conf->dynsys_coeffs = att.text();
            }
        }

    }

    if(cmode==teamDeWittV4A){
        QDomNodeList key_int = root.elementsByTagName("int");
        for(int i = 0; i < key_int.count(); i++)
        {
            QDomNode keynode = key_int.at(i);
            if(keynode.isElement())
            {
                QDomElement att = keynode.toElement();
                QString key = att.attribute("name");

                QString value = att.attribute("value");
                if (key == "65585")conf->analogx_mode = value;
                else if (key == "65587")conf->channelpan = value;
                else if (key == "65554;65556")conf->colorfulmusic_coeffs = value; //For some reason not a semicolon-seperated list (-> depth missing)
                else if (key == "65555")conf->colorfulmusic_midimage = value;
                else if (key == "65543")conf->convolver_crosschannel = value;
                else if (key == "65582")conf->cure_crossfeed = value;           
                else if (key == "65573")conf->dynsys_bassgain = value;
                else if (key == "65558")conf->diffsurr_delay = value;
                else if (key == "65625")conf->fetcompressor_adapt = value;
                else if (key == "65617")conf->fetcompressor_attack = value;
                else if (key == "65624")conf->fetcompressor_crest = value;
                else if (key == "65615")conf->fetcompressor_gain = value;
                else if (key == "65613")conf->fetcompressor_knee = value;
                else if (key == "65621")conf->fetcompressor_kneemulti = value;
                else if (key == "65622")conf->fetcompressor_maxattack = value;
                else if (key == "65623")conf->fetcompressor_maxrelease = value;
                else if (key == "65612")conf->fetcompressor_ratio = value;
                else if (key == "65619")conf->fetcompressor_release = value;
                else if (key == "65611")conf->fetcompressor_threshold = value;
                else if (key == "65576")conf->fidelity_bass_freq = value;
                else if (key == "65577")conf->fidelity_bass_gain = value;
                else if (key == "65580")conf->fidelity_clarity_gain = value;
                else if (key == "65588")conf->limiter = value;
                else if (key == "65586")conf->outvol = value;
                else if (key == "65567")conf->playbackgain_maxscaler = value;
                else if (key == "65568")conf->playbackgain_ratio = value;
                else if (key == "65566")conf->playbackgain_volume = value;
                else if (key == "65562")conf->reverb_damp = value;
                else if (key == "65564")conf->reverb_dry = value;
                else if (key == "65560")conf->reverb_roomsize = value;
                else if (key == "65561")conf->reverb_roomwidth = value;
                else if (key == "65563")conf->reverb_wet = value;
                else if (key == "65545")conf->vhs_qual = value;
                else if (key == "65549;65550")conf->vse_value = value;
            }
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
    out += conf->fetcompressor_ratio.toUtf8().constData() + n;

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
    QString colm_mi(conf->colorfulmusic_midimage);
    out += to_string((int)(colm_mi.toInt() * 4)) + n;

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
    QString agcgain(conf->playbackgain_ratio);
    out += to_string(agcgain.toInt()) + n;
    out += "agc_maxgain=";
    QString agc_mg(conf->playbackgain_maxscaler);
    out += to_string(agc_mg.toInt()) + n;
    out += "agc_volume=";
    QString agc_vl(conf->playbackgain_volume);
    out += to_string(agc_vl.toInt()) + n;

    //MASTER
    out += "lim_threshold=";
    out += conf->limiter.toUtf8().constData() + n;
    out += "out_pan=";
    out += conf->channelpan.toUtf8().constData() + n;
    out += "out_volume=";
    out += conf->outvol.toUtf8().constData() + n;

    //DYNAMICSYSTEM
    out += "dynsys_enable=";
    if(conf->dynsys_enable)out += "true" + n;
    else out += "false" + n;

    int count_dyn = 0;
    QStringList list_dyn = conf->dynsys_coeffs.split(QRegExp(";"));
    for ( const auto& i : list_dyn  )
    {
        qDebug() << i;//Dynsys Coeffs are split by a semicolon on android
        if (count_dyn == 0)out += "dynsys_xcoeff1=";
        else if (count_dyn == 1)out += "dynsys_xcoeff2=";
        else if (count_dyn == 2)out += "dynsys_ycoeff1=";
        else if (count_dyn == 3)out += "dynsys_ycoeff2=";
        else if (count_dyn == 4)out += "dynsys_sidegain1=";
        else if (count_dyn == 5)out += "dynsys_sidegain2=";
        out += i.toUtf8().constData() + n;
        count_dyn++;
    }

    out += "dynsys_bassgain=";
    QString dyn_bg(conf->dynsys_bassgain);
    out += to_string((int)((dyn_bg.toInt() * 20) + 100)) + n;

    string info = "";
    if(conf->found_ddc) info += "Viper DDC not supported\n";
    if(conf->found_spkopt) info += "Speaker Optimization not supported\n";
    if(conf->found_irs) info += "IRS must be manually reselected\n";
    string resp = out + "|" + info;
    return resp;
}
//Returns an error description or the converted android config
string converter::toAndroid(string path,configtype cmode){
    configmodel *conf = new configmodel();

    std::ifstream cFile(path);
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)){
            if(QString::fromStdString(line).trimmed()[0] == '#' || line.empty()) continue; //Skip commented lines
            auto delimiterInlineComment = line.find('#'); //Look for config properties mixed up with comments
            auto extractedProperty = line.substr(0, delimiterInlineComment);
            auto delimiterPos = extractedProperty.find('=');
            auto name = extractedProperty.substr(0, delimiterPos);
            auto value = extractedProperty.substr(delimiterPos + 1);
            decodeLinuxKey(name,value,conf);
        }
        cFile.close();
    }
    else {
        std::cerr << "Couldn't open config file for reading.\n";
        QMessageBox msgBox;
        msgBox.setText("Viper Configuration File not found at \n" + QString::fromStdString(path) + "");
        msgBox.setInformativeText("You can change the path in the settings.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Icon::Critical);
        msgBox.exec();
    }
    QString prefix = "viper4android.headphonefx";
    QDomDocument doc;
    QDomElement map = doc.createElement("map");

    QString dynsyscoeffs;
    for(int dynindex=0;dynindex<6;dynindex++){
        dynsyscoeffs.append(conf->temp_dynsys_coeffs[dynindex]);
        if(dynindex<5)dynsyscoeffs.append(";");
    }
    QString eqbands;
    for(int eqindex=0;eqindex<10;eqindex++){
        eqbands.append(QString::number(conf->temp_eqbands[eqindex].toDouble() / 100));
        if(eqindex<9)eqbands.append(";");
    }

    if(conf->temp_colm_widening.toInt()>200)conf->temp_colm_widening="200";
    if(conf->fidelity_bass_gain.toInt()>600)conf->fidelity_bass_gain="600";
    if(conf->fidelity_clarity_gain.toInt()>450)conf->fidelity_clarity_gain="450";
    if(conf->colorfulmusic_coeffs.toInt()>200)conf->fidelity_clarity_gain="200";

    if(cmode==officialV4A){
        //Bools...
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".enable","true"));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".analogx.enable",boolToQString(conf->analogx_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".colorfulmusic.enable",boolToQString(conf->colorfulmusic_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".cure.enable",boolToQString(conf->cure_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".diffsurr.enable",boolToQString(conf->diffsurr_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".dynamicsystem.enable",boolToQString(conf->dynsys_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fetcompressor.enable",boolToQString(conf->fetcompressor_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fetcompressor.autoattack",boolToQString(conf->fetcompressor_autoattack)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fetcompressor.autogain",boolToQString(conf->fetcompressor_autogain)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fetcompressor.autoknee",boolToQString(conf->fetcompressor_autoknee)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fetcompressor.autorelease",boolToQString(conf->fetcompressor_autorelease)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fetcompressor.noclipenable",boolToQString(conf->fetcompressor_noclipenable)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fidelity.bass.enable",boolToQString(conf->fidelity_bass_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fidelity.clarity.enable",boolToQString(conf->fidelity_clarity_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fireq.enable",boolToQString(conf->fireq_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".playbackgain.enable",boolToQString(conf->playbackgain_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".reverb.enable",boolToQString(conf->reverb_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".tube.enable",boolToQString(conf->tube_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".vhs.enable",boolToQString(conf->vhs_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".vse.enable",boolToQString(conf->vse_enable)));

        //Strings...
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".analogx.mode",conf->analogx_mode));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".channelpan",conf->channelpan));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".colorfulmusic.midimage",QString::number(conf->colorfulmusic_midimage.toInt()/4)));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".colorfulmusic.coeffs",conf->temp_colm_widening + ";" + conf->temp_colm_depth));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".convolver.crosschannel",conf->convolver_crosschannel));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".cure.crossfeed",conf->cure_crossfeed));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".diffsurr.delay",QString::number(conf->diffsurr_delay.toInt()*20)));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".dynamicsystem.bass",QString::number((conf->dynsys_bassgain.toInt()-100)/20)));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".dynamicsystem.coeffs",dynsyscoeffs));

        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.adapt",conf->fetcompressor_adapt));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.attack",conf->fetcompressor_attack));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.crest",conf->fetcompressor_crest));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.gain",conf->fetcompressor_gain));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.knee",conf->fetcompressor_knee));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.kneemulti",conf->fetcompressor_kneemulti));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.maxattack",conf->fetcompressor_maxattack));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.maxrelease",conf->fetcompressor_maxrelease));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.ratio",conf->fetcompressor_ratio));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.release",conf->fetcompressor_release));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.threshold",conf->fetcompressor_threshold));

        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fidelity.bass.freq",conf->fidelity_bass_freq));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fidelity.bass.gain",conf->fidelity_bass_gain));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fidelity.bass.mode",conf->fidelity_bass_mode));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fidelity.clarity.gain",conf->fidelity_clarity_gain));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fidelity.clarity.mode",conf->fidelity_clarity_mode));

        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fireq.custom",eqbands));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".limiter",conf->limiter));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".outvol",conf->outvol));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".playbackgain.volume",conf->playbackgain_volume));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".playbackgain.ratio",conf->playbackgain_ratio));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".playbackgain.maxscaler",conf->playbackgain_maxscaler));

        map.appendChild(generateXmlEntry(&doc,"string",prefix+".reverb.damp",conf->reverb_damp));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".reverb.dry",conf->reverb_dry));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".reverb.roomsize",conf->reverb_roomsize));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".reverb.roomwidth",conf->reverb_roomwidth));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".reverb.wet",conf->reverb_wet));

        map.appendChild(generateXmlEntry(&doc,"string",prefix+".vhs.qual",conf->vhs_qual));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".vse.value",QString::number(translate(conf->vse_value.toInt(),0.01f,0.1f,0.1f,1)/1000)));
    }
    else if(cmode==teamDeWittV4A){
        //Bools...
        map.appendChild(generateXmlEntry(&doc,"boolean","65584",boolToQString(conf->analogx_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65553",boolToQString(conf->colorfulmusic_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65581",boolToQString(conf->cure_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65557",boolToQString(conf->diffsurr_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65569",boolToQString(conf->dynsys_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65610",boolToQString(conf->fetcompressor_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65618",boolToQString(conf->fetcompressor_autoattack)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65616",boolToQString(conf->fetcompressor_autogain)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65614",boolToQString(conf->fetcompressor_autoknee)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65620",boolToQString(conf->fetcompressor_autorelease)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65626",boolToQString(conf->fetcompressor_noclipenable)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65574",boolToQString(conf->fidelity_bass_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65578",boolToQString(conf->fidelity_clarity_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65551",boolToQString(conf->fireq_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65565",boolToQString(conf->playbackgain_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65559",boolToQString(conf->reverb_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65583",boolToQString(conf->tube_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65544",boolToQString(conf->vhs_enable)));
        map.appendChild(generateXmlEntry(&doc,"boolean","65548",boolToQString(conf->vse_enable)));

        //Strings...
        map.appendChild(generateXmlEntry(&doc,"int","65585",conf->analogx_mode));
        map.appendChild(generateXmlEntry(&doc,"int","65587",conf->channelpan));
        map.appendChild(generateXmlEntry(&doc,"int","65555",QString::number(conf->colorfulmusic_midimage.toInt()/4)));
        map.appendChild(generateXmlEntry(&doc,"int","65554;65556",conf->temp_colm_widening + ";" + conf->temp_colm_depth));
        map.appendChild(generateXmlEntry(&doc,"int","65543",conf->convolver_crosschannel));
        map.appendChild(generateXmlEntry(&doc,"int","65582",conf->cure_crossfeed));
        map.appendChild(generateXmlEntry(&doc,"int","65558",QString::number(conf->diffsurr_delay.toInt()*20)));
        map.appendChild(generateXmlEntry(&doc,"int","65573",QString::number((conf->dynsys_bassgain.toInt()-100)/20)));
        map.appendChild(generateXmlEntry(&doc,"string","65570;65571;65572",dynsyscoeffs));

        map.appendChild(generateXmlEntry(&doc,"int","65625",conf->fetcompressor_adapt));
        map.appendChild(generateXmlEntry(&doc,"int","65617",conf->fetcompressor_attack));
        map.appendChild(generateXmlEntry(&doc,"int","65624",conf->fetcompressor_crest));
        map.appendChild(generateXmlEntry(&doc,"int","65615",conf->fetcompressor_gain));
        map.appendChild(generateXmlEntry(&doc,"int","65613",conf->fetcompressor_knee));
        map.appendChild(generateXmlEntry(&doc,"int","65621",conf->fetcompressor_kneemulti));
        map.appendChild(generateXmlEntry(&doc,"int","65622",conf->fetcompressor_maxattack));
        map.appendChild(generateXmlEntry(&doc,"int","65623",conf->fetcompressor_maxrelease));
        map.appendChild(generateXmlEntry(&doc,"int","65612",conf->fetcompressor_ratio));
        map.appendChild(generateXmlEntry(&doc,"int","65619",conf->fetcompressor_release));
        map.appendChild(generateXmlEntry(&doc,"int","65611",conf->fetcompressor_threshold));

        map.appendChild(generateXmlEntry(&doc,"int","65576",conf->fidelity_bass_freq));
        map.appendChild(generateXmlEntry(&doc,"int","65677",conf->fidelity_bass_gain));

        map.appendChild(generateXmlEntry(&doc,"string","65575",conf->fidelity_bass_mode));
        map.appendChild(generateXmlEntry(&doc,"int","66580",conf->fidelity_clarity_gain));
        map.appendChild(generateXmlEntry(&doc,"string","65579",conf->fidelity_clarity_mode));

        map.appendChild(generateXmlEntry(&doc,"string","65552",eqbands));
        map.appendChild(generateXmlEntry(&doc,"int","65588",conf->limiter));
        map.appendChild(generateXmlEntry(&doc,"int","65586",conf->outvol));
        map.appendChild(generateXmlEntry(&doc,"int","65566",conf->playbackgain_volume));
        map.appendChild(generateXmlEntry(&doc,"int","65568",conf->playbackgain_ratio));
        map.appendChild(generateXmlEntry(&doc,"int","65567",conf->playbackgain_maxscaler));
        map.appendChild(generateXmlEntry(&doc,"int","65562",conf->reverb_damp));
        map.appendChild(generateXmlEntry(&doc,"int","65564",conf->reverb_dry));
        map.appendChild(generateXmlEntry(&doc,"int","65560",conf->reverb_roomsize));
        map.appendChild(generateXmlEntry(&doc,"int","65561",conf->reverb_roomwidth));
        map.appendChild(generateXmlEntry(&doc,"int","65563",conf->reverb_wet));
        map.appendChild(generateXmlEntry(&doc,"int","65545",conf->vhs_qual));
        map.appendChild(generateXmlEntry(&doc,"int","65549;65550",QString::number(translate(conf->vse_value.toInt(),0.01f,0.1f,0.1f,1)/1000)));
    }
    else{
        return "Error: Unknown config type";
    }
    doc.appendChild(map);
    return doc.toString().toUtf8().constData();
}
QDomElement converter::generateXmlEntry(QDomDocument* doc,QString type,QString name,QString value){
    QDomElement obj = doc->createElement(type);
    obj.setAttribute("name", name);
    if(type == "boolean"||type=="int")obj.setAttribute("value", value);
    else obj.appendChild( doc->createTextNode(value));
    return obj;
}
QString converter::boolToQString(bool b){
    if(b) return "true";
    else return "false";
}
void converter::decodeLinuxKey(const string& key,string value,configmodel* conf){
    //cout << key << " -> " << value << endl;
    if(value==""||is_only_ascii_whitespace(value)){
        cerr << "[WARNING] Key " + key + " is empty" << endl;
        return;
    }
    switch (resolveConfig(key)) {
    case tube_enable: {
        conf->tube_enable = value=="true";
        break;
    }
    case colm_enable: {
        conf->colorfulmusic_enable = value=="true";
        break;
    }
    case colm_widening: {
        conf->temp_colm_widening = QString::fromStdString(value);
        break;
    }
    case colm_depth: {
        conf->temp_colm_depth = QString::fromStdString(value);
        break;
    }
    case colm_midimage: {
        qDebug() << QString::fromStdString(value);
        conf->colorfulmusic_midimage = QString::fromStdString(value);
        break;
    }
    case vc_enable: {
        conf->fidelity_clarity_enable = value=="true";
    }
    case vc_mode: {
        conf->fidelity_clarity_mode = QString::fromStdString(value);
        break;
    }
    case vc_level: {
        conf->fidelity_clarity_gain = QString::fromStdString(value);
        break;
    }
    case vb_enable: {
        conf->fidelity_bass_enable = value=="true";
        break;
    }
    case vb_mode: {
        conf->fidelity_bass_mode = QString::fromStdString(value);
        break;
    }
    case vb_freq: {
        conf->fidelity_bass_freq = QString::fromStdString(value);
        break;
    }
    case vb_gain: {
        conf->fidelity_bass_gain = QString::fromStdString(value);
        break;
    }
    case vhe_enable: {
        conf->vhs_enable = value=="true";
        break;
    }
    case vhe_level: {
        conf->vhs_qual = QString::fromStdString(value);
        break;
    }
    case ds_enable: {
        conf->diffsurr_enable = value=="true";
        break;
    }
    case ds_level: {
        conf->diffsurr_delay = QString::fromStdString(value);
        break;
    }
    case reverb_enable: {
        conf->reverb_enable = value=="true";
        break;
    }
    case reverb_roomsize: {
        conf->reverb_roomsize = QString::fromStdString(value);
        break;
    }
    case reverb_width: {
        conf->reverb_roomwidth = QString::fromStdString(value);
        break;
    }
    case reverb_damp: {
        conf->reverb_damp = QString::fromStdString(value);
        break;
    }
    case reverb_wet: {
        conf->reverb_wet = QString::fromStdString(value);
        break;
    }
    case reverb_dry: {
        conf->reverb_dry = QString::fromStdString(value);
        break;
    }
    case agc_enable: {
        conf->playbackgain_enable = value=="true";
        break;
    }
    case agc_ratio: {
        int v = atoi(value.c_str());
        if(v>300)v=300;
        conf->playbackgain_ratio = QString::number(v);
        break;
    }
    case agc_maxgain: {
        conf->playbackgain_maxscaler = QString::fromStdString(value);
        break;
    }
    case agc_volume: {
        int v = atoi(value.c_str());
        if(v>100)v=100;
        conf->playbackgain_volume = QString::number(v);
        break;
    }
    case lim_threshold: {
        conf->limiter = QString::fromStdString(value);
        break;
    }
    case out_pan: {
        conf->channelpan = QString::fromStdString(value);
        break;
    }
    case out_volume: {
        conf->outvol = QString::fromStdString(value);
        break;
    }
    case eq_enable: {
        conf->fireq_enable = value=="true";
        break;
    }
    case eq_band1: {
        conf->temp_eqbands[0] = QString::fromStdString(value);
        break;
    }
    case eq_band2: {
        conf->temp_eqbands[1] = QString::fromStdString(value);
        break;
    }
    case eq_band3: {
        conf->temp_eqbands[2] = QString::fromStdString(value);
        break;
    }
    case eq_band4: {
        conf->temp_eqbands[3] = QString::fromStdString(value);
        break;
    }
    case eq_band5: {
        conf->temp_eqbands[4] = QString::fromStdString(value);
        break;
    }
    case eq_band6: {
        conf->temp_eqbands[5] = QString::fromStdString(value);
        break;
    }
    case eq_band7: {
        conf->temp_eqbands[6] = QString::fromStdString(value);
        break;
    }
    case eq_band8: {
        conf->temp_eqbands[7] = QString::fromStdString(value);
        break;
    }
    case eq_band9: {
        conf->temp_eqbands[8] = QString::fromStdString(value);
        break;
    }
    case eq_band10: {
        conf->temp_eqbands[9] = QString::fromStdString(value);
        break;
    }
    case fetcomp_enable: {
        conf->fetcompressor_enable = value=="true";
        break;
    }
    case fetcomp_autogain: {
        conf->fetcompressor_autogain = value=="true";
        break;
    }
    case fetcomp_autoknee: {
        conf->fetcompressor_autoknee = value=="true";
        break;
    }
    case fetcomp_autoattack: {
        conf->fetcompressor_autoattack = value=="true";
        break;
    }
    case fetcomp_autorelease: {
        conf->fetcompressor_autorelease = value=="true";
        break;
    }
    case fetcomp_noclip: {
        conf->fetcompressor_noclipenable = value=="true";
        break;
    }
    case fetcomp_threshold: {
        conf->fetcompressor_threshold = QString::fromStdString(value);
        break;
    }
    case fetcomp_gain: {
        conf->fetcompressor_gain = QString::fromStdString(value);
        break;
    }
    case fetcomp_kneewidth: {
        conf->fetcompressor_knee = QString::fromStdString(value);
        break;
    }
    case fetcomp_ratio: {
        conf->fetcompressor_ratio = QString::fromStdString(value);
        break;
    }
    case fetcomp_attack: {
        conf->fetcompressor_attack = QString::fromStdString(value);
        break;
    }
    case fetcomp_release: {
        conf->fetcompressor_release = QString::fromStdString(value);
        break;
    }
    case fetcomp_meta_adapt: {
        conf->fetcompressor_adapt = QString::fromStdString(value);
        break;
    }
    case fetcomp_meta_crest: {
        conf->fetcompressor_crest = QString::fromStdString(value);
        break;
    }
    case fetcomp_meta_maxattack: {
        conf->fetcompressor_maxattack = QString::fromStdString(value);
        break;
    }
    case fetcomp_meta_maxrelease: {
        conf->fetcompressor_maxrelease = QString::fromStdString(value);
        break;
    }
    case fetcomp_meta_kneemulti: {
        conf->fetcompressor_kneemulti = QString::fromStdString(value);
        break;
    }
    case cure_enable: {
        conf->cure_enable = value=="true";
        break;
    }
    case cure_level: {
         conf->cure_crossfeed = QString::fromStdString(value);
        break;
    }
    case ax_enable: {
        conf->analogx_enable = value=="true";
        break;
    }
    case ax_mode: {
         conf->analogx_mode = QString::fromStdString(value);
        break;
    }
    case vse_enable: {
        conf->vse_enable = value=="true";
        break;
    }
    case vse_ref_bark: {
        //Fixed to 7600 in Viper4Android
        break;
    }
    case vse_bark_cons: {
         conf->vse_value = QString::fromStdString(value);
        break;
    }
    case conv_enable: {
        conf->found_irs = value=="true";
        break;
    }
    case conv_cc_level: {
        conf->convolver_crosschannel = QString::fromStdString(value);
        break;
    }
    case conv_ir_path: {
        //IRS have to be set manually
        break;
    }
    case dynsys_enable: {
        conf->dynsys_enable = value=="true";
        break;
    }
    case dynsys_xcoeff1: {
        conf->temp_dynsys_coeffs[0] = QString::fromStdString(value);
        break;
    }
    case dynsys_xcoeff2: {
        conf->temp_dynsys_coeffs[1] = QString::fromStdString(value);
        break;
    }
    case dynsys_ycoeff1: {
        conf->temp_dynsys_coeffs[2] = QString::fromStdString(value);
        break;
    }
    case dynsys_ycoeff2: {
        conf->temp_dynsys_coeffs[3] = QString::fromStdString(value);
        break;
    }
    case dynsys_bassgain: {
        conf->dynsys_bassgain = QString::fromStdString(value);
        break;
    }
    case dynsys_sidegain1: {
        conf->temp_dynsys_coeffs[4] = QString::fromStdString(value);
        break;
    }
    case dynsys_sidegain2: {
        conf->temp_dynsys_coeffs[5] = QString::fromStdString(value);
        break;
    }
    case unknown: {
        cout << "Config-List Enum: Unknown" << endl;
        break;
    }
    }
}
bool converter::is_only_ascii_whitespace(const std::string& str){
    auto it = str.begin();
    do {
        if (it == str.end()) return true;
    } while (*it >= 0 && *it <= 0x7f && std::isspace(*(it++)));
    return false;
}
string converter::intToString(int i){
    stringstream ss;
    ss << i;
    return ss.str();
}
float converter::translate(int value,float leftMin,float leftMax,float rightMin,float rightMax){
    float leftSpan = leftMax - leftMin;
    float rightSpan = rightMax - rightMin;
    float valueScaled = float(value - leftMin) / float(leftSpan);
    return rightMin + (valueScaled * rightSpan);
}
