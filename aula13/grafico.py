import numpy as np

original = [4.31913791783154,  4.31905581708997, 4.31907455157489]
for_paralelo = [1.07860666699708, 1.07864463329315, 1.07878881227225]
OMP = [1.61961036361754, 1.61911043338478, 1.61846658214927]


omp_atomic = [1.06763365492225, 1.08088230993599, 1.05567545536906]
omp_critical = [1.08101776428521, 1.08094983454794, 1.05164075270295]


print(f'Original: {np.mean(original):.3f}')
print(f'For paralelo: {np.mean(for_paralelo):.3f}')
print(f'OMP: {np.mean(OMP):.3f}')
print(f'OMP atomic: {np.mean(omp_atomic):.3f}')
print(f'OMP critical: {np.mean(omp_critical):.3f}')