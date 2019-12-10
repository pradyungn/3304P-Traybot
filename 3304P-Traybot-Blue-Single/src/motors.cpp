#include "main.h"
using namespace okapi;

Motor intaker(5, false, Motor::gearset::green);//Initializing motors
Motor intakel(6, true, Motor::gearset::green);

Motor tray1(8, false, Motor::gearset::red);
Motor tray2(3, true, Motor::gearset::red);

MotorGroup intake({intaker, intakel});
MotorGroup tray({tray1, tray2});

ChassisControllerPID drivetrain = ChassisControllerFactory::create(//Initializing drivetrain with three PID instances: Angular PID, Distance PID, and Turn PID. 
	//Values here are stock.
	{2,7},{-1,-4},
	IterativePosPIDController::Gains{0.02, 0, 0},
	IterativePosPIDController::Gains{0.02, 0, 0},
	okapi::AbstractMotor::gearset::green,
	{4_in, 14_in});