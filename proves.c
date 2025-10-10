#include <stdio.h>
#include <unistd.h> // Para sleep/usleep

int main() {
    for (int i = 0; i <= 100; i++) {
        printf("\rProgreso: %d%%", i);
        fflush(stdout); // Asegura que se imprima inmediatamente
        usleep(50000);  // 50 ms
    }
    printf("\n"); // Salto de línea al final
    return 0;
}
