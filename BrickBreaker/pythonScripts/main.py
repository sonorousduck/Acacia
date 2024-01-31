import datetime
import argparse

import gymnasium as gym
from gymnasium import spaces
import pickle
import random
import numpy as np

import brickbreaker
import cpp_module

import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import os
import random
import time
from stable_baselines3 import A2C
from stable_baselines3.common.env_checker import check_env



class BrickBreakerEnv(gym.Env):
    metadata = {"render_modes": ["human"], "render_fps": 30}
    
    def __init__(self, render_mode=None):
        super(BrickBreakerEnv, self).__init__()
        self.game = brickbreaker.BrickBreaker(True, False)
        self.game.Init()
        self.game.LoadContent()
        self.observation_space = spaces.Box(low=0.0, high=1.0, shape=(1, 36), dtype=np.float32)        
        self.action_space = spaces.Discrete(4)
        
        done = False
        rendering = True
        self.game.python_init(rendering, False)
        
        self.update_time = 10000
    
    def handle_state(self, state):
        handled_state = np.zeros(shape=(1, 36), dtype=np.float32)
        
        paddlePosition = state.getPaddlePosition().getBox()
        paddlePosition[0] /= 480
        paddlePosition[1] /= 320
        
        ballPosition = state.getBallPosition().getBox()
        
        ballPosition[0] /= 480
        ballPosition[1] /= 320
        # score = reward.getReward()
        brick_states = state.getBrickPositions()
        powerup_states = state.getPowerupPositions()
        handled_state[0][0] = paddlePosition[0]
        handled_state[0][1] = paddlePosition[1]
        handled_state[0][4] = ballPosition[0]
        handled_state[0][5] = ballPosition[1]
    
        
        for i, box in enumerate(brick_states):
            normalized_box = box.getBox()
            normalized_box[0] /= 480
            normalized_box[1] /= 320
            
            index = i * 4 + 7
            
            handled_state[0][index] = normalized_box[0]
            handled_state[0][index + 1] = normalized_box[1]
                    
        for i, box in enumerate(powerup_states):
            normalized_box = box.getBox()
            normalized_box[0] /= 480
            normalized_box[1] /= 320
            
            index = i * 4 + 27
            
            handled_state[0][index] = normalized_box[0]
            handled_state[0][index + 1] = normalized_box[1]
        
        return handled_state
        
        
        
    
    def step(self, action):
        # print("Stepping")
        # 100 microseconds have passed       
        state, reward, done, _, _ = self.game.step(action, self.update_time)
        state = self.handle_state(state)
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
        
        
        state, reward, done, _, _ = self.game.step(self.action_space.sample(), self.update_time)
        
        state = self.handle_state(state)
        print("Success!")
        return state, {}
    
    def render(self, mode="human"):
        self.game.render(self.update_time)
        return
    
    def close(self):
        pass



def StartGames():
    # print("Starting up")
    
    rendering = True
    env = BrickBreakerEnv("human")

    
    model = SAC("MlpPolicy", env)
    model.learn(total_timesteps=10000, log_interval=4)
    model.save("sac_pendulum")
    # print("Checking env")
    # check_env(env)
    # print("Checked!")
    
    # state, _ = env.reset()
    
    # while not False:
    #     # print("Attempting a step")
    #     # Or agent step
    #     action = randomAction(state)
        
    #     state, reward, done, _, _ = env.step(action)
                
    #     if rendering:
    #         env.render()
        
    #     if done:
    #         env.game.reset()
    #         env.game.python_init(rendering, True)
            
    #         state, _ = env.reset()
            
        
    # env.close()
    
    
    # Run(env)

    # test_time = datetime.timedelta(microseconds=100)
    
    # game.ProcessInput(test_time)


# def Run(env: BrickBreakerEnv):
#     print("Running game")

    
#     state, _ = env.reset()
    
#     while not False:
#         # print("Attempting a step")
#         # Or agent step
#         action = randomAction(state)
        
#         state, reward, done, _, _ = env.step(action)
                
#         if rendering:
#             env.render()
        
#         if done:
#             env.game.reset()
#             env.game.python_init(rendering, True)
            
#             state, _ = env.reset()
            
        
#     env.close()


def Start():
   pass


if __name__ == "__main__":
    print("Hello there!")