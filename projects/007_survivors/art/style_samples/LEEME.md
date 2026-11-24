# Muestras de estilo (G-44, seguimiento)

Cuatro propuestas para sustituir el arte actual (brillante, chibi algo tosco). **Sólo muestras**: no tocan
`art/*.aseprite`, `graphics/` ni `src/`. El usuario elige una y luego se rehace el set en `.aseprite`.

Cada `style_X.png` es una maqueta 240×160 a 1x (`style_X_3x.png` para verla grande): Wendel en el centro lanzando un
hechizo, 9 enemigos (slime, murciélago, esqueleto, goblin, orco), 5 gemas, decorado, suelo y un HUD mínimo (barra de
XP arriba, vida bajo el mago). Figuras de 16–32 px, cada sprite reducido a ≤15 colores + transparente (4bpp).

| Estilo | Qué lo define | Coste de rehacer el set | Riesgo con ~50 enemigos |
|---|---|---|---|
| **D Chibi pulido** (prioridad) | Cuerpos redondos de ~2 cabezas, ojos grandes, contorno grueso de color oscuro (no negro), sombreado en 2–3 tonos, colores vivos; suelo de formas limpias sin ruido. Aire de juguete. | Medio: es la línea más cercana al set actual, se reutilizan tamaños y animaciones; ~150–200 generaciones para personajes, jefe y efectos. | Muy legible: silueta y color fuertes sobre suelo plano. Los enemigos salen a 24–28 px; con 50 conviene bajar slime/murciélago/goblin a 16–20 px o la pantalla se llena. |
| **A Oscuro gótico** | Fantasía seria: paleta fría y apagada, sombras duras, proporciones realistas, monstruos amenazantes; sólo la magia (violeta) brilla. | Alto: todo nuevo (personajes, suelo, props, HUD en tono gris); ~200–250 generaciones. | El peor: monstruos grises sobre suelo oscuro se funden. Necesita suelo aún más plano, borde claro o halo, y gemas con brillo; probar en la ROM. |
| **B 16 bits JRPG** | Más detalle y sombreado rico, colores saturados pero adultos, cabeza menos grande, aire de aventura. | Alto: mucho detalle por sprite que hay que limpiar a mano en las animaciones; ~200–250 generaciones. | Medio: el detalle se convierte en ruido en grupos grandes; suelo y enemigos verdes/marrones compiten. Hay que reservar colores fuertes para enemigos. |
| **C Paleta limitada con brillo** | Fondo casi negro, figuras en silueta con borde frío de 1 px, magia, ojos y gemas en neón (magenta, cian, lima). Muy llamativo en capturas. | Bajo/medio: pocos colores, suelo trivial; el borde se añade por script. ~120–160 generaciones. | Bueno si cada enemigo conserva su borde y su color de ojos; sin borde los goblins desaparecen. Abusar del brillo (mezcla aditiva) cuesta capas en la GBA. |

Lámina comparativa (actual + D, A, B, C a 2x y a 1x): se generó en el scratchpad de la sesión; se rehace con el
mismo método (ver abajo) si hace falta.

## Cómo se hizo

- PixelLab `create_1_direction_object` (32 px → 64 candidatos por llamada; 64 px → 16): una llamada de personajes,
  gemas, FX y props y otra de decorado por estilo. 8 llamadas, 80 generaciones de suscripción, 0 créditos.
- Originales en `art/raw/style_samples/<X>_chars|<X>_deco/NN.png` (índices en `_src/compose.py`).
- `_src/fetch.py` descarga candidatos; `_src/compose.py` pinta el suelo (ruido + tramado Bayer; D sin ruido), recorta,
  limita colores, pone sombras, halos (imitan la mezcla aditiva de la GBA) y el HUD. `python -I compose.py [A B C D]`.

## Pendiente si se elige uno

Limpiar a mano cada sprite a su tamaño final (slime/goblin/murciélago a 16–20 px), animaciones, jefe Slime King,
HUD y cartas en el mismo tono, y comprobar en la ROM con 50 enemigos. Los textos siguen siendo del generador de Butano.
