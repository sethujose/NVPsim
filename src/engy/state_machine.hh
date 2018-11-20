//
// Created by lf-z on 3/13/17.
//

#ifndef GEM5_STATE_MACHINE_HH
#define GEM5_STATE_MACHINE_HH

#include "sim/sim_object.hh"
#include "engy/energy_mgmt.hh"
#include "params/BaseEnergySM.hh"
#include "params/SimpleEnergySM.hh"

class EnergyMgmt;

/********* Basic Energy SM ***********/
class BaseEnergySM : public SimObject
{
	/* Set EnergyMgmt as friend class so that this->mgmt can be initialized. */
	friend class EnergyMgmt;
public:
	typedef	 BaseEnergySMParams Params;
	const Params *params() const
	{
		return reinterpret_cast<const Params *>(_params);
	}
	BaseEnergySM(const Params *p);
	virtual ~BaseEnergySM() {}
	virtual void init() {}
	virtual void update(double _energy, double _engy_read, char* sender) {}

protected:
	EnergyMgmt *mgmt;
	void broadcastMsg(const EnergyMsg &msg);
	double energy_consume_lower_bound;
};

/********* Simple Energy SM ***********/
class SimpleEnergySM : public BaseEnergySM
{
public:
	typedef SimpleEnergySMParams Params;
	const Params *params() const
	{
		return reinterpret_cast<const Params *>(_params);
	}
	SimpleEnergySM(const Params *p);
	virtual ~SimpleEnergySM() {}
	virtual void init();
	virtual void update(double _energy, double _engy_read, char* sender);

	// The states defined by energy managing algorithm
	enum State {
		STATE_POWER_OFF = 0,
		STATE_POWER_RETENTION = 1,
		STATE_POWER_ON = 2
	};

	// Messages
	enum MsgType {
		CONSUME_ENERGY = 0,
		POWER_OFF = 1,
		POWER_RET = 2,
		POWER_ON = 3
	};


	int outage_times = 0;

protected:
	State state;
	double thres_ret_to_off;
	double thres_1_to_ret;
	double thres_ret_to_1; 
};

#endif //GEM5_STATE_MACHINE_HH
