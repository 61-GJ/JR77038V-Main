#include "main.h" // IWYU pragma: keep
#include "lemlib/api.hpp" // IWYU pragma: keep

// Initlizing the controller object
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// Creating the Pneumatics
pros::adi::Pneumatics Hood('h', true); // PS: 'true' means the pistons start retracted
pros::adi::Pneumatics scraperPistion('g', false);

//Motors responsible for Intake & Outtake 
pros::Motor IO2 (2, pros::MotorGearset::green);
pros::Motor IO3 (3, pros::MotorGearset::blue);
pros::Motor IO4 (4, pros::MotorGearset::green);
// Intake/Outtake motors on ports 2, 3, and 4 (all forwards)

// Creating the components for the chassis
pros::MotorGroup leftmotors({-11, 17, -15}, pros::MotorGearset::blue); // left motors use 600 RPM cartridges
pros::MotorGroup rightmotors({16, -14, 13}, pros::MotorGearset::blue); // right motors use 600 RPM cartridges
// left motors on ports 11 (reversed), 17 (forwards), and 15 (reversed)
// right motors on ports 16 (forwards), 14 (reversed), and 13 (forwards)

/**  Note:
  *  Front Width: 13 inches
  *  Side Length: 15.5 inches
  *
  *  True Length: 16.5 Inches
  *  True Width: 13 Inches
  */  
lemlib::Drivetrain drivetrain(&leftmotors, // left motor group
                              &rightmotors, // right motor group
                              10, // Filler input 
                              lemlib::Omniwheel::OLD_325, // using old 3.25" omnis
                              360, // drivetrain rpm is 4360
                              8 // horizontal drift is 2 (for now)
);

pros::Imu imu(10); // creates an V5 Inertial Sensors (IMU) on port 10

// sensor group for odometry
lemlib::OdomSensors sensors(nullptr, // no vertical tracking wheels, set to nullptr
                    nullptr, // no second vertical tracking wheel, set to nullptr
                    nullptr, // no horizontal tracking wheels, set to nullptr
                    nullptr, // no second horizontal tracking wheel, set to nullptr
                    &imu); // IMU

// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain,
                        lateral_controller,
                        angular_controller,
                        sensors,
                        &throttle_curve, 
                        &steer_curve
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Creating A motor Group for the outtake motors
void IO_velocities(int bottom, int middle, int top)
{
    IO2.move_velocity(bottom);
    IO3.move_velocity(middle);
    IO4.move_velocity(top);
}