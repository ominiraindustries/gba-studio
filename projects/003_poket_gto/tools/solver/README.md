# Solver offline de POKER DOJO

Herramienta en Python/numpy que calcula, sin descargar nada, las tablas de póker que usa el juego:

1. **Equidades preflop all-in exactas 169×169** (AA, AKs, AKo...), ponderadas por combinaciones con eliminación de
   cartas.
2. **Equilibrio Nash push/fold heads-up** (el SB va all-in o se retira; la BB paga o se retira) de 1 a 25 bb en
   pasos de 0,5 bb, con y sin ante.
3. **Exportación** a cabeceras C++ `constexpr` para Butano (sin memoria dinámica) en `../../include/`.

## Cómo se ejecuta

Desde la raíz del repo (Git Bash o PowerShell):

```
python -I projects/003_poket_gto/tools/solver/solve.py                  # usa la caché si existe
python -I projects/003_poket_gto/tools/solver/solve.py --recompute-equity
python -I projects/003_poket_gto/tools/solver/test_solver.py            # validación (exit 1 si algo falla)
```

Tiempos (este PC): equidades **~10 min** la primera vez (se guardan en `cache/equity169.npz`, ignorada por git);
push/fold de las 98 tablas **~7 s**; tests **~12 s**.

| Fichero | Qué hace |
|---|---|
| `hands.py` | cartas, 1326 combos, clases 0..168 y pesos de pares de combos sin cartas compartidas |
| `evaluator.py` | evaluador vectorizado de 7 cartas (categoría << 20 + 5 kickers de 4 bits) |
| `equity.py` | equidades exactas 169×169 (caché en `cache/`) |
| `pushfold.py` | juego push/fold y solver CFR+ |
| `export_headers.py` | escribe `poker_tables_pushfold.h` y `poker_tables_equity.h` |
| `solve.py` | todo el proceso y resumen por consola |
| `test_solver.py` | validación con propiedades conocidas |

## Método

- **Equidades**: se enumeran los 134.459 tableros de 5 cartas distintos salvo permutación de palos (peso = tamaño de
  la órbita; suman C(52,5) = 2.598.960). En cada tablero se puntúan los 1326 combos, se ordenan y un conteo acumulado
  por clase da victorias/empates de cada combo contra cada clase. Los ~100 combos que comparten carta con el héroe se
  restan de forma exacta. Como la suma por clase es invariante por palos, el resultado es **exacto** (error 0, no
  Monte Carlo). E[a,b] = P(gana) + P(empate)/2, promediado sobre todos los pares de combos sin cartas compartidas.
- **Push/fold**: juego de suma cero sobre las 169 clases, con probabilidad conjunta P[h,c] = pares de combos
  disjuntos / (1326·1225). Se resuelve con **CFR+** (regret matching+, actualizaciones alternas, media lineal) hasta
  NashConv < 1e-6 bb por mano (lo pedido era < 0,01). Comprobado contra un programa lineal (scipy/HiGHS): mismo valor
  del juego a 4, 10 y 20 bb.

## Convenciones

- Carta = rango·4 + palo, rango 0..12 = 2..A (igual que `include/poker_cards.h`).
- **Índice de clase 0..168**: matriz clásica 13×13, filas y columnas A, K, Q... 2 (posición 0 = A), índice =
  fila·13 + columna. Diagonal = parejas (AA=0, KK=14, 22=168); **encima** de la diagonal suited (AKs=1); **debajo**
  offsuit (AKo=13, 32o=167). En C++: `poker::class_index(rank_a, rank_b, suited)` y `poker::class_names[i]`.
- Ciegas 0,5/1. **Pila efectiva S** = todo lo que tiene cada jugador antes de poner nada. **Ante** (variante 1): cada
  jugador pone además 0,1 bb muerta de la misma pila (bote inicial 1,7 bb). EV medida desde el inicio de la mano:
  SB fold = −0,5−A; push y fold de la BB = +1+A; push y call = (2e−1)·S. A 1 bb con ante la fórmula se aplica tal
  cual (la BB paga siempre; irrelevante en la práctica).
