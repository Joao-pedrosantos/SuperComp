#!/bin/bash

# Loop para os cromossomos de 1 a 22
for i in {1..22}
do
    echo "Processando cromossomo $i..."

    # Baixar o arquivo correspondente
    wget ftp://hgdownload.cse.ucsc.edu/goldenPath/hg19/snp147Mask/chr${i}.subst.fa.gz

    # Descompactar o arquivo baixado
    gunzip chr${i}.subst.fa.gz

    echo "Cromossomo $i processado com sucesso!"
done
