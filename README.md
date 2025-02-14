# INTRODUCTION
This fork aims at preserving the original GTK GUI and keep this nice plugin suite available



# CONTENTS

The following plugins are included:

Delay Plugins
* Delay Munge - Mono         => Two channel delay with non-linear feedback (munged) plus delay calculator. 
* Delay Munge - Stereo       => Two channel delay with non-linear feedback (munged) plus delay calculator. 

Distortion Plugins
* Tube - Mono                => Valve warmth/distortion simulation
* Tube - Stereo              => Valve warmth/distortion simulation

Dynamics Plugins
* Compressor - Mono          => Peak/RMS softclipping compressor
* Compressor - Stereo        => Peak/RMS softclipping compressor

Filter Plugins
* Low Pass Mono              => gentle low pass filter
* Low Pass Stereo            => gentle low pass filter
* High Pass Mono             => gentle high pass filter
* High Pass Stereo           => gentle high pass filter

Phaser Plugins
* Stereo Phaser - Mono In    => slow stereo phaser
* Stereo Phaser - Stereo In  => slow stereo phaser
* Stereo Phaser - Sum L+R In => slow stereo phaser

Reverb Plugins
* ER Reverb - Mono In        => early reflection based reverb
* ER Reverb - Sum L+R In     => early reflection based reverb

Utility Plugins
* Input Module               => alter gain, balance, width, phase on a stereo signal
* Meters                     => peak, VU, phase and spectrograph meters
* Test Tones                 => Generate test tones at standard and muscial frequencies.



# INSTALLATION

No binaries are included, you'll need to compile the plugins yourself. 



# BUILDING

To build the plugins the you'll need the following items installed:
* a working build environment with core includes
* the lv2 header file (lv2.h)
* the gtk2 developer libraries & includes
* the cairo developer libraries & includes

Go to the directory that was created when you unpacked this archive and then run:

make
make install-user (to install in ~/.lv2)
or
make install-sys (as root to install in /usr/local/lib/lv2)

Change the Makefile if you'd like to have them in an alternate system location.



# BUILDING (Debian and derivatives)

Packages for Debian and derivatives (Ubuntu etc) can be built from this archive. You'll need a Debian build environment.
To install the build environment and dependancies run the following:

sudo apt-get install build-essential debhelper fakeroot lv2core libgtk2.0-dev

Go to the directory that was created when you unpacked this archive and then run:

dpkg-buildpackage -rfakeroot -tc

Once done you'll find a 'deb' package for your system in the directory above. Use your favourite package manager to install.
The plugins will install into "/usr" by default. 



# PACKAGES (Ubuntu and derivatives)

Pre built binary packagaes for ubuntu are available from https://launchpad.net/~invada/+archive/ppa



# PACKAGING FOR DISTROS

An alternative download is available at launchpad which has no 'debian' packaging information in it. Look for the 'nopkg' tarball.
When building in fakeroot type environments, the installation directory can be prefixed with a destination directory like so:
make install-sys DESTDIR='/a/fake/root/location'
which will install into /a/fake/root/location/usr/local/lib/lv2



Enjoy!
fraser@arkhostings.com
https://launchpad.net/invada-studio
