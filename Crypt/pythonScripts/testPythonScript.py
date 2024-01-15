import os
import torch
import numpy as np
import os, sys

import example
import cpp_module

def Start():
    print(testString)
    
def Update(state):
    testList = state.getPlayerPosition()
    
    # testList = state.testPlayerPosition()
    print(testList)
    
    # print(state)
    p = example.Pet("Test")
    print(p.getName())    
    p.setName("Charly")
    print(p.getName())
    print(torch.cuda.is_available())
    
    return 1

def Reset():
    pass

testString = "Hello there!"