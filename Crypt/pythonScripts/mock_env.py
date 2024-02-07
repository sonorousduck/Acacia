import gymnasium as gym
from gymnasium import spaces
import numpy as np
import random

from stable_baselines3 import A2C, SAC
from stable_baselines3.common.env_checker import check_env
import os
import random
import time
import math
import matplotlib.pyplot as plt
from collections import namedtuple, deque
from itertools import count
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from stable_baselines3.common.vec_env import DummyVecEnv
from stable_baselines3.common.env_util import make_vec_env

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
Transition = namedtuple('Transition',
                        ('state', 'action', 'next_state', 'reward'))

BATCH_SIZE = 128
GAMMA = 0.99
EPS_START = 0.9
EPS_END = 0.05
EPS_DECAY = 1000
TAU = 0.005
LR = 1e-4


class Crypt:
    def __init__(self):
        self.observation_space = spaces.Box(low=0.0, high=1.0, shape=(1, 44), dtype=np.float32)
        self.action_space = spaces.Box(low=np.array([0, 0, -1, 0]), high=np.array([1, 359, 1, 1]), dtype=np.int16)
        
    
    def step(self, action, timestep):
        return self.observation_space.sample(), 0, False, False, {}
    
    def render(self, timestep):
        return


class CryptEnv(gym.Env):
    metadata = {"render_modes": ["human"], "render_fps": 30}
    
    def __init__(self, render_mode=None):
        super(CryptEnv, self).__init__()
        self.observation_space = spaces.Box(low=0.0, high=1.0, shape=(1, 44), dtype=np.float32)
        self.action_space = spaces.Box(low=np.array([0, 0, -1, 0]), high=np.array([1, 359, 1, 1]), dtype=np.int16)
        self.game = Crypt()
    
    def handle_state(self, state):
        handled_state = np.zeros(shape=(1, 44), dtype=np.float32)
        playerPosition = state[0][0:4]
        playerPosition[0] /= 480
        playerPosition[1] /= 320
        playerPosition[2] /= 480
        playerPosition[3] /= 320
        
        handled_state[0][0] = playerPosition[0]
        handled_state[0][1] = playerPosition[1]
        handled_state[0][2] = playerPosition[2]
        handled_state[0][3] = playerPosition[3]
        
        # score = reward.getReward()
        enemy_states = state[0][4:24]
        bullet_states = state[0][24:44]
        
        enemy_states_array = []
        bullet_states_array = []
        
        for i in range(0, len(enemy_states), 2):
            # normalized_box = i.getBox()
            normalized_box = enemy_states[i:i+2]
            normalized_box[0] /= 480
            normalized_box[1] /= 320
            
            enemy_states_array.extend(normalized_box)
        
        for i in range(0, len(bullet_states), 4):
            normalized_bullet = bullet_states[i:i+4]
 
            # normalized_bullet = i.getBox()
            normalized_bullet[0] /= 480
            normalized_bullet[1] /= 320
            normalized_bullet[2] /= 30
            normalized_bullet[3] /= 30
            
            bullet_states_array.extend(normalized_bullet)
        
        while len(enemy_states_array) != 20:
            enemy_states_array.append(0.0)
                    
        
        while len(bullet_states_array) != 20:
            bullet_states_array.append(0.0)
        
        
        for i, box in enumerate(enemy_states_array):
            index = i + len(playerPosition)
            handled_state[0][index] = box
        
        for i, box in enumerate(bullet_states_array):
            index = i + len(playerPosition) + len(bullet_states_array) # Add player position amount and add enemy state amount
            
            handled_state[0][index] = box
                
        return handled_state[0]
    
    
    def step(self, action):
        state, reward, done, _, _ = self.game.step(action, 10000)
        state = self.handle_state(state)
        print("Stepping")
        
        
        # if np.random.randint(0, 100) > 90:
        #     done = True
        
        return state, reward, done, done, {}

    def reset(self, seed=None, options=None):
        super().reset(seed=seed)
        print("Resetting")
        state, reward, done, _, _ = self.game.step(self.action_space.sample(), 10000)
        
        state = self.handle_state(state)
        
        return state, {}
    
    def render(self, mode="human"):
        print("Rendering")
        
        self.game.render(10000)
    
    def close(self):
        pass
    

if __name__ == "__main__":
    # env = CryptEnv("human")
    
    env = DummyVecEnv([lambda: CryptEnv()])
    
    model = SAC("MlpPolicy", env, verbose=1)
    model.learn(total_timesteps=10000)
    model.save("Mock_env_crypt")
    
    # num_actions = env.action_space.shape[0]
    # state, _ = env.reset()
    # num_observations = len(state)
        
    # epochs = 100
    
    # for i in range(epochs):
    #     state, _ = env.reset()
    #     state = torch.tensor(state, dtype=torch.float32, device=device).unsqueeze(0)    
    #     print("Created state as a tensor")
    #     for t in count():
    #         action = env.action_space.sample()
    #         observation, reward, terminated, truncated, _ = env.step(action)
            
    #         env.render()
            # Move to the next state
                

    # model = A2C("MlpPolicy", env)
    # model.learn(total_timesteps=10000, log_interval=4)
    # model.save("a2c_brickbreaker")
