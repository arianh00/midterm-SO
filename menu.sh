#!/bin/bash
leer_codigo ()
{
    echo 'Escriba el archivo que desea visualizar (El archivo tiene que estar contenido en este directorio)'
    echo
    read archivo
    if [[ -f $archivo ]]; then
        cat $archivo
        echo
    else
        echo 'Dicho archivo no existe o no se puede visualizar'
    fi
}

compilar_codigo ()
{
    echo 'Elija el archivo que desea compilar. Este archivo debe de tener una terminación .c'
    echo 'El archivo se compilara como: (simulador)'
    echo
    read archivo
    re="^[^[:space:]]+\.c$"
    if [[ -f $archivo && $archivo =~ $re ]]; then
        gcc $archivo -o simulador
    else
        echo 'Dicho archivo no existe o no se puede compilar'
    fi
}

ejecutar_script ()
{
    echo 'Elije el número de asistentes de la simulación (Tiene que haber al menos 1)'
    echo
    read asistentes
    if [[ $asistentes -gt 0 ]]; then
        ./simulador $asistentes
    else
        echo 'El numero de asistentes elejido no es correcto'
    fi
}

menu ()
{
    case $1 in
        1) leer_codigo
            echo
        ;;
        2) compilar_codigo
            echo
        ;;
        3) if [[ -f "simulador" && -x 'simulador' ]]; then
            ejecutar_script
            echo
        else
            printf "No se ha encontrado el archivo o bien este no se puede ejecutar\n"
            echo
        fi
        ;;
        4) echo "Hasta luego"
        ;;
        *) echo "La opción no es valida, pruebe otra vez"
            echo
        ;;
    esac        
}

print_menu ()
{
    echo 'Elija una opción:'
    echo '1)Leer código del programa'
    echo '2)Compilar el programa' 
    echo '3)Ejecutar el programa'
    echo '4)Salir'
    echo 
}

seleccion=0    
while [[ $seleccion -ne 4 ]]; do
    print_menu
    read seleccion
    echo
    menu $seleccion
done

