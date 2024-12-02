#!/bin/bash
#SBATCH --job-name=projekt
#SBATCH --output=projekt_%j.out
#SBATCH --error=projekt_%j.err
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=10
#SBATCH --time=02:00:00
#SBATCH --mem=8G
#SBATCH --partition=espec

# Processar todos os cromossomos de 1 a 22
for CHR_NUM in {1..22}
do
    echo "Processando cromossomo $CHR_NUM..."


    # Executar o programa paralelo
    ./aug RNA_chr${CHR_NUM}.subst.fa_processed.fa.txt > rna${CHR_NUM}.txt

    echo "Cromossomo $CHR_NUM processado com sucesso!"
done
