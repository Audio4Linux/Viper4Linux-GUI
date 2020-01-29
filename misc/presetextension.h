/*
 *  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  ThePBone <tim.schneeberger(at)outlook.de> (c) 2020
 */
#ifndef PRESETEXTENSION_H
#define PRESETEXTENSION_H
#include <QString>
namespace EQ {
inline const std::initializer_list<float> lookupEQPreset(QString preset){
    if(preset == "Pop")
        return std::initializer_list<float>({0,0,0,1.25,2.50,5.00,-1.50,-3.00,-3.00,-3.00});
    else if(preset == "Rock")
        return std::initializer_list<float>({0,0,3.00,-10.00,-1.50,0.75,3.00,3.00,3.00,3.00});
    else if(preset == "Jazz")
        return std::initializer_list<float>({0,0,2.73,6.00,-6.00,-2.50,2.50,-0.75,-0.75,-0.75});
    else if(preset == "Classic")
        return std::initializer_list<float>({0,0,-9.00,0,1.50,0,0,9.00,9.00,9.00});
    else if(preset == "Bass")
        return std::initializer_list<float>({11.50,8.50,5.00,2.00,0,0,0,0,0,0});
    else if(preset == "Clear")
        return std::initializer_list<float>({3.50,6.50,9.50,6.50,3.50,1.25,5.00,9.00,11.00,9.00});
    else if(preset == "Volume Boost")
        return std::initializer_list<float>({12.00,12.00,12.00,12.00,12.00,12.00,12.00,12.00,12.00,12.00});
    else if(preset == "Hip-Hop")
        return std::initializer_list<float>({4.50,4.00,1.50,3.00,-1.50,-1.50,1.50,-1.00,1.50,3.00});
    else if(preset == "Dubstep")
        return std::initializer_list<float>({12.00,0.50,-2.00,-5.00,-5.00,-4.50,-2.50,0,-3.00,-.050});
    else if(preset == "Movie")
        return std::initializer_list<float>({3.00,6.00,9.00,7.00,6.00,5.00,6.00,3.50,10.50,8.00});
    else if(preset == "Metal")
        return std::initializer_list<float>({10.50,7.50,0,5.50,0,0,6.00,0,9.00,12.00});
    else if(preset == "Vocal Booster")
        return std::initializer_list<float>({-1.50,-3.00,-3.00,1.50,3.50,3.50,3.00,1.50,0,-1.50});
    else if(preset == "Hardstyle")
        return std::initializer_list<float>({6.00,12.00,0,-12.00,3.00,6.50,0,-4.50,-8.00,-10.50});
    else if(preset == "Acoustic")
        return std::initializer_list<float>({5.00,4.50,3.50,1.00,1.50,1.50,3.00,3.50,3.00,3.00});
    else if(preset == "R&B")
        return std::initializer_list<float>({3.00,7.00,6.00,1.50,-2.00,-1.50,2.00,3.00,3.50,4.00});
    else if(preset == "Electronic")
        return std::initializer_list<float>({4.00,3.50,0.50,-0.50,-2.00,1.50,0,0.50,3.00,4.50});
    else if(preset == "Deep Bass")
        return std::initializer_list<float>({12.00,0,-12.00,-9.00,-3.50,-6.00,0,-5.00,0,3.00});
    else if(preset == "Beats")
        return std::initializer_list<float>({-5.50,-4.50,-4.00,-3.00,-1.50,0,0,0,0,0});
    else if(preset == "Soft Bass")
        return std::initializer_list<float>({12.00,10.34,9.00,7.00,6.00,5.00,6.00,3.50,10.50,8.00});
    return std::initializer_list<float>({});
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

inline const std::initializer_list<float> defaultEQPreset(){
    return std::initializer_list<float>({0,0,0,0,0,0,0,0,0,0});
}
}
#endif // PRESETEXTENSION_H
