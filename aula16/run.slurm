#!/bin/bash
#SBATCH --job-name=mpi_exercises
#SBATCH --output=output_%j.txt
#SBATCH --error=error_%j.txt
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=4
#SBATCH --time=00:10:00
#SBATCH --partition=espec
#SBATCH --mem=2G



# Garantir que os binários sejam executáveis
chmod +x ex1 ex2 ex3 ex4

# Executar os programas
echo "Executando Exercício 1"
mpirun -np 8 ./ex1

echo "Executando Exercício 2"
mpirun -np 8 ./ex2

echo "Executando Exercício 3"
mpirun -np 8 ./ex3

echo "Executando Exercício 4"
mpirun -np 8 ./ex4