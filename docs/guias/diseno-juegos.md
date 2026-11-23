# Diseño de juegos

Léela **antes de diseñar** juego, nivel, mundo, enemigo o jefe, y cita las reglas que aplicas. Resume principios
contrastados (fuentes al final) para juegos 2D pequeños de cualquier género. Adaptada de la guía del estudio de SNES
(2026-11-15); los capítulos largos de allí (`Desktop\SNES\docs\guias\diseno-juegos\`) sirven de consulta puntual.

En un clon o adaptación **manda el original** en reglas y números; esta guía sirve para entenderlo y para los huecos.

## Reglas de oro
1. **El verbo primero.** Si moverse y la acción central no gustan en una sala vacía, ningún nivel lo arregla. Cierra
   física y números antes de hacer niveles.
2. **Perdona sin que se note:** tiempo de coyote, búfer de entrada, corrección de esquinas; cajas de daño generosas
   para el jugador y ajustadas para los enemigos.
3. **Una idea por tramo: presentar → desarrollar → girar → cerrar.** Presenta en sitio seguro; retírala antes de que
   canse.
4. **Ritmo con silencios:** retos cortos separados por pausas seguras.
5. **Todo peligro se ve antes de dañar:** sin saltos de fe, sin enemigos que nacen encima, ataques con anticipación.
6. **Enseña con el espacio, no con texto;** si hace falta texto, una línea en el momento en que se usa.
7. **Cerradura antes que llave, y cada llave abre muchas cerraduras** (mundos conectados).
8. **Volver debe dar algo nuevo;** los atajos acortan el mundo a medida que crece.
9. **Un estado del mundo siempre se ve y nunca bloquea** (sin softlocks en ninguna combinación alcanzable).
10. **Fallar cuesta poco:** reintento inmediato y puntos de control cerca de cada reto.
11. **Jefes que examinan lo aprendido,** con fases que cambian el comportamiento, no sólo la vida.
12. **Lo opcional regula la dificultad:** secretos y mejoras ayudan a quien explora y retan a quien lo busca.
13. **Mide, no opines:** tiempos, muertes por sitio, dónde se pierde la gente, con entrada reproducible.
14. **Simple antes que profundo:** controles inmediatos; la complejidad, opcional. (El usuario abandonó juegos por
    controles liosos y por alcance excesivo.)

## Diseño con psicología (cada decisión lleva su porqué)
Cada tabla de diseño (enemigos, objetos, ritmo, colores) lleva una columna "principio" y la métrica que lo comprobará:
flujo (reto ≈ habilidad), andamiaje, riesgo/recompensa, aversión a la pérdida, anticipación (telegraphing),
recompensa variable, tensión/alivio, lenguaje de color (peligro, recogible, fondo), gradiente de meta.

## Checklist del primer minuto (antes de dar una ROM al usuario)
- [ ] Se entiende qué controlo y qué hago en 10 s, sin manual.
- [ ] Los controles se enseñan en su momento (espacio o rótulo breve).
- [ ] Empiezo con lo necesario y en un sitio seguro; el primer peligro se presenta solo.
- [ ] Nada mata sin aviso visible; cada acción tiene respuesta (sonido, animación, número).
- [ ] Reintentar cuesta < 5 s.

## Checklist de un nivel / sala
**Idea**: se resume en una frase; una sola idea nueva principal, presentada en sitio seguro, desarrollada, girada y
cerrada; camino crítico claro; al menos un momento memorable.
**Legibilidad**: huecos y alturas dentro de la física **medida** (no la del documento) con margen; ningún salto de fe;
todo peligro reconocible antes de dañar; plataformas, peligros, fondo y objetos se distinguen por forma y color en la
captura a 1x (240×160); la cámara muestra lo que viene.
**Ritmo**: retos cortos con pausas; punto de control antes de cada reto grande (segundos de reintento anotados);
dificultad sube con al menos un descanso; una recompensa con pista para quien explora.
**Técnica**: sin parpadeo de sprites en el momento de más carga; CPU dentro del frame; recorrido reproducible con
`gbarun.py` y capturas.
**Prueba**: bot o entrada grabada con tiempo por tramo y muertes por sitio; revisado por alguien que no lo diseñó.

## Checklist de un mundo conectado
- [ ] Grafo de zonas, llaves, cerraduras y estados; desde cualquier estado alcanzable hay camino a la siguiente llave.
- [ ] Ninguna combinación de estados produce softlock; las roturas de secuencia previstas tienen salida.
- [ ] Cada cerradura se ve antes de tener su llave; símbolos consistentes en todo el mundo.
- [ ] Mapa con salas visitadas y puertas pendientes; hitos visuales por zona.
- [ ] Guardado antes de jefes y zonas peligrosas; los puntos sin retorno avisan.
- [ ] Curva de dificultad en dientes de sierra; tiempo del camino crítico medido con bot.

## Antipatrones frecuentes
| Antipatrón | Arreglo |
|---|---|
| El jugador muere al aparecer o sin ver la amenaza | sala inicial segura; aparición fuera de pantalla o con aviso |
| Controles que el jugador debe aprender antes de jugar | esquema por defecto inmediato; avanzado como opción |
| Números del documento distintos de los reales | medir en la ROM y validar niveles contra lo medido |
| Novedad por novedad (un truco por enemigo sin relación con el bucle) | contenido al servicio del bucle principal |
| Repetir la misma idea sin giro | presentar → desarrollar → girar → cerrar |
| Texto largo para explicar | enseñar con el espacio; una línea en el momento |
| Castigo largo (volver al principio) | reintento inmediato, puntos de control |

## Fuentes principales
- *The Level Design Book* (Robert Yang y otros), https://book.leveldesignbook.com
- Itay Keren, "Scroll Back: The Theory and Practice of Cameras in Side-Scrollers" (GDC 2015)
- Smith, Cha, Whitehead, "A Framework for Analysis of 2D Platformer Levels" (2008)
- Maddy Thorson, "Celeste & Forgiveness", https://maddymakesgames.com/articles/celeste_and_forgiveness/
- "The Invisible Hand of Super Metroid" (Game Developer)
- Mike Stout, "Boss Battle Design and Structure" (2010)
- Mark Brown (Game Maker's Toolkit): *Boss Keys*, "Super Mario's Invisible Difficulty Settings"
- Jenova Chen, *Flow in Games*; Dan Cook, "The Chemistry of Game Design" (2007)
- Libros: Swink *Game Feel*; Schell *The Art of Game Design*; Anthropy y Clark *A Game Design Vocabulary*;
  Rogers *Level Up!*; Koster *A Theory of Fun*.
