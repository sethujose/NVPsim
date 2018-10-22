//
// Created by lf-z on 12/28/16.
//
#include <cmath>
#include <cstring>
#include <fstream>

#include "debug/EnergyMgmt.hh"
#include "engy/energy_mgmt.hh"
#include "sim/eventq.hh"

EnergyMgmt::EnergyMgmt(const Params *p)
        : SimObject(p),
                system_leakage(p->system_leakage),
                energy_profile_mult(p->energy_profile_mult),
          time_unit(p->energy_time_unit),
          energy_remained(0),
          //event_msg(this, false, Event::Energy_Pri),
          //event_energy_harvest(this, false, Event::Energy_Pri),
          //state_machine(p->state_machine),
          //harvest_module(p->harvest_module),
          capacity(p->capacity),
          _path_energy_profile(p->path_energy_profile)
{
    //msg_togo.resize(0);
}

EnergyMgmt::~EnergyMgmt() {}

void EnergyMgmt::init()
{
    /* Read energy profile. */
    energy_harvest_data = readEnergyProfile();
    /* Reset energy remained to 0. */
    energy_remained = 0;
    /* Set mgmt pointer in state machine. */
    /*
    if (state_machine) {
        state_machine->mgmt = this;
    }
    */

    DPRINTF(EnergyMgmt, "[EngyMgmt] Energy Management module initialized!\n");
    DPRINTF(EnergyMgmt, "[EngyMgmt] Energy profile: %s(Time unit: %d ticks)\n",
            _path_energy_profile.c_str(), time_unit);
    DPRINTF(EnergyMgmt, \
      "[EngyMgmt] The capacitypython line length is %lf.\n", capacity);

    /* Start to harvest energy */
    energyHarvest();
}

std::vector<double> EnergyMgmt::readEnergyProfile()
{
    std::vector<double> data;
    data.resize(0);
    /* Return empty vector is no energy profile is provided. */
    if (_path_energy_profile == "")
        return data;

    double temp;
    std::ifstream fin;
    fin.open(_path_energy_profile.c_str());
    /* Read energy profile and store the data into vector. */
    while (fin>>temp) {
        data.push_back(temp);
    }
    /* Reverse the energy harvest queue so that the first energy
    unit pops first */
    reverse(data.begin(), data.end());
    fin.close();

    DPRINTF(EnergyMgmt, "[EngyMgmt] Input file read!\n");
    return data;
}

/* Energy harvest function provides an harvest event, where an
Energy_Consume_Msg with negative value is generated. One by one, this event
will trigger the next harvest event. */
void
EnergyMgmt::energyHarvest()
{
    // read the energy supply data from the energy profile
    if (energy_harvest_data.empty())
        return;

    double energy_val = energy_harvest_data.back();
    char dev_name[100];
    strcpy(dev_name, "harvester");
    //consumeEnergy(dev_name, -energy_val);
    energy_harvest_data.pop_back();

    DPRINTF(EnergyMgmt, "[EngyMgmt] Energy read: %lf!\n", energy_val);

    /* Trigger the next harvest function. */
    //schedule(event_energy_harvest, curTick() + time_unit);

    // DPRINTF(EnergyMgmt, "Energy %lf harvested.\n", energy_val);
}

EnergyMgmt *
EnergyMgmtParams::create()
{
    return new EnergyMgmt(this);
}
