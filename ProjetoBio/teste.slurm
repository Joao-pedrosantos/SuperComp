#!/bin/bash
#SBATCH --job-name=teste
#SBATCH --output=projekt_%j.out
#SBATCH --error=projekt_%j.err
#SBATCH --ntasks=2
#SBATCH --cpus-per-task=2
#SBATCH --time=02:00:00
#SBATCH --mem=8G
#SBATCH --partition=espec


./process_fasta_parallel chr1.subst.fa > counts_chr1.txt

