<!--
Author: Michael Garcia, M&E Design
License: MIT
Date: 2026-04-17
Contact: michael@mandedesign.studio
Website: https://mandedesign.studio
-->

I started with the idea that my soon-to-be toddlers are always looking for my phone or the TV remote to play with. They have buttons that either highlight, click, or have different textures and feel to them which attracts their curiosity.

The original prototype - found in archive/ - was made for tinkerCAD circuit simulator. Once I got the schematic layout the way I wanted it and my simple sketch to work, it was time to break the code into classes and re-usable modules as I intend to expand beyond it's original scope.

### LED Driver Strategy

The LED subsystem is split into two layers:

- `Led` = semantic LED behavior
- `LedDriver595` = physical hardware output driver

This preserves a stable runtime API while allowing the hardware backend
to change later without rewriting application logic.

### headers.h

I use a single routing header into main.cpp to keep modularity simple and expandable. Adding new modules and features is trivial as you just add or drop new includes to its list.

### Modular code

I want this to be as hardware agnostic as I can make it. There are some hardware choices I made - such as using the Gannsle debounce network - that are practical solutions for behavioral issues that are common, but are not detrimental to the overall scope of the codebase.