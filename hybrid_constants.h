//hybrid constants


//Servo Open Values
static const uint8_t SERVO_CLOSED = 45;
static const uint8_t SERVO_OPEN = 150;

//Solenoid Open Values
static const uint8_t SOL_OPEN = 0;
static const uint8_t SOL_CLOSED = 1;

//Fuse Open Values
static const uint8_t FUSE_OPEN = 0;
static const uint8_t FUSE_CLOSED = 1;

//Significant Values
static const uint16_t OVER_PRESSURE = 1000; //PSI

//Initial States
static const uint8_t FILL_INIT = 0;
static const uint8_t VENT_INIT = 0;
static const uint8_t SERVO_INIT = SERVO_CLOSED;

//Emergency Behavior
static const uint8_t VENT_EMER = 1;

//Timing
static const uint32_t SEND_TIME = 100;
static const uint32_t RECEIVE_TIME = 50;


static const uint8_t ANALOG_SAMPLES = 100;

