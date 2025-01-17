import m5
from m5.objects import *

system = System()
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '100MHz'
system.clk_domain.voltage_domain = VoltageDomain()
system.mem_mode = 'atomic'
system.mem_ranges = [AddrRange('512MB')]

#this needs to be passed from console
cap = 1 * 0.2

system.energy_mgmt = EnergyMgmt(path_energy_profile = 'profile/nvp_sim/sine_wave/high_res_sine', energy_time_unit = '10ns')
#system.energy_mgmt = EnergyMgmt(path_energy_profile = 'profile/nvp_sim/real/real_input', energy_time_unit = '10ns')

# Threshold Design for the state machine
system.energy_mgmt.state_machine.thres_ret_to_off = 40
system.energy_mgmt.state_machine.thres_1_to_ret = 80
system.energy_mgmt.state_machine.thres_ret_to_1 = 500
system.energy_mgmt.capacity = cap;				# uF
system.energy_mgmt.system_leakage = 0.2;			# leakage

# CPU: basic params
system.cpu = AtomicSimpleCPU(
			power_cpu = [0, 0.3, 1.3], 	# nJ/cycle
			cycle_backup = 5, 		# nJ/cycle
			cycle_restore = 3 		# nJ/cycle
		)

system.cpu.s_energy_port = system.energy_mgmt.m_energy_port

system.membus = SystemXBar()

system.cpu.icache_port = system.membus.slave
system.cpu.dcache_port = system.membus.slave

system.cpu.createInterruptController()

system.mem_ctrl = DDR3_1600_8x8()
system.mem_ctrl.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.master

system.system_port = system.membus.slave

process = Process()
process.cmd = ['tests/test-progs/nvpsim/bin/arm/linux/nvp_hello']
system.cpu.workload = process
system.cpu.createThreads()

root = Root(full_system = False, system = system)
m5.instantiate()

print ("Beginning simulation!")
exit_event = m5.simulate()
print ('Exiting @ tick %i because %s' % (m5.curTick(), exit_event.getCause()))
