import torch
import datetime
import argparse

import gymnasium as gym
from gymnasium import spaces
import pickle
import random
import numpy as np

import crypt
import cpp_module

from stable_baselines3 import SAC


def randomAction(state):
    action = cpp_module.Box()
    
    shouldFlipGravity = 0.0 if np.random.random() < 0.5 else 1.0
    cursorAngle = np.random.randint(0, 360)
    random_int = np.random.randint(0, 3)
    speed_control = random_int - 1.0
    shouldShoot = 0.0 if np.random.random() < 0.5 else 1.0
    
    action.setBox(shouldFlipGravity, cursorAngle, speed_control, shouldShoot)
    
    return action

def randomActionNoState():
    action = cpp_module.Box()
    
    shouldFlipGravity = 0.0 if np.random.random() < 0.5 else 1.0
    cursorAngle = np.random.randint(0, 360)
    random_int = np.random.randint(0, 3)
    speed_control = random_int - 1.0
    shouldShoot = 0.0 if np.random.random() < 0.5 else 1.0
    
    action.setBox(shouldFlipGravity, cursorAngle, speed_control, shouldShoot)
    
    return action

class CryptEnv(gym.Env):
    metadata = {"render_modes": ["human"], "render_fps": 30}
    
    def __init__(self, render_mode=None):
        self.game = crypt.Crypt(True, False)
        self.game.Init()
        self.game.LoadContent()
        self.observation_space = spaces.Box(low=0.0, high=1.0, shape=(44, 1), dtype=np.float32)
        self.action_space = spaces.Box(low=np.array([0, 0, -1, 0]), high=np.array([1, 359, 1, 1]), dtype=np.int16)
    
    def handle_received_observation(self, state):
        handled_state = []
        
        
        playerPosition = state.getPlayerPosition()
        playerPosition[0] /= 480
        playerPosition[1] /= 320
        playerPosition[2] /= 480
        playerPosition[3] /= 320
        
        # score = reward.getReward()
        enemy_states = state.getEnemyPositions()[:5]
        bullet_states = state.getBulletInformation()[:5]
        
        while len(enemy_states) != 5:
            enemy_states.append(cpp_module.Box())
        
        while len(bullet_states) != 5:
            bullet_states.append(cpp_module.Box())
                
        # reward_structure.setReward(score)

        
        
        handled_state.extend(playerPosition)
        
        for i in enemy_states:
            normalized_box = i.getBox()
            normalized_box[0] /= 480
            normalized_box[1] /= 320
            handled_state.extend(normalized_box)
        
        for i in bullet_states:
            normalized_bullet = i.getBox()
            normalized_bullet[0] /= 480
            normalized_bullet[1] /= 320
            normalized_bullet[2] /= 30
            normalized_bullet[3] /= 30
            
            handled_state.extend(normalized_bullet)
        
        return handled_state
        # rewardAmount = reward_structure.reward
        
    def step(self, action):
        # print("Stepping")
        # 100 microseconds have passed
        state, reward, done, _, _ = self.game.step(action, 50000)
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
        
        
        
        self.game.step(randomActionNoState(), 50000)
        observation = self.handle_received_observation(self.game.getObservation())
        # Returns observation and information
        return observation, {}
    
    def render(self):
        self.game.render(50000)
    
    def close(self):
        pass



def StartGames():
    # print("Starting up")
    
    rendering = True
    
    env = CryptEnv("human")

    # model = SAC("MlpPolicy", env)
    # model.learn(total_timesteps=10000, log_interval=4)
    # model.save("sac_pendulum")
    
    
    
    Run(env)

    # test_time = datetime.timedelta(microseconds=100)
    
    # game.ProcessInput(test_time)


def Run(env: CryptEnv):
    print("Running game")
    done = False
    rendering = True
    env.game.python_init(rendering, False)
    
    state, _ = env.reset()
    while not False:
        # print("Attempting a step")
        # Or agent step
        action = randomAction(state)
        
        state, reward, done, _, _ = env.step(action)
                
        if rendering:
            env.render()
        
        if done:
            print("RESETTING!")
            env.game.reset()
            env.game.python_init(rendering, True)
            
            state, _ = env.reset()
            
        
    env.close()


def Start():
   pass


if __name__ == "__main__":
    print("Hello there!")