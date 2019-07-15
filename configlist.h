#ifndef CONFIGLIST_H
#define CONFIGLIST_H


#include <string>

enum Config {
    fx_enable,
    tube_enable,
    colm_enable,
    colm_widening,
    colm_depth,
    colm_midimage,
    vc_enable,
    vc_mode,
    vc_level,
    vb_enable,
    vb_mode,
    vb_freq,
    vb_gain,
    vhe_enable,
    vhe_level,
    ds_enable,
    ds_level,
    reverb_enable,
    reverb_roomsize,
    reverb_width,
    reverb_damp,
    reverb_wet,
    reverb_dry,
    agc_enable,
    agc_ratio,
    agc_maxgain,
    agc_volume,
    lim_threshold,
    out_pan,
    out_volume,
    eq_enable,
    eq_band1,
    eq_band2,
    eq_band3,
    eq_band4,
    eq_band5,
    eq_band6,
    eq_band7,
    eq_band8,
    eq_band9,
    eq_band10,
    fetcomp_enable,
    fetcomp_autogain,
    fetcomp_autoknee,
    fetcomp_autoattack,
    fetcomp_autorelease,
    fetcomp_noclip,
    fetcomp_threshold,
    fetcomp_gain,
    fetcomp_kneewidth,
    fetcomp_ratio,
    fetcomp_attack,
    fetcomp_release,
    fetcomp_meta_adapt,
    fetcomp_meta_crest,
    fetcomp_meta_maxattack,
    fetcomp_meta_maxrelease,
    fetcomp_meta_kneemulti,
    cure_enable,
    cure_level,
    ax_enable,
    ax_mode,
    vse_enable,
    vse_ref_bark,
    vse_bark_cons,
    conv_enable,
    conv_cc_level,
    conv_ir_path,
    dynsys_xcoeff1,
    dynsys_xcoeff2,
    dynsys_ycoeff1,
    dynsys_ycoeff2,
    dynsys_sidegain1,
    dynsys_sidegain2,
    dynsys_enable,
    dynsys_bassgain,
    unknown
};
enum AppConfig {
    configpath,
    autoapply,
    mutedRestart,
    glavafix,
    stylesheet,
    thememode,
    colorpalette,
    custompalette,
    customwhiteicons,
    unknownApp
};
inline Config resolveConfig(const std::string& input) {
    if( input == "fx_enable" ) return fx_enable;
    if( input == "tube_enable" ) return tube_enable;
    if( input == "colm_enable" ) return colm_enable;
    if( input == "colm_widening" ) return colm_widening;
    if( input == "colm_depth" ) return colm_depth;
    if( input == "colm_midimage" ) return colm_midimage;
    if( input == "vc_enable" ) return vc_enable;
    if( input == "vc_mode" ) return vc_mode;
    if( input == "vc_level" ) return vc_level;
    if( input == "vb_enable" ) return vb_enable;
    if( input == "vb_mode" ) return vb_mode;
    if( input == "vb_freq" ) return vb_freq;
    if( input == "vb_gain" ) return vb_gain;
    if( input == "vhe_enable" ) return vhe_enable;
    if( input == "vhe_level" ) return vhe_level;
    if( input == "ds_enable" ) return ds_enable;
    if( input == "ds_level" ) return ds_level;
    if( input == "reverb_enable" ) return reverb_enable;
    if( input == "reverb_roomsize" ) return reverb_roomsize;
    if( input == "reverb_width" ) return reverb_width;
    if( input == "reverb_damp" ) return reverb_damp;
    if( input == "reverb_wet" ) return reverb_wet;
    if( input == "reverb_dry" ) return reverb_dry;
    if( input == "agc_enable" ) return agc_enable;
    if( input == "agc_ratio" ) return agc_ratio;
    if( input == "agc_maxgain" ) return agc_maxgain;
    if( input == "agc_volume" ) return agc_volume;
    if( input == "lim_threshold" ) return lim_threshold;
    if( input == "out_pan" ) return out_pan;
    if( input == "out_volume" ) return out_volume;
    if( input == "eq_enable" ) return eq_enable;
    if( input == "eq_band1" ) return eq_band1;
    if( input == "eq_band2" ) return eq_band2;
    if( input == "eq_band3" ) return eq_band3;
    if( input == "eq_band4" ) return eq_band4;
    if( input == "eq_band5" ) return eq_band5;
    if( input == "eq_band6" ) return eq_band6;
    if( input == "eq_band7" ) return eq_band7;
    if( input == "eq_band8" ) return eq_band8;
    if( input == "eq_band9" ) return eq_band9;
    if( input == "eq_band10" ) return eq_band10;
    if( input == "fetcomp_enable" ) return fetcomp_enable;
    if( input == "fetcomp_autogain" ) return fetcomp_autogain;
    if( input == "fetcomp_autoknee" ) return fetcomp_autoknee;
    if( input == "fetcomp_autoattack" ) return fetcomp_autoattack;
    if( input == "fetcomp_autorelease" ) return fetcomp_autorelease;
    if( input == "fetcomp_noclip" ) return fetcomp_noclip;
    if( input == "fetcomp_threshold" ) return fetcomp_threshold;
    if( input == "fetcomp_gain" ) return fetcomp_gain;
    if( input == "fetcomp_kneewidth" ) return fetcomp_kneewidth;
    if( input == "fetcomp_ratio" ) return fetcomp_ratio;
    if( input == "fetcomp_attack" ) return fetcomp_attack;
    if( input == "fetcomp_release" ) return fetcomp_release;
    if( input == "fetcomp_meta_adapt" ) return fetcomp_meta_adapt;
    if( input == "fetcomp_meta_crest" ) return fetcomp_meta_crest;
    if( input == "fetcomp_meta_maxattack" ) return fetcomp_meta_maxattack;
    if( input == "fetcomp_meta_maxrelease" ) return fetcomp_meta_maxrelease;
    if( input == "fetcomp_meta_kneemulti" ) return fetcomp_meta_kneemulti;
    if( input == "cure_enable" ) return cure_enable;
    if( input == "cure_level" ) return cure_level;
    if( input == "ax_enable" ) return ax_enable;
    if( input == "ax_mode" ) return ax_mode;
    if( input == "vse_enable" ) return vse_enable;
    if( input == "vse_ref_bark" ) return vse_ref_bark;
    if( input == "vse_bark_cons" ) return vse_bark_cons;
    if( input == "conv_enable" ) return conv_enable;
    if( input == "conv_cc_level" ) return conv_cc_level;
    if( input == "conv_ir_path" ) return conv_ir_path;
    if( input == "dynsys_enable" ) return dynsys_enable;
    if( input == "dynsys_bassgain" ) return dynsys_bassgain;
    if( input == "dynsys_xcoeff1" ) return dynsys_xcoeff1;
    if( input == "dynsys_xcoeff2" ) return dynsys_xcoeff2;
    if( input == "dynsys_ycoeff1" ) return dynsys_ycoeff1;
    if( input == "dynsys_ycoeff2" ) return dynsys_ycoeff2;
    if( input == "dynsys_sidegain1" ) return dynsys_sidegain1;
    if( input == "dynsys_sidegain2" ) return dynsys_sidegain2;
   return unknown;
}
inline AppConfig resolveAppConfig(const std::string& input) {
    if( input == "configpath" ) return configpath;
    if( input == "autoapply" ) return autoapply;
    if( input == "glavafix" ) return glavafix;
    if( input == "muteOnRestart" ) return mutedRestart;
    if( input == "stylesheet" ) return stylesheet;
    if( input == "thememode" ) return thememode;
    if( input == "colorpalette" ) return colorpalette;
    if( input == "customwhiteicons" ) return customwhiteicons;
    if( input == "custompalette" ) return custompalette;
   return unknownApp;
}

