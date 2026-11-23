# Lecciones del estudio de SNES que valen aquí

Destilado de `Desktop\SNES` (oct–nov 2026: SNUTS, ~60 proyectos, una publicación en itch.io) y de los estudios de
Game Boy y Mega Drive. Sólo lo que se traslada a GBA con Butano. Cada punto: qué pasó → regla.

## Alcance y forma de trabajar
1. **El usuario abandona lo complejo.** Space Solo, el beat'em up Super FX, Topos, Short Count, Around the World,
   Dungeon 3D e Iron Wings se pararon por lío o alcance. Mars Lander (pequeño, una idea, efectos vistosos) se terminó y
   publicó en un día largo. → Un verbo, demo jugable en el primer bloque, crecer por bloques, publicar.
2. **Baterías largas = ritmo lento.** Repetir la batería completa en cada relevo marcaba el paso más que el código
   ("vamos muy lentos"). → Prueba del bloque + arranque; batería una vez al integrar o antes de push.
3. **No preguntar lo obvio, no esperar.** "No me preguntes si le das". Una noche entera parada esperando a un agente.
   → Seguir el plan; mientras un agente trabaja, hacer otra tarea sin solape. Preguntar sólo decisiones reales.
4. **Estimar con datos.** Un cambio de HUD que se estimó en "un día" eran ~27 sitios: un `grep` lo decía.
5. **Agentes largos y reanudados cuestan una fortuna** (coste ≈ llamadas × contexto). → Relevos cortos con nota
   (`docs/harness/coste-tokens.md`).
6. **Notas perdidas**: `bd update --notes` sustituía las notas. → `tasks.py` sólo añade salvo `--replace-notes`.

## Biblioteca primero
7. Cada juego reescribía sistemas que ya existían; casi la mitad de los "huecos de la lib" reportados ya estaban
   resueltos. → Butano primero (`que-usar-butano.md`), comprobar antes de reportar, y lo que falte va a `lib/` con
   tarea y prueba, nunca como rodeo dentro del juego.
8. **Fuentes inventadas otra vez** ("siempre estamos igual con las putas fuentes inventadas"): el encargo de interfaz
   no nombraba el módulo de texto y el agente hizo su propio motor. → El encargo nombra
   `bn::sprite_text_generator` y la fuente; se decide al crear el armazón.
9. **Arte que el usuario pueda tocar**: quiere editar en Aseprite y recompilar con doble clic. → `art/*.aseprite` como
   fuente de verdad + `compilar.bat`.
10. Las pruebas de un cambio de lib se hacen en el juego que lo pidió; el comportamiento existente no cambia por
    defecto (opciones o funciones nuevas).

## Medir antes de optimizar
11. **Fidelidad/jugabilidad primero, optimizar después**: optimizaciones tempranas movían referencias y gastaban
    verificación. Y unos tirones que el usuario no notaba se llevaron agentes enteros. → Optimizar sólo lo que se ve.
12. **C en caminos calientes**: en SNES el C del compilador era lento y las funciones por frame de la lib pasaron a asm
    (tarea BL-p1zt) — pero sólo tras perfilar varios juegos. Antes, el mayor ahorro vino del algoritmo: una colisión
    todos contra todos en asm seguía costando 40 líneas hasta podar pares. → En GBA: Thumb desde ROM por defecto;
    perfilar con `BN_PROFILER_*`; primero podar (rejilla, sólo cerca de la cámara, listas de activos, mitad de
    enemigos por frame); después ARM en IWRAM (`BN_CODE_IWRAM`, `*.bn_iwram.cpp`) con cifra antes/después y prueba de
    equivalencia.
13. **Bots de medida que no avanzan** dan cifras falsamente buenas (GB: un bot atascado dio "85 líneas"). → El bot
    comprueba progreso; se mide en la zona con más carga y en las transiciones.
14. **"La VRAM no da" no es excusa**: el usuario quiere el arte completo. → Repartir VRAM y paletas es ingeniería
    (tiles bajo demanda, paletas compartidas); sólo es límite si las cifras lo demuestran.

## Pruebas y hardware
15. **Probar lo que se ve.** GB: un diálogo que nunca salía en pantalla y NPCs con tiles del título, con todas las
    pruebas en verde. → Capturas revisadas con `Read`; aserciones sobre pantalla, no sólo variables.
16. **Determinismo**: el emulador es determinista; SNES se ahorró horas saltando pruebas de ROMs idénticas y comparando
    hashes de generadores. → Misma ROM + misma entrada = mismo resultado; si no, hay un bug (azar con reloj, memoria
    sin iniciar). Semillas fijas en pruebas.
17. **Estado de arranque**: Mesen puede arrancar con RAM aleatoria; una batería con RAM a unos nunca encontró nada
    porque crt0 pone la WRAM a cero, y el usuario la quitó por lenta. → Butano inicializa sus datos; no montar baterías
    de RAM aleatoria. Pero VRAM, OAM y paletas no se dan por iniciadas en hardware real: inicializa lo que leas.
18. **Escrituras fuera de tiempo**: SNES escribía registros de DMA/HDMA fuera del VBlank y metía basura; en GB las
    copias a VRAM fuera de VBlank se perdían en silencio y medir con más código cambiaba el fallo. Un frame perdido no
    debe subir nada a medias (SMW). → Con Butano, el hardware se toca en `bn::core::update()`: no escribir registros
    ni VRAM/OAM a mano por detrás de Butano; si un efecto lo exige, por su API (HBE, HDMA) o en `lib/` con prueba.
19. **Fallos del usuario se reproducen con su grabación** (Mega Drive: grabar y reproducir). → Keypad logger de Butano.

## Diseño
20. **Primer minuto**: en Short Count el jugador moría al aparecer, sin controles ni arma ("está mal diseñado"). →
    Revisión de `gba-designer` antes de dar una ROM.
21. **Problemas de aspecto o juego al diseñador**: los implementadores los parcheaban por lo barato. → `design-question`.
22. **Controles simples** (Space Solo: "súper lioso"; "Star Fox era una basura por eso"). → Esquema inmediato por
    defecto; lo complejo, opción.
23. **Bucle antes que novedades** (Chivi Panic: monstruos con trucos sin relación con el bucle). → Estudiar qué hace
    divertido el original del género; el contenido sirve al bucle.
24. **Audio**: dos versiones completas de música rechazadas por estilo. → Demos cortas para elegir antes de integrar.

## Publicación y disco
25. **Kit de release**: Mars Lander salió con web (EmulatorJS fijado con SHA-256), zip de ROM, portada, capturas
    reales, texto de página y lista de subida. → Skill `gba-release`, núcleo mGBA.
26. **Disco lleno** con ~107 worktrees completos; y una junction a un directorio versionado borró ficheros reales. →
    `worktree.py` (disperso, junctions sólo a lo no versionado, retirada al integrar) y nada destructivo en bloque sin
    el OK del usuario.
