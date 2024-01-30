import torch
import datetime
import argparse

import gymnasium as gym
from gymnasium import spaces
import pickle
import random
import numpy as np

import brickbreaker
import cpp_module

from stable_baselines3 import A2C
from stable_baselines3.common.env_util import make_vec_env
from stable_baselines3.common.env_checker import check_env


def randomAction(state):
    action = cpp_module.Discrete()
    movement = np.random.randint(0, 4)
    action.setValue(movement)
    return action

def randomActionNoState():
    action = cpp_module.Discrete()  
    movement = np.random.randint(0, 4)
    
    action.setValue(movement)
    
    return action

class BrickBreakerEnv(gym.Env):
    metadata = {"render_modes": ["human"], "render_fps": 30}
    
    def __init__(self, render_mode=None):
        super(BrickBreakerEnv, self).__init__()
        self.game = brickbreaker.BrickBreaker(True, False)
        self.game.Init()
        self.game.LoadContent()
        self.observation_space = spaces.Box(low=0.0, high=1.0, shape=(36, 1), dtype=np.float32)
        self.action_space = spaces.Discrete(4)
        
        done = False
        rendering = True
        self.game.python_init(rendering, False)
    
    def handle_received_observation(self, state):
        handled_state = []
        
        paddlePosition = state.getPaddlePosition().getBox()
        paddlePosition[0] /= 480
        paddlePosition[1] /= 320
        
        ballPosition = state.getBallPosition().getBox()
        
        ballPosition[0] /= 480
        ballPosition[1] /= 320
        # score = reward.getReward()
        brick_states = state.getBrickPositions()
        powerup_states = state.getPowerupPositions()
        
        while len(brick_states) != 5:
            brick_states.append(cpp_module.Box())
        
        while len(powerup_states) != 5:
            powerup_states.append(cpp_module.Box())
                
        # reward_structure.setReward(score)

        
        
        handled_state.extend(paddlePosition)
        handled_state.extend(ballPosition)
        
        for i in brick_states:
            normalized_box = i.getBox()
            normalized_box[0] /= 480
            normalized_box[1] /= 320
            handled_state.extend(normalized_box)
        
        for i in powerup_states:
            normalized_box = i.getBox()
            normalized_box[0] /= 480
            normalized_box[1] /= 320
            
            handled_state.extend(normalized_box)
        
        return handled_state
        # rewardAmount = reward_structure.reward
        
    def step(self, action):
        # print("Stepping")
        # 100 microseconds have passed
        state, reward, done, _, _ = self.game.step(action, 10000)
        # print("Finished step")
        # Advance the game a single frame
        # observation = self.handle_received_observation(self.game.getObservation())
        # # print("Might have failed getting an observation..?")
        # reward = self.game.getReward()
        # # print("Might have failed getting a reward..?")
        # done = self.game.getTerminated()
        
                
        # observation, reward, terminated, truncated, info
        return state, reward, done, False, {}
        
        
    
    def reset(self, seed=None, options=None):
        super().reset(seed=seed)
        
        
        
        state, reward, done, _, _ = self.game.step(randomActionNoState(), 10000)
        observation = self.handle_received_observation(state)
        
        # Returns observation and information
        return observation, {}
    
    def render(self, mode="human"):
        self.game.render(10000)
    
    def close(self):
        pass



def StartGames():
    # print("Starting up")
    
    rendering = True
    env = BrickBreakerEnv("human")
    # model = SAC("MlpPolicy", env)
    # model.learn(total_timesteps=10000, log_interval=4)
    # model.save("sac_pendulum")
    # print("Checking env")
    # check_env(env)
    # print("Checked!")
    
    vec_env = make_vec_env(env, n_envs=1)
    model = A2C("MlpPolicy", vec_env, verbose=1)
    model.learn(total_timesteps=25000)
    model.save("a2c_brickbreaker")

    
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