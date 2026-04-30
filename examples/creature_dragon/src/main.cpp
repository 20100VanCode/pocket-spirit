#include <Arduino.h>
#include <PocketSpirit.h>

using namespace PocketSpirit;

PocketSpirit::PocketSpirit app;

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("=== Pocket Spirit: Dragon ===");

    if (!app.begin("Dragon")) {
        Serial.println("Failed to initialize PocketSpirit!");
        while (1) delay(1000);
    }

    app.onEvolve([](EvolutionStage stage) {
        Serial.printf("Dragon evolved to: %s\n", evolutionStageToStr(stage));
    });

    app.onInteraction([](const char* type) {
        Serial.printf("Interaction: %s\n", type);
    });

    Serial.println("Dragon is alive!");
}

void loop() {
    app.update();
    delay(5);
}
