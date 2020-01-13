#include "converter.h"
#include "misc/loghelper.h"
#include "config/container.h"
#include "config/io.h"
#include "misc/common.h"

#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;
conversion_result_t converter::toLinux(const QString& path,configtype cmode){
    ConfigContainer *conf = new ConfigContainer();
    conversion_result_t result;
    result.error = false;
    result.found_ddc = false;
    result.found_irs = false;
    result.found_spkopt = false;

    int errorLine, errorColumn;
    QString errorMsg;
    QFile modelFile(path);
    QDomDocument document;
    if (!document.setContent(&modelFile, &errorMsg, &errorLine, &errorColumn))
    {
        QString error("Syntax error line %1, column %2:\n%3");
        error = error
                .arg(errorLine)
                .arg(errorColumn)
                .arg(errorMsg);
        LogHelper::writeLog("Converter (a-to-l;mode=" + QString::number(cmode) + "): " + error + " (converter/syntaxcheck)");
        result.error = true;
        result.notices = error;
        return result;
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
            if (key == "65584" || key == prefix + ".analogx.enable")conf->setValue("ax_enable",att.attribute("value") == "true");
            else if (key == "65553" || key == prefix + ".colorfulmusic.enable")conf->setValue("colm_enable",att.attribute("value") == "true");
            else if (key == "65581" || key == prefix + ".cure.enable")conf->setValue("cure_enable", att.attribute("value") == "true");
            else if (key == "65557" || key == prefix + ".diffsurr.enable")conf->setValue("ds_enable", att.attribute("value") == "true");
            else if (key == "65618" || key == prefix + ".fetcompressor.autoattack")conf->setValue("fetcomp_autoattack", att.attribute("value") == "true");
            else if (key == "65616" || key == prefix + ".fetcompressor.autogain")conf->setValue("fetcomp_autogain", att.attribute("value") == "true");
            else if (key == "65614" || key == prefix + ".fetcompressor.autoknee")conf->setValue("fetcomp_autoknee", att.attribute("value") == "true");
            else if (key == "65620" || key == prefix + ".fetcompressor.autorelease")conf->setValue("fetcomp_autorelease", att.attribute("value") == "true");
            else if (key == "65610" || key == prefix + ".fetcompressor.enable")conf->setValue("fetcomp_enable", att.attribute("value") == "true");
            else if (key == "65626" || key == prefix + ".fetcompressor.noclipenable")conf->setValue("fetcomp_noclip", att.attribute("value") == "true");
            else if (key == "65574" || key == prefix + ".fidelity.bass.enable")conf->setValue("vb_enable", att.attribute("value") == "true");
            else if (key == "65578" || key == prefix + ".fidelity.clarity.enable")conf->setValue("vc_enable", att.attribute("value") == "true");
            else if (key == "65551" || key == prefix + ".fireq.enable")conf->setValue("eq_enable", att.attribute("value") == "true");
            else if (key == "65565" || key == prefix + ".playbackgain.enable")conf->setValue("agc_enable", att.attribute("value") == "true");
            else if (key == "65559" || key == prefix + ".reverb.enable")conf->setValue("reverb_enable", att.attribute("value") == "true");
            else if (key == "65583" || key == prefix + ".tube.enable")conf->setValue("tube_enable", att.attribute("value") == "true");
            else if (key == "65544" || key == prefix + ".vhs.enable")conf->setValue("vhe_enable", att.attribute("value") == "true");
            else if (key == "65548" || key == prefix + ".vse.enable")conf->setValue("vse_enable", att.attribute("value") == "true");
            else if (key == "65538" || key == prefix + ".convolver.enable"){
                conf->setValue("conv_enable",false);
                result.found_irs = true;
            }
            else if (key == "65569" || key == prefix + ".dynamicsystem.enable")conf->setValue("dynsys_enable", att.attribute("value") == "true");
            else if ((key == "65646" || key == prefix + ".viperddc.enable")&&att.attribute("value") == "true")result.found_ddc = true;
            else if (key == "viper4android.speakerfx.spkopt.enable")result.found_spkopt = true;
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
                if (key == prefix + ".analogx.mode")conf->setValue("ax_mode", att.text());
                else if (key == prefix + ".channelpan")conf->setValue("out_pan", att.text());
                else if (key == prefix + ".colorfulmusic.coeffs"){
                    int count = 0;
                    for(auto part:att.text().split(";")){
                        int number = part.toInt();
                        if (count == 0)conf->setValue("colm_widening", number);
                        else if (count == 1)conf->setValue("colm_depth", number);
                        else break;
                        count++;
                    }
                }
                else if (key == prefix + ".colorfulmusic.midimage")conf->setValue("colm_midimage", ((int)(att.text().toInt()*4)));
                else if (key == prefix + ".convolver.crosschannel")conf->setValue("conv_cc_level", att.text());
                else if (key == prefix + ".cure.crossfeed")conf->setValue("cure_level", att.text());
                else if (key == prefix + ".diffsurr.delay")conf->setValue("ds_level", ((int)(att.text().toInt()/20)));
                else if (key == prefix + ".dynamicsystem.bass")conf->setValue("dynsys_bassgain", (int)((att.text().toInt() * 20) + 100));
                else if (key == prefix + ".dynamicsystem.coeffs"){
                    int count = 0;
                    QStringList list_dyn = att.text().split(";");
                    for ( const auto& i : list_dyn  )
                    {
                        if (count == 0)conf->setValue("dynsys_xcoeff1", i);
                        else if (count == 1)conf->setValue("dynsys_xcoeff2", i);
                        else if (count == 2)conf->setValue("dynsys_ycoeff1", i);
                        else if (count == 3)conf->setValue("dynsys_ycoeff2", i);
                        else if (count == 4)conf->setValue("dynsys_sidegain1", i);
                        else if (count == 5)conf->setValue("dynsys_sidegain2", i);
                        count++;
                    }
                }
                else if (key == prefix + ".fetcompressor.adapt")conf->setValue("fetcomp_meta_adapt", att.text());
                else if (key == prefix + ".fetcompressor.attack")conf->setValue("fetcomp_attack", att.text());
                else if (key == prefix + ".fetcompressor.crest")conf->setValue("fetcomp_meta_crest", att.text());
                else if (key == prefix + ".fetcompressor.gain")conf->setValue("fetcomp_gain", att.text());
                else if (key == prefix + ".fetcompressor.knee")conf->setValue("fetcomp_kneewidth", att.text());
                else if (key == prefix + ".fetcompressor.kneemulti")conf->setValue("fetcomp_meta_kneemulti", att.text());
                else if (key == prefix + ".fetcompressor.maxattack")conf->setValue("fetcomp_meta_maxattack", att.text());
                else if (key == prefix + ".fetcompressor.maxrelease")conf->setValue("fetcomp_meta_maxrelease", att.text());
                else if (key == prefix + ".fetcompressor.ratio")conf->setValue("fetcomp_ratio", att.text());
                else if (key == prefix + ".fetcompressor.release")conf->setValue("fetcomp_release", att.text());
                else if (key == prefix + ".fetcompressor.threshold")conf->setValue("fetcomp_threshold", att.text());
                else if (key == prefix + ".fidelity.bass.freq")conf->setValue("vb_freq", att.text());
                else if (key == prefix + ".fidelity.bass.gain")conf->setValue("vb_gain", att.text());
                else if (key == prefix + ".fidelity.bass.mode")conf->setValue("vb_mode", att.text());
                else if (key == prefix + ".fidelity.clarity.gain")conf->setValue("vc_level", att.text());
                else if (key == prefix + ".fidelity.clarity.mode")conf->setValue("vc_mode", att.text());
                else if (key == prefix + ".fireq.custom"){
                    int count = 0;
                    for(auto part:att.text().split(";")){
                        int number = (int)(part.toDouble() * 100);
                        if (count == 0)conf->setValue("eq_band1", number);
                        else if (count == 1)conf->setValue("eq_band2", number);
                        else if (count == 2)conf->setValue("eq_band3", number);
                        else if (count == 3)conf->setValue("eq_band4", number);
                        else if (count == 4)conf->setValue("eq_band5", number);
                        else if (count == 5)conf->setValue("eq_band6", number);
                        else if (count == 6)conf->setValue("eq_band7", number);
                        else if (count == 7)conf->setValue("eq_band8", number);
                        else if (count == 8)conf->setValue("eq_band9", number);
                        else if (count == 9)conf->setValue("eq_band10", number);
                        else break;
                        count++;
                    }
                }
                else if (key == prefix + ".limiter")conf->setValue("lim_threshold", att.text());
                else if (key == prefix + ".outvol")conf->setValue("out_volume", att.text());
                else if (key == prefix + ".playbackgain.maxscaler")conf->setValue("agc_maxgain", att.text());
                else if (key == prefix + ".playbackgain.ratio")conf->setValue("agc_ratio", att.text());
                else if (key == prefix + ".playbackgain.volume")conf->setValue("agc_volume", att.text());
                else if (key == prefix + ".reverb.damp")conf->setValue("reverb_damp", att.text());
                else if (key == prefix + ".reverb.dry")conf->setValue("reverb_dry", att.text());
                else if (key == prefix + ".reverb.roomsize")conf->setValue("reverb_roomsize", att.text());
                else if (key == prefix + ".reverb.roomwidth")conf->setValue("reverb_width", att.text());
                else if (key == prefix + ".reverb.wet")conf->setValue("reverb_wet", att.text());
                else if (key == prefix + ".vhs.qual")conf->setValue("vhe_level", att.text());
                else if (key == prefix + ".vse.value")conf->setValue("vse_bark_cons",(int)round(att.text().toFloat() * 5.6f * 100.0f));
            }
            else if(cmode==teamDeWittV4A){
                QString value = att.attribute("value");
                if (key == "65575")conf->setValue("vb_mode", att.text());
                else if (key == "65579")conf->setValue("vc_mode", att.text());
                else if (key == "65552"){
                    int count = 0;
                    for(auto part:att.text().split(";")){
                        int number = (int)(part.toDouble() * 100);
                        if (count == 0)conf->setValue("eq_band1", number);
                        else if (count == 1)conf->setValue("eq_band2", number);
                        else if (count == 2)conf->setValue("eq_band3", number);
                        else if (count == 3)conf->setValue("eq_band4", number);
                        else if (count == 4)conf->setValue("eq_band5", number);
                        else if (count == 5)conf->setValue("eq_band6", number);
                        else if (count == 6)conf->setValue("eq_band7", number);
                        else if (count == 7)conf->setValue("eq_band8", number);
                        else if (count == 8)conf->setValue("eq_band9", number);
                        else if (count == 9)conf->setValue("eq_band10", number);
                        else break;
                        count++;
                    }
                }
                else if (key == "65570;65571;65572"){
                    int count = 0;
                    QStringList list_dyn = att.text().split(";");
                    for ( const auto& i : list_dyn  )
                    {
                        if (count == 0)conf->setValue("dynsys_xcoeff1", i);
                        else if (count == 1)conf->setValue("dynsys_xcoeff2", i);
                        else if (count == 2)conf->setValue("dynsys_ycoeff1", i);
                        else if (count == 3)conf->setValue("dynsys_ycoeff2", i);
                        else if (count == 4)conf->setValue("dynsys_sidegain1", i);
                        else if (count == 5)conf->setValue("dynsys_sidegain2", i);
                        count++;
                    }
                }
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
                if (key == "65585")conf->setValue("ax_mode",value);
                else if (key == "65587")conf->setValue("out_pan",value);
                else if (key == "65554;65556"){
                    //For some reason not a semicolon-seperated list (-> depth missing)
                    int count = 0;
                    for(auto part:att.text().split(";")){
                        int number = part.toInt();
                        if (count == 0)conf->setValue("colm_widening", number);
                        else if (count == 1)conf->setValue("colm_depth", number);
                        else break;
                        count++;
                    }
                }
                else if (key == "65555")conf->setValue("colm_midimage",((int)(value.toInt()*4)));
                else if (key == "65543")conf->setValue("conv_cc_level",value);
                else if (key == "65582")conf->setValue("cure_level",value);
                else if (key == "65573")conf->setValue("dynsys_bassgain",((int)((value.toInt() * 20) + 100)));
                else if (key == "65558")conf->setValue("ds_level",((int)(value.toInt()/20)));
                else if (key == "65625")conf->setValue("fetcomp_meta_adapt",value);
                else if (key == "65617")conf->setValue("fetcomp_attack",value);
                else if (key == "65624")conf->setValue("fetcomp_meta_crest",value);
                else if (key == "65615")conf->setValue("fetcomp_gain",value);
                else if (key == "65613")conf->setValue("fetcomp_kneewidth",value);
                else if (key == "65621")conf->setValue("fetcomp_meta_kneemulti",value);
                else if (key == "65622")conf->setValue("fetcomp_meta_maxattack",value);
                else if (key == "65623")conf->setValue("fetcomp_meta_maxrelease",value);
                else if (key == "65612")conf->setValue("fetcomp_ratio",value);
                else if (key == "65619")conf->setValue("fetcomp_release",value);
                else if (key == "65611")conf->setValue("fetcomp_threshold",value);
                else if (key == "65576")conf->setValue("vb_freq",value);
                else if (key == "65577")conf->setValue("vb_gain",value);
                else if (key == "65580")conf->setValue("vc_level",value);
                else if (key == "65588")conf->setValue("lim_threshold",value);
                else if (key == "65586")conf->setValue("out_volume",value);
                else if (key == "65567")conf->setValue("agc_maxgain",value);
                else if (key == "65568")conf->setValue("agc_ratio",value);
                else if (key == "65566")conf->setValue("agc_volume",value);
                else if (key == "65562")conf->setValue("reverb_damp",value);
                else if (key == "65564")conf->setValue("reverb_dry",value);
                else if (key == "65560")conf->setValue("reverb_roomsize",value);
                else if (key == "65561")conf->setValue("reverb_width",value);
                else if (key == "65563")conf->setValue("reverb_wet",value);
                else if (key == "65545")conf->setValue("vhe_level",value);
                else if (key == "65549;65550")conf->setValue("vse_bark_cons",(int)round(value.toFloat() * 5.6f * 100.0f));
            }
        }
    }

    conf->setValue("vse_ref_bark",7600);
    conf->setValue("fx_enable",true);
    conf->setValue("conv_ir_path","");
    result.configuration = ConfigIO::writeString(conf->getConfigMap());

    QString info;
    if(result.found_ddc) info += "Viper DDC not supported\n";
    if(result.found_spkopt) info += "Speaker Optimization not supported\n";
    if(result.found_irs) info += "IRS must be manually reselected\n";
    result.notices = info;
    return result;
}
conversion_result_t converter::toAndroid(const QString& path,configtype cmode){
    ConfigContainer* conf = new ConfigContainer();
    conf->setConfigMap(ConfigIO::readFile(path));
    conversion_result_t result;
    result.error = false;
    result.found_ddc = false;
    result.found_irs = false;
    result.found_spkopt = false;

    QString prefix = "viper4android.headphonefx";
    QDomDocument doc;
    QDomElement map = doc.createElement("map");

    QString dynsyscoeffs;
    QString eqbands;

    QString temp_eqbands[10] = {conf->getString("eq_band1"),conf->getString("eq_band2"),conf->getString("eq_band3"),
                                conf->getString("eq_band4"),conf->getString("eq_band5"),conf->getString("eq_band6"),
                                conf->getString("eq_band7"),conf->getString("eq_band8"),conf->getString("eq_band9"),
                                conf->getString("eq_band10")};
    QString temp_dynsys_coeffs[6] = {conf->getString("dynsys_xcoeff1"),conf->getString("dynsys_xcoeff2"),
                                     conf->getString("dynsys_ycoeff1"),conf->getString("dynsys_ycoeff2"),
                                     conf->getString("dynsys_sidegain1"),conf->getString("dynsys_sidegain2")};

    for(int dynindex=0;dynindex<6;dynindex++){
        dynsyscoeffs.append(temp_dynsys_coeffs[dynindex]);
        if(dynindex<5)dynsyscoeffs.append(";");
    }
    for(int eqindex=0;eqindex<10;eqindex++){
        eqbands.append(QString::number(temp_eqbands[eqindex].toDouble() / 100));
        if(eqindex<9)eqbands.append(";");
    }

    if(conf->getInt("vb_gain")>600)conf->setValue("vb_gain","600");
    if(conf->getInt("vc_level")>450)conf->setValue("vc_level","450");

    if(cmode==officialV4A){
        //Bools...
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".enable","true"));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".analogx.enable",boolToQString(conf->getBool("ax_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".colorfulmusic.enable",boolToQString(conf->getBool("colm_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".cure.enable",boolToQString(conf->getBool("cure_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".diffsurr.enable",boolToQString(conf->getBool("ds_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".dynamicsystem.enable",boolToQString(conf->getBool("dynsys_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fetcompressor.enable",boolToQString(conf->getBool("fetcomp_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fetcompressor.autoattack",boolToQString(conf->getBool("fetcomp_autoattack"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fetcompressor.autogain",boolToQString(conf->getBool("fetcomp_autogain"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fetcompressor.autoknee",boolToQString(conf->getBool("fetcomp_autoknee"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fetcompressor.autorelease",boolToQString(conf->getBool("fetcomp_autorelease"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fetcompressor.noclipenable",boolToQString(conf->getBool("fetcomp_noclip"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fidelity.bass.enable",boolToQString(conf->getBool("vb_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fidelity.clarity.enable",boolToQString(conf->getBool("vc_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".fireq.enable",boolToQString(conf->getBool("eq_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".playbackgain.enable",boolToQString(conf->getBool("agc_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".reverb.enable",boolToQString(conf->getBool("reverb_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".tube.enable",boolToQString(conf->getBool("tube_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".vhs.enable",boolToQString(conf->getBool("vhe_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean",prefix+".vse.enable",boolToQString(conf->getBool("vse_enable"))));
        //Strings...
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".analogx.mode",conf->getString("ax_mode")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".channelpan",conf->getString("out_pan")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".colorfulmusic.midimage",QString::number(conf->getInt("colm_midimage")/4)));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".colorfulmusic.coeffs",conf->getString("colm_widening") + ";" + conf->getString("colm_depth")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".convolver.crosschannel",conf->getString("conv_cc_level")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".cure.crossfeed",conf->getString("cure_level")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".diffsurr.delay",QString::number(conf->getInt("ds_level")*20)));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".dynamicsystem.bass",QString::number((conf->getInt("dynsys_bassgain")-100)/20)));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".dynamicsystem.coeffs",dynsyscoeffs));

        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.adapt",conf->getString("fetcomp_meta_adapt")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.attack",conf->getString("fetcomp_attack")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.crest",conf->getString("fetcomp_meta_crest")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.gain",conf->getString("fetcomp_gain")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.knee",conf->getString("fetcomp_kneewidth")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.kneemulti",conf->getString("fetcomp_meta_kneemulti")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.maxattack",conf->getString("fetcomp_meta_maxattack")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.maxrelease",conf->getString("fetcomp_meta_maxrelease")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.ratio",conf->getString("fetcomp_ratio")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.release",conf->getString("fetcomp_release")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fetcompressor.threshold",conf->getString("fetcomp_threshold")));

        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fidelity.bass.freq",conf->getString("vb_freq")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fidelity.bass.gain",conf->getString("vb_gain")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fidelity.bass.mode",conf->getString("vb_mode")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fidelity.clarity.gain",conf->getString("vc_level")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fidelity.clarity.mode",conf->getString("vc_mode")));

        map.appendChild(generateXmlEntry(&doc,"string",prefix+".fireq.custom",eqbands));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".limiter",conf->getString("lim_threshold")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".outvol",conf->getString("out_volume")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".playbackgain.volume",conf->getString("agc_volume")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".playbackgain.ratio",conf->getString("agc_ratio")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".playbackgain.maxscaler",conf->getString("agc_maxgain")));

        map.appendChild(generateXmlEntry(&doc,"string",prefix+".reverb.damp",conf->getString("reverb_damp")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".reverb.dry",conf->getString("reverb_dry")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".reverb.roomsize",conf->getString("reverb_roomsize")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".reverb.roomwidth",conf->getString("reverb_width")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".reverb.wet",conf->getString("reverb_wet")));

        map.appendChild(generateXmlEntry(&doc,"string",prefix+".vhs.qual",conf->getString("vhe_level")));
        map.appendChild(generateXmlEntry(&doc,"string",prefix+".vse.value",QString::number(translate(conf->getInt("vse_bark_cons"),0.01f,0.1f,0.1f,1)/1000)));
    }
    else if(cmode==teamDeWittV4A){
        //Bools...
        map.appendChild(generateXmlEntry(&doc,"boolean","65584",boolToQString(conf->getBool("ax_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65553",boolToQString(conf->getBool("colm_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65581",boolToQString(conf->getBool("cure_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65557",boolToQString(conf->getBool("ds_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65569",boolToQString(conf->getBool("dynsys_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65610",boolToQString(conf->getBool("fetcomp_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65618",boolToQString(conf->getBool("fetcomp_autoattack"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65616",boolToQString(conf->getBool("fetcomp_autogain"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65614",boolToQString(conf->getBool("fetcomp_autoknee"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65620",boolToQString(conf->getBool("fetcomp_autorelease"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65626",boolToQString(conf->getBool("fetcomp_noclip"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65574",boolToQString(conf->getBool("vb_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65578",boolToQString(conf->getBool("vc_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65551",boolToQString(conf->getBool("eq_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65565",boolToQString(conf->getBool("agc_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65559",boolToQString(conf->getBool("reverb_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65583",boolToQString(conf->getBool("tube_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65544",boolToQString(conf->getBool("vhe_enable"))));
        map.appendChild(generateXmlEntry(&doc,"boolean","65548",boolToQString(conf->getBool("vse_enable"))));
        //Strings...
        map.appendChild(generateXmlEntry(&doc,"int","65585",conf->getString("ax_mode")));
        map.appendChild(generateXmlEntry(&doc,"int","65587",conf->getString("out_pan")));
        map.appendChild(generateXmlEntry(&doc,"int","65555",QString::number(conf->getInt("colm_midimage")/4)));
        map.appendChild(generateXmlEntry(&doc,"int","65554;65556",conf->getString("colm_widening") + ";" + conf->getString("colm_depth")));
        map.appendChild(generateXmlEntry(&doc,"int","65543",conf->getString("conv_cc_level")));
        map.appendChild(generateXmlEntry(&doc,"int","65582",conf->getString("cure_level")));
        map.appendChild(generateXmlEntry(&doc,"int","65558",QString::number(conf->getInt("ds_level")*20)));
        map.appendChild(generateXmlEntry(&doc,"int","65573",QString::number((conf->getInt("dynsys_bassgain")-100)/20)));
        map.appendChild(generateXmlEntry(&doc,"string","65570;65571;65572",dynsyscoeffs));

        map.appendChild(generateXmlEntry(&doc,"int","65625",conf->getString("fetcomp_meta_adapt")));
        map.appendChild(generateXmlEntry(&doc,"int","65617",conf->getString("fetcomp_attack")));
        map.appendChild(generateXmlEntry(&doc,"int","65624",conf->getString("fetcomp_meta_crest")));
        map.appendChild(generateXmlEntry(&doc,"int","65615",conf->getString("fetcomp_gain")));
        map.appendChild(generateXmlEntry(&doc,"int","65613",conf->getString("fetcomp_kneewidth")));
        map.appendChild(generateXmlEntry(&doc,"int","65621",conf->getString("fetcomp_meta_kneemulti")));
        map.appendChild(generateXmlEntry(&doc,"int","65622",conf->getString("fetcomp_meta_maxattack")));
        map.appendChild(generateXmlEntry(&doc,"int","65623",conf->getString("fetcomp_meta_maxrelease")));
        map.appendChild(generateXmlEntry(&doc,"int","65612",conf->getString("fetcomp_ratio")));
        map.appendChild(generateXmlEntry(&doc,"int","65619",conf->getString("fetcomp_release")));
        map.appendChild(generateXmlEntry(&doc,"int","65611",conf->getString("fetcomp_threshold")));

        map.appendChild(generateXmlEntry(&doc,"int","65576",conf->getString("vb_freq")));
        map.appendChild(generateXmlEntry(&doc,"int","65677",conf->getString("vb_gain")));

        map.appendChild(generateXmlEntry(&doc,"string","65575",conf->getString("vb_mode")));
        map.appendChild(generateXmlEntry(&doc,"int","66580",conf->getString("vc_level")));
        map.appendChild(generateXmlEntry(&doc,"string","65579",conf->getString("vc_mode")));

        map.appendChild(generateXmlEntry(&doc,"string","65552",eqbands));
        map.appendChild(generateXmlEntry(&doc,"int","65588",conf->getString("lim_threshold")));
        map.appendChild(generateXmlEntry(&doc,"int","65586",conf->getString("out_volume")));
        map.appendChild(generateXmlEntry(&doc,"int","65566",conf->getString("agc_volume")));
        map.appendChild(generateXmlEntry(&doc,"int","65568",conf->getString("agc_ratio")));
        map.appendChild(generateXmlEntry(&doc,"int","65567",conf->getString("agc_maxgain")));
        map.appendChild(generateXmlEntry(&doc,"int","65562",conf->getString("reverb_damp")));
        map.appendChild(generateXmlEntry(&doc,"int","65564",conf->getString("reverb_dry")));
        map.appendChild(generateXmlEntry(&doc,"int","65560",conf->getString("reverb_roomsize")));
        map.appendChild(generateXmlEntry(&doc,"int","65561",conf->getString("reverb_width")));
        map.appendChild(generateXmlEntry(&doc,"int","65563",conf->getString("reverb_wet")));
        map.appendChild(generateXmlEntry(&doc,"int","65545",conf->getString("vhe_level")));
        map.appendChild(generateXmlEntry(&doc,"int","65549;65550",QString::number(translate(conf->getInt("vse_bark_cons"),0.01f,0.1f,0.1f,1)/1000)));
    }
    else{
        result.notices = "Error: Unknown config type";
        return result;
    }
    doc.appendChild(map);
    result.configuration = doc.toString().toUtf8().constData();
    return result;
}
QDomElement converter::generateXmlEntry(QDomDocument* doc,QString type,const QString& name,const QString& value){
    QDomElement obj = doc->createElement(type);
    obj.setAttribute("name", name);
    if(type == "boolean"||type=="int")obj.setAttribute("value", value);
    else obj.appendChild( doc->createTextNode(value));
    return obj;
}
QString converter::boolToQString(bool b){
    if(b) return "true";
    return "false";
}
