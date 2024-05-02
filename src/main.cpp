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
pros::Motor wheel1(1, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES); //Assign wheel class to certain ports
pros::Motor wheel2(2, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor wheel3(3, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor wheel4(4, pros::E_MOTOR_GEAR_GREEN,false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor wheel5(5, pros::E_MOTOR_GEAR_GREEN,false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor wheel6(6, pros::E_MOTOR_GEAR_GREEN,false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group left_wheels({wheel1, wheel2, wheel3});
pros::Motor_Group right_wheels({wheel4, wheel5, wheel6});
pros::Motor arm (7, MOTOR_GEARSET_36); // The arm motor has the 100rpm (red) gearset
pros::Motor spin (8, MOTOR_GEARSET_36);
pros::Motor cat1 (9, MOTOR_GEARSET_36); //catapult variables
pros::Motor cat2 (10, MOTOR_GEARSET_36, true);
pros::Motor_Group catapult({cat1,cat2});

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


void moveforward(int x){ // x should be mm
	left_wheels.move_relative(x,127);
	right_wheels.move_relative(x,127);
}

void turnleft(int angle){
	left_wheels.move_relative(angle,127);
}

void grabBall() {
	spin.move(127);
	left_wheels.move_relative(1000,127);
	right_wheels.move_relative(1000,127);
	spin.move(0);
	pros::delay(2);
	arm.move(-127);
	pros::delay(2);
	arm.move(0);
}

void depoBall() {
	spin.move(10); //NEEDS TO BE ENOUGH TO LET THE BALL OUT
	pros::delay(2);
	arm.move(-127); // YOU May need to repast 'arm.move(-127);' a few times
	pros::delay(2);
	arm.move(0);
}

void turnright(int angle){
	right_wheels.move_relative(angle,127);
}
/*
void resetdrive(){
	left_wheels.tare_position();
	right_wheels.tare_position();
}
*/

void launchBall(){
	catapult.move_relative(120,127);
	catapult.move_relative(-120,127);
} 

void flex(){
	//This is to break the band which keeps the catapult down
}

void autonomous() {
/*
An Autonomous Win Point is awarded to any Alliance that ends the Autonomous Period with the following tasks completed::

1. Removed the Triball from the Alliance’sMatch Load Zone that coincides with their Starting Tiles. For example, in Figure 21, the red Alliance must remove the Triball that begins in the bottom-left Match Load Zone, adjacent to Robot 1’s Starting Tiles.
2. Scored at least one of their own Alliance Triballs in the Alliance’s own Goal.
3. Ended the Autonomous Period with at least one Robot contacting their own Elevation Bar.
4. Not violated any other rules.
*/
	int foot = 3600; //UPDATE THIS TO WHAT AN ACTUAL FOOT WOULD BE
	bool WeNeedToHeadBackToTheBar = true; //These needs to be worked out with cooperating team before the match
	bool extraball = true;
	bool teamred = true; //This needs to be figured out before the match

	//ALL OF THIS CODE IS UNTESTED. INDIVIDUAL LAYOUTS (blue team + extraball) CAN BE ADJUSTED AS NECESSARY
	//as of right now Red/Blue team code is copy/paste.

	flex();
	if(teamred && extraball) { 
		moveforward((foot * 4));
		turnright(80); // This needs to be enough to turn towards the goal
		depoBall();
		if(WeNeedToHeadBackToTheBar){
			turnleft(120); //we are heading back to the bar now, measurments will not be percise
			launchBall();
			moveforward(foot * 5);
			turnright(180);
			moveforward(foot * 2);
		} else{
			turnleft(180); //turn around
			moveforward(foot * 3);
			depoBall();
			turnright(160); // now going for extra ball
			moveforward(foot * 5);
			grabBall();
			turnleft(180); //turn around
			moveforward(foot * 5);
			depoBall();
		}
	} 
	else if(!(teamred) && extraball){
		moveforward((foot * 4));
		turnright(80); // This needs to be enough to turn towards the goal
		depoBall();
		turnleft(120); //MUST BE ENOUGH TO TURN TOWARDS BALL IN THE EDGE
		moveforward((foot * 3));
		grabBall();
		if(WeNeedToHeadBackToTheBar){
			turnleft(120); //we are heading back to the bar now, measurments will not be percise
			launchBall();
			moveforward(foot * 5);
			turnright(180);
			moveforward(foot * 2);
		} else{
			turnleft(180); //turn around
			moveforward(foot * 3);
			depoBall();
			turnright(160); // now going for extra ball
			moveforward(foot * 5);
			grabBall();
			turnleft(180); //turn around
			moveforward(foot * 5);
			depoBall();
		}
	} 
	
	else if(teamred && !(extraball)){
		moveforward((foot * 4));
		turnright(80); // This needs to be enough to turn towards the goal
		depoBall();
		turnleft(220); //Turn around
		moveforward((foot*2)); // we are going for corner ball now
		turnright(40);
		moveforward(foot);
		grabBall(); //grab corner ball
		if(WeNeedToHeadBackToTheBar){
			turnleft(160);
			moveforward(foot*3);
		} else{
			moveforward(-foot*3);
			launchBall();
		}
	} 
	
	else{
		moveforward((foot * 4));
		turnright(80); // This needs to be enough to turn towards the goal
		depoBall();
		turnleft(220); //Turn around
		moveforward((foot*2)); // we are going for corner ball now
		turnright(40);
		moveforward(foot);
		grabBall(); //grab corner ball
		if(WeNeedToHeadBackToTheBar){
			turnleft(160);
			moveforward(foot*3);
		} else{
			moveforward(-foot*3);
			launchBall();
		}
	}
}


void opcontrol() {
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
		
		 if (master.get_digital(DIGITAL_L1)) {spin.move_velocity(127);} else if (master.get_digital(DIGITAL_L2)) {spin.move_velocity(-127);} else {spin.move_velocity(0);}


		 pros::delay(2);
 }
}