#include "vex.h"

using namespace vex;
using namespace cchs;

vex::brain Brain;

// Motors
competition Competition;
controller Controller;
menu Menu(Competition, Controller);
motor RightBack(PORT13, ratio18_1, true);
motor LeftBack(PORT14, ratio18_1, false);
motor RightFront(PORT12, ratio18_1, true);
motor LeftFront(PORT11, ratio18_1, false);
motor Intake(PORT16, ratio18_1, true);
motor Lift(PORT18, ratio18_1, true);
motor_group LeftMotors(LeftBack, LeftFront);
motor_group RightMotors(RightBack, RightFront);
digital_out mogomech(Brain.ThreeWirePort.A);

bool mogomechsnap=false;


// Arcade Drive
void driver () {
 
 while (true) {

         RightBack.spin(reverse, Controller.Axis3.value() - Controller.Axis1.value(), percent);
         RightFront.spin(reverse, Controller.Axis3.value() - Controller.Axis1.value(), percent);
         LeftBack.spin(reverse, Controller.Axis3.value() + Controller.Axis1.value(), percent);
         LeftFront.spin(reverse, Controller.Axis3.value() + Controller.Axis1.value(), percent);
         wait(20, msec);

 }
  
}


void IntakeSpinFwd() {
       Intake.spin(fwd, 1000, pct);
}

void IntakeStop() {
       {Intake.stop();}
}

void IntakeSpinRev() {
       Intake.spin(reverse, 1000, pct);
}

void Liftfwd() {
       Lift.spin(reverse, 1000, pct);
}

void Liftrev() {
       Lift.spin(forward, 1000, pct);
}

void LiftStop() {
       {Lift.stop();}

}


void mogomechrelease() {
       
        mogomechsnap =! mogomechsnap;
        mogomech.set(mogomechsnap);
              
}


void nearauton() {
       IntakeSpinFwd();
       move(25);
       IntakeStop();
       turn(180);
       mogomech.set(true);
       move(-10);
       Liftfwd();
       wait(3, sec); 
       mogomech.set(false);
}


void farauton() {
       move(-15);
       mogomech.set(true);
       turn(-145);
       move(12);
       IntakeSpinFwd();
       wait(3, sec);
       IntakeStop();
       Liftfwd();
       wait(3, sec);
       LiftStop();
       mogomech.set(false);
}      



// Driving
void move (int distance, int speed = 100, double timeout =3) {
       double t = distance / 12.556;
       LeftMotors.setVelocity(speed, percent);
       LeftMotors.setStopping(brake);
       LeftMotors.setTimeout(timeout, seconds);
       LeftMotors.spinFor(t, turns, false);
       RightMotors.setVelocity(speed, percent);
       RightMotors.setStopping(brake);
       RightMotors.setTimeout(timeout, seconds);
       RightMotors.spinFor(t, turns);
}


void turn (double degrees, int speed = 75, double timeout=1.5) {
       double t = degrees / 360 * M_1_PI * 12.5 / 12.556;
       LeftMotors.setVelocity(speed, percent);
       LeftMotors.setStopping(brake);
       LeftMotors.setTimeout(timeout, seconds);
       LeftMotors.spinFor(t, turns, false);
       RightMotors.setVelocity(speed, percent);
       RightMotors.setStopping(brake);
       RightMotors.setTimeout(timeout, seconds);
       RightMotors.spinFor(reverse, t, turns);
}




// Controls
int main() {

  Competition.drivercontrol(driver);
  Controller.ButtonR2.pressed(IntakeSpinFwd);      
  Controller.ButtonR1.pressed(IntakeSpinRev);
  Controller.ButtonR2.released(IntakeStop);
  Controller.ButtonR1.released(IntakeStop);
  Controller.ButtonY.pressed(mogomechrelease);
  Controller.ButtonL1.pressed(Liftfwd);
  Controller.ButtonL2.pressed(Liftrev);
  Controller.ButtonL1.released(LiftStop);
  Controller.ButtonL2.released(LiftStop);
  
  Menu.registerAuton("Near", nearauton);
  Menu.registerAuton("Far", farauton);
}

