#include "main.h"
using namespace okapi;

//Notes:
//This code is *not* meant to be for newer programmers. If you're new to programming, go back to VexCode ASAP
//If you are coming off of vexcode, note that PROS utilizes *regular* C++, not the modded version in VexCode
//Please consult with someone that knows PROS before flashing your code, just to ensure that you don't break your bot

//Motor definition carries over from autonomous.cpp

Controller controller;//Initializing controller here

ControllerButton tray_up(ControllerDigital::L1);
ControllerButton tray_down(ControllerDigital::L2);

ControllerButton intake_in(ControllerDigital::R2);
ControllerButton intake_out(ControllerDigital::R1);

void crash() {//Kill-switch function. Stops all motors.
	if (tray_up.isPressed() && tray_down.isPressed() && intake_in.isPressed()  && intake_out.isPressed()) {
		drivetrain.moveDistance(0);
		
		tray.moveVoltage(0);

		intake.moveVoltage(0);
	}
}


void opcontrol()  {//Your actual loop for the driver control

	tray.setBrakeMode(Motor::brakeMode::brake);

	intake.setBrakeMode(Motor::brakeMode::brake);

	while (true) {
		drivetrain.tank(controller.getAnalog(ControllerAnalog::leftY),//Controls the drivetrain as a tank chassis.
             controller.getAnalog(ControllerAnalog::rightY));
		if (tray_down.isPressed() && tray_up.isPressed()) {
			tray.moveVoltage(12100);
		} else if (tray_up.isPressed()) {
			tray.moveVoltage(4500);
		} else if (tray_down.isPressed()) {
			tray.moveVoltage(6100);
		} else {
			intake.moveVoltage(0);
		}

		if (intake_in.isPressed()) {//Controls for intake
			intake.moveVoltage(12100);
		} else if (intake_out.isPressed()) {
			intake.moveVoltage(12100);
		} else {
			intake.moveVoltage(0);
		}

		crash();//Calling the killswitch function
		pros::lcd::set_text(1,"For Honor and Glory.");

		pros::delay(2);//Slight delay to not significantly halt operations, but to conserve resources

	}
}
