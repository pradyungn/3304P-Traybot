#include "main.h"
using namespace okapi;

class turnOut: public ControllerOutput<double> {//subclassing the ControllerOutput class to rotate chassis
  public:
  
	virtual void controllerSet(double ivalue) override {
    	drivetrain.rotate(ivalue);
		controllerSet(ivalue);
  	}
};


//Turn Gyro PID

double tkP = 0.02;
double tkI = 0;
double tkD = 0;
AsyncPosPIDController tPID = AsyncControllerFactory::posPID(std::make_shared<ADIGyro>(ADIGyro('A')), std::make_shared<turnOut>(turnOut()), tkP, tkI,tkD, 0,std::move(std::make_unique<PassthroughFilter>()), TimeUtilFactory::create());

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