#!/bin/bash -e

function printHelp {
    echo "$0 [-hcsr] 
    Help:
    Crea otro directorio con los archivos necesarios para que el cliente se ejecute.
    Util cuando queres ejecutar varios clientes sin que todos escriban en el mismo log.
    " >&2
}

function createCopies {
    # Validacion
    is_a_number_regex='^[0-9]$'
    if ! [[ $1 =~ $is_a_number_regex ]] ; then
       echo "Error! La cantidad no es un numero." >&2
       exit 1
    fi

    echo "Creando $1 directorio/s..."

    i=0
    while [ $i -lt $1 ]; do
        new_client_dir="copy"$i"OfClient"
        # Si existia, limpiar lo de adentro.
        rm -fr "$new_client_dir/"
        # Crear los directorios.
        echo "Creando: $new_client_dir..."
        mkdir -p $new_client_dir
        # Copiar archivos.
        cp client/main "$new_client_dir/"
        cp -R client/config "$new_client_dir/"
        cp -R client/images "$new_client_dir/"
        cp -R client/lazy.ttf "$new_client_dir/"
        i=$[$i+1]
    done

    echo "Directorio/s creados."
}

function deleteCopies {
    echo "Borrando copias..."
    rm -fr copy*OfClient
    echo "Copias borradas."
}

showHelpFlag=
deleteFlag=

while getopts 'hd' flag; do
    case "${flag}" in
        h)    showHelpFlag=1 ;;
        d)    deleteFlag=1 ;;
        *)    echo "Unexpected option ${flag}" ;;
    esac
done

if [ ! -z "$showHelpFlag" ]; then
    printHelp
    exit 0
fi

if [ ! -z "$deleteFlag" ]; then
    deleteCopies
else
    client_quantity=1
    if [ ! -z $1 ]; then
        client_quantity=$1
    fi
    createCopies $client_quantity
fi

exit 0
