#!/bin/bash

echo "==========================================="
echo "Iniciando configuración de puertos USB..."
echo "==========================================="

# --- CONFIGURACIÓN ---
# ¡Cambia estos valores por los de tu hardware!

U2D2_SERIAL="FT3WHS11"
# ---------------------

echo "[1/3] Generando archivo de reglas udev en /etc/udev/rules.d/..."

# Usamos EOF para escribir el archivo directamente con permisos sudo
cat << EOF | sudo tee /etc/udev/rules.d/99-mis-robots.rules > /dev/null
# 1. Placa RR-SSK (Interfaz principal - Servos Rozum)
SUBSYSTEM=="tty", ENV{ID_VENDOR_ID}=="0483", ENV{ID_MODEL_ID}=="5740", ENV{ID_USB_INTERFACE_NUM}=="00", SYMLINK+="rozum_api"

# 2. Placa RR-SSK (Interfaz secundaria - Logs)
SUBSYSTEM=="tty", ENV{ID_VENDOR_ID}=="0483", ENV{ID_MODEL_ID}=="5740", ENV{ID_USB_INTERFACE_NUM}=="02", SYMLINK+="rozum_log"

# 3. U2D2 (Servos Dynamixel) + Reducción de latencia a 1ms
SUBSYSTEM=="tty", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6014", ATTRS{serial}=="$U2D2_SERIAL", SYMLINK+="u2d2_dyn", ATTR{device/latency_timer}="1"
EOF

echo "[2/3] Recargando el administrador de dispositivos de Linux..."
sudo udevadm control --reload-rules
sudo udevadm trigger

echo "[3/3] ¡Configuración completada exitosamente!"
echo ""
echo "Tus puertos actuales disponibles son:"
ls -l /dev/rozum_api /dev/rozum_log /dev/u2d2* 2>/dev/null || echo "(No hay dispositivos conectados en este momento)"
echo "==========================================="
