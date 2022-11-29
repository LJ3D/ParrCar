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

  // Set all the set vars to false *doesnt update the pins*
  void zeroAllVariables(){
    this->set_LMF = false;
    this->set_LMR = false;
    this->set_RMF = false;
    this->set_RMR = false;
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
    validateSetMotors provides a safe way to set motor states, if you try to set the pins in an unsafe way it just wont do it :)
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

  // Zero the variables, then set the pins
  void zeroSetVars(){
    this->zeroAllVariables();
    this->validateSetMotors();
  }

public:

  safeMotorControl(int LMF_PIN, int LMR_PIN, int RMF_PIN, int RMR_PIN){
    // Obviously this class can only save you if you set the pin numbers correctly! Always check them :)
    this->LMF_PIN = LMF_PIN;
    this->LMR_PIN = LMR_PIN;
    this->RMF_PIN = RMF_PIN;
    this->RMR_PIN = RMR_PIN;
    pinMode(this->LMF_PIN, OUTPUT);
    pinMode(this->LMR_PIN, OUTPUT);
    pinMode(this->RMF_PIN, OUTPUT);
    pinMode(this->RMR_PIN, OUTPUT);
    this->zeroSetVars();
  }
  ~safeMotorControl(){
    this->zeroSetVars();
  }

  void driveForward(){
    this->zeroSetVars();
    set_LMF = true;
    set_RMF = true;
    this->validateSetMotors();
  }

  void driveBackward(){
    this->zeroSetVars();
    set_LMR = true;
    set_RMR = true;
    this->validateSetMotors();
  }

  void turnLeft(){
    this->zeroSetVars();
    set_LMR = true;
    set_RMF = true;
    this->validateSetMotors();
  }

  void turnRight(){
    this->zeroSetVars();
    set_LMF = true;
    set_RMR = true;
    this->validateSetMotors();
  }

  void stop(){
    this->zeroSetVars();
  }

};