- Cabecera `poker_tables_pushfold.h` (namespace `poker`), tablas `[variante][pila][clase]`, variante 0 = sin ante,
  1 = ante; pila `i` = `pushfold_stack_halves[i]` medias bb (2..50), `pushfold_stack_index(stack*2)` da la fila:
  - `sb_push_freq`, `bb_call_freq`: uint8, 0 = nunca, 255 = siempre (mixtas en medio).
  - `sb_push_ev`: int16, EV(push) − EV(fold) del SB en centésimas de bb contra la BB de Nash → si es positiva y el
    jugador se retira, pierde exactamente eso.
  - `bb_call_ev`: int16, EV(call) − EV(fold) de la BB **dado que el SB ha empujado** con su rango de Nash.
- Cabecera `poker_tables_equity.h`: `preflop_equity[a][b]` uint16 en 1/10000 (5000 = 50 %).
- Datos: pushfold 97 KB (2·49·169·6 bytes), equidad 56 KB. Fuente: 365 KB y 201 KB de texto.

## Resultados clave

Porcentaje de las 1326 manos (ponderado por combos):

| Pila | Push SB | Call BB | Push SB (ante) | Call BB (ante) |
|---|---|---|---|---|
| 25 bb | 36,3 % | 17,3 % | 38,4 % | 19,9 % |
| 20 bb | 40,2 % | 21,7 % | 42,9 % | 24,1 % |
| 15 bb | 45,7 % | 28,5 % | 50,1 % | 30,7 % |
| 10 bb | 58,3 % | 37,4 % | 61,7 % | 41,2 % |
| 8 bb | 61,9 % | 45,0 % | 67,1 % | 48,6 % |
| 6 bb | 68,4 % | 54,4 % | 71,9 % | 59,9 % |
| 4 bb | 73,8 % | 74,1 % | 76,8 % | 80,9 % |
| 2 bb | 89,6 % | 100 % | 95,5 % | 100 % |

- 10 bb sin ante, push: todas las parejas, cualquier Ax y Kx, Q7o+, Q2s+, J8o+, J3s+, T8o+, T4s+, 97o+, 95s+, 87o,
  84s+, 76o, 74s+, 64s+, 53s+; 43s mixta. Call: 22+, A2o+, K5o+, K2s+, Q9o+, Q7s+, JTo, J8s+, T9s.
- Otros datos: AA vs KK 81,95 %; AA contra mano aleatoria 85,20 %; a 10 bb 72o pierde 0,49 bb al empujar.
- Hay pocas mezclas (51 casos de push en total, sobre todo en la frontera a pilas cortas); el resto son 0/255.

## Validación (`test_solver.py`)

Frecuencias exactas de las 9 categorías sobre las 133.784.560 manos de 7 cartas; victorias + derrotas + empates =
todos los repartos; E[a,b] + E[b,a] = 1; AKs vs 22 igual a una enumeración independiente por fuerza bruta; suma de
combos 1326 y de pares 1326·1225; AA push y call siempre; push a 10 bb en 50–65 % (sale 58,3 %); los rangos se
ensanchan al bajar la pila; el ante ensancha; signo de la EV coherente con la acción; la explotabilidad converge;
valor del juego igual al LP; y las cabeceras compilan con `arm-none-eabi-g++ -std=c++20` con `static_assert`.

## Límites

- Sólo **heads-up SB contra BB**, sólo push/fold (sin open-raise, limp ni juego postflop).
- Sin **ICM** (fichas = dinero) y sin **6-max**: pendientes.
- Ante fijo de 0,1 bb por jugador; otras antes requieren volver a ejecutar `solve.py` (cambiar `VARIANTS`).
- Las mezclas de Nash no son únicas: en clases de frontera otra frecuencia igual de válida puede existir; la EV
  (casi 0 en esas clases) es lo fiable para puntuar errores.
