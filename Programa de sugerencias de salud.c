#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <stdlib.h>

// Configuración especial para Windows (API para UTF-8)
#ifdef _WIN32
#include <windows.h>
#endif

// Prototipos de funciones
void configure_encoding();  // Configuración regional UTF-8
void mostrarDatosEstudiante();  // Muestra datos del desarrollador
void limpiarBuffer();  // Limpia el buffer de entrada
void registroUsuario(char nombre[], char estadoSalud[], char *sexo, int *edad, float *peso, float *altura);  // Captura y valida datos del usuario
float calcularCalorias(char sexo, int edad, float peso, float altura);  // Aplica fórmula nutricional
void generarSugerencia(char estadoSalud[]);  // Genera recomendaciones personalizadas

// ========== CONFIGURACIÓN UTF-8 MULTIPLATAFORMA ==========
void configure_encoding() {
    #ifdef _WIN32
        // Windows: Cambia la codificación de la consola a UTF-8
        SetConsoleOutputCP(CP_UTF8);  // Salida
        SetConsoleCP(CP_UTF8);        // Entrada
        system("chcp 65001 > nul");   // Forzar UTF-8 en Dev-C++
    #else
        // Linux/Mac: Configura localización para caracteres especiales
        setlocale(LC_ALL, "C.UTF-8");  
    #endif
}

// ========== FUNCIÓN PRINCIPAL ==========
int main() {
    configure_encoding();  // Asegura compatibilidad con tildes/ñ
    
    // Variables para almacenar datos del usuario
    char nombre[50], estadoSalud[50], sexo, respuesta;
    int edad;
    float peso, altura;

    mostrarDatosEstudiante();  // Muestra información del desarrollador

    // Bucle principal: permite múltiples ejecuciones
    do {
        registroUsuario(nombre, estadoSalud, &sexo, &edad, &peso, &altura);  // Paso 1: Captura datos
        float calorias = calcularCalorias(sexo, edad, peso, altura);          // Paso 2: Cálculo nutricional
        
        // Paso 3: Mostrar resultados
        printf("\nHola, %s. Aquí están tus resultados:\n", nombre);
        printf("Calorías diarias recomendadas: %.2f\n", calorias);
        generarSugerencia(estadoSalud);  // Paso 4: Recomendaciones

        // Preguntar por otra ejecución
        printf("\n¿Desea calcular nuevamente para otro usuario? (S/N): ");
        scanf(" %c", &respuesta);
        limpiarBuffer();  // Elimina residuos del buffer

    } while(toupper(respuesta) == 'S');  // Repetir si respuesta es 'S' o 's'

    // Pausa solo en Windows antes de cerrar
    #ifdef _WIN32
    system("pause");
    #endif

    return 0;
}

// ========== IMPLEMENTACIÓN DE FUNCIONES ==========

// Muestra información del desarrollador (estática)
void mostrarDatosEstudiante() {
	printf("----------------------------------------------------\n");
    printf("--------------- Datos del estudiante ---------------\n");
    printf("Nombre: Orlando Antonio Maturano Pizaña\n");
    printf("Matrícula: ES251107915\n");
    printf("Grupo: DS-DFPR-2501-B1-013\n\n");
}

// Limpia el buffer de entrada para evitar errores
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);  // Lee hasta fin de línea o archivo
}

// Captura y valida datos del usuario con bucles de verificación
void registroUsuario(char nombre[], char estadoSalud[], char *sexo, int *edad, float *peso, float *altura) {
    printf("\n=== Registro de usuario ===\n");
    
    // Validación 1: Nombre no vacío (usa fgets para espacios)
    do {
        printf("Nombre: ");
        fgets(nombre, 50, stdin);
        nombre[strcspn(nombre, "\n")] = '\0';  // Elimina el salto de línea
    } while(strlen(nombre) == 0);  // Repite si está vacío

    // Validación 2: Estado de salud válido
    do {
        printf("Estado de salud [bueno/malo/lesiones/afecciones]: ");
        fgets(estadoSalud, 50, stdin);
        estadoSalud[strcspn(estadoSalud, "\n")] = '\0';
    } while(strlen(estadoSalud) == 0);

    // Validación 3: Sexo solo H/M (convierte a mayúsculas)
    do {
        printf("Sexo (H/M): ");
        scanf(" %c", sexo);
        limpiarBuffer();
        *sexo = toupper(*sexo);  // Estandariza entrada
    } while(*sexo != 'H' && *sexo != 'M');

    // Validación 4: Edad entre 1 y 120 años
    do {
        printf("Edad: ");
        scanf("%d", edad);
        limpiarBuffer();
    } while(*edad <= 0 || *edad > 120);  // Rango realista

    // Validación 5: Peso positivo
    do {
        printf("Peso (kg): ");
        scanf("%f", peso);
        limpiarBuffer();
    } while(*peso <= 0);

    // Validación 6: Altura positiva
    do {
        printf("Altura (cm): ");
        scanf("%f", altura);
        limpiarBuffer();
    } while(*altura <= 0);
}

// Calcula calorías usando fórmula Harris-Benedict modificada
float calcularCalorias(char sexo, int edad, float peso, float altura) {
    const float C_HOMBRE = 5.0f;     // Constante para hombres
    const float C_MUJER = -161.0f;   // Constante para mujeres
    float constante = (toupper(sexo) == 'H') ? C_HOMBRE : C_MUJER;  // Asigna valor según sexo
    
    return (10.0f * peso) +         // Fórmula:
           (6.25f * altura) - 
           (5.0f * edad) + 
           constante;              // 10*peso + 6.25*altura -5*edad + C
}

// Genera recomendaciones según estado de salud (comparación insensible)
void generarSugerencia(char estadoSalud[]) {
    // Normaliza a minúsculas para comparar cualquier formato
    for(int i = 0; estadoSalud[i]; i++) {
        estadoSalud[i] = tolower(estadoSalud[i]);
    }

    printf("\nSugerencia de actividad física:\n");
    // Busca coincidencias en el texto ingresado
    if(strstr(estadoSalud, "bueno") || strstr(estadoSalud, "sin lesiones")) {
        printf("- 30 minutos de caminata\n- 15 minutos de ejercicios de fuerza\n- 10 minutos de estiramientos\n");
    } else {
        printf("- Consultar a un especialista para un plan personalizado\n");  // Caso por defecto
    }
}