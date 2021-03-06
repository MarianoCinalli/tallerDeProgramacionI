#!/bin/bash

function compile {
    echo '=================================================================' >&2
    echo "Compiling $1" >&2
    echo '=================================================================' >&2
    cd "$1"
    make
    if [ ! -z "$2" ]; then
        ./main &
    fi
    cd -
}

function cleanServerAndClient {
    echo '=================================================================' >&2
    echo "Cleaning $1" >&2
    echo '=================================================================' >&2
    cd "$1"
    make clean
    cd -
}

function printHelp {
    echo "$0 [-hcsr] 
    Help:
    -c Compila solo el cliente.
    -s Compila solo el server.
    -e Limpia el proyecto.
    -r Corre las applicaciones luego de compilar. Corren en background.
    Si no se especifica, server o cliente, compila ambos.
    " >&2
}

clear

compileServerFlag=1
compileClientFlag=1
runAfterBuildFlag=
showHelpFlag=
cleanFlag=

while getopts 'hcsre' flag; do
    case "${flag}" in
        h)    showHelpFlag=1 ;;
        s)    compileServerFlag=1
              compileClientFlag='' ;;
        c)    compileClientFlag=1
              compileServerFlag='' ;;
        r)    runAfterBuildFlag=1 ;;
        e)    cleanFlag=1 ;;
        *)    echo "Unexpected option ${flag}\n" ;;
    esac
done

if [ ! -z "$showHelpFlag" ]; then
    printHelp
    exit 0
fi

if [ ! -z "$cleanFlag" ]; then
    cleanServerAndClient "server"
    cleanServerAndClient "client"
fi

if [ ! -z "$compileServerFlag" ]; then
    compile "server" "$runAfterBuildFlag"
fi

if [ ! -z "$compileClientFlag" ]; then
    compile "client" "$runAfterBuildFlag"
fi

exit 0
