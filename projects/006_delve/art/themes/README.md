# Temas de mazmorra de DELVE (G-36)

Cada tema es un **juego de entorno completo** con los mismos nombres, tamaños y máscaras de abertura que la cloaca
(`art/export/`). La cloaca (estilo B, `art/*.aseprite`) sigue siendo el **tema 1**. Mismo estilo: pintura con sombreado
interno y tramado suave, sin contorno de dibujo animado; la única luz cálida es la antorcha del jugador.

| # | Carpeta | Qué es | Colores base |
|---|---|---|---|
| 1 | `art/` (cloaca) | ladrillo verde negruzco, limo bajo, charcos | 43 |
| 2 | `ice_cavern` | roca helada azul negra, escarcha en los cantos, carámbanos bajo las juntas, láminas de hielo, grieta azul, cascada helada | 47 |
| 3 | `timber_mine` | mina entibada: tablones entre postes, viga de cabeza, hollín, clavos oxidados, tornapuntas, tablas rotas sobre la roca | 47 |
| 4 | `mirror_hall` | losas de obsidiana con repisa de plata, espejos que reflejan un pasillo y un doble fantasmal, espejo roto, cristales violeta | 48 |
| 5 | `bone_crypt` | catacumba de arenisca polvorienta: nichos con calavera y huesos, osario de calaveras y fémures, amortajado | 46 |
| 6 | `lava_forge` | basalto columnar negro, grietas incandescentes con halo rojo, colada, boca de fragua con brasas, costra con vetas | 47 |
| 7 | `root_temple` | templo hundido de caliza gris: greca tallada, máscara en relieve, raíces que parten los sillares, línea de marea, suelo inundado | 47 |
| 8 | `flesh_warren` (opcional) | madriguera viva: lóbulos de carne con pliegues, venas, costillas, hendidura con dientes, ampollas, sangre en el suelo | 47 |

"Colores base" = colores de la paleta del tema (índice 0 negro incluido; ≤ 63, el 63 queda libre como transparente
del `.aseprite`). `build_view.py` cuantiza a sus 191 colores con el fundido por distancia: todos caben (el informe de
cada maqueta dice cuántos usa).

## Contenido de cada carpeta

- `tex_*.aseprite`: fuente de verdad (indexado, 64 colores, índice transparente 63, `tex_wall_torch` con la etiqueta
  `flame`). Los 20 mismos que `art/`.
- `export/tex_*.bmp`: exportados (los que lee `tools/build_view.py`), tamaños idénticos a `art/export/`.
- `export/deco_*.bmp` y `deco_palette.json`: los adornos (`deco_*`) son los mismos dibujos de `graphics/`; sólo cambian
  sus paletas de 16 colores (A antorcha/cadenas/palanca, B estandarte, G halo) con el mismo orden de huecos que
  `_seed/deco_art.py`. `mirror_hall` lleva fuego fatuo violeta en las antorchas; el resto, llama cálida.
- `seed.py`: semilla del tema (paleta + pintura). `_out/`: PNG semilla.
- `mockup.png` / `mockup_3x.png`: vista 240×160 proyectada con una copia de `build_view.py` y el HUD actual.

## Regenerar

    python -I projects/006_delve/art/themes/<tema>/seed.py              # semillas + .aseprite que falten + exportar
    python -I projects/006_delve/art/themes/<tema>/seed.py export-only  # sólo exportar (tras editar los .aseprite)
    python -I projects/006_delve/art/themes/<tema>/seed.py reseed       # BORRA los tex_*.aseprite del tema y los rehace

`reseed` es sólo para el artista: si has retocado un `.aseprite`, no lo uses. Código común: `theme_kit.py` (usa
`_seed/sewer_art.py` para puertas, arco, reja, escalera y adornos pintados, con las máscaras exactas del motor).

## Rampas (orden de la paleta, igual en todos)

`stone` 10 (material principal: roca, tablón, obsidiana, arenisca, basalto, caliza, carne), `stain` 5 (escarcha,
hollín, cristal, polvo, roca al rojo, raíces, venas), `wood` 6 (puerta), `iron` 5, `rust` 3, `flame` 5 (en la forja es
la lava: buena para ciclar paleta), `bone` 5, `cloth` 3, `acc` 3–5 (hielo profundo, roca, vidrio, ocre, costra,
agua, brillo húmedo). Los monstruos siguen legibles: sus valores y su borde cálido (índice 12) quedan por encima de
los muros; el tema más claro es `ice_cavern`, con la roca contenida en tonos medios.

## Decisiones abiertas (para el usuario)

- Qué temas entran y en qué orden de pisos (propuesta: cloaca → mina → cripta → hielo → templo → espejos → forja;
  la carne como piso secreto).
- El motor aún no carga temas (otro agente está añadiendo varios temas a `build_view`).
- `mirror_hall` pone un espejo en el muro base: un pasillo entero de espejos. Si cansa, quitarlo del base y dejarlo
  sólo en `_v1`/`_v2`.
