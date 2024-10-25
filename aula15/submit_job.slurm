#!/bin/bash
#SBATCH --job-name=mpic
#SBATCH --output=SAIDA.txt
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=4
#SBATCH --time=00:05:00
#SBATCH --mem=2048MB

mpirun ./exercise1
echo '================================='

mpirun ./exercise2
echo '================================='
mpirun ./exercise3
echo '================================='
mpirun ./exercise4
echo '================================='
mpirun ./exercise5
echo '================================='
mpirun ./exercise6

