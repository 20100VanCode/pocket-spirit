#include <Arduino.h>
#include <PocketSpirit.h>

using namespace PocketSpirit;

PocketSpirit::PocketSpirit app;

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("=== Pocket Spirit: Lumy ===");

    if (!app.begin("Lumy")) {
        Serial.println("Failed to initialize PocketSpirit!");
        while (1) delay(1000);
    }

    app.onEvolve([](EvolutionStage stage) {
        Serial.printf("Lumy evolved to: %s\n", evolutionStageToStr(stage));
    });

    app.onStatsLow([]() {
        Serial.println("Lumy needs attention!");
    });

    Serial.println("Lumy is alive!");
}

void loop() {
    app.update();
    delay(5);
}
