//
// Created by lf-z on 3/13/17.
// Update by tongda on 3/14/18.
//

#include "engy/state_machine.hh"
#include "debug/EnergyMgmt.hh"
#include "debug/SM_Retention.hh"
#include <fstream>

/******* BaseEnergySM *******/
BaseEnergySM::BaseEnergySM(const Params *p) : SimObject(p), mgmt(NULL)
{
	energy_consume_lower_bound = 0;
}

void 
BaseEnergySM::broadcastMsg(const EnergyMsg &msg)
{
	mgmt->broadcastMsgAsEvent(msg);
}

/******* SimpleEnergySM *******/
SimpleEnergySM::SimpleEnergySM(const Params *p) :
	BaseEnergySM(p),
	state(SimpleEnergySM::State::STATE_POWER_OFF),
	thres_ret_to_off(p->thres_ret_to_off),
	thres_1_to_ret(p->thres_1_to_ret),
	thres_ret_to_1(p->thres_ret_to_1)
{
	// when the system cannot consume energy
	energy_consume_lower_bound = thres_ret_to_off;
}

void
SimpleEnergySM::init()
{
	EnergyMsg msg;
	msg.val = 0;
	state = State::STATE_POWER_OFF;
	msg.type = MsgType::POWER_OFF;
	broadcastMsg(msg);
	
	DPRINTF(EnergyMgmt, "[SimpleEngySM] State machine Initialized\n");
}

void SimpleEnergySM::update(double _energy)
{
	EnergyMsg msg;
	msg.val = 0;

	// if (state == State::STATE_POWER_RETENTION)
	DPRINTF(SM_Retention, "[SM_Retention] State = %s, energy=%lf, thres_ret_1 = %lf\n", state, _energy, thres_ret_to_1);

	// power failure
	if ( (state == State::STATE_POWER_ON || state == State::STATE_POWER_RETENTION) 
		&& _energy <= thres_ret_to_off ) 
	{
		state = State::STATE_POWER_OFF;
		msg.type = MsgType::POWER_OFF;

		if (state == State::STATE_POWER_ON)
			DPRINTF(SM_Retention, "[SM_Retention] State change: POWER_ON->POWER_OFF, energy=%lf, thres=%lf.\n", _energy, thres_ret_to_off);
		else if (state == State::STATE_POWER_RETENTION)
			DPRINTF(SM_Retention, "[SM_Retention] State change: POWER_RET->POWER_OFF, energy=%lf, thres=%lf.\n", _energy, thres_ret_to_off);

		broadcastMsg(msg);
	} 

	// power retention
	else if (state == State::STATE_POWER_ON && _energy <= thres_1_to_ret)
	{
		state = State::STATE_POWER_RETENTION;
		msg.type = MsgType::POWER_RET;
		DPRINTF(SM_Retention, "[SM_Retention] State change: POWER_ON->POWER_RET, energy=%lf, thres=%lf.\n", _energy, thres_1_to_ret);
		broadcastMsg(msg);
	}

	// power recovery
	else if ( (state == State::STATE_POWER_OFF || state == State::STATE_POWER_RETENTION)
		&& _energy >= thres_ret_to_1) 
	{
		if (state == State::STATE_POWER_OFF)
			DPRINTF(SM_Retention, "[SM_Retention] State change: POWER_OFF->POWER_ON, energy=%lf, thres=%lf.\n", _energy, thres_ret_to_1);
		else if (state == State::STATE_POWER_RETENTION)
			DPRINTF(SM_Retention, "[SM_Retention] State change: POWER_RET->POWER_ON, energy=%lf, thres=%lf.\n", _energy, thres_ret_to_1);

		state = State::STATE_POWER_ON;
		msg.type = MsgType::POWER_ON;
		broadcastMsg(msg);
	}
}

BaseEnergySM *
BaseEnergySMParams::create()
{
	return new BaseEnergySM(this);
}

SimpleEnergySM *
SimpleEnergySMParams::create()
{
	return new SimpleEnergySM(this);
}
