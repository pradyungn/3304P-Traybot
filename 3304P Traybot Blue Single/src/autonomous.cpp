#include "main.h"//Setup
using namespace okapi;


/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

//Defining motors that were declared in motors.h


void autonomous() {//Actual auton code: 5 point blue
/*
	tray.setBrakeMode(Motor::brakeMode::brake);//Setting brake mode of lift to brake, opposed to the default coast.
	a_tray2.setBrakeMode(Motor::brakeMode::brake);
	intake.setBrakeMode(Motor::brakeMode::brake);//Setting brake mode of claw to brake, for accurate grabbing
	a_intakel.setBrakeMode(Motor::brakeMode::brake);

	drivetrain.setMaxVelocity(200);
	drivetrain.moveDistance(-700);

	drivetrain.setMaxVelocity(150);
	drivetrain.moveDistance(500);	
										*/

	tray.setBrakeMode(Motor::brakeMode::brake);//Setting brake mode of lift to brake, opposed to the default coast.

	intake.setBrakeMode(Motor::brakeMode::brake);//Setting brake mode of claw to brake, for accurate grabbing

	tray.moveVoltage(12100);
	pros::delay(750);
	
	tray.moveVoltage(-12100);
	pros::delay(1000);

	tray.moveVoltage(0);
	
	drivetrain.moveDistance(100);

	intake.moveVoltage(12100);

	drivetrain.setMaxVelocity(65);
	drivetrain.moveDistance(1350);

	drivetrain.setMaxVelocity(150);
	drivetrain.moveDistance(-900);

	drivetrain.setMaxVelocity(100);
	drivetrain.turnAngle(120_deg);

	drivetrain.moveDistance(350);

	intake.moveVoltage(-12100);
	pros::delay(250);

	intake.moveVoltage(0);

	intake.moveVoltage(1200);

	tray.moveVoltage(12100);
	pros::delay(275);

	tray.moveVoltage(5500);
	pros::delay(1500);

	tray.moveVoltage(2000);
	pros::delay(1000);

	drivetrain.setMaxVelocity(100);

	intake.moveVoltage(2000);
	tray.moveVoltage(-2000);
	drivetrain.moveDistance(-1000);

}

//prosv5 upload --slot 4
//prosv5 v5 rm-file slot_4.bin --erase-all