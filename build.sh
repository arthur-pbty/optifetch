#!/bin/bash
echo "Compilation d'Optifetch..."

# On nettoie l'ancienne compilation puis on lance make
make clean
make

if [ $? -eq 0 ]; then
    echo "Build réussi ! Exécutable créé : ./optifetch"
else
    echo "Erreur lors de la compilation."
    exit 1
fi