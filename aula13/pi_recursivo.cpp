#include <omp.h>
#include <iostream>
#include <iomanip>

static long num_steps = 1024l*1024*1024*2;
#define MIN_BLK  1024*1024*1024

double sum = 0;

// Função original
void pi_r(long Nstart, long Nfinish, double step) {
    long i, iblk;
    if (Nfinish - Nstart < MIN_BLK) {
        for (i = Nstart; i < Nfinish; i++) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x); 
        }
    } else {
        iblk = Nfinish - Nstart;
        pi_r(Nstart, Nfinish - iblk / 2, step);
        pi_r(Nfinish - iblk / 2, Nfinish, step);
    }
}

// Função paralelizada com parallel for
void pi_rFP(long Nstart, long Nfinish, double step) {
    long i, iblk;
    if (Nfinish - Nstart < MIN_BLK) {
        double local_sum = 0;  // soma parcial local
        #pragma omp parallel for reduction(+:local_sum)
        for (i = Nstart; i < Nfinish; i++) {
            double x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }
        #pragma omp critical
        {
            sum += local_sum;  // atualização global
        }
    } else {
        iblk = Nfinish - Nstart;
        pi_rFP(Nstart, Nfinish - iblk / 2, step);
        pi_rFP(Nfinish - iblk / 2, Nfinish, step);
    }
}

// Função paralelizada com tasks usando omp_critical
void pi_rOMPTASK_critical(long Nstart, long Nfinish, double step) {
    long iblk = Nfinish - Nstart;
    if (iblk < MIN_BLK) {
        double local_sum = 0;
        for (long i = Nstart; i < Nfinish; i++) {
            double x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }
        #pragma omp critical
        {
            sum += local_sum;  // Protegendo a variável sum com omp_critical
        }
    } else {
        #pragma omp task
        pi_rOMPTASK_critical(Nstart, Nfinish - iblk / 2, step);
        #pragma omp task
        pi_rOMPTASK_critical(Nfinish - iblk / 2, Nfinish, step);
        #pragma omp taskwait
    }
}

// Função paralelizada com tasks usando omp_atomic
void pi_rOMPTASK_atomic(long Nstart, long Nfinish, double step) {
    long iblk = Nfinish - Nstart;
    if (iblk < MIN_BLK) {
        double local_sum = 0;
        for (long i = Nstart; i < Nfinish; i++) {
            double x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }
        #pragma omp atomic
        sum += local_sum;  // Usando atomic para somar local_sum diretamente a sum
    } else {
        #pragma omp task
        pi_rOMPTASK_atomic(Nstart, Nfinish - iblk / 2, step);
        #pragma omp task
        pi_rOMPTASK_atomic(Nfinish - iblk / 2, Nfinish, step);
        #pragma omp taskwait
    }
}

int main () {
    long i;
    double step, pi;
    double init_time, final_time;
    step = 1.0 / (double) num_steps;

    // Execução da função original
    sum = 0;  // Reiniciar a variável sum
    init_time = omp_get_wtime();
    pi_r(0, num_steps, step);
    pi = step * sum;
    final_time = omp_get_wtime() - init_time;
    std::cout << "Original: for " << num_steps << " steps pi = " << std::setprecision(15) << pi << " in " << final_time << " secs\n";

    // Execução da função paralelizada com parallel for
    sum = 0;  // Reiniciar a variável sum
    init_time = omp_get_wtime();
    pi_rFP(0, num_steps, step);
    pi = step * sum;
    final_time = omp_get_wtime() - init_time;
    std::cout << "Parallel For: for " << num_steps << " steps pi = " << std::setprecision(15) << pi << " in " << final_time << " secs\n";

    // Execução da função paralelizada com tasks usando omp_critical
    sum = 0;  // Reiniciar a variável sum
    init_time = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        pi_rOMPTASK_critical(0, num_steps, step);
    }
    pi = step * sum;
    final_time = omp_get_wtime() - init_time;
    std::cout << "OMP Task (Critical): for " << num_steps << " steps pi = " << std::setprecision(15) << pi << " in " << final_time << " secs\n";

    // Execução da função paralelizada com tasks usando omp_atomic
    sum = 0;  // Reiniciar a variável sum
    init_time = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        pi_rOMPTASK_atomic(0, num_steps, step);
    }
    pi = step * sum;
    final_time = omp_get_wtime() - init_time;
    std::cout << "OMP Task (Atomic): for " << num_steps << " steps pi = " << std::setprecision(15) << pi << " in " << final_time << " secs\n";

    return 0;
}
