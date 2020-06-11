#!/bin/bash
#DEB build script for Travis-CI
#V4L_Frontend binary needs to be in working directory

debname="viper4linux-gui_"$base_version"-build"$TRAVIS_BUILD_NUMBER
echo $debname
mkdir $debname
mkdir $debname"/DEBIAN"
mkdir -p $debname"/usr/bin"
mkdir -p $debname"/usr/share/applications"
mkdir -p $debname"/usr/share/pixmaps"
cp "V4L_Frontend" $debname"/usr/bin/viper-gui"

cp "viper.png" $debname"/usr/share/pixmaps/viper-gui.png"
cp "LICENSE" $debname"/DEBIAN"

cat <<EOT >> $debname"/usr/share/applications/viper-gui.desktop"
[Desktop Entry]
Name=Viper4Linux
GenericName=Equalizer
Comment=Official UI for Viper4Linux
Keywords=equalizer
Categories=AudioVideo;Audio;
Exec=viper-gui
Icon=/usr/share/pixmaps/viper-gui.png
StartupNotify=false
Terminal=false
Type=Application
EOT

cat <<EOT >> $debname"/DEBIAN/control"
Package: viper4linux-gui
Version: $base_version-$TRAVIS_BUILD_NUMBER
Section: sound
Priority: optional
Architecture: amd64
Depends: qtbase5-dev (>= 5.9.5), libqt5core5a (>= 5.9.5), libqt5widgets5 (>= 5.9.5), libqt5gui5 (>= 5.9.5), libqt5core5a (>= 5.9.5), libgl1-mesa-dev
Maintainer: ThePBone <tim.schneeberger@gmail.com>
Description: Official UI for Viper4Linux
Homepage: https://github.com/Audio4Linux/Viper4Linux-GUI
EOT

dpkg-deb --build $debname
