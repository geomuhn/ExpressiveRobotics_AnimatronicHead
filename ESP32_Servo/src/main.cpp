#include <Arduino.h>
#include <ESP32Servo.h>
#include <Ps3Controller.h>

Servo LipsR;  // create servo object to control a servo
Servo LipsL;
Servo EyeV_R;
Servo EyeH_R;
Servo EyeV_L;
Servo EyeH_L;
Servo Neck_side;
Servo Neck_r;
Servo Neck_l;


int pos = 0;    // variable to store the servo position
int pos2 = 90;
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
// Possible PWM GPIO pins on the ESP32-S2: 0(used by on-board button),1-17,18(used by on-board LED),19-21,26,33-42

int servoPin12 = 12;
int servoPin14 = 14;
int servoPin27 = 27;
int servoPin25 = 25;
int servoPin33 = 33;
int servoPin32 = 32;
int servoPin15 = 15;
int servoPin4 = 4;
int servoPin16 = 16;
int lx;int ly;int rx;int ry;
int eyeLH_control;int eyeLV_control;int eyeRH_control;int eyeRV_control;
int up;int down;
int left_lip = 90;
int right_lip = 90;
int lipL_max = 120;
int lipL_min = 50;
int lipR_max = 140;
int lipR_min = 30;
int lipL_k = 0;
int lipR_k = 0;
int neck_deg = 90; int neckr_deg = 75; int neckl_deg = 100;
int neck_r_min = 130; int neck_r_max = 0;int neck_l_min = 40; int neck_l_max = 175;
int neck_yaw = 90;
int L1;int L2;
int R1;int R2;
int x_off = 0; int y_off = 0; int z_off = 0; 
int accel_init = 0;
float ax; float ay; float az;
float roll; float pitch; 
float z_left; float z_right;
int k = 100; //damping Coefficient Should be 100

int mode = 2;

void readController(){
	lx = Ps3.data.analog.stick.lx;
	ly = Ps3.data.analog.stick.ly;
	rx = Ps3.data.analog.stick.rx;
	ry = Ps3.data.analog.stick.ry;
	up = Ps3.data.button.up;
	down = Ps3.data.button.down;
	L1 = Ps3.data.button.l1;
	L2 = Ps3.data.button.l2;
	R1 = Ps3.data.button.r1;
	R2 = Ps3.data.button.r2;
	ax = Ps3.data.sensor.accelerometer.x;// + x_off;
	ay = Ps3.data.sensor.accelerometer.y;// + y_off;
	az = Ps3.data.sensor.accelerometer.z;// + z_off;
	
}

void eyecontrol(){
  eyeLH_control = map(rx,-128,128,0,40);
	eyeLV_control = map(ry,128,-128,0,40);
	eyeRH_control = map(rx,-128,128,0,40);
	eyeRV_control = map(ry,128,-128,0,40);
	EyeH_L.write(eyeLH_control);
	EyeV_L.write(eyeLV_control);
	EyeH_R.write(eyeRH_control);
	EyeV_R.write(eyeRV_control);
  Serial.print(eyeLH_control);Serial.print(','); 
  Serial.print(eyeLV_control);Serial.print(',');
  Serial.print(eyeRH_control);Serial.print(',');  
  Serial.print(eyeRV_control);//Serial.print(','); 

}

// void neckcontrol(){
//   roll = atan(ay / sqrt(pow(ax, 2) + pow(az, 2)))/2;// * 180 / PI;
//   pitch = atan(-1 * ax / sqrt(pow(ay, 2) + pow(az, 2)))/2;// * 180 / PI;
// 	roll = roll + 0.09;
// 	pitch = pitch - 0.04;
// 	z_left =  -(7*sin(pitch))/2 - (5*cos(pitch)*sin(roll))/2;
// 	z_right =  (7*sin(pitch))/2 - (5*cos(pitch)*sin(roll))/2;
// 	neckl_deg = map(z_left*k,-1.5*k,1.5*k,neck_l_min,neck_l_max);
// 	neckr_deg = map(z_right*k,-1.5*k,1.5*k,neck_r_min,neck_r_max);
// 	neck_yaw = map(lx*k,-128*k,128*k,130,50);
// 	Neck_side.write(neck_yaw);
// 	if (neckl_deg>=neck_l_min && neckl_deg<neck_l_max){
// 		Neck_l.write(neckl_deg);
// 		//Serial.print(neckl_deg); Serial.print('\t');
// 		}
// 	if (neckr_deg>=neck_r_max && neckr_deg<neck_r_min){
// 		Neck_r.write(neckr_deg);
// 		//Serial.println(neckr_deg);
// 		}
//   Serial.print(neckl_deg);Serial.print(',');
//   Serial.print(neckr_deg);Serial.print(','); 
// 	Serial.print(neck_yaw);//Serial.print(','); 
// }

