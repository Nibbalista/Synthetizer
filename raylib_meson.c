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

// Note frequencies Table
float note_frequencies[] = {
    261.63f,  // C (Do)
    277.18f,  // C# (Do#)
    293.66f,  // D (Ré)
    311.13f,  // D# (Ré#)
    329.63f,  // E (Mi)
    349.23f,  // F (Fa)
    369.99f,  // F# (Fa#)
    392.00f,  // G (Sol)
    415.30f,  // G# (Sol#)
    440.00f,  // A (La) -> A4/La440
    466.16f,  // A# (La#)
    493.88f,  // B (Si)
    523.25f,  // C (Do) (octave supérieur)
    554.37f,  // C# (Do#)
    587.33f,  // D (Ré)
    622.25f,  // D# (Ré#)
    659.25f,  // E (Mi)
    698.46f,  // F (Fa)
    739.99f,  // F# (Fa#)
};

// Function to generate a signal/wave from a type and a frequency
float GetFrequencyFromKey(int key) {
    switch (key) {
        case KEY_TAB: return note_frequencies[0];  // Do (C)
        case '1': return note_frequencies[1];      // Do# (C#)
        case 'Q': return note_frequencies[2];      // Ré (D)
        case '2': return note_frequencies[3];      // Ré# (D#)
        case 'W': return note_frequencies[4];      // Mi (E)
        case 'E': return note_frequencies[5];      // Fa (F)
        case '4': return note_frequencies[6];      // Fa# (F#)
        case 'R': return note_frequencies[7];      // Sol (G)
        case '5': return note_frequencies[8];      // Sol# (G#)
        case 'T': return note_frequencies[9];      // La (A)
        case '6': return note_frequencies[10];     // La# (A#)
        case 'Y': return note_frequencies[11];     // Si (B)
        case 'U': return note_frequencies[12];     // Do (C)
        case '8': return note_frequencies[13];     // Do# (C#)
        case 'I': return note_frequencies[14];     // Ré (D)
        case '9': return note_frequencies[15];     // Ré# (D#)
        case 'O': return note_frequencies[16];     // Mi (E)
        case 'P': return note_frequencies[17];     // Fa (F)
        default: return -1.0f;  // Not a Valid Key
    }
}

int main(void) {
    // Window and Audio Device Init
    InitWindow(800, 600, "Synthétiseur");
    InitAudioDevice();                    
    SetTargetFPS(60);

    // Sine Wave Creation from Raylib's Wave struct
    Wave wave = {
        .frameCount = BUFFER_SIZE,
        .sampleRate = SAMPLE_RATE,
        .sampleSize = 16,  // En bits (16 bits)
        .channels = 1      // Mono
    };

    // Malloc for audio datas
    wave.data = malloc(BUFFER_SIZE * sizeof(short));
    if (wave.data == NULL) {
        printf("Erreur d'allocation mémoire.\n");
        CloseAudioDevice();
        CloseWindow();
        return EXIT_FAILURE;
    }

    short* buffer = (short*)wave.data;

    // Wave parameter
    float frequency = -1.0f;   // Default frequency (Invalid frequency so no Note is played)
    WaveType waveType = SINUS; // Default wave type (Sine Wave)

    // Generate the default Wave 
    GenerateWave(buffer, waveType, 440.0f);  // Generating a default wave

    Sound sound = LoadSoundFromWave(wave);
    StopSound(sound);  // Default is not playing

    //  App Main Loop
    while (!WindowShouldClose()) {
        // Detecting key presses
        int key = GetKeyPressed();
        if (key != 0) {
            // Changing Wave Type with Hotkeys : F1, F2, F3
            if (key == KEY_F1) {
                waveType = SINUS;
            } else if (key == KEY_F2) {
                waveType = SAWTOOTH;
            } else if (key == KEY_F3) {
                waveType = SQUARE;
            }
            
            // Getting the frequency associated to a hotkey
            frequency = GetFrequencyFromKey(key);
            if (frequency > 0.0f) {
                GenerateWave(buffer, waveType, frequency);
                StopSound(sound);  // Stop the previous note
                sound = LoadSoundFromWave(wave);  // Load the new one
                PlaySound(sound);  // Then play it
            }
        }

        // Drawing UI
        BeginDrawing();
            ClearBackground(BLACK);

            DrawText("Synthétiseur", 250, 50, 40, PURPLE); 
            DrawText("Appuyez sur les touches pour jouer des notes", 180, 100, 20, WHITE);
            DrawText("F1: Onde Sinusoïdale | F2: Dent de Scie | F3: Signal Carré", 120, 200, 20, WHITE);

            // Showing the frequency of the note playing
            if (frequency > 0.0f) {
                char frequencyText[100];
                snprintf(frequencyText, sizeof(frequencyText), "Fréquence: %.2f Hz", frequency);
                DrawText(frequencyText, 250, 150, 20, GREEN);
            } else {
                DrawText("Aucune note jouée", 250, 150, 20, RED);
            }
        EndDrawing();
    }

    // Cleaning and Closing
    UnloadSound(sound);
    free(wave.data);       
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
