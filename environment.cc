#include <rlglue/Environment_common.h>
#include <rlglue/utils/C/RLStruct_util.h>

#include "box.h"

static observation_t this_observation;
static reward_observation_terminal_t this_reward_observation;

int calculate_reward(double angle, double angleVel, double vel);
int check_terminal(double angle, double angleVel, double vel);

const char *env_init(){
	const char *task_spec_string = "[VERSION RL-GLue-3.0] [PROBLEMTYPE episodic] "
									"[OBSERVATIONS angle(-90 90) angle velocity(-50  50) velocity(-50 50) ] "
									"[ACTIONS (-3 3) ] [REWARDS 100 0 ] "
									"[EXTRA environment(C/C++) by PSC, CBH]";

	allocateRLStruct(&this_observation,0,3,0);
	this_reward_observation.observation = &this_observation;
	this_reward_observation.reward = 0;
	this_reward_observation.terminal = 0;

	return task_spec_string;
}

const observation_t *env_start(){
	box_init();

	double angle = 0.0;
	double angleVel = 0.0;
	double vel = 0.0;

	angle = get_angle();
	angleVel = get_angleVelocity();
	angleVel += 3;
	vel = get_velocity();
	vel += 50;

	this_observation.doubleArray[0] = angle;
	this_observation.doubleArray[1] = angleVel;
	this_observation.doubleArray[2] = vel;
	return &this_observation; 
}

const reward_observation_terminal_t *env_step(const action_t *this_action){
	double the_reward = 0.0;
	int force = 0;
	int terminal = 0;

	double angle = 0.0;
	double angleVel = 0.0;
	double vel = 0.0;

	force = this_action->intArray[0];

	apply_force(force);

	angle = get_angle();
	//angle += 90;
	angleVel = get_angleVelocity();
	angleVel += 3;
	vel = get_velocity();
	vel += 50;
	
	the_reward = calculate_reward(angle, angleVel, vel);
	terminal = check_terminal(angle, angleVel, vel);

	this_reward_observation.observation->doubleArray[0] = angle;
	this_reward_observation.observation->doubleArray[1] = angleVel;
	this_reward_observation.observation->doubleArray[2] = vel;

	this_reward_observation.reward = the_reward;
	this_reward_observation.terminal = terminal;
	
	return &this_reward_observation;
}

void env_cleanup(){
	clearRLStruct(&this_observation);
}

const char* env_message(const char* inMessage){
	return inMessage;
}

int calculate_reward(double angle, double angleVel, double vel){
    if((int)angle<=94 && (int)angle>=86 && (int)angleVel==3 && (int)vel==50){
        return 100;
    }
    return 0;
}
  
int check_terminal(double angle, double angleVel, double vel){
	//printf("check terminal angle : %lf, angleVel : %lf, vel : %lf\n",angle, angleVel, vel);
    if((int)angle<=94 && (int)angle>=86 && (int)angleVel==3 && (int)vel==50){
        printf("success angle : %lf\n",angle);
        return 1;
    }
    if((int)angle<=20 || (int)angle>=160){
    	printf("fail angle : %lf\n",angle);
    	return 1;
    }
    return 0;
}
