#ifndef PRESETEXTENSION_H
#define PRESETEXTENSION_H
#include <QString>
namespace EQ {
inline const int* lookupEQPreset(QString preset){
    if(preset == "Pop")
        return std::initializer_list<int>({0,0,0,125,250,500,-150,-300,-300,-300}).begin();
    else if(preset == "Rock")
        return std::initializer_list<int>({0,0,300,-1000,-150,75,300,300,300,300}).begin();
    else if(preset == "Jazz")
        return std::initializer_list<int>({0,0,273,600,-600,-250,250,-75,-75,-75}).begin();
    else if(preset == "Classic")
        return std::initializer_list<int>({0,0,-900,0,150,0,0,900,900,900}).begin();
    else if(preset == "Bass")
        return std::initializer_list<int>({1150,850,500,200,0,0,0,0,0,0}).begin();
    else if(preset == "Clear")
        return std::initializer_list<int>({350,650,950,650,350,125,500,900,1100,900}).begin();
    else if(preset == "Volume Boost")
        return std::initializer_list<int>({1200,1200,1200,1200,1200,1200,1200,1200,1200,1200}).begin();
    else if(preset == "Hip-Hop")
        return std::initializer_list<int>({450,400,150,300,-150,-150,150,-100,150,300}).begin();
    else if(preset == "Dubstep")
        return std::initializer_list<int>({1200,50,-200,-500,-500,-450,-250,0,-300,-50}).begin();
    else if(preset == "Movie")
        return std::initializer_list<int>({300,600,900,700,600,500,600,350,1050,800}).begin();
    else if(preset == "Metal")
        return std::initializer_list<int>({1050,750,0,550,0,0,600,0,900,1200}).begin();
    else if(preset == "Vocal Booster")
        return std::initializer_list<int>({-150,-300,-300,150,350,350,300,150,0,-150}).begin();
    else if(preset == "Hardstyle")
        return std::initializer_list<int>({600,1200,0,-1200,300,650,0,-450,-800,-1050}).begin();
    else if(preset == "Acoustic")
        return std::initializer_list<int>({500,450,350,100,150,150,300,350,300,300}).begin();
    else if(preset == "R&B")
        return std::initializer_list<int>({300,700,600,150,-200,-150,200,300,350,400}).begin();
    else if(preset == "Electronic")
        return std::initializer_list<int>({400,350,50,-50,-200,150,0,50,300,450}).begin();
    else if(preset == "Deep Bass")
        return std::initializer_list<int>({1200,0,-1200,-900,-350,-600,0,-500,0,300}).begin();
    else if(preset == "Beats")
        return std::initializer_list<int>({-550,-450,-400,-300,-150,0,0,0,0,0}).begin();
    else if(preset == "Soft Bass")
        return std::initializer_list<int>({1200,1034,900,700,600,500,600,350,1050,800}).begin();
    return nullptr;
}
inline const int* lookupDynsysPreset(QString preset){
    if(preset == "Extreme Headphone (v2)")
        return std::initializer_list<int>({140,6200,40,60,10,80}).begin();
    else if(preset == "High-end Headphone (v2)")
        return std::initializer_list<int>({180,5800,55,80,10,70}).begin();
    else if(preset == "Common Headphone (v2)")
        return std::initializer_list<int>({300,5600,60,105,10,50}).begin();
    else if(preset == "Low-end Headphone (v2)")
        return std::initializer_list<int>({600,5400,60,105,10,20}).begin();
    else if(preset == "Common Earphone (v2)")
        return std::initializer_list<int>({100,5600,40,80,50,50}).begin();
    else if(preset == "Extreme Headphone (v1)")
        return std::initializer_list<int>({1200,6200,40,80,0,20}).begin();
    else if(preset == "High-end Headphone (v1)")
        return std::initializer_list<int>({1000,6200,40,80,0,10}).begin();
    else if(preset == "Common Headphone (v1)")
        return std::initializer_list<int>({800,6200,40,80,10,0}).begin();
    else if(preset == "Common Earphone (v1)")
        return std::initializer_list<int>({400,6200,40,80,10,0}).begin();
    else if(preset == "Apple Earphone")
        return std::initializer_list<int>({1200,6200,50,90,15,10}).begin();
    else if(preset == "Monster Earphone")
        return std::initializer_list<int>({1000,6200,50,90,30,10}).begin();
    else if(preset == "Moto Earphone")
        return std::initializer_list<int>({1100,6200,60,100,20,0}).begin();
    else if(preset == "Philips Earphone")
        return std::initializer_list<int>({1200,6200,50,100,10,50}).begin();
    else if(preset == "SHP2000")
        return std::initializer_list<int>({1200,6200,60,100,0,30}).begin();
    else if(preset == "SHP9000")
        return std::initializer_list<int>({1200,6200,40,80,0,30}).begin();
    else if(preset == "Unknown Type I")
        return std::initializer_list<int>({1000,6200,60,100,0,0}).begin();
    else if(preset == "Unknown Type II")
        return std::initializer_list<int>({1000,6200,60,120,0,0}).begin();
    else if(preset == "Unknown Type III")
        return std::initializer_list<int>({1000,6200,80,140,0,0}).begin();
    else if(preset == "Unknown Type IV")
        return std::initializer_list<int>({800,6200,80,140,0,0}).begin();
}
inline const int* lookupColmPreset(QString preset){
    if(preset == "Slight")
        return std::initializer_list<int>({120,200}).begin();
    else if(preset == "Level 1")
        return std::initializer_list<int>({130,275}).begin();
    else if(preset == "Level 2")
        return std::initializer_list<int>({140,350}).begin();
    else if(preset == "Level 3")
        return std::initializer_list<int>({150,425}).begin();
    else if(preset == "Level 4")
        return std::initializer_list<int>({160,500}).begin();
    else if(preset == "Level 5")
        return std::initializer_list<int>({170,575}).begin();
    else if(preset == "Level 6")
        return std::initializer_list<int>({180,650}).begin();
    else if(preset == "Level 7")
        return std::initializer_list<int>({190,725}).begin();
    else if(preset == "Extreme")
        return std::initializer_list<int>({200,800}).begin();
}

inline const int* defaultEQPreset(){
    return std::initializer_list<int>({0,0,0,0,0,0,0,0,0,0}).begin();
}
}
#endif // PRESETEXTENSION_H
