#include <stdbool.h>



//Declarations for ctr_functions.c
void initPorts(void);

//LED controlling
void setPin(uint8_t cathodeState, uint8_t anodeState, uint8_t LEDNumber);
void checkAndBlinkLEDs(uint8_t loopTimes, uint8_t period_us);


//Image controlling
void lightWaveAll(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void shutWaveAll(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);

void lightLine(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void shutLine(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);

void LightWaves(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);

void lightVertical(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void shutVertical(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void lightHorizonta(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void shutHorizonta(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);

void lightCenter(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void shutCenter(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void lightBorder(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void shutBorder(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);

void lightBorderHorizontal(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void shutBorderHorizontal(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);

void lightDownAndUp(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void shutDownAndUp(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void lightMiddleToTop(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void shutMiddleToTop(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void lightMiddleToCenter(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void shutMiddleToCenter(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);

//Images

void lightShutWaveAll(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void lightShutLineWave(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void lightShutHorizontalVertical(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);

void lightShutBorderOrCenter(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);
void lightShutBorderAndCenter(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);

void lightShutLineWavesMix(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);







