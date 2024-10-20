# PythonToC-Transpiler

**PythonToC-Transpiler** es un transpiler diseñado para convertir código Python en código C utilizando PLY (Python Lex-Yacc). El propósito principal de este proyecto es traducir la sintaxis de Python a su equivalente en C, permitiendo que los programas escritos en Python se ejecuten como ejecutables en C.

## Requisitos

Para instalar las dependencias necesarias, se requiere tener Python 3.x instalado. Las dependencias del proyecto se pueden instalar ejecutando el siguiente comando:

```bash
pip install -r requirements.txt
```

## Manual de uso

Para usar el transpiler, el usuario debe ejecutar el siguiente comando en la terminal:

```bash
    python PythonLexerParser/main.py <ruta_del_archivo>
```

Donde `<ruta_del_archivo>` es la dirección del archivo que se desea analizar. Es importante proporcionar la ruta completa o relativa al archivo que se quiere procesar.

Por ejemplo:

```bash
    python PythonLexerParser/main.py ./codigo_ejemplo.py
```

Este comando permitirá analizar el archivo especificado y generará la salida correspondiente al proceso de análisis.

## Ejecutar Pruebas
Las pruebas para este proyecto están escritas usando pytest. Para ejecutar las pruebas, simplemente ejecute el siguiente comando en la terminal:

```bash
    pytest
```
Este comando descubrirá y ejecutará automáticamente todos los casos de prueba ubicados en el directorio `PythonLexerParser/Tests`.

Dentro de este directorio, se encuentran los siguientes archivos de prueba:

- `test_lexer.py`: Contiene los casos de prueba para el lexer.
- `test_parser.py`: Contiene los casos de prueba para el parser.

Ambos archivos se ejecutarán como parte del conjunto de pruebas al ejecutar el comando pytest.

## Funcionalidades No Implementadas

Actualmente, **PythonToC-Transpiler** no soporta las siguientes características de Python. Estas funcionalidades están en nuestra lista de posibles mejoras futuras:

- **Funciones anidadas**: Definir funciones dentro de otras funciones no está soportado.
- **Comprehensions**: La creación de listas, diccionarios y conjuntos usando comprehensions no es compatible.
- **Lambdas**: Las funciones anónimas (lambda) no están implementadas.
- **Funciones como argumentos**: Pasar funciones como argumentos a otras funciones no está permitido.
- **Punteros**: El manejo explícito de punteros no está soportado.
- **Decoradores**: Los decoradores para modificar el comportamiento de funciones o métodos no están disponibles.
- **Generadores y `yield`**: La creación de generadores y el uso de la palabra clave `yield` no están implementados.
- **Programación asíncrona (async/await)**: El soporte para programación asíncrona utilizando `async` y `await` no está disponible.
- **Metaclases**: La definición de clases utilizando metaclases no está soportada.
- **Herencia múltiple**: No se soporta la herencia múltiple en clases.
- **Administradores de contexto (`with` statement)**: El uso de administradores de contexto no está implementado.
- **Acceso dinámico a atributos (`getattr`, `setattr`, etc.)**: La manipulación dinámica de atributos de objetos no es posible.
- **Anotaciones (type hints)**: El soporte para anotaciones de tipo no está implementado.
- **Imports**: Solo se permiten los módulos básicos predefinidos; las importaciones de módulos externos no están soportadas.
- **Comprensión de clases**: La creación de clases dentro de comprehensions no está implementada.
- **Bloques `try-finally` y `try-except-else`**: El manejo avanzado de excepciones con estos bloques no está disponible.
- **F-strings**: Las cadenas formateadas mediante `f-strings` no son compatibles.
- **Asignaciones dentro de condicionales (`if`)**: La asignación de valores dentro de declaraciones `if` no está soportada.
- **Expresiones generadoras**: Las expresiones generadoras no están disponibles.