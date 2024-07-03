# jogg - Application runner

jogg is an application launcher/command runner for wlroots-based Wayland
compositors. The pun in the name is probably obvious.

## Motivation

Given that there already exist launchers that target the same audience, what
makes _this one_ special? Well, not much, other than NIH. I’ve been using
[Fuzzel](https://codeberg.org/dnkl/fuzzel) for a while and it’s great, but it
lacked mouse support, so I decided to employ my rusted GTK skills and make my
own.

## Preview

[screencast.webm](https://github.com/ernestask/jogg/assets/26867637/b63af672-5d8f-41ed-b4ef-86a389134bce)

## Building

https://mesonbuild.com/Quick-guide.html#compiling-a-meson-project

### Requirements

- GTK 4 >= 4.14.0
