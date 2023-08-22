<p align="center">
  <img src="docs/shizle.png" />
</p>

## About

Schizle is an *In-progress* Lightweight scripting language with builtin support for compilation into executables written in 99% C with the objective of being simple and easy to learn. Schizle is currently very early in development and schizle is only avaible on windows and the interpreter can only be used within the same directory as the .ski file. In addition there is a lot of features lacking.
<!--
## Getting started

[Getting started](docs/NOTDONE.md)
-->
## Helloworld.ski
Create a file named firstprogram.ski and write the below into it
```
_ Hello world program

params _name
call cmdl_out
params " says hello world"
call cmdl_out
```
Then run the following command with Schizle.exe in the same directory, or use the relative path.
```
.\schizle.exe run firstprogram.ski hello_world
```
The output should be
```
hello_world out:

hello_world says hello world

successfully freed memory
interpreter exited hello_world with code: 0
```
> In the futue the output will be much cleaner
<!--
## Documentation

[View docs online](docs/NOTDONE.md)

[View docs on github](docs/NOTDONE.md)

[Download Documentation](docs/NOTDONE.md)
-->
