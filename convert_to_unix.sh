#!/bin/bash
cd tests
for file in *.txt; do
    tr -d '\r' < "$file" > "${file}.tmp"
    mv "${file}.tmp" "$file"
    echo "Convertido: $file"
done

cd ..
tr -d '\r' < "entrada" > "entrada.tmp"
mv "entrada.tmp" "entrada"
echo "Convertido: entrada"

echo "¡Todos los archivos convertidos a formato Unix!"
