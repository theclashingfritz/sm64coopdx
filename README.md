![sm64coopdx Logo](textures/segment2/custom_coopdx_logo.rgba32.png)

sm64coopdx is an online multiplayer project for the Super Mario 64 PC port that synchronizes all entities and every level for multiple players. The project was started by the Coop Deluxe Team. The purpose is to actively maintain and improve, but also continue sm64ex-coop, created by djoslin0. More features, customization, and power to the Lua API allow modders and players to enjoy Super Mario 64 more than ever!

Feel free to report bugs or contribute to the project. 

## Initial Goal (Accomplished)
Create a mod for the PC port where multiple people can play together online.

Unlike previous multiplayer projects, this one synchronizes enemies and events. This allows players to interact with the same world at the same time.

Interestingly enough though, the goal of the project has slowly evolved over time from simply just making a Super Mario 64 multiplayer mod to constantly maintaining and improving the project (notably the Lua API.)

## Lua
sm64coopdx is moddable via Lua, similar to Roblox and Garry's Mod's Lua APIs. To get started, click [here](docs/lua/lua.md) to see the Lua documentation.

## Wiki
The wiki is made using GitHub's wiki feature, you can go to the wiki tab or click [here](https://github.com/coop-deluxe/sm64coopdx/wiki).

## About this Fork
This fork is a modified version of the [Sm64coopdx TheClashingFritz nx-support](https://github.com/theclashingfritz/sm64coopdx/tree/nx-support) with the support of the newest version 1.4.
Thanks to her work it was relatively easy to merge the 1.4 version

## Build instructions
To build you need to use this command, with a proper DevKitPro installation and using the msys2_shell.cmd
```console
foo@bar MSYS /c/path
$ make TARGET_NX=1 CONTROLLER_API=SWITCH 
Compiling for Nintendo Switch
==== Build Options ====
Version:        us
Microcode:      f3dex2e
Build Matching: no
=======================

```
