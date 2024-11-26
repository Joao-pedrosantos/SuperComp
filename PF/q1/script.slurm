#!/bin/bash
#SBATCH --job-name=aula7                         # Nome do job
#SBATCH --output=limitado_%j.txt                 # Gera um novo arquivo de saída a cada execução
#SBATCH --ntasks=5                               # Número de tarefas
#SBATCH --cpus-per-task=4                        # Número de threads por tarefa 
#SBATCH --mem=16348MB                            # Memória total alocada por nó
#SBATCH --time=01:00:00                          # Tempo máximo de execução
#SBATCH --partition=espec                        # Fila do cluster a ser utilizada

# Exporta a variável de ambiente para configurar o número de threads OpenMP
export OMP_NUM_THREADS=15

./programa