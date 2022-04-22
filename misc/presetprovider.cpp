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
#include "presetprovider.h"
#include "common.h"
namespace PresetProvider {
const FLOAT_LIST EQ::defaultPreset(){
    return EQ_LOOKUP_TABLE().first();
}

const FLOAT_LIST EQ::lookupPreset(const QString& preset){
    auto table = EQ_LOOKUP_TABLE();
    if(table.contains(preset))
        return table[preset];
    else
        return table.first();
}

const QString EQ::reverseLookup(const QVector<float>& data){
    auto table = EQ_LOOKUP_TABLE();
    for(auto key : table.keys()){
        QVector<float> row(table[key]);
        int it = 0;
        bool different = false;
        for(auto cur_data : row){
            bool equal = isApproximatelyEqual<float>(cur_data,data.at(it),0.01);
            if(!equal){
                different = true;
                break;
            }
            it++;
        }
        if(!different)
            return key;
    }
    return "Custom";
}

const QMap<EQ_UNIT> EQ::EQ_LOOKUP_TABLE(){
    InitializableQMap<EQ_UNIT> table;
    table << QPair<EQ_UNIT>(QObject::tr("Default"),FLOAT_LIST({0,0,0,0,0,0,0,0,0,0}))
          << QPair<EQ_UNIT>(QObject::tr("Pop"),FLOAT_LIST({0,0,0,1.25,2.50,5.00,-1.50,-3.00,-3.00,-3.00}))
          << QPair<EQ_UNIT>(QObject::tr("Rock"),FLOAT_LIST({0,0,3.00,-10.00,-1.50,0.75,3.00,3.00,3.00,3.00}))
          << QPair<EQ_UNIT>(QObject::tr("Jazz"),FLOAT_LIST({0,0,2.73,6.00,-6.00,-2.50,2.50,-0.75,-0.75,-0.75}))
          << QPair<EQ_UNIT>(QObject::tr("Classic"),FLOAT_LIST({0,0,-9.00,0,1.50,0,0,9.00,9.00,9.00}))
          << QPair<EQ_UNIT>(QObject::tr("Bass"),FLOAT_LIST({11.50,8.50,5.00,2.00,0,0,0,0,0,0}))
          << QPair<EQ_UNIT>(QObject::tr("Clear"),FLOAT_LIST({3.50,6.50,9.50,6.50,3.50,1.25,5.00,9.00,11.00,9.00}))
          << QPair<EQ_UNIT>(QObject::tr("Hip-Hop"),FLOAT_LIST({4.50,4.00,1.50,3.00,-1.50,-1.50,1.50,-1.00,1.50,3.00}))
          << QPair<EQ_UNIT>(QObject::tr("Dubstep"),FLOAT_LIST({12.00,0.50,-2.00,-5.00,-5.00,-4.50,-2.50,0,-3.00,-.050}))
          << QPair<EQ_UNIT>(QObject::tr("Movie"),FLOAT_LIST({3.00,6.00,9.00,7.00,6.00,5.00,6.00,3.50,10.50,8.00}))
          << QPair<EQ_UNIT>(QObject::tr("Metal"),FLOAT_LIST({10.50,7.50,0,5.50,0,0,6.00,0,9.00,12.00}))
          << QPair<EQ_UNIT>(QObject::tr("Vocal Booster"),FLOAT_LIST({-1.50,-3.00,-3.00,1.50,3.50,3.50,3.00,1.50,0,-1.50}))
          << QPair<EQ_UNIT>(QObject::tr("Hardstyle"),FLOAT_LIST({6.00,12.00,0,-12.00,3.00,6.50,0,-4.50,-8.00,-10.50}))
          << QPair<EQ_UNIT>(QObject::tr("Acoustic"),FLOAT_LIST({5.00,4.50,3.50,1.00,1.50,1.50,3.00,3.50,3.00,3.00}))
          << QPair<EQ_UNIT>(QObject::tr("R&B"),FLOAT_LIST({3.00,7.00,6.00,1.50,-2.00,-1.50,2.00,3.00,3.50,4.00}))
          << QPair<EQ_UNIT>(QObject::tr("Electronic"),FLOAT_LIST({4.00,3.50,0.50,-0.50,-2.00,1.50,0,0.50,3.00,4.50}))
          << QPair<EQ_UNIT>(QObject::tr("Deep Bass"),FLOAT_LIST({12.00,0,-12.00,-9.00,-3.50,-6.00,0,-5.00,0,3.00}))
          << QPair<EQ_UNIT>(QObject::tr("Beats"),FLOAT_LIST({-5.50,-4.50,-4.00,-3.00,-1.50,0,0,0,0,0}))
          << QPair<EQ_UNIT>(QObject::tr("Soft Bass"),FLOAT_LIST({12.00,10.34,9.00,7.00,6.00,5.00,6.00,3.50,10.50,8.00}));
    return std::move(table);
}

const QMap<DYNSYS_UNIT> Dynsys::DYNSYS_LOOKUP_TABLE(){
    InitializableQMap<DYNSYS_UNIT> table;
    table << QPair<DYNSYS_UNIT>(QObject::tr("Unknown"),INT_LIST({0}))
          << QPair<DYNSYS_UNIT>(QObject::tr("Extreme Headphone (v2)"),INT_LIST({140,6200,40,60,10,80}))
          << QPair<DYNSYS_UNIT>(QObject::tr("High-end Headphone (v2)"),INT_LIST({180,5800,55,80,10,70}))
          << QPair<DYNSYS_UNIT>(QObject::tr("Common Headphone (v2)"),INT_LIST({300,5600,60,105,10,50}))
          << QPair<DYNSYS_UNIT>(QObject::tr("Low-end Headphone (v2)"),INT_LIST({600,5400,60,105,10,20}))
          << QPair<DYNSYS_UNIT>(QObject::tr("Common Earphone (v2)"),INT_LIST({100,5600,40,80,50,50}))
          << QPair<DYNSYS_UNIT>(QObject::tr("Extreme Headphone (v1)"),INT_LIST({1200,6200,40,80,0,20}))
          << QPair<DYNSYS_UNIT>(QObject::tr("High-end Headphone (v1)"),INT_LIST({1000,6200,40,80,0,10}))
          << QPair<DYNSYS_UNIT>(QObject::tr("Common Headphone (v1)"),INT_LIST({800,6200,40,80,10,0}))
          << QPair<DYNSYS_UNIT>(QObject::tr("Common Earphone (v1)"),INT_LIST({400,6200,40,80,10,0}))
          << QPair<DYNSYS_UNIT>(QObject::tr("Apple Earphone"),INT_LIST({1200,6200,50,90,15,10}))
          << QPair<DYNSYS_UNIT>(QObject::tr("Monster Earphone"),INT_LIST({1000,6200,50,90,30,10}))
          << QPair<DYNSYS_UNIT>(QObject::tr("Moto Earphone"),INT_LIST({1100,6200,60,100,20,0}))
          << QPair<DYNSYS_UNIT>(QObject::tr("Philips Earphone"),INT_LIST({1200,6200,50,100,10,50}))
          << QPair<DYNSYS_UNIT>(QObject::tr("SHP2000"),INT_LIST({1200,6200,60,100,0,30}))
          << QPair<DYNSYS_UNIT>(QObject::tr("SHP9000"),INT_LIST({1200,6200,40,80,0,30}))
          << QPair<DYNSYS_UNIT>(QObject::tr("Unknown Type I"),INT_LIST({1000,6200,60,100,0,0}))
          << QPair<DYNSYS_UNIT>(QObject::tr("Unknown Type II"),INT_LIST({1000,6200,60,120,0,0}))
          << QPair<DYNSYS_UNIT>(QObject::tr("Unknown Type III"),INT_LIST({1000,6200,80,140,0,0}))
          << QPair<DYNSYS_UNIT>(QObject::tr("Unknown Type IV"),INT_LIST({800,6200,80,140,0,0}));
    return std::move(table);
}

const INT_LIST Dynsys::lookupPreset(const QString& preset){
    auto table = DYNSYS_LOOKUP_TABLE();
    if(table.contains(preset))
        return table[preset];
    else
        return table.first();
}


const QString Dynsys::reverseLookup(const QVector<int>& data){
    auto table = DYNSYS_LOOKUP_TABLE();
    for(auto key : table.keys()){
        QVector<int> row(table[key]);
        int it = 0;
        bool different = false;
        for(auto cur_data : row){
            bool equal = cur_data == data.at(it);
            if(!equal){
                different = true;
                break;
            }
            it++;
        }
        if(!different)
            return key;
    }
    return "Custom";
}

const QMap<COLM_UNIT> Colm::COLM_LOOKUP_TABLE(){
    InitializableQMap<COLM_UNIT> table;
    table << QPair<COLM_UNIT>(QObject::tr("Unknown"),INT_LIST({0,0}))
          << QPair<COLM_UNIT>(QObject::tr("Slight"),INT_LIST({120,200}))
          << QPair<COLM_UNIT>(QObject::tr("Level 1"),INT_LIST({130,275}))
          << QPair<COLM_UNIT>(QObject::tr("Level 2"),INT_LIST({140,350}))
          << QPair<COLM_UNIT>(QObject::tr("Level 3"),INT_LIST({150,425}))
          << QPair<COLM_UNIT>(QObject::tr("Level 4"),INT_LIST({160,500}))
          << QPair<COLM_UNIT>(QObject::tr("Level 5"),INT_LIST({170,575}))
          << QPair<COLM_UNIT>(QObject::tr("Level 6"),INT_LIST({180,650}))
          << QPair<COLM_UNIT>(QObject::tr("Level 7"),INT_LIST({190,725}))
          << QPair<COLM_UNIT>(QObject::tr("Extreme"),INT_LIST({200,800}));
    return std::move(table);
}

const INT_LIST Colm::lookupPreset(const QString& preset){
    auto table = COLM_LOOKUP_TABLE();
    if(table.contains(preset))
        return table[preset];
    else
        return table.first();
}

const QString Colm::reverseLookup(const QVector<int>& data){
    auto table = COLM_LOOKUP_TABLE();
    for(auto key : table.keys()){
        QVector<int> row(table[key]);
        int it = 0;
        bool different = false;
        for(auto cur_data : row){
            bool equal = cur_data == data.at(it);
            if(!equal){
                different = true;
                break;
            }
            it++;
        }
        if(!different)
            return key;
    }
    return "...";
}

}
