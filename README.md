![docker-automated](https://img.shields.io/docker/automated/pavelsr/mysqludf_sys)
![docker-build](https://img.shields.io/docker/build/pavelsr/mysqludf_sys)

<!-- TOC depthFrom:1 depthTo:6 withLinks:1 updateOnSave:1 orderedList:0 -->

- [DESCRIPTION](#description)
- [FUNCTIONS](#functions)
	- [sys_eval](#syseval)
	- [sys_exec](#sysexec)
	- [sys_get](#sysget)
	- [sys_set](#sysset)
- [INSTALLATION](#installation)
- [DOCKER IMAGE](#docker-image)
	- [Build](#build)
	- [Run](#run)
- [EXAMPLES](#examples)
- [SECUIRITY PRECUTIONS](#secuirity-precutions)

<!-- /TOC -->

# DESCRIPTION

Functions that allow to interact with the operating system

# FUNCTIONS

## sys_eval

executes an arbitrary command, and returns it's output

syntax: `sys_eval(arg1)`

## sys_exec

executes an arbitrary command, and returns it's exit code

syntax: `sys_exec(arg1)`

## sys_get

gets the value of an environment variable

syntax: `sys_get(arg1)`

## sys_set

create an environment variable, or update the value of an existing environment variable

syntax: `sys_set(arg1, arg2)`

arg1 - name of an environment value

arg2 - expression that contains the value that is to be assigned to the environment variabl

# INSTALLATION

On runnning MySQL server just run `install.sh` script (don't forget to change here mysql root login and password)

# DOCKER IMAGE

## Build

```
docker build -t mysqludf/sys .
```

## Run

```
docker run -p 3306:3306 -e MYSQL_ROOT_PASSWORD=my-secret-pw -d mysqludf/sys
```

# EXAMPLES

```
SELECT sys_eval('id')
SELECT sys_eval('echo $HOME')
SELECT sys_eval('~/hello_world.sh')
SELECT sys_eval('bash /root/lib_mysqludf_sys/hello_world.sh')
```

# SECUIRITY PRECUTIONS

UDFs are available to all database users - you cannot grant EXECUTE privileges for them.
As the commandstring passed to `sys_exec` or `sys_eval` can do pretty much everything, 
exposing the function poses a very real security hazard.

Even for a benign user, it is possible to accidentally do a lot of damage with it.
The call will be executed with the privileges of the os user that runs MySQL, 
so it is entirely feasible to delete MySQL's data directory, or worse.
