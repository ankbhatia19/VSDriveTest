/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <Commands/Subsystem.h>
#include "WPILib.h"
#include "ctre/Phoenix.h"

class Drivetrain : public frc::Subsystem {
 private:
  WPI_TalonSRX* leftTalon;
  WPI_TalonSRX* rightTalon;
  DifferentialDrive* drivetrain;
  // It's desirable that everything possible under private except
  // for methods that implement subsystem capabilities

 public:
  Drivetrain();
  void drive(double left, double right);
  void InitDefaultCommand() override;
};
