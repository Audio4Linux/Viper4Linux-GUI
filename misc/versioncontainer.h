#ifndef VERSIONCONTAINER_H
#define VERSIONCONTAINER_H

/* Temporary license banner
 * ThePBone
 * License: GPLv3
 */

#include <QObject>

class VersionContainer
{
protected:
    unsigned int version[3];
public:
    VersionContainer()
    {
    }

    VersionContainer( const QString val )
    {
        QStringList list = val.split('.');
        if(list.count() != 3)
            for(int i = 0; i < 3; i++)
                this->version[i] = 0;
        else{
            this->version[0] = list.at(0).toUInt();
            this->version[1] = list.at(1).toUInt();
            this->version[2] = list.at(2).toUInt();
        }
    }

    VersionContainer( const VersionContainer& val )
    {
        *this   = val;
    }

    operator QString() const
    {
        return QString("%1.%2.%3").arg(version[0]).arg(version[1]).arg(version[2]);
    }

    VersionContainer& operator =( const VersionContainer& input )
    {
        version[0]   = input.version[0];
        version[1]   = input.version[1];
        version[2]   = input.version[2];
        return *this;
    }

    VersionContainer& operator =( const QString input )
    {
        QStringList list = input.split('.');
        if(list.count() != 3)
            for(int i = 0; i < 3; i++)
                this->version[i] = 0;
        else{
            version[0]   = list.at(0).toUInt();
            version[1]   = list.at(1).toUInt();
            version[2]   = list.at(2).toUInt();
        }
        return *this;
    }

    bool operator ==( const VersionContainer& val ) const
    {
        if (version[0] == val.version[0] &&
                version[1] == val.version[1] &&
                version[2] == val.version[2]) return 1;
        return 0;
    }

    bool operator !=( const VersionContainer& val ) const
    {
        if (version[0] == val.version[0] &&
                version[1] == val.version[1] &&
                version[2] == val.version[2]) return 0;
        return 1;
    }

    bool operator >=( const VersionContainer& val ) const
    {
        if (version[0] >= val.version[0]) return 1;
        if (version[1] >= val.version[1]) return 1;
        if (version[2] >= val.version[2]) return 1;
        return 0;
    }

    bool operator <=( const VersionContainer& val ) const
    {
        if (version[0] <= val.version[0]) return 1;
        if (version[1] <= val.version[1]) return 1;
        if (version[2] <= val.version[2]) return 1;
        return 0;
    }

    bool operator >( const VersionContainer& val ) const
    {
        if (version[0] > val.version[0]) return 1;
        if (version[1] > val.version[1]) return 1;
        if (version[2] > val.version[2]) return 1;
        return 0;
    }

    bool operator <( const VersionContainer& val ) const
    {
        if (version[0] < val.version[0]) return 1;
        if (version[1] < val.version[1]) return 1;
        if (version[2] < val.version[2]) return 1;
        return 0;
    }
};
#endif // VERSIONCONTAINER_H
