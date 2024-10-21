#!/bin/bash
#SBATCH --job-name=omp_schedulers
#SBATCH --output=SAIDA.txt
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=4
#SBATCH --time=00:05:00
#SBATCH --mem=2048MB

./ex