// The safeMotoControlPWM class performs validation on all motor control commands
// This class is specific to this project (to some extent)

#define PWM_FREQ 20000
#define PWM_RES 8

class safeMotorControlPWM{
protected:
  bool set_LMF = false;
  bool set_LMR = false;
  bool set_RMF = false;
  bool set_RMR = false;
  int LMF_DUTY = -1;
  int LMR_DUTY = -1;
  int RMF_DUTY = -1;
  int RMR_DUTY = -1;
  int LMF_PIN = -1;
  int LMR_PIN = -1;
  int RMF_PIN = -1;
  int RMR_PIN = -1;
  int LMF_CHANNEL = -1;
  int LMR_CHANNEL = -1;
  int RMF_CHANNEL = -1;
  int RMR_CHANNEL = -1;

  // Set all the set vars to false *doesnt update the pins*
  void zeroAllVariables(){
    this->set_LMF = false;
    this->LMF_DUTY = 0;
    this->set_LMR = false;
    this->LMR_DUTY = 0;
    this->set_RMF = false;
    this->RMF_DUTY = 0;
    this->set_RMR = false;
    this->RMR_DUTY = 0;
  }

  // Verify that pin settings wont blow shit up
  bool validatePinSettings(){
    // Verify write to pins:
    // (Ensure that we arent trying to run either motor forward and reverse at the same time)
    if(this->set_LMF && this->set_LMR){ // Left motor forward and reverse at the same time
      this->zeroAllVariables(); // Zero all the variables to prevent anything going wrong
      return false;
    }
    if(this->set_RMF && this->set_RMR){ // Right motor forward and reverse at the same time
      this->zeroAllVariables(); // Zero all the variables to prevent anything going wrong
      return false;
    }
    // Verify duty cycles are between 0 and 255:
    // Error wont be returned here, the duties will just be corrected
    // (This may not be required at all, the ledcWrite function probably does this already, but I would rather be safe)
    this->LMF_DUTY = constrain(this->LMF_DUTY, 0, 255); // This macro is part of arduino.h
    this->LMR_DUTY = constrain(this->LMR_DUTY, 0, 255);
    this->RMF_DUTY = constrain(this->RMF_DUTY, 0, 255);
    this->RMR_DUTY = constrain(this->RMR_DUTY, 0, 255);
    return true;
  }

  /*
    validateSetMotors provides a safe way to set motor states, if you try to set the pins in an unsafe way it just wont do it :)
  */
  bool validateSetMotors(){
    if(!validatePinSettings()){
      return false;
    }else{
      digitalWrite(this->LMF_PIN, this->set_LMF?HIGH:LOW);
      ledcWrite(this->LMF_CHANNEL, this->LMF_DUTY);
      digitalWrite(this->LMR_PIN, this->set_LMR?HIGH:LOW);
      ledcWrite(this->LMR_CHANNEL, this->LMR_DUTY);
      digitalWrite(this->RMF_PIN, this->set_RMF?HIGH:LOW);
      ledcWrite(this->RMF_CHANNEL, this->RMF_DUTY);
      digitalWrite(this->RMR_PIN, this->set_RMR?HIGH:LOW);
      ledcWrite(this->RMR_CHANNEL, this->RMR_DUTY);
    }
    return true;
  }

  // Zero the variables, then set the pins
  void zeroSetVars(){
    this->zeroAllVariables();
    this->validateSetMotors();
  }

public:

  safeMotorControlPWM(int LMF_PIN_NUM, int LMR_PIN_NUM, int RMF_PIN_NUM, int RMR_PIN_NUM,
                      int LMF_PWM_CHANNEL=0, int LMR_PWM_CHANNEL=1, int RMF_PWM_CHANNEL=2, int RMR_PWM_CHANNEL=3){
    // Obviously this class can only save you if you set the pin numbers correctly! Always check them :)
    this->LMF_PIN = LMF_PIN_NUM;
    this->LMR_PIN = LMR_PIN_NUM;
    this->RMF_PIN = RMF_PIN_NUM;
    this->RMR_PIN = RMR_PIN_NUM;
    this->LMF_CHANNEL = LMF_PWM_CHANNEL;
    this->LMR_CHANNEL = LMR_PWM_CHANNEL;
    this->RMF_CHANNEL = RMF_PWM_CHANNEL;
    this->RMR_CHANNEL = RMR_PWM_CHANNEL;
    // Set the pins to output:
    pinMode(this->LMF_PIN, OUTPUT);
    pinMode(this->LMR_PIN, OUTPUT);
    pinMode(this->RMF_PIN, OUTPUT);
    pinMode(this->RMR_PIN, OUTPUT);
    // Create 4 PWM channels
    ledcSetup(this->LMF_CHANNEL, PWM_FREQ, PWM_RES);
    ledcSetup(this->LMR_CHANNEL, PWM_FREQ, PWM_RES);
    ledcSetup(this->RMF_CHANNEL, PWM_FREQ, PWM_RES);
    ledcSetup(this->RMR_CHANNEL, PWM_FREQ, PWM_RES);
    // Attach the PWM channels to the pins
    ledcAttachPin(this->LMF_PIN, this->LMF_CHANNEL);
    ledcAttachPin(this->LMR_PIN, this->LMR_CHANNEL);
    ledcAttachPin(this->RMF_PIN, this->RMF_CHANNEL);
    ledcAttachPin(this->RMR_PIN, this->RMR_CHANNEL);
    // Zero everything
    this->zeroSetVars();
  }
  ~safeMotorControlPWM(){
    this->zeroSetVars();
  }

  void driveForward(int dutyCycle){
    this->zeroSetVars();
    this->set_LMF = true;
    this->set_RMF = true;
    this->LMF_DUTY = dutyCycle;
    this->RMF_DUTY = dutyCycle;
    this->validateSetMotors();
  }

  void driveBackward(int dutyCycle){
    this->zeroSetVars();
    this->set_LMR = true;
    this->set_RMR = true;
    this->LMR_DUTY = dutyCycle;
    this->RMR_DUTY = dutyCycle;
    this->validateSetMotors();
  }

  void turnLeft(int dutyCycle){
    this->zeroSetVars();
    set_LMR = true;
    set_RMF = true;
    this->LMR_DUTY = dutyCycle;
    this->RMF_DUTY = dutyCycle;
    this->validateSetMotors();
  }

  void turnRight(int dutyCycle){
    this->zeroSetVars();
    set_LMF = true;
    set_RMR = true;
    this->LMF_DUTY = dutyCycle;
    this->RMR_DUTY = dutyCycle;
    this->validateSetMotors();
  }

  void stop(){
    this->zeroSetVars();
  }

};