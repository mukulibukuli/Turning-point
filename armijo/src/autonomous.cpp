//#include "main.h"
#include "okapi/api.hpp"
#include "autonselection.h"
#include "motordefs.hpp"
using namespace okapi;

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
void frontAuton(bool redAlliance);
void backAuton();
void noAuton();

void autonomous() {
	switch (autonSelected) {
		case 0:
			pros::lcd::set_text(2, "front red 3 flag running!");
			frontAuton(true /* red alliance */);
			break;
		case 1:
			pros::lcd::set_text(2, "front blue 3 flag running!");
			frontAuton(false /* blue alliance */);
			break;
		case 2:
			pros::lcd::set_text(2, "back auton");
			backAuton();
			break;
		default:
			pros::lcd::set_text(2, "no auton");
			noAuton();
			break;
	}
}

void frontAuton(bool redAlliance){
	MotorDefs *mtr_defs = MotorDefs::getMotorDefs();
	if (mtr_defs == NULL) {
		pros::lcd::set_text(2, "MotorDefs is NULL!");
	} else if (mtr_defs->left_mtr_f == NULL) {
		pros::lcd::set_text(2, "left motor front is NULL!");
	} else {
		auto drive = ChassisControllerFactory::create(*(mtr_defs->left_mtr_f), *(mtr_defs->right_mtr_f),
														*(mtr_defs->right_mtr_b), *(mtr_defs->left_mtr_b), 
														AbstractMotor::gearset::green, {4_in, 15.5_in});
		mtr_defs->left_mtr_f->setVoltageLimit(9000);
		mtr_defs->left_mtr_b->setVoltageLimit(9000);
		mtr_defs->right_mtr_f->setVoltageLimit(9000);
		mtr_defs->right_mtr_b->setVoltageLimit(9000);
		//intake_mtr->move(127);
		pros::Task::delay(200);
		drive.moveDistance(40_in);
		mtr_defs->intake_mtr->move(127);
		drive.moveDistance(2_in);
		pros::Task::delay(300);

		// turning to not hit the pole
		// move back with ball and preload ball towards fence
		mtr_defs->left_mtr_f->move(-75);
		mtr_defs->left_mtr_b->move(-75);
		mtr_defs->right_mtr_f->move(-75);
		mtr_defs->right_mtr_b->move(-75);
		pros::Task::delay(1500);
		mtr_defs->left_mtr_f->move(0);
		mtr_defs->right_mtr_f->move(0);
		mtr_defs->left_mtr_b->move(0);
		mtr_defs->right_mtr_b->move(0);

		pros::Task::delay(500);
		if (redAlliance) {
			drive.moveDistance(14_in);
		} else {
			drive.moveDistance(4_in);
		}	
		pros::Task::delay(100);

		//turn to face the flags
		if (redAlliance){
			drive.turnAngle(-1*90_deg);
		} else {
			drive.turnAngle(83_deg);
		}
		pros::Task::delay(200);
		drive.moveDistance(4_in);
		pros::Task::delay(200);
		mtr_defs->catapult_mtr->move_relative(415, 127);
		pros::Task::delay(500);
		mtr_defs->intake_mtr->move(0);
		if (redAlliance) {
			drive.turnAngle(-1*15_deg);
		} else {
			drive.turnAngle(7_deg);
		}
		
		drive.moveDistance(45_in);
		pros::Task::delay(300);
		drive.moveDistance(-1*20_in);
	}
}


void backAuton(){
	MotorDefs *mtr_defs = MotorDefs::getMotorDefs();
	Motor catapult_mtr(5, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
	pros::Task::delay(12000);
	mtr_defs->catapult_mtr->move(127);
	pros::Task::delay(500);
	mtr_defs->catapult_mtr->move(0);
}

void noAuton(){
}