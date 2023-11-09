<p align="center">
  <img src="docs/shizle.png" />
</p>

## About

Schizle is a Lightweight scripting language written in pure ANSI C with the objective of being simple ,easy to learn, and lightweight. Get start with Schizle Script [here](docs/NOTDONE.md).
<br>
**Current release: 0.5.0**
## Quick start
The quickest way to get started to with Schizle Script is to download the latest binary for your respective platform from the [releases page](https://github.com/Sk1-z/Schizle/releases).
## Building from source
To build from source you must have premake5. By default, the configuration is for make and windows with compilation by clang. If this does not work for you, you can generate new files using
```
premake5 --cc=<cc> --os<platform> <build tool>
```
Where cc is your C compiler, either gcc, clang, or mingw, and platform is the platform you are using. To generate makefiles you should use gmake as your build tool arg. To see other possible options or configurations you can use 
```
premake5 --help
```
or visit the [docs](https://premake.github.io/docs/Using-Premake). If you used gmake, you can build it using
```
make config=release_<platform>
```
where platform is either windows, linux, or mac.
