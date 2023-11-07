#!/bin/bash
menu ()
{
    while [[ true ]]; do
        printf "Elija una opción:\n1)Leer código del programa\n2)Compilar programa\n3)Ejecutar programa\n4)Salir\n\n"
        read seleccion
        echo
        case $seleccion in
            1) cat simulador.c
                echo; echo
            ;;
            2) gcc simulador.c -o simulador
            ;;
            3) if [[ -f "simulador" && -x 'simulador' ]]; then
                echo "TODO"; echo
            else
                printf "No se ha encontrado el archivo o bien este no se puede ejecutar\n"
                echo
            fi
            ;;
            4) echo "Hasta luego"
                break 
            ;;
            *) echo "La opción no es valida, pruebe otra vez"
            ;;
        esac
    done
        
}
    
menu
