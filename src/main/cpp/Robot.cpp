/**
 * Enable robot and slowly drive leftPowerard.
 * [1] If DS reports errors, adjust CAN IDs and firmware update.
 * [2] If motors are spinning incorrectly, first check gamepad.
 * [3] If motors are still spinning incorrectly, correct motor inverts.
 * [4] Now that motors are driving correctly, check sensor phase.  If sensor is out of phase, adjust sensor phase.
 */

#include <iostream>
#include <string>

#include <IterativeRobot.h>
#include "ctre/Phoenix.h"
#include "Drive/DifferentialDrive.h"
#include "Joystick.h"

class Robot: public frc::IterativeRobot {
public:
	/* ------ [1] Update CAN Device IDs and switch to WPI_VictorSPX where necessary ------*/
	WPI_TalonSRX * rightTalon = new WPI_TalonSRX(3);
	
	WPI_TalonSRX * leftTalon = new WPI_TalonSRX(2);
	

	DifferentialDrive * _diffDrive = new DifferentialDrive(*leftTalon, *rightTalon);

	Joystick * leftStick = new Joystick(0);
	Joystick* rightStick = new Joystick(1);

	Faults _faults_L;
	Faults _faults_R;

	void TeleopPeriodic() {

		std::stringstream work;

		/* get gamepad stick values */
		double leftPower = +1 * leftStick->GetY(); /* positive is leftPowerard */
		double rightPower = +1 * leftStick->GetX(); /* positive is right */

		/* deadband gamepad 10%*/
		if (fabs(leftPower) < 0.10)
			leftPower = 0;
		if (fabs(rightPower) < 0.10)
			rightPower = 0;

		/* drive robot */
		//_diffDrive->ArcadeDrive(leftPower, rightPower, false);
		_diffDrive->TankDrive(leftPower, rightPower, false);

		/* -------- [2] Make sure Gamepad leftPowerard is positive for leftPowerARD, and GZ is positive for RIGHT */
		work << " GF:" << leftPower << " GT:" << rightPower;

		/* get sensor values */
		//double leftPos = leftTalon->GetSelectedSensorPosition(0);
		//double rghtPos = rightTalon->GetSelectedSensorPosition(0);
		double leftVelUnitsPer100ms = leftTalon->GetSelectedSensorVelocity(0);
		double rightVelUnitsPer100ms = rightTalon->GetSelectedSensorVelocity(0);

		work << " L:" << leftVelUnitsPer100ms << " R:" << rightVelUnitsPer100ms;

		/* drive motor at least 25%, Talons will auto-detect if sensor is out of phase */
		leftTalon->GetFaults(_faults_L);
		rightTalon->GetFaults(_faults_R);

		if (_faults_L.SensorOutOfPhase) {
			work << " L sensor is out of phase";
		}
		if (_faults_R.SensorOutOfPhase) {
			work << " R sensor is out of phase";
		}

		/* print to console */
		std::cout << work.str() << std::endl;
	}

	void RobotInit() {

		/* [4] adjust sensor phase so sensor moves
		 * positive when Talon LEDs are green */
		
		rightTalon->SetSensorPhase(true);
		leftTalon->SetSensorPhase(true);
	}

private:
};

START_ROBOT_CLASS(Robot)