#!/bin/bash

# Check the distribution type and install dependencies accordingly

if [ -f /etc/os-release ]; then
    # Detect distribution using os-release
    . /etc/os-release
    OS=$ID
    VERSION=$VERSION_ID
elif type lsb_release >/dev/null 2>&1; then
    # Detect distribution using lsb_release
    OS=$(lsb_release -si)
    VERSION=$(lsb_release -sr)
else
    echo "Install deps manually: gtk3-devel, vte3-devel"
    exit 1
fi

case $OS in
    "debian" | "ubuntu")
        sudo apt update
        sudo apt install -y libgtk-3-dev libvte-2.91-dev
        ;;
    "fedora")
        sudo dnf install -y gtk3-devel vte291-devel.x86_64
        ;;
    "centos" | "rhel" | "rocky")
        sudo yum install -y gtk3-devel vte3-devel
        ;;
    "arch")
        sudo pacman -Syu --noconfirm gtk3 vte3
        ;;
    "void")
        sudo xbps-install -y gtk+3-devel vte3-devel
        ;;
    *)
        echo "Install deps manually: gtk3-devel, vte3-devel"
        exit 1
        ;;
esac

echo "installing dependencies for $OS $VERSION..."

