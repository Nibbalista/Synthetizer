#include "raylib.h"       
#include <math.h>         // sin(), PI
#include <stdlib.h>       // malloc, exit()
#include "stdio.h"        // printf


#define SAMPLE_RATE 44100 
#define BUFFER_SIZE 44100 //Size for 1 second
#define AMPLITUDE_MAX 32000

int main(void)
{
    // 1. App Init
    InitWindow(800, 600, "Synthétiseur");
    InitAudioDevice();                    
    SetTargetFPS(60);                     

    // 2. Sin Wave Creation from Raylib's Wave struct
    Wave wave = {
        .frameCount = BUFFER_SIZE,  
        .sampleRate = SAMPLE_RATE, 
        .sampleSize = 16,          // in bits (16 bits)
        .channels = 1              // 1 = Mono ; TO DO : 2 = Stéréo
    };

    // 3. Malloc
    wave.data = malloc(BUFFER_SIZE * sizeof(short)); // Dynamic Allocation !
    //Error
    if (wave.data == NULL) {
        printf("Malloc Error");
        exit(EXIT_FAILURE); 
    }

    short* buffer = (short*)wave.data; // Pointeur pour manipuler les données audio

    // 4. Wave parameter
    float frequency = 440.0f; // (440 Hz = LA/A)

    // 5. Génération d'une onde sinusoïdale
    for (int i = 0; i < BUFFER_SIZE; i++) {
        float time = (float)i / SAMPLE_RATE;     // time per sample
        float sinValue = sin(2.0f * PI * frequency * time); 
        buffer[i] = (short)(sinValue * AMPLITUDE_MAX);     // Cast into Amplitude
    }

    printf("First 10 samples: \n ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");
    // 6. Wave to Sound Conversion
    Sound sound = LoadSoundFromWave(wave);

    // 7. Sound Playing
    PlaySound(sound);

    // 8. App Main Loop
    while (!WindowShouldClose()) {
        // Drawing
        BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Synthétiseur", 190, 200, 40, PURPLE); 
            DrawText("Onde Sinusoïdale (LA - 440 Hz)", 190, 250, 20, WHITE);
        EndDrawing();
    }

    // 9. Cleaning
    UnloadSound(sound);
    UnloadWave(wave);  
    free(wave.data);        // Free memory from malloc
    CloseAudioDevice();
    CloseWindow();         

    return 0;
}
