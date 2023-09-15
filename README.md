<p align="center">
  <img src="docs/shizle.png" />
</p>

## About

Schizle is an *In-progress* Lightweight scripting language with builtin support for compilation into executables written in pure ANSI C with the objective of being simple ,easy to learn, and lightweight.
Current release: 0.01
<!--
## Getting started

[Getting started](docs/NOTDONE.md)
-->
## Helloworld.ski
Create a file named first_program.ski and write the below into it
```
_ Hello world program

get "schizle standard library" as std
end

!["Hello world"]
call @std::cli_out
```
Then run the following command with Schizle.exe in the same directory, or use the relative path.
```
.\schizle.exe run first_program.ski hello_world
```
The output should be
```
hello_world out:

Hello world

successfully freed memory
interpreter exited hello_world with code: 0
```
> In the futue the output will be cleaned up
> As of right now errors and warning are not very informative, the program will only throw errors if it is called wrong, however the interpreter will only print ERROR_HERE or WARNING_HERE when it encounters an error or warning, respectively.
<!--
## Documentation

[View docs online](docs/NOTDONE.md)

[View docs on github](docs/NOTDONE.md)

[Download Documentation](docs/NOTDONE.md)
-->
