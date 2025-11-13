#pragma once
#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

//controller 
extern pros::Controller controller;

// Creating the Pneumatics
extern pros::adi::Pneumatics MainPistons;

// Motors responsible for Intake & Outtake 
extern pros::Motor IO4;
extern pros::Motor IO5;
extern pros::Motor IO7;

// Creating the components for the chassis
extern pros::MotorGroup leftmotors;
extern pros::MotorGroup rightMotors;

extern lemlib::Drivetrain drivetrain;
extern pros::Imu imu;
extern lemlib::OdomSensors sensors;
extern lemlib::ControllerSettings lateral_controller;
extern lemlib::ControllerSettings angular_controller;
extern lemlib::ExpoDriveCurve throttle_curve;
extern lemlib::ExpoDriveCurve steer_curve;

// create the chassis
extern lemlib::Chassis chassis;