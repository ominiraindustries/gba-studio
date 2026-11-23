# Coste de los agentes

Medido en SNES (164 agentes, 17–21 oct 2026: ~880 M tokens ponderados): el 90 % es **releer el contexto**. Cada
llamada vuelve a leer todo lo acumulado, así que **coste ≈ llamadas × tamaño del contexto**. Lo que escriben es < 1 %.
Los más caros fueron agentes reanudados una y otra vez (2841 llamadas uno solo).

## Reglas
1. **Encargos cortos**: un bloque cerrado, ~120 llamadas (máx. 150 salvo permiso del usuario). Si no cabe, se parte:
   nota de relevo en la tarea y agente **nuevo**.
2. **Contexto limpio**: nada de logs enteros, transcritos, JSONL ni binarios con `cat`. `tail`, `grep -n`, `Read` con
   offset. Las pruebas imprimen PASS/FAIL y lo justo.
3. **Pocos agentes**: 2–3 a la vez con rutas disjuntas. Mejor pocos bien definidos que muchos leyendo lo mismo.
4. **Mensajes agrupados** a un agente en marcha: cada mensaje cuesta su contexto entero.
5. **No delegar lo pequeño**: búsquedas, un cambio de un fichero, un resumen o una revisión simple se hacen en la
   sesión.
6. **Pruebas proporcionales**: la del bloque + arranque. La batería completa, una vez al integrar o antes de push. En
   SNES las baterías por relevo marcaban el ritmo más que el código.
7. **Skills bajo demanda**: carga sólo la que necesitas; las skills son cortas y apuntan a cabeceras y ejemplos.
8. **Esperas con tope** (`timeout`), y detener agentes que ya entregaron.
9. Al final de una fase larga el coordinador abre sesión nueva si su propio contexto es enorme.
