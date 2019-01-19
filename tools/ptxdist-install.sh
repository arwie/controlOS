
# as root

pacman -S --needed debootstrap debian-archive-keyring

cd /var/lib/machines
debootstrap stretch ptxdist/


# within container as root
apt-get install -y \
	build-essential libncurses-dev gawk flex bison texinfo file gettext python-dev python3-dev pkg-config bc libelf-dev ccache zip bzip2 xz-utils \
	python3-systemd python3-tornado systemd-journal-remote \
	sudo git man bash-completion less vim

apt-get clean

ln -fs /bin/journalctl /usr/bin/journalctl

chmod 777 /opt


install ptxdist(s)
install toolchain(s)


# within container as user
ccache --max-size=1G
ptxdist setup
	#-> set src directory
	#-> enable ccache
