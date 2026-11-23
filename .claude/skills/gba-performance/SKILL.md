---
name: gba-performance
description: Rendimiento en GBA con Butano - presupuesto del ARM7TDMI por frame, memorias (IWRAM/EWRAM/ROM) y sus esperas, ARM frente a Thumb, BN_CODE_IWRAM y *.bn_iwram.cpp, profiler de Butano y uso de CPU. Úsala sólo con una medición en la mano o al diseñar algo caro.
---

# Rendimiento GBA

## Cifras
- ARM7TDMI a 16,78 MHz: **280 896 ciclos por frame** (60 Hz; 228 líneas × 1232 ciclos; 160 visibles + 68 de VBlank).
- Memorias: **IWRAM 32 KB** (bus 32 bits, sin esperas: código ARM caliente y datos calientes); **EWRAM 256 KB**
  (16 bits, 2 esperas: datos grandes); **ROM** (16 bits, esperas + prefetch: código Thumb normal y datos const);
  VRAM/OAM/paletas a 16 bits. Butano configura las esperas de ROM.
- Código: Thumb desde ROM por defecto (compacto). Bucles calientes: ARM en IWRAM, `BN_CODE_IWRAM` en la función o el
  fichero entero como `*.bn_iwram.cpp` (se compila `-marm -mlong-calls`, sin LTO). La IWRAM es pequeña: sólo lo medido.
- Sin FPU ni división hardware: `bn::fixed` (punto fijo), divisiones por potencia de 2 o tablas; `bn::sin/cos`
  por tabla. Nada de `float`/`double` en el juego.

## Medir primero
- `bn::core::last_cpu_usage()` (fracción de frame de la última actualización): muéstralo en una build de depuración o
  cópialo a una global `test_cpu` que lea `gbarun.py --check`.
- Profiler: `BN_CFG_PROFILER_ENABLED=true` (en `USERFLAGS`: `-DBN_CFG_PROFILER_ENABLED=true`), envolver con
  `BN_PROFILER_START("id")`/`BN_PROFILER_STOP()` y `bn::profiler::show()`; `BN_CFG_PROFILER_LOG_ENGINE` mide
  también los subsistemas de Butano. Ejemplo `engine/butano/examples/profiler`.
- Bots que miden deben **comprobar que avanzan**: uno atascado da cifras falsamente buenas. Mide en la zona con más
  objetos y en las cargas (cambio de sala, aparición de muchos a la vez).

## Lecciones que se trasladan (SNES/GB)
- El coste suele estar en el algoritmo, no en la instrucción: podar pares de colisión (rejilla, cubos por columna,
  sólo cerca de la cámara) antes de pasar nada a ARM/IWRAM.
- Picos: limitar apariciones por frame; repartir trabajo lento en frames alternos; listas compactas de activos en vez
  de recorrer huecos vacíos.
- Pasar a ARM/IWRAM (o asm) sólo con cifra antes/después y una prueba que compare salida con la versión original.
- Fidelidad y jugabilidad antes que optimizar; un frame perdido ocasional que el usuario no nota no es prioridad.