static std::string default_config = "fx_enable=true\nconv_enable=false\nconv_ir_path=\nconv_cc_level=0\nvhe_enable=false\nvhe_level=0\nvse_enable=false\nvse_ref_bark=7600\nvse_bark_cons=10\neq_enable=false\neq_band1=0\neq_band2=0\neq_band3=0\neq_band4=0\neq_band5=0\neq_band6=0\neq_band7=0\neq_band8=0\neq_band9=0\neq_band10=0\ncolm_enable=false\ncolm_widening=100\ncolm_depth=0\nds_enable=false\nds_level=0\nreverb_enable=false\nreverb_roomsize=30\nreverb_width=40\nreverb_damp=10\nreverb_wet=20\nreverb_dry=80\nagc_enable=false\nagc_ratio=100\nagc_volume=100\nagc_maxgain=100\nvb_enable=false\nvb_mode=0\nvb_freq=76\nvb_gain=0\nvc_enable=false\nvc_mode=0\nvc_level=0\ncure_enable=false\ncure_level=0\ntube_enable=false\nax_enable=false\nax_mode=0\nfetcomp_enable=false\nfetcomp_threshold=0\nfetcomp_ratio=0\nfetcomp_kneewidth=0\nfetcomp_autoknee=true\nfetcomp_gain=0\nfetcomp_autogain=true\nfetcomp_attack=51\nfetcomp_autoattack=true\nfetcomp_release=38\nfetcomp_autorelease=true\nfetcomp_meta_kneemulti=50\nfetcomp_meta_maxattack=88\nfetcomp_meta_maxrelease=88\nfetcomp_meta_crest=61\nfetcomp_meta_adapt=66\nfetcomp_noclip=true\nout_volume=100\nout_pan=0\nlim_threshold=100\ndynsys_bassgain=0\ndynsys_xcoeff1=0\ndynsys_xcoeff2=0\ndynsys_ycoeff1=0\ndynsys_ycoeff2=0\ndynsys_sidegain1=0\ndynsys_sidegain2=0\ndynsys_enabled=false";
static std::string default_appconfig = "path=\"\"\nautoapply=false\nglavafix=false\nmuteOnRestart=true";
#endif // CONFIGLIST_H
