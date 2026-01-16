#!/usr/bin/env python3
import os

def parse_pronto(pronto_hex_list):
    """
    Convierte una lista de hex PRONTO en:
    - frecuencia (Hz)
    - lista RAW de pulsos en microsegundos (alternado + / -)
    """

    if len(pronto_hex_list) < 4:
        raise ValueError("PRONTO demasiado corto")

    freq_raw = int(pronto_hex_list[1], 16)
    if freq_raw == 0:
        freq_hz = 38000  # fallback
    else:
        freq_hz = int(1_000_000 / (freq_raw * 0.241246))

    n_intro = int(pronto_hex_list[2], 16)
    n_repeat = int(pronto_hex_list[3], 16)

    pulses_hex = pronto_hex_list[4:4 + (n_intro + n_repeat) * 2]
    pulses_raw = [int(x, 16) for x in pulses_hex]

    pulses_us = [
        int(p * (1_000_000 / freq_hz))
        for p in pulses_raw
    ]

    raw_final = []
    sign = 1
    for p in pulses_us:
        raw_final.append(sign * p)
        sign *= -1

    return freq_hz, raw_final


def convert_file(filename):
    """
    Lee un archivo con formato ESPHome log y extrae el PRONTO en una sola línea.
    """
    with open(filename, "r", encoding="utf-8") as f:
        data = f.read()

    # sacar todo menos los hex
    tokens = data.replace("\n", " ").replace("\r", " ").split()
    pronto_hex = []

    for t in tokens:
        if all(c in "0123456789ABCDEFabcdef" for c in t):
            if len(t) == 4:
                pronto_hex.append(t)

    if len(pronto_hex) < 10:
        raise ValueError(f"No se pudo extraer PRONTO válido de {filename}")

    return parse_pronto(pronto_hex)


def print_yaml_block(name, freq, raw):
    """
    Imprime YAML listo para pegar en ESPHome.
    """
    print(f"  - platform: template")
    print(f"    name: \"{name}\"")
    print(f"    turn_on_action:")
    print(f"      - remote_transmitter.transmit_raw:")
    print(f"          carrier_frequency: {freq}")
    print(f"          code: [")
    
    # imprimir trozos ordenados
    line = "            "
    for i, val in enumerate(raw):
        line += f"{val}, "
        if len(line) > 110:
            print(line)
            line = "            "
    if line.strip():
        print(line)
    print("          ]\n")


def main():
    print("\n\n================= CONVERTIDOR PRONTO → ESPHOME =================\n")

    freq_on, raw_on = convert_file("on_pronto.txt")
    freq_off, raw_off = convert_file("off_pronto.txt")

    freq = (freq_on + freq_off) // 2  # promedio por seguridad

    print("remote_transmitter:")
    print("  pin: GPIO25")
    print("  carrier_duty_percent: 50%\n")
    print("switch:")

    print_yaml_block("AC ON", freq, raw_on)
    print_yaml_block("AC OFF", freq, raw_off)


if __name__ == "__main__":
    main()
