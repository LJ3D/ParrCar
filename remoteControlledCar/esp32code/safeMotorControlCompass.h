#include "safeMotorControl.h"
#include <QMC5883LCompass.h>

class safeMotorControlCompass : public safeMotorControl{
protected:
QMC5883LCompass* compass;

public:
    safeMotorControlCompass(int LMF_PIN, int LMR_PIN, int RMF_PIN, int RMR_PIN, QMC5883LCompass* compass) : safeMotorControl(LMF_PIN, LMR_PIN, RMF_PIN, RMR_PIN){
        // Obviously this class can only save you if you set the pin numbers correctly! Always check them :)
        this->LMF_PIN = LMF_PIN;
        this->LMR_PIN = LMR_PIN;
        this->RMF_PIN = RMF_PIN;
        this->RMR_PIN = RMR_PIN;
        this->compass = compass;
        pinMode(this->LMF_PIN, OUTPUT);
        pinMode(this->LMR_PIN, OUTPUT);
        pinMode(this->RMF_PIN, OUTPUT);
        pinMode(this->RMR_PIN, OUTPUT);
        this->zeroSetVars();
    }

    void turnToBearing(int bearing){
        this->stop();
        int currentBearing = this->compass->getAzimuth();
        while(currentBearing != bearing){
            this->turnRight();
            currentBearing = this->compass->getAzimuth();
        }
        this->stop();
    }
};