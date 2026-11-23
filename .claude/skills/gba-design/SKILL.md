---
name: gba-design
description: Diseño de juegos GBA - cómo arrancar un juego pequeño, escribir DISENO.md, decidir design-question, revisar el primer minuto antes de entregar una ROM y diseñar para 240x160 con controles de GBA. Úsala al diseñar o revisar juego, niveles, enemigos o jefes.
---

# Diseño

Base obligatoria: `docs/guias/diseno-juegos.md` (reglas de oro y checklist). Esta skill añade el método del estudio.

## Arranque de un juego (alcance pequeño)
1. Una frase y un verbo central. Si no cabe en una frase, es demasiado grande.
2. Referencia comercial del género estudiada (qué la hace divertida: bucle, riesgo, recompensa), sin copiar recursos.
3. Primer bloque jugable en días, no semanas: greybox (cuadros de colores, texto con la fuente de Butano), toda la
   lógica del verbo. El arte final llega cuando el usuario ya lo ha jugado.
4. `DISENO.md`: frase, verbo, controles, números (px/frame), pantallas, primer minuto, alcance del bloque 1, qué NO
   entra, riesgos, "Decisiones de diseño" (decisión · principio · métrica).
5. Crecer por bloques jugables. Lo que no entra en el alcance va a tareas `P4`, no al juego.

## Pantalla y mando de GBA
- 240×160, visto en pantalla pequeña: personajes legibles (≥16 px de alto el jugador, salvo motivo), HUD mínimo,
  texto corto. Cruceta, A, B, L, R, START, SELECT: A = acción principal, B = salto o secundaria (decídelo una vez por
  juego y no lo cambies), L/R para extras.
- Controles simples por defecto; esquemas avanzados sólo como opción. El jugador debe hacer algo con sentido en los
  primeros 10 s.

## Primer minuto (revisión antes de entregar una ROM)
- [ ] Se ve qué controlo y qué hago, sin leer un manual.
- [ ] Los controles se enseñan con el espacio o un rótulo breve en su momento.
- [ ] Empiezo con lo necesario (arma, vida, herramienta) y en un sitio seguro.
- [ ] Nada me mata sin aviso previo visible; el primer peligro se presenta solo.
- [ ] Cada acción tiene respuesta (sonido, animación, número).
- [ ] Reintentar tras perder cuesta < 5 s.

## design-question
Problema que afecta a aspecto o juego → tarea `design-question` (problema, opciones, coste). El diseñador elige,
documenta y cierra; el implementador aplica. El usuario sólo decide dirección artística y cambios caros del núcleo.

## Clon o adaptación
Si el juego adapta otro, el original manda en reglas y números (citados); la resolución distinta se adapta sin
preguntar (llenar 240×160). El contenido (enemigos, arte, música) es propio.
