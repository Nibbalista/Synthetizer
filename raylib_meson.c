#include "raylib.h"
#include <math.h>         // sin(), floor(), PI
#include <stdlib.h>       // malloc, free
#include <stdio.h>        // printf

#define SAMPLE_RATE 44100
#define BUFFER_SIZE 44100   // Taille pour 1 seconde = 44100
#define AMPLITUDE_MAX 32000

typedef enum { SINUS, SAWTOOTH, SQUARE } WaveType; // Waves Types

// Function to generate a signal/wave from a type and a frequency
void GenerateWave(short* buffer, WaveType type, float frequency) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        float time = (float)i / SAMPLE_RATE;
        float value = 0.0f;

        switch (type) {
            case SINUS:
                value = sin(2.0f * PI * frequency * time);
                break;
            case SAWTOOTH:
                value = 2.0f * (time * frequency - floor(time * frequency)) - 1.0f;
                break;
            case SQUARE:
                value = (fmod(time * frequency, 1.0f) < 0.5f) ? 1.0f : -1.0f;
                break;
        }

        buffer[i] = (short)(value * AMPLITUDE_MAX);
    }
}

int main(void) {
    // Window Init
    InitWindow(800, 600, "Synthétiseur");
    InitAudioDevice();                    
    SetTargetFPS(60);

    // Sin Wave Creation from Raylib's Wave struct
    Wave wave = {
        .frameCount = BUFFER_SIZE,
        .sampleRate = SAMPLE_RATE,
        .sampleSize = 16,  // En bits (16 bits)
        .channels = 1      // Mono
    };

    // Malloc
    wave.data = malloc(BUFFER_SIZE * sizeof(short));
    if (wave.data == NULL) {
        printf("Erreur d'allocation mémoire.\n");
        CloseAudioDevice();
        CloseWindow();
        return EXIT_FAILURE;
    }

    short* buffer = (short*)wave.data;

    // Wave parameter
    float frequency = 440.0f;         // Default frequency (A/LA 440 Hz)
    WaveType waveType = SINUS;        // Default wave type (Sine Wave)

    // Generate the default Wave -> Turn it into a sound -> Play the sound
    GenerateWave(buffer, waveType, frequency);
    Sound sound = LoadSoundFromWave(wave);
    PlaySound(sound);

    // UI Buttons for Wave Type
    Rectangle sinusButton = { 50, 400, 200, 50 };
    Rectangle sawtoothButton = { 300, 400, 200, 50 };
    Rectangle squareButton = { 550, 400, 200, 50 };

    //  App Main Loop
    while (!WindowShouldClose()) {
        if (!IsSoundPlaying(sound)) {
            PlaySound(sound); // Replaying sound when it ends 
        }

        // Buttons Pressing
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePoint = GetMousePosition();

            // Check collision for each button
            if (CheckCollisionPointRec(mousePoint, sinusButton)) {
                waveType = SINUS;
            } else if (CheckCollisionPointRec(mousePoint, sawtoothButton)) {
                waveType = SAWTOOTH;
            } else if (CheckCollisionPointRec(mousePoint, squareButton)) {
                waveType = SQUARE;
            }

            // Updating new wave form
            if (wave.data != NULL) {
                StopSound(sound); // stoping the previous sound so it doesn't crash

                // Generate a new wave from the new wave type
                GenerateWave(buffer, waveType, frequency);

                // Convet it into a sound an play it
                UnloadSound(sound);
                sound = LoadSoundFromWave(wave);
                PlaySound(sound);

            } else {
                printf("Erreur : données audio invalides.\n");
            }
        }

        // Drawing UI
        BeginDrawing();
            ClearBackground(BLACK);


            DrawText("Synthétiseur", 250, 50, 40, PURPLE); 
            DrawText("Cliquez sur un bouton pour changer l'onde", 190, 100, 20, WHITE);

            // Buttons
            DrawRectangleRec(sinusButton, GRAY);
            DrawText("Sinusoïdale", sinusButton.x + 20, sinusButton.y + 15, 20, WHITE);

            DrawRectangleRec(sawtoothButton, DARKGRAY);
            DrawText("Dent de Scie", sawtoothButton.x + 20, sawtoothButton.y + 15, 20, WHITE);

            DrawRectangleRec(squareButton, LIGHTGRAY);
            DrawText("Carrée", squareButton.x + 50, squareButton.y + 15, 20, BLACK);

            // Displaying active Wave type playing
            if (waveType == SINUS) {
                DrawText("Type d'onde actuel : Sinusoïdale", 250, 200, 20, GREEN);
            } else if (waveType == SAWTOOTH) {
                DrawText("Type d'onde actuel : Dent de Scie", 250, 200, 20, ORANGE);
            } else if (waveType == SQUARE) {
                DrawText("Type d'onde actuel : Carrée", 250, 200, 20, RED);
            }
        EndDrawing();
    }

    // Cleaning and closing
    UnloadSound(sound);
    free(wave.data);       
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
