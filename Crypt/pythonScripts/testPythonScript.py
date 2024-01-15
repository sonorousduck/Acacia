import os
import torch
import numpy as np
import os, sys

import cpp_module

def Start():
    print(testString)
    
def Update(state):
    testList = state.getPlayerPosition()
    
    # testList = state.testPlayerPosition()
    print(testList)

    print(torch.cuda.is_available())
    
    # element 0: flipGravity (0.0 for don't, 1.0 for do)
    # element 1: angleOfCursor (0-360)
    # element 2: -1, 0, or 1 for slow, regular, fast speed
    # element 3: shoot (0.0 for don't, 1.0 for do)
    print("here")
    action = cpp_module.Action()
    print("now here")
    action.setBox(0.0, 10.0, 0.0, 0.0)
    print("Made it ehre")
    print(action)
    return action

def Reset():
    pass

testString = "Hello there!"