// void lipcontrol(){
//   if (R1){
//     if(lipL_k<200){
//       lipL_k++;
//       left_lip = map(lipL_k,-200,200,lipL_min,lipL_max);
//       LipsL.write(left_lip);
//     }
//     if(lipR_k>-200){
//       lipR_k--;
// 			right_lip = map(lipR_k,-200,200,lipR_min,lipR_max);
// 			LipsR.write(right_lip);
// 			}
//   }else if(L1){
//     if(lipL_k>-200){
//       lipL_k--;
//       left_lip = map(lipL_k,-200,200,lipL_min,lipL_max);
// 			LipsL.write(left_lip);
//     }
//     if(lipR_k<200){
// 			lipR_k++;
// 			right_lip = map(lipR_k,-200,200,lipR_min,lipR_max);
// 			LipsR.write(right_lip);
// 			}
//   }
//   Serial.print(left_lip);Serial.print(','); 
//   Serial.print(right_lip);

// }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Ps3.begin("50:63:13:5f:f2:e2");
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3); 
	LipsR.setPeriodHertz(50);    // standard 50 hz servo
	LipsL.setPeriodHertz(50);
	EyeH_L.setPeriodHertz(50);
	EyeH_R.setPeriodHertz(50);
	EyeV_L.setPeriodHertz(50);
	// Neck_side.setPeriodHertz(50);
	// Neck_l.setPeriodHertz(50);
	// Neck_r.setPeriodHertz(50);
	LipsR.attach(servoPin12, 500, 2500); // attaches the servo on pin 18 to the servo object
	LipsL.attach(servoPin14,500, 2500);
	EyeH_L.attach(servoPin27, 500, 2500);
	EyeV_L.attach(servoPin25, 500, 2500);
	EyeH_R.attach(servoPin33, 500, 2500);
	EyeV_R.attach(servoPin32, 500, 2500);
	// Neck_side.attach(servoPin15,771,2740);
	// Neck_l.attach(servoPin4,771,2740);
	// Neck_r.attach(servoPin16,771,2740);
  pinMode(LED_BUILTIN,OUTPUT);

  LipsR.write(90);
	LipsL.write(90);
	// Neck_side.write(neck_yaw);
	// Neck_l.write(neckl_deg);
	// Neck_r.write(neckr_deg);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN,LOW);
  while(Ps3.isConnected()){
	if (Ps3.data.button.ps){Ps3.end();}
  readController();
  digitalWrite(BUILTIN_LED,HIGH);
	//Serial.print(ax); Serial.print('\t'); Serial.print(ay); Serial.print('\t'); Serial.print(az); Serial.print('\t');
  switch (mode)
  {
  case 1:
    /* code */
    //neckcontrol();
    break;
  case 2:
    /* code */
    //eyecontrol();
  	eyeLH_control = map(rx,-128,128,0,40);
  	eyeLV_control = map(ry,128,-128,0,40);
  	eyeRH_control = map(rx,-128,128,0,40);
  	eyeRV_control = map(ry,128,-128,0,40);
  	// EyeH_L.write(eyeLH_control);
  	// EyeV_L.write(eyeLV_control);
  	// EyeH_R.write(eyeRH_control);
  	// EyeV_R.write(eyeRV_control);
    
  	Serial.print(eyeLH_control);Serial.print(','); 
  	Serial.print(eyeLV_control);Serial.print(',');
  	Serial.print(eyeRH_control);Serial.print(',');  
  	Serial.print(eyeRV_control);//Serial.print(','); 
    break;
  case 3:
    /* code */
    //lipcontrol();
    break;
case 4:
	if (Ps3.data.button.ps){Ps3.end();}
	//neckcontrol();
	eyecontrol();
	//lipcontrol();
break;
  default:
    break;
  }
  Serial.println(); 
  }
}