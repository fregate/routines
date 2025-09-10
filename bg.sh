#!/bin/bash

shopt -s nullglob

abs_path=$(realpath .)
wallpapers=(
  ./*.{jpg,jpeg,png}
)

wallpapers_size=${#wallpapers[*]}
idx=$(($RANDOM % $wallpapers_size))
gsettings set org.gnome.desktop.background picture-uri "$abs_path/${wallpapers[$idx]}"

# put this script into dir with images
# crontab: @daily bash <path>/bg.sh
