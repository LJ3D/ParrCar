// The safeMotoControl class performs validation on all motor control commands
// This class is specific to this project
class safeMotorControl{
private:
  bool set_LMF = false;
  bool set_LMR = false;
  bool set_RMF = false;
  bool set_RMR = false;
  int LMF_PIN = -1;
  int LMR_PIN = -1;
  int RMF_PIN = -1;
  int RMR_PIN = -1;

  void writeZeroToAll(){
    digitalWrite(LMF_PIN, LOW);
    digitalWrite(LMR_PIN, LOW);
    digitalWrite(RMF_PIN, LOW);
    digitalWrite(RMR_PIN, LOW);
  }

  // Verify that pin settings wont blow shit up
  bool validatePinSettings(){
    if(this->set_LMF && this->set_LMR){ // Left motor forward and reverse at the same time
      this->zeroAllVariables(); // Zero all the variables to prevent anything going wrong
      return false;
    }
    if(this->set_RMF && this->set_RMR){ // Right motor forward and reverse at the same time
      this->zeroAllVariables(); // Zero all the variables to prevent anything going wrong
      return false;
    }
    return true;
  }

  /*
    validateSetMotors provides a safe way to set motor states, if you try to set the pins up in an unsafe way it just wont do it :)
  */
  bool validateSetMotors(){
    if(!validatePinSettings()){
      return false;
    }else{
      digitalWrite(this->LMF_PIN, this->set_LMF?HIGH:LOW);
      digitalWrite(this->LMR_PIN, this->set_LMR?HIGH:LOW);
      digitalWrite(this->RMF_PIN, this->set_RMF?HIGH:LOW);
      digitalWrite(this->RMR_PIN, this->set_RMR?HIGH:LOW);
    }
    return true;
  }


public:
  void zeroAllVariables(){
    set_LMF = false;
    set_LMR = false;
    set_RMF = false;
    set_RMR = false;
  }

  safeMotorControl(int LMF_PIN, int LMR_PIN, int RMF_PIN, int RMR_PIN){
    this->LMF_PIN = LMF_PIN;
    this->LMR_PIN = LMR_PIN;
    this->RMF_PIN = RMF_PIN;
    this->RMR_PIN = RMR_PIN;
    pinMode(this->LMF_PIN, OUTPUT);
    pinMode(this->LMR_PIN, OUTPUT);
    pinMode(this->RMF_PIN, OUTPUT);
    pinMode(this->RMR_PIN, OUTPUT);
    this->writeZeroToAll();
  }
  ~safeMotorControl(){
    this->zeroAllVariables();
    this->validateSetMotors();
  }

  void driveForward(){
    this->zeroAllVariables();
    this->validateSetMotors();
    set_LMF = true;
    set_RMF = true;
    this->validateSetMotors();
  }

  void driveBackward(){
    this->zeroAllVariables();
    this->validateSetMotors();
    set_LMR = true;
    set_RMR = true;
    this->validateSetMotors();
  }

  void turnLeft(){
    this->zeroAllVariables();
    this->validateSetMotors();
    set_LMR = true;
    set_RMF = true;
    this->validateSetMotors();
  }

  void turnRight(){
    this->zeroAllVariables();
    this->validateSetMotors();
    set_LMF = true;
    set_RMR = true;
    this->validateSetMotors();
  }

  void stop(){
    this->zeroAllVariables();
    this->validateSetMotors();
  }

};