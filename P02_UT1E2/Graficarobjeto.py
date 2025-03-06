import matplotlib.pyplot as plt
import numpy as np

# Datos de distancia filtrada
distancia_filtrada = [
    72.86, 73.40, 73.05, 73.03, 72.53, 72.89, 72.92, 73.19, 73.64, 73.98,
    73.99, 74.24, 74.18, 74.89, 74.66, 73.75, 73.56, 73.42, 73.57, 73.42,
    73.82, 74.11, 73.84, 74.13, 73.59, 73.20, 73.40, 72.05, 72.29, 73.21,
    69.91, 61.43, 54.32, 49.49, 44.87, 41.90, 40.18, 38.38, 37.54, 36.15,
    35.61, 34.71, 34.03, 32.77, 32.58, 32.69, 33.01, 33.51, 33.63, 32.97,
    33.23, 33.67, 33.26, 34.19, 43.89, 50.67, 56.00, 60.25, 63.94, 66.70,
    69.53, 71.15, 72.86, 73.89, 75.17, 74.88, 75.16, 75.62, 75.71, 76.04,
    76.28, 76.21, 77.16, 76.87, 76.90, 76.43, 76.82, 77.86
]

# Crear un eje de tiempo suponiendo intervalos de 1 segundo
tiempo = np.arange(len(distancia_filtrada))

# Graficar
plt.figure(figsize=(10, 5))
plt.plot(tiempo, distancia_filtrada, marker='o', linestyle='-', color='b', label='Distancia Filtrada (mm)')
plt.xlabel("Tiempo (s)")
plt.ylabel("Distancia Filtrada (mm)")
plt.title("Evolución de la Distancia Filtrada en el Tiempo")
plt.legend()
plt.grid()

# Mostrar gráfica
plt.show()
