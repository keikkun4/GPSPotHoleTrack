  #ifndef _BOTONES_H_
  #define _BOTONES_H_
  
  class BUTTON {
  public:
    BUTTON(int pin);
    void begin();
    void read();
    bool pressed();
  private:
    const int nPin;
    bool state_ant;
    bool state_act;
  };
  
  #endif //_BOTONES_H_

