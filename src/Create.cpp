#include "main.h"
#include "liblvgl/lvgl.h" // For LVGL Screen Interface
#include "lemlib/api.hpp" // IWYU pragma: keep

pros::Controller controller(pros::E_CONTROLLER_MASTER);

// Creating the Pneumatics

pros::adi::Pneumatics MainPistons('a', false);

//Motors responsible for Intake & Outtake 

pros::Motor IO4 (4, pros::MotorGearset::green);
pros::Motor IO5 (5, pros::MotorGearset::green);
pros::Motor IO7 (7, pros::MotorGearset::green);

pros::MotorGroup leftmotors({-11, -12, -13}, pros::MotorGearset::blue); // left motors use 600 RPM cartridges
pros::MotorGroup rightMotors({20, 19, 18}, pros::MotorGearset::blue); // right motors use 600 RPM cartridges
// left motors on ports 11 (forwards), 12 (reversed), and 13 (reversed)
// right motors on ports 18 (reversed), 19 (forwards), and 20 (forwards)

lemlib::Drivetrain drivetrain(&leftmotors, // left motor group
                              &rightMotors, // right motor group
                              10.75, // 11 inch track width
                              lemlib::Omniwheel::OLD_325, // using old 3.25" omnis
                              450, // drivetrain rpm is 360
                              2 // horizontal drift is 2 (for now)
);

pros::Imu imu(10); // creates an V5 Inertial Sensors (IMU) on port 10

lemlib::OdomSensors sensors(nullptr, // no vertical tracking wheels, set to nullptr
                    nullptr, // no second vertical tracking wheel, set to nullptr
                    nullptr, // no horizontal tracking wheels, set to nullptr
                    nullptr, // no second horizontal tracking wheel, set to nullptr
                    &imu); // IMU


// lateral PID controller
lemlib::ControllerSettings lateral_controller(40, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              39, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(3, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              27, // derivative gain (kD)
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