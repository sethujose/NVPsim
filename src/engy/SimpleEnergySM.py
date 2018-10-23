from m5.SimObject import SimObject
from BaseEnergySM import BaseEnergySM
from m5.params import *
from m5.proxy import *

class SimpleEnergySM(BaseEnergySM):
    type = 'SimpleEnergySM'
    cxx_header = "engy/state_machine.hh"
    thres_1_to_off = Param.Float(Parent.thres_1_to_off ,"power off threshold")
    thres_off_to_1 = Param.Float(Parent.thres_off_to_1 ,"power on threshold")