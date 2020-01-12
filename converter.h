#ifndef CONVERTER_H
#define CONVERTER_H
#include <QString>
#include <QDomElement>
class configmodel{
public:
    bool analogx_enable = false;
    bool colorfulmusic_enable = false;
    bool convolver_enable = false;
    bool cure_enable = false;
    bool diffsurr_enable = false;
    bool dynsys_enable = false;
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
    QString colorfulmusic_coeffs = "120;200"; //Unused with linux-to-android
    QString colorfulmusic_midimage = "150";
    QString convolver_crosschannel = "0";
    QString cure_crossfeed = "0";
    QString diffsurr_delay = "500";
    QString dynsys_bassgain = "0";
    QString dynsys_coeffs = "100;5600;40;40;50;50"; //Unused with linux-to-android
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
    QString fireq_custom = "0.0;0.0;0.0;0.0;0.0;0.0;0.0;0.0;0.0;0.0;"; //Unused with linux-to-android
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
    bool found_irs = false;
    bool found_spkopt = false;

    //Variables needed as temporary storage (linux-to-android only):
    QString temp_colm_widening = "120";
    QString temp_colm_depth = "200";
    QString temp_eqbands[10] = {"0","0","0","0","0","0","0","0","0","0"};
    QString temp_dynsys_coeffs[6] = {"100","5600","40","40","50","50"};
};
class converter
{
public:
    enum configtype{
        officialV4A,
        teamDeWittV4A
    };
    static std::string toLinux(const std::string&,configtype);
    static std::string toAndroid(const std::string&,configtype);
private:
    static QString boolToQString(bool b);
    static QDomElement generateXmlEntry(QDomDocument*,QString type,const QString& name,const QString& value);
    static void decodeLinuxKey(const std::string& key,const std::string& value,configmodel*);
    static bool is_only_ascii_whitespace(const std::string& str);
    static void readLinux(std::string path);
    static std::string intToString(int);
    static float translate(int value,float leftMin,float leftMax,float rightMin,float rightMax);

};

#endif // CONVERTER_H
