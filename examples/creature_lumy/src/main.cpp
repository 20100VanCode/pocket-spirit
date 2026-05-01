#include <Arduino.h>
#include <PocketSpirit.h>

using namespace PocketSpirit;

PocketSpirit::PocketSpirit app;

// ============================================
// LILYGO T-S3 BUTTON SUPPORT (No Touch Screen)
// ============================================
// Lilygo T-S3 GPIO pins:
#define BUTTON_1_PIN    0    // Boot button
#define BUTTON_2_PIN    14   // Second button (check your board - some use GPIO 21)

// Button states
bool btn1Pressed = false;
bool btn2Pressed = false;
unsigned long btn1PressTime = 0;
unsigned long btn2PressTime = 0;

// Debounce
const unsigned long DEBOUNCE_MS = 50;
const unsigned long LONG_PRESS_MS = 800;  // Hold for "play"

void initButtons() {
    pinMode(BUTTON_1_PIN, INPUT_PULLUP);
    pinMode(BUTTON_2_PIN, INPUT_PULLUP);
    Serial.println("Buttons initialized (GPIO 0 & 14)");
}

void handleButtons() {
    unsigned long now = millis();

    // Button 1 (Boot) - Pet / Play (long press)
    bool b1 = digitalRead(BUTTON_1_PIN) == LOW;
    if (b1 && !btn1Pressed && (now - btn1PressTime > DEBOUNCE_MS)) {
        btn1Pressed = true;
        btn1PressTime = now;
    } else if (!b1 && btn1Pressed) {
        unsigned long duration = now - btn1PressTime;
        btn1Pressed = false;

        if (duration > LONG_PRESS_MS) {
            // Long press = Play
            Serial.println("Button 1: PLAY (long press)");
            app.play();
        } else if (duration > DEBOUNCE_MS) {
            // Short press = Pet
            Serial.println("Button 1: PET");
            app.pet();
        }
    }

    // Button 2 - Feed / Toggle stats (long press)
    bool b2 = digitalRead(BUTTON_2_PIN) == LOW;
    if (b2 && !btn2Pressed && (now - btn2PressTime > DEBOUNCE_MS)) {
        btn2Pressed = true;
        btn2PressTime = now;
    } else if (!b2 && btn2Pressed) {
        unsigned long duration = now - btn2PressTime;
        btn2Pressed = false;

        if (duration > LONG_PRESS_MS) {
            // Long press = Toggle stats overlay
            Serial.println("Button 2: Toggle stats");
            static bool statsVisible = true;
            statsVisible = !statsVisible;
            app.showStats(statsVisible);
        } else if (duration > DEBOUNCE_MS) {
            // Short press = Feed
            Serial.println("Button 2: FEED");
            app.feed();
        }
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);  // Longer delay for serial to connect

    Serial.println("\n\n=== Pocket Spirit: Lumy (Lilygo T-S3) ===");
    Serial.println("Controls:");
    Serial.println("  Button 1 short: PET");
    Serial.println("  Button 1 long:  PLAY");
    Serial.println("  Button 2 short: FEED");
    Serial.println("  Button 2 long:  Toggle stats");
    Serial.println();

    initButtons();

    Serial.println("Starting PocketSpirit...");
    if (!app.begin("Lumy")) {
        Serial.println("ERROR: Failed to initialize PocketSpirit!");
        while (1) delay(1000);
    }
    Serial.println("PocketSpirit initialized OK");

    app.onEvolve([](EvolutionStage stage) {
        Serial.printf("Lumy evolved to: %s\n", evolutionStageToStr(stage));
    });

    app.onStatsLow([]() {
        Serial.println("Lumy needs attention!");
    });

    // Show stats overlay by default
    app.showStats(true);

    Serial.println("Lumy is alive!");
}

void loop() {
    handleButtons();
    app.update();
    delay(5);
}
