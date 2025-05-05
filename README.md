# GanttChart_grapho

Generador de gráficos de Gantt en HTML a partir de archivos CSV.

## Descripción
Este proyecto permite visualizar la programación de tareas y sus dependencias mediante un diagrama de Gantt generado automáticamente en HTML. El programa toma como entrada un archivo CSV con la información de las tareas, sus predecesores y duración.

## Uso
1. Compila el archivo `grapho.cpp` con tu compilador de C++ favorito (g++, clang++, etc.).
2. Ejecuta el programa. Cuando se solicite, ingresa el nombre del archivo CSV (sin la extensión).
3. Se generará un archivo `GGrantt.html` que puedes abrir en tu navegador para ver el diagrama de Gantt.

## Formato del archivo CSV
El archivo debe tener la siguiente estructura:

```
ID;Task;Predecessors;Duration
1;Start;~;0
2;a;1;4
3;b;1;5.33
4;c;2;5.17
5;d;2;6.33
6;e;3,4;5.17
7;f;5;4.5
8;g;6;5.17
9;Finish;7,8;0
```

- **ID**: Identificador de la tarea
- **Task**: Nombre de la tarea
- **Predecessors**: IDs de tareas predecesoras (separadas por coma, o ~ si no tiene)
- **Duration**: Duración de la tarea

## Requisitos
- C++11 o superior
- Sistema operativo: Windows, Linux o MacOS

## Autor
joseh