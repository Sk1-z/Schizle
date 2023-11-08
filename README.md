<p align="center">
  <img src="docs/shizle.png" />
</p>

## About

Schizle is an *In-progress* Lightweight scripting language with builtin support for compilation into executables written in pure ANSI C with the objective of being simple ,easy to learn, and lightweight.
<br>
**Current release: 0.4.1**
<!--
## Getting started

[Getting started](docs/NOTDONE.md)
-->
## Helloworld.ski
Create a file named first_program.ski and write the below into it
```
_ Hello world program

get "std lib" as std
end

!["Hello world"]
call @std::sout
```
or if you want something simpler
```
get std
!["Hello world"] >> @std::sout
```
Then run the following command with the Schizle binary for your platform in the same directory, or on the path.
```
Schizle first_program.ski
```
The output should be
```
Hello world
```
<!--
## Documentation

[View docs online](docs/NOTDONE.md)

[View docs on github](docs/NOTDONE.md)

[Download Documentation](docs/NOTDONE.md)
-->
