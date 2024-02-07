
import sys
import torch  
import gymnasium as gym
import numpy as np  
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
from torch.autograd import Variable
import matplotlib.pyplot as plt
import pandas as pd
from gymnasium import spaces
from stable_baselines3.common.vec_env import DummyVecEnv
from stable_baselines3.common.env_util import make_vec_env

from stable_baselines3 import A2C, PPO
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import os
import random
import time
import math
import matplotlib.pyplot as plt
from collections import namedtuple, deque
from itertools import count
import datetime
import argparse

import gymnasium as gym
from gymnasium import spaces
import pickle
import random
import numpy as np
import brickbreaker
import cpp_module

RENDERING = False
LOAD = True

class BrickBreakerEnv(gym.Env):
    metadata = {"render_modes": ["human"], "render_fps": 30}
    
    def __init__(self, render_mode=None):
        super(BrickBreakerEnv, self).__init__()
        # self.games = [brickbreaker.BrickBreaker(True, False)]
        
        self.game = brickbreaker.BrickBreaker(True, False)
        self.game.Init()
        self.game.LoadContent()
        self.observation_space = spaces.Box(low=0.0, high=1.0, shape=(1, 6), dtype=np.float32)        
        self.action_space = spaces.Discrete(4)
        self.render_mode = render_mode
        done = False
        
        if (render_mode == "human"):
            self.game.python_init(True, False)
        else:
            self.game.python_init(RENDERING, False)
                
        
        self.update_time = 16000
    
    def handle_state(self, state):
        handled_state = np.zeros(shape=(1, 6), dtype=np.float32)
        
        paddlePosition = state.getPaddlePosition().getBox()
        paddlePosition[0] /= 480
        paddlePosition[1] /= 320
        
        ballPosition = state.getBallPosition().getBox()
        
        ballPosition[0] /= 480
        ballPosition[1] /= 320
        # score = reward.getReward()
        # brick_states = state.getBrickPositions()
        # powerup_states = state.getPowerupPositions()
        handled_state[0][0] = paddlePosition[0]
        handled_state[0][1] = paddlePosition[1]
        handled_state[0][2] = ballPosition[0]
        handled_state[0][3] = ballPosition[1]
        handled_state[0][4] = ballPosition[2]
        handled_state[0][5] = ballPosition[3]
                
        # for i, box in enumerate(brick_states):
        #     normalized_box = box.getBox()
        #     normalized_box[0] /= 480
        #     normalized_box[1] /= 320
            
        #     index = i * 4 + 7
            
        #     handled_state[0][index] = normalized_box[0]
        #     handled_state[0][index + 1] = normalized_box[1]
                    
        # for i, box in enumerate(powerup_states):
        #     normalized_box = box.getBox()
        #     normalized_box[0] /= 480
        #     normalized_box[1] /= 320
            
        #     index = i * 4 + 27
            
        #     handled_state[0][index] = normalized_box[0]
        #     handled_state[0][index + 1] = normalized_box[1]
        print(handled_state[0])
        return handled_state[0]
        
        
        
    
    def step(self, action):
        # print("Stepping")
        # 100 microseconds have passed
        state, reward, done, _, _ = self.game.step(action, self.update_time)
        state = self.handle_state(state)
        
        
        # if RENDERING:
        #     self.render()
        # print("Finished step")
        # Advance the game a single frame
        # observation = self.handle_received_observation(self.game.getObservation())
        # # print("Might have failed getting an observation..?")
        # reward = self.game.getReward()
        # # print("Might have failed getting a reward..?")
        # done = self.game.getTerminated()
                
        # observation, reward, terminated, truncated, info
        return state, reward.getValue(), done, done, {}
        
        
    
    def reset(self, seed=None, options=None):
        super().reset(seed=seed)
        self.game.reset()
        self.game.python_init(True if self.render_mode == "human" else RENDERING, True)
        
        state, reward, done, _, _ = self.game.step(self.action_space.sample(), self.update_time)
        self.reward = reward.getValue()
        
        state = self.handle_state(state)
        return state, {}
    
    def render(self, mode="human"):
        self.game.render(self.update_time)
        return
    
    def close(self):
        pass
    
    
def StartGames():
    # env = make_vec_env(BrickBreakerEnv, n_envs=4)
    # # env = DummyVecEnv([lambda: BrickBreakerEnv()])
    # model = PPO("MlpPolicy", env, verbose=1)
    # model.learn(total_timesteps=5000000)
    # model.save("stable_baselines3_brickbreaker")
    # print("SAVED!")
    
    
    # del model
    # print("DELETED!")
    env = BrickBreakerEnv(render_mode="human")
    model = PPO.load("stable_baselines3_brickbreaker")
    obs, _ = env.reset()
    while True:
        action, _states = model.predict([obs])
        obs, rewards, dones, _, info = env.step(action)
        env.render()
        
        if dones:
            env.reset()
    
    # a2c(env)
    
    
    #     env = DummyVecEnv([lambda: BrickBreakerEnv()])
    # model = PPO("MlpPolicy", env, verbose=1)
    # model.learn(total_timesteps=200000)
    # model.save("stable_baselines3_brickbreaker")
    # print("SAVED!")