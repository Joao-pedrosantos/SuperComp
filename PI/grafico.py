import matplotlib.pyplot as plt

testes = [20, 25, 30, 35, 40, 45]
tempo_UN = [188, 1747, 9346, 57635, 630805, 7152704]
tempo_OPT = [1, 1, 2, 2, 2, 2]


plt.plot(testes, tempo_UN, label='UN')
plt.plot(testes, tempo_OPT, label='OPT')

plt.xlabel('Número de testes')
plt.ylabel('Tempo (us)')

plt.title('Tempo de execução UN x OPT')

plt.legend()

plt.show()