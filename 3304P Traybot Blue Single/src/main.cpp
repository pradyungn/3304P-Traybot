#include "main.h"
using namespace okapi;

Controller controller;//Initializing controller here

ControllerButton tray_up(ControllerDigital::L1);
ControllerButton tray_down(ControllerDigital::L2);

ControllerButton intake_in(ControllerDigital::R2);
ControllerButton intake_out(ControllerDigital::R1);

Motor intaker(5, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);//Initializing motors
Motor intakel(6, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

Motor tray1(8, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor tray2(3, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);

MotorGroup intake({intaker, intakel});
MotorGroup tray({tray1, tray2});

auto drivetrain = ChassisControllerBuilder()
	.withMotors({2,7}, {-1,-4})
	.withDimensions(AbstractMotor::gearset::green, {{4_in, 14_in}, imev5GreenTPR})
	.withGains({0.02, 0, 0}, {0, 0, 0})
	.build();

class turnOut: public ControllerOutput<double> {//subclassing the ControllerOutput class to rotate chassis
  public:

	virtual void controllerSet(double ivalue) override {
    	drivetrain -> getModel() -> rotate(ivalue);
		controllerSet(ivalue);
  	}
};

//Turn Gyro PID

double tkP = 0.02;
double tkI = 0;
double tkD = 0;

AsyncPosPIDController tPID(std::make_shared<ADIGyro>(ADIGyro('A')), std::make_shared<turnOut>(turnOut()), TimeUtilFactory::createDefault(), tkP, tkI,tkD);

void crash() {//Kill-switch function. Stops all motors.
	if (tray_up.isPressed() && tray_down.isPressed() && intake_in.isPressed()  && intake_out.isPressed()) {
		drivetrain -> stop();
		
		tray.moveVoltage(0);

		intake.moveVoltage(0);
	}
}

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Mine now beetch");
	
	pros::lcd::register_btn1_cb(on_center_button);

	pros::lcd::set_text(1,"Initialization Complete.");
}
/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

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
void autonomous(){
	tray.setBrakeMode(Motor::brakeMode::brake);//Setting brake mode of lift to brake, opposed to the default coast.

	intake.setBrakeMode(Motor::brakeMode::brake);//Setting brake mode of claw to brake, for accurate grabbing

	tray.moveVoltage(12100);
	pros::delay(750);
	
	tray.moveVoltage(-12100);
	pros::delay(1000);

	tray.moveVoltage(0);

	drivetrain -> moveDistance(100_cm);

	intake.moveVoltage(12100);

	drivetrain -> getModel() -> setMaxVelocity(65);
	drivetrain -> moveDistance(1350_cm);

	drivetrain -> getModel() -> setMaxVelocity(150);
	drivetrain -> moveDistance(-900_cm);

	drivetrain -> getModel() -> setMaxVelocity(100);

	drivetrain -> turnAngle(120_deg);
	drivetrain -> moveDistance(350_cm);

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

	drivetrain -> getModel() -> setMaxVelocity(100);

	intake.moveVoltage(2000);
	tray.moveVoltage(-2000);
	drivetrain -> moveDistance(-1000_cm);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	tray.setBrakeMode(Motor::brakeMode::brake);

	intake.setBrakeMode(Motor::brakeMode::brake);

	while (true) {
		drivetrain -> getModel() -> tank(controller.getAnalog(ControllerAnalog::leftY),controller.getAnalog(ControllerAnalog::rightY));
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
