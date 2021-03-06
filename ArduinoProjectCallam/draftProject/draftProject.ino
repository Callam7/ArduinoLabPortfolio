//This was my original idea, but after much trial and error, as well as adding more parts to the mix
//, it just did not seem possible to produce this version of the project in a timely mannor


#include <Wire.h>
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#define Wire.write(x) Wire.send(x)
#define Wire.read() Wire.receive()
#endif
static uint8_t nunchuck_buf[6];   // array to store nunchuck data,
// Uses port C (analog in) pins as power & ground for Nunchuck
static void nunchuck_setpowerpins() {
#define pwrpin PORTC3
#define gndpin PORTC2
 DDRC |= _BV(pwrpin) | _BV(gndpin);
 PORTC &= ~ _BV(gndpin);
 PORTC |=  _BV(pwrpin);
 delay(100);  // wait for things to stabilize
}
// initialize the I2C system, join the I2C bus,
// and tell the nunchuck we're talking to it
static void nunchuck_init() {
 Wire.begin();                // join i2c bus as master
 Wire.beginTransmission(0x52);// transmit to device 0x52
#if (ARDUINO >= 100)
 Wire.write((uint8_t)0x40);// sends memory address
 Wire.write((uint8_t)0x00);// sends sent a zero.
#else
 Wire.send((uint8_t)0x40);// sends memory address
 Wire.send((uint8_t)0x00);// sends sent a zero.
#endif
 Wire.endTransmission();// stop transmitting
}
// Send a request for data to the nunchuck
// was "send_zero()"
static void nunchuck_send_request() {
 Wire.beginTransmission(0x52);// transmit to device 0x52
#if (ARDUINO >= 100)
 Wire.write((uint8_t)0x00);// sends one byte
#else
 Wire.send((uint8_t)0x00);// sends one byte
#endif
 Wire.endTransmission();// stop transmitting
}
// Encode data to format that most wiimote drivers except
// only needed if you use one of the regular wiimote drivers
static char nunchuk_decode_byte (char x) {
 x = (x ^ 0x17) + 0x17;
 return x;
}
// Receive data back from the nunchuck,
// returns 1 on successful read. returns 0 on failure
static int nunchuck_get_data() {
 int cnt = 0;
 Wire.requestFrom (0x52, 6);// request data from nunchuck
 while (Wire.available ()) {
   // receive byte as an integer
#if (ARDUINO >= 100)
   nunchuck_buf[cnt] = nunchuk_decode_byte( Wire.read() );
#else
   nunchuck_buf[cnt] = nunchuk_decode_byte( Wire.receive() );
#endif
   cnt++;
 }
 nunchuck_send_request();  // send request for next data payload
 // If we recieved the 6 bytes, then go print them
 if (cnt >= 5) {
   return 1;   // success
 }
 return 0; //failure
}
// Print the input data we have recieved
// accel data is 10 bits long
// so we read 8 bits, then we have to add
// on the last 2 bits.  That is why I
// multiply them by 2 * 2
static void nunchuck_print_data() {
 static int i = 0;
 int joy_x_axis = nunchuck_buf[0];
 int joy_y_axis = nunchuck_buf[1];
 int accel_x_axis = nunchuck_buf[2]; // * 2 * 2;
 int accel_y_axis = nunchuck_buf[3]; // * 2 * 2;
 int accel_z_axis = nunchuck_buf[4]; // * 2 * 2;
 int z_button = 0;
 int c_button = 0;
 // byte nunchuck_buf[5] contains bits for z and c buttons
 // it also contains the least significant bits for the accelerometer data
 // so we have to check each bit of byte outbuf[5]
 if ((nunchuck_buf[5] >> 0) & 1)
   z_button = 1;
 if ((nunchuck_buf[5] >> 1) & 1)
   c_button = 1;
 if ((nunchuck_buf[5] >> 2) & 1)
   accel_x_axis += 1;
 if ((nunchuck_buf[5] >> 3) & 1)
   accel_x_axis += 2;
 if ((nunchuck_buf[5] >> 4) & 1)
   accel_y_axis += 1;
 if ((nunchuck_buf[5] >> 5) & 1)
   accel_y_axis += 2;
 if ((nunchuck_buf[5] >> 6) & 1)
   accel_z_axis += 1;
 if ((nunchuck_buf[5] >> 7) & 1)
   accel_z_axis += 2;
 Serial.print(i, DEC);
 Serial.print("\t");
 Serial.print("joy:");
 Serial.print(joy_x_axis, DEC);
 Serial.print(",");
 Serial.print(joy_y_axis, DEC);
 Serial.print("  \t");
 Serial.print("acc:");
 Serial.print(accel_x_axis, DEC);
 Serial.print(",");
 Serial.print(accel_y_axis, DEC);
 Serial.print(",");
 Serial.print(accel_z_axis, DEC);
 Serial.print("\t");
 Serial.print("but:");
 Serial.print(z_button, DEC);
 Serial.print(",");
 Serial.print(c_button, DEC);
 Serial.print("\r\n");  // newline
 i++;
}
// returns zbutton state: 1=pressed, 0=notpressed
static int nunchuck_zbutton() {
 return ((nunchuck_buf[5] >> 0) & 1) ? 0 : 1;  // voodoo
}
// returns zbutton state: 1=pressed, 0=notpressed
static int nunchuck_cbutton() {
 return ((nunchuck_buf[5] >> 1) & 1) ? 0 : 1;  // voodoo
}
// returns value of x-axis joystick
static int nunchuck_joyx() {
 return nunchuck_buf[0];
}
// returns value of y-axis joystick
static int nunchuck_joyy() {
 return nunchuck_buf[1];
}
// returns value of x-axis accelerometer
static int nunchuck_accelx() {
 return nunchuck_buf[2];   // FIXME: this leaves out 2-bits of the data
}
// returns value of y-axis accelerometer
static int nunchuck_accely() {
 return nunchuck_buf[3];   // FIXME: this leaves out 2-bits of the data
}
// returns value of z-axis accelerometer
static int nunchuck_accelz() {
 return nunchuck_buf[4];   // FIXME: this leaves out 2-bits of the data
}
int loop_cnt = 0;
byte joyx, joyy, zbut, cbut, accx, accy, accz;
void _print() {
 Serial.print("\tX Joy:  ");
 Serial.print(map(joyx, 15, 221, 0, 255));
 Serial.print("\tY Joy:  ");
 Serial.println(map(joyy, 29, 229, 0, 255));
}
int joyx1 = 129;     //  15 - 221
int joyy1 = 124;     //  29 - 229
void setup() {
 Serial.begin(9600);
 nunchuck_setpowerpins();
 nunchuck_init(); // send the initilization handshake
 Serial.println("Wii Nunchuck Ready");
 pinMode(13, OUTPUT);
 pinMode(12, OUTPUT);
 pinMode(9, OUTPUT);
 pinMode(8, OUTPUT);

 //type();
}
void loop() {
 if ( loop_cnt > 10 ) { // every 100 msecs get new data
   loop_cnt = 0;
   nunchuck_get_data();
   zbut = nunchuck_zbutton();
   joyx = nunchuck_joyx();     //  15 - 221
   joyy = nunchuck_joyy();     //  29 - 229
   _print();
 }
 loop_cnt++;
 if (zbut == 1) {
   type();
   zbut = 0;
 }
 else {
   if (joyx > (joyx1 + 20)) {
     int speed1 = map(joyx - joyx1, 0, 80, 40, 255);
     speed1 = constrain(speed1, 0, 255);
     analogWrite(13, 0);
     analogWrite(12, speed1);
   }
   else if (joyx < (joyx1 - 20)) {
     int speed2 = map(joyx1 - joyx, 0, 90, 40, 255);
     speed2 = constrain(speed2, 0, 255);
     analogWrite(13, speed2);
     analogWrite(12, 0);
   }
   else {
     analogWrite(13, 0);
     analogWrite(12, 0);
   }
   if (joyy > (joyy1 + 20)) {
     int speed3 = map(joyy - joyy1, 0, 80, 40, 255);
     speed3 = constrain(speed3, 0, 255);
     analogWrite(9, 0);
     analogWrite(8, speed3);
   }
   else if (joyy < (joyy1 - 20)) {
     int speed4 = map(joyy1 - joyy, 0, 90, 40, 255);
     speed4 = constrain(speed4, 0, 255);
     analogWrite(9, speed4);
     analogWrite(8, 0);
   }
   else {
     analogWrite(9, 0);
     analogWrite(8, 0);
   }
 }
 delay(1);
}
void type() {
 int rltime = 200;
 digitalWrite(9, 1);//origin
 digitalWrite(13, 0);
 digitalWrite(8, 1);
 digitalWrite(12, 0);
 delay(1000);
 //H==============
 digitalWrite(8, 0);//wait
 digitalWrite(9, 0);
 digitalWrite(12, 0);
 digitalWrite(13, 0);
 delay(250);
 digitalWrite(8, 0);//up
 digitalWrite(9, 1);
 delay(500);
 digitalWrite(8, 0);//wait
 digitalWrite(9, 0);
 digitalWrite(12, 0);
 digitalWrite(13, 0);
 delay(250);
 digitalWrite(8, 1);//down
 digitalWrite(9, 0);
 delay(250);
 digitalWrite(8, 0);//wait
 digitalWrite(9, 0);
 digitalWrite(12, 0);
 digitalWrite(13, 0);
 delay(250);
 digitalWrite(12, 0);//right
 digitalWrite(13, 1);
 delay(rltime);
 digitalWrite(8, 0);//wait
 digitalWrite(9, 0);
 digitalWrite(12, 0);
 digitalWrite(13, 0);
 delay(250);
 digitalWrite(8, 0);//up
 digitalWrite(9, 1);
 delay(250);
 digitalWrite(8, 0);//wait
 digitalWrite(9, 0);
 digitalWrite(12, 0);
 digitalWrite(13, 0);
 delay(250);
 digitalWrite(8, 1);//down
 digitalWrite(9, 0);
 delay(500);
 digitalWrite(8, 0);//wait
 digitalWrite(9, 0);
 digitalWrite(12, 0);
 digitalWrite(13, 0);
 delay(250);
 digitalWrite(12, 0);//right
 digitalWrite(13, 1);
 delay(rltime);
 //I========================
 digitalWrite(8, 0);//wait
 digitalWrite(9, 0);
 digitalWrite(12, 0);
 digitalWrite(13, 0);
 delay(250);
 digitalWrite(8, 0);//up
 digitalWrite(9, 1);
 delay(500);
 digitalWrite(8, 0);//wait
 digitalWrite(9, 0);
 digitalWrite(12, 0);
 digitalWrite(13, 0);
 delay(250);
 digitalWrite(12, 0);//right
 digitalWrite(13, 1);
 delay(100);
 digitalWrite(8, 0);//wait
 digitalWrite(9, 0);
 digitalWrite(12, 0);
 digitalWrite(13, 0);
 delay(250);
 digitalWrite(8, 1);//left
 digitalWrite(13, 0);
 delay(rltime);
 digitalWrite(8, 0);//wait
 digitalWrite(9, 0);
 digitalWrite(12, 0);
 digitalWrite(13, 0);
 delay(250);
 digitalWrite(12, 0);//right
 digitalWrite(13, 1);
 delay(100);
 digitalWrite(8, 0);//wait
 digitalWrite(9, 0);
 digitalWrite(12, 0);
 digitalWrite(13, 0);
 delay(250);
 digitalWrite(8, 1);//down
 digitalWrite(9, 0);
 delay(500);
 digitalWrite(8, 0);//wait
 digitalWrite(9, 0);
 digitalWrite(12, 0);
 digitalWrite(13, 0);
 delay(250);
 digitalWrite(9, 0);//right
 digitalWrite(13, 1);
 delay(100);
 digitalWrite(8, 0);//wait
 digitalWrite(9, 0);
 digitalWrite(12, 0);
 digitalWrite(13, 0);
 delay(250);
 digitalWrite(12, 1);//left
 digitalWrite(13, 0);
 delay(rltime);
 digitalWrite(8, 0);//wait
 digitalWrite(9, 0);
 digitalWrite(12, 0);
 digitalWrite(13, 0);
 delay(250);
}
