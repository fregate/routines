#!/bin/bash

shopt -s nullglob

abs_path="$(dirname "$0")"
wallpapers=(
    $abs_path/*.{jpg,jpeg,png}
)

wallpapers_size=${#wallpapers[*]}
idx=$(($RANDOM % $wallpapers_size))
gsettings set org.gnome.desktop.background picture-uri "${wallpapers[$idx]}"

# put this script into dir with images
# crontab: @daily bash <path>/bg.sh
