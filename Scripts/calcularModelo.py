import numpy as np
from scipy.optimize import curve_fit

# Modelo correcto del LDR:
# R = k * LUX^(-n)
def ldr_model(lux, k, n):
    return k * lux**(-n)

# voltaje -> resistencia
def voltage_to_resistance(v, Rref):
    return Rref * (3.3/v - 1)

# Ejemplos de datos (reemplazar por los tuyos)
voltages = np.array([0.36,1.16,2.12])
lux = np.array([1.4,2.3,3.9])
Rref = 10000

Rldr = voltage_to_resistance(voltages, Rref)

params, _ = curve_fit(ldr_model, lux, Rldr, p0=[50000, 0.7])

k_est, n_est = params
print("k =", k_est)
print("n =", n_est)

