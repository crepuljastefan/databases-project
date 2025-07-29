#!/bin/bash

# Pronađi sve .c fajlove rekurzivno i sačuvaj ih u promenljivu
c_files=$(find . -name "*.c")

# Kompajliraj sve .c fajlove u jedan izvršni fajl "program"
gcc $c_files -o program

# Proveri da li je kompajlacija uspela
if [ $? -eq 0 ]; then
    echo "Kompajlacija uspela, pokrećem program..."
    ./program
else
    echo "Kompajlacija nije uspela."
fi
