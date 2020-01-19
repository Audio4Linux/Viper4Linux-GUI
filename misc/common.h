#ifndef COMMON_H
#define COMMON_H
#include <QDir>
#include <string>
#include <algorithm>
static std::string default_config = "fx_enable=true\nconv_enable=false\nconv_ir_path=\nconv_cc_level=0\nvhe_enable=false\nvhe_level=0\nvse_enable=false\nvse_ref_bark=7600\nvse_bark_cons=10\neq_enable=false\neq_band1=0\neq_band2=0\neq_band3=0\neq_band4=0\neq_band5=0\neq_band6=0\neq_band7=0\neq_band8=0\neq_band9=0\neq_band10=0\ncolm_enable=false\ncolm_widening=100\ncolm_depth=0\nds_enable=false\nds_level=0\nreverb_enable=false\nreverb_roomsize=30\nreverb_width=40\nreverb_damp=10\nreverb_wet=20\nreverb_dry=80\nagc_enable=false\nagc_ratio=100\nagc_volume=100\nagc_maxgain=100\nvb_enable=false\nvb_mode=0\nvb_freq=76\nvb_gain=0\nvc_enable=false\nvc_mode=0\nvc_level=0\ncure_enable=false\ncure_level=0\ntube_enable=false\nax_enable=false\nax_mode=0\nfetcomp_enable=false\nfetcomp_threshold=0\nfetcomp_ratio=0\nfetcomp_kneewidth=0\nfetcomp_autoknee=true\nfetcomp_gain=0\nfetcomp_autogain=true\nfetcomp_attack=51\nfetcomp_autoattack=true\nfetcomp_release=38\nfetcomp_autorelease=true\nfetcomp_meta_kneemulti=50\nfetcomp_meta_maxattack=88\nfetcomp_meta_maxrelease=88\nfetcomp_meta_crest=61\nfetcomp_meta_adapt=66\nfetcomp_noclip=true\nout_volume=100\nout_pan=0\nlim_threshold=100\ndynsys_bassgain=0\ndynsys_xcoeff1=0\ndynsys_xcoeff2=0\ndynsys_ycoeff1=0\ndynsys_ycoeff2=0\ndynsys_sidegain1=0\ndynsys_sidegain2=0\ndynsys_enable=false";
static inline bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}
static float translate(int value,int leftMin,int leftMax,float rightMin,float rightMax){
    float leftSpan = leftMax - leftMin;
    float rightSpan = rightMax - rightMin;
    float valueScaled = float(value - leftMin) / float(leftSpan);
    return rightMin + (valueScaled * rightSpan);
}
static inline bool is_only_ascii_whitespace( const std::string& str )
{
    auto it = str.begin();
    do {
        if (it == str.end()) return true;
    } while (*it >= 0 && *it <= 0x7f && std::isspace(*(it++)));
    return false;
}

static inline QString pathAppend(const QString& path1, const QString& path2)
{
    return QDir::cleanPath(path1 + QDir::separator() + path2);
}

#endif // COMMON_H
