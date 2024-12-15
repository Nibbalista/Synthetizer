#include "raylib.h"
#include <math.h>         // sin(), floor(), PI
#include <stdlib.h>       // malloc, free
#include <stdio.h>        // printf

#define SAMPLE_RATE 44100
#define BUFFER_SIZE 44100   // Taille pour 1 seconde = 44100
#define AMPLITUDE_MAX 32000

typedef enum { SINUS, SAWTOOTH, SQUARE } WaveType; // Types d'onde disponibles

// Fonction pour générer une onde en fonction du type
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

// Table des fréquences des notes, basé sur le Do (C) à 440 Hz (A4)
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

// Fonction pour obtenir la fréquence de la touche
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
        default: return -1.0f;  // Invalide
    }
}

int main(void) {
    // Initialisation de la fenêtre et du périphérique audio
    InitWindow(800, 600, "Synthétiseur");
    InitAudioDevice();                    
    SetTargetFPS(60);

    // Création de l'onde de sinusoïdale via la structure Wave de Raylib
    Wave wave = {
        .frameCount = BUFFER_SIZE,
        .sampleRate = SAMPLE_RATE,
        .sampleSize = 16,  // En bits (16 bits)
        .channels = 1      // Mono
    };

    // Allocation mémoire pour les données audio
    wave.data = malloc(BUFFER_SIZE * sizeof(short));
    if (wave.data == NULL) {
        printf("Erreur d'allocation mémoire.\n");
        CloseAudioDevice();
        CloseWindow();
        return EXIT_FAILURE;
    }

    short* buffer = (short*)wave.data;

    // Paramètre de l'onde
    float frequency = -1.0f;         // Pas de note par défaut
    WaveType waveType = SINUS;        // Type d'onde par défaut (Onde Sinusoïdale)

    // Générer l'onde sans la jouer
    GenerateWave(buffer, waveType, 440.0f);  // Générer une onde initiale (A440)

    Sound sound = LoadSoundFromWave(wave);
    StopSound(sound);  // On arrête le son par défaut

    // Boucle principale de l'application
    while (!WindowShouldClose()) {
        // Détection des touches du clavier pour changer la fréquence
        int key = GetKeyPressed();
        if (key != 0) {
            frequency = GetFrequencyFromKey(key);
            if (frequency > 0.0f) {
                GenerateWave(buffer, waveType, frequency);  // Générer la nouvelle onde
                StopSound(sound);  // Arrêter l'ancienne note
                sound = LoadSoundFromWave(wave);  // Charger la nouvelle note
                PlaySound(sound);  // Jouer la nouvelle note
            }
        }

        // Dessiner l'interface utilisateur
        BeginDrawing();
            ClearBackground(BLACK);

            DrawText("Synthétiseur", 250, 50, 40, PURPLE); 
            DrawText("Appuyez sur les touches pour jouer des notes", 180, 100, 20, WHITE);

            // Affichage de la fréquence actuelle (ou message si aucune touche pressée)
            if (frequency > 0.0f) {
                char frequencyText[100];
                snprintf(frequencyText, sizeof(frequencyText), "Fréquence: %.2f Hz", frequency);
                DrawText(frequencyText, 250, 150, 20, GREEN);
            } else {
                DrawText("Aucune note jouée", 250, 150, 20, RED);
            }
        EndDrawing();
    }

    // Nettoyage et fermeture
    UnloadSound(sound);
    free(wave.data);       
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
