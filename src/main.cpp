#include "main.h"
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
	pros::lcd::set_text(1, "Self Destruct Activated!");

	pros::lcd::register_btn1_cb(on_center_button);
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
void autonomous() {
	int speed = 100;
	left_wheels.move(speed);
    right_wheels.move(speed);
}

void opcontrol() {
	pros::Motor wheel1(1); //Assign wheel class to certain ports
	pros::Motor wheel2(2);
	pros::Motor wheel3(3);
	pros::Motor wheel4(4, true);
	pros::Motor wheel5(5, true);
	pros::Motor wheel6(6, true);
	pros::Motor_Group left_wheels({wheel1, wheel2, wheel3});
	pros::Motor_Group right_wheels({wheel4, wheel5, wheel6});
	pros::Motor arm (7, MOTOR_GEARSET_36); // The arm motor has the 100rpm (red) gearset
	pros::Motor spin (8, MOTOR_GEARSET_36);
	pros::Motor cat1 (9, MOTOR_GEARSET_36); //catapult variables
	pros::Motor cat2 (10, MOTOR_GEARSET_36, true);
	pros::Motor_Group catapult({cat1,cat2});

	pros::Controller master (CONTROLLER_MASTER);

	while (true) {
		int power = master.get_analog(ANALOG_LEFT_Y); //Gets a Y value from joystick
		int turn = master.get_analog(ANALOG_LEFT_X); //Gets a X value from joystick
		int right = power + turn; //Premade calcs
		int left = power - turn;
		left_wheels.move(left);
    	right_wheels.move(right);

		int arm_move = master.get_analog(ANALOG_RIGHT_Y);
		arm.move_velocity(arm_move);

		if (master.get_digital(DIGITAL_R1)) {
			catapult.move_velocity(100); // This is 100 because it's a 36 gearset motor
		} else if (master.get_digital(DIGITAL_R2)) {
			catapult.move_velocity(-100);
		} else {catapult.move_velocity(0);}
		
		if (master.get_digital(DIGITAL_L1)) {spin.move_velocity(100);} else if (master.get_digital(DIGITAL_L2)) {spin.move_velocity(-100);} else {spin.move_velocity(0);}

    		pros::delay(2);
  }
}

