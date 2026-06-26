#!/bin/bash
echo "Compilation d'Optifetch..."

# Compilation directe sans Makefile
gcc -Wall -Wextra -O3 -s -o optifetch main.c sysinfo.c logos.c config.c renderer.c

if [ $? -eq 0 ]; then
    echo "Build réussi ! Exécutable créé : ./optifetch"
else
    echo "Erreur lors de la compilation."
    exit 1
fi