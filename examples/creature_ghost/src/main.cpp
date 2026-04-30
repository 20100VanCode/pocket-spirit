#include <Arduino.h>
#include <PocketSpirit.h>

using namespace PocketSpirit;

PocketSpirit::PocketSpirit app;

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("=== Pocket Spirit: Ghost ===");

    if (!app.begin("Ghost")) {
        Serial.println("Failed to initialize PocketSpirit!");
        while (1) delay(1000);
    }

    app.onEvolve([](EvolutionStage stage) {
        Serial.printf("Ghost evolved to: %s\n", evolutionStageToStr(stage));
    });

    app.onStatsLow([]() {
        Serial.println("Ghost is feeling down...");
    });

    Serial.println("Ghost is alive!");
}

void loop() {
    app.update();
    delay(5);
}
