#!/bin/bash -e

function printHelp {
    echo "$0 [-hcsr] 
    Help:
    Crea otro directorio con los archivos necesarios para que el cliente se ejecute.
    Util cuando queres ejecutar varios clientes sin que todos escriban en el mismo log.
    " >&2
}

showHelpFlag=

while getopts 'h' flag; do
    case "${flag}" in
        h)    showHelpFlag=1 ;;
        *)    echo "Unexpected option ${flag}" ;;
    esac
done

if [ ! -z "$showHelpFlag" ]; then
    printHelp
    exit 0
fi



if [ ! -z $1 ]; then
    client_quantity=$1
else
    client_quantity=1
fi

# Validacion
is_a_number_regex='^[0-9]$'
if ! [[ $client_quantity =~ $is_a_number_regex ]] ; then
   echo "Error! La cantidad no es un numero." >&2
   exit 1
fi

echo "Creando $client_quantity directorio/s..."

i=0
while [ $i -lt $client_quantity ]; do
    new_client_dir="copy"$i"OfClient"
    # Si existia, limpiar lo de adentro.
    rm -fr "$new_client_dir/"
    # Crear los directorios.
    echo "Creando: $new_client_dir..."
    mkdir -p $new_client_dir
    # Copiar archivos.
    cp client/main "$new_client_dir/"
    cp -R client/config "$new_client_dir/"
    i=$[$i+1]
done

echo "Directorio/s creados."

exit 0
