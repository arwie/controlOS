
RELEASE=bullseye



# as root

pacman -S --needed debootstrap debian-archive-keyring

cd /var/lib/machines
debootstrap $RELEASE ptxdist/


# within container as root

echo "deb http://debian.pengutronix.de/debian/ $RELEASE main contrib non-free" > /etc/apt/sources.list.d/pengutronix.list
apt-get update --allow-insecure-repositories
apt-get install pengutronix-archive-keyring

apt-get install -y bash-completion wget ccache git \
	build-essential pkg-config libncurses-dev gawk flex bison texinfo file \
	zip bc lzop python3 python3-setuptools python3-jinja2 device-tree-compiler \
	gperf python3-pip python3-virtualenv cmake ninja-build libffi-dev libssl-dev dfu-util

#install toolchain(s)
apt-get install oselas.toolchain-2018.12.0-arm-v7a-linux-gnueabihf-gcc-8.2.1-glibc-2.28-binutils-2.31.1-kernel-4.19-sanitized

apt-get clean


#install ptxdist(s)

# within container as user
ptxdist setup
	#-> set src directory: ${HOME}/src
	#-> enable ccache


# EPS-IDF (as user)
cd /opt
git clone --depth=1 --shallow-submodules --recursive -b v4.4 https://github.com/espressif/esp-idf.git
export IDF_TOOLS_PATH=/opt/esp-idf
cd esp-idf
./install.sh esp32
