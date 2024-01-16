import torch
import numpy as np
import os

import cpp_module

def randomAction(state: list):
    action = cpp_module.Action()
    
    shouldFlipGravity = 0.0 if np.random.random() < 0.5 else 1.0
    cursorAngle = np.random.randint(0, 360)
    random_int = np.random.randint(0, 3)
    speed_control = random_int - 1.0
    shouldShoot = 0.0 if np.random.random() < 0.5 else 1.0
    
    action.setBox(shouldFlipGravity, cursorAngle, speed_control, shouldShoot)
    
    return action

def Start():
    print(testString)
    
def Update(state, reward):
    playerPosition = state.getPlayerPosition()
    
    print(reward.getReward())    
    # testList = state.testPlayerPosition()
    
    # element 0: flipGravity (0.0 for don't, 1.0 for do)
    # element 1: angleOfCursor (0-360)
    # element 2: -1, 0, or 1 for slow, regular, fast speed
    # element 3: shoot (0.0 for don't, 1.0 for do)
    action = randomAction(playerPosition)
    return action

def Reset():
    pass

testString = "Hello there!"