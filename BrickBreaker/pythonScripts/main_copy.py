
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

RENDERING = True
LOAD = True

class BrickBreakerEnv(gym.Env):
    metadata = {"render_modes": ["human"], "render_fps": 30}
    
    def __init__(self, render_mode=None):
        super(BrickBreakerEnv, self).__init__()
        self.games = [brickbreaker.BrickBreaker(True, False)]
        
        # self.game = brickbreaker.BrickBreaker(True, False)
        # self.game.Init()
        # self.game.LoadContent()
        self.observation_space = spaces.Box(low=0.0, high=1.0, shape=(1, 6), dtype=np.float32)        
        self.action_space = spaces.Discrete(4)
        
        done = False
        # self.game.python_init(RENDERING, False)
        
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
        
        return handled_state[0]
        
        
        
    
    def step(self, action):
        # print("Stepping")
        # 100 microseconds have passed       
        state, reward, done, _, _ = self.games[0].step(action, self.update_time)
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
        
        self.games.append(brickbreaker.BrickBreaker(True, False))
        
        self.games[-1].Init()
        self.games[-1].LoadContent()
        self.games[-1].python_init(RENDERING, False)
                
        state, reward, done, _, _ = self.games[-1].step(self.action_space.sample(), self.update_time)
        self.reward = reward.getValue()
        
        state = self.handle_state(state)
        return state, {}
    
    def render(self, mode="human"):
        self.games[0].render(self.update_time)
        return
    
    def close(self):
        pass


# hyperparameters
hidden_size = 256
learning_rate = 3e-4

# Constants
GAMMA = 0.99
num_steps = 500000
max_episodes = 2000

class ActorCritic(nn.Module):
    def __init__(self, num_inputs, num_actions, hidden_size, learning_rate=3e-4):
        super(ActorCritic, self).__init__()

        self.num_actions = num_actions
        self.critic_linear1 = nn.Linear(num_inputs, hidden_size)
        self.critic_linear2 = nn.Linear(hidden_size, 1)

        self.actor_linear1 = nn.Linear(num_inputs, hidden_size)
        self.actor_linear2 = nn.Linear(hidden_size, num_actions)
    
    def forward(self, state):
        state = Variable(torch.from_numpy(state).float().unsqueeze(0))
        value = F.relu(self.critic_linear1(state))
        
        value = self.critic_linear2(value)
        policy_dist = F.relu(self.actor_linear1(state))
        policy_dist = F.softmax(self.actor_linear2(policy_dist), dim=1)

        return value, policy_dist
    
def a2c(env):
    num_inputs = env.observation_space.shape[1]
    num_outputs = env.action_space.n
    
    actor_critic = ActorCritic(num_inputs, num_outputs, hidden_size)
    ac_optimizer = optim.Adam(actor_critic.parameters(), lr=learning_rate)

    all_lengths = []
    average_lengths = []
    all_rewards = []
    entropy_term = 0
    best_length = 0
    
    if LOAD:
        print("LOADING")
        actor_critic.load_state_dict(torch.load("a2c_brickbreaker.pth"))
        print("LOADED!")
        
    for episode in range(max_episodes):
        log_probs = []
        values = []
        rewards = []
        state, _ = env.reset()
        for steps in range(num_steps):
            value, policy_dist = actor_critic(state)
            
            value = value.detach().numpy()[0,0]
            
            dist = policy_dist.detach().numpy() 

            action = np.random.choice(num_outputs, p=np.squeeze(dist))
            log_prob = torch.log(policy_dist.squeeze(0)[action])
            entropy = -np.sum(np.mean(dist) * np.log(dist))
            new_state, reward, done, _, _ = env.step(action)
            if RENDERING:
                env.render()
            rewards.append(reward)
            values.append(value)
            log_probs.append(log_prob)
            entropy_term += entropy
            state = new_state
            
            if done or steps == num_steps-1:
                Qval, _ = actor_critic.forward(new_state)
                Qval = Qval.detach().numpy()[0,0]
                all_rewards.append(np.sum(rewards))
                all_lengths.append(steps)
                average_lengths.append(np.mean(all_lengths[-10:]))
                if episode % 10 == 0:                    
                    sys.stdout.write("episode: {}, reward: {}, total length: {}, average length: {} \n".format(episode, np.sum(rewards), steps, average_lengths[-1]))
                
                if (average_lengths[-1] > best_length):
                    print(f"Improvement! Saved model ({best_length}->{average_lengths[-1]})")
                    best_length = average_lengths[-1]
                    
                    torch.save(actor_critic.state_dict(), "a2c_brickbreaker.pth")
                
                env.games[0].reset()
                env.games[0].python_init(RENDERING, True)
                break
        
        # compute Q values
        Qvals = np.zeros_like(values)
        for t in reversed(range(len(rewards))):
            Qval = rewards[t] + GAMMA * Qval
            Qvals[t] = Qval
  
        #update actor critic
        values = torch.FloatTensor(values)
        Qvals = torch.FloatTensor(Qvals)
        log_probs = torch.stack(log_probs)
        
        advantage = Qvals - values
        actor_loss = (-log_probs * advantage).mean()
        critic_loss = 0.5 * advantage.pow(2).mean()
        ac_loss = actor_loss + critic_loss + 0.001 * entropy_term

        ac_optimizer.zero_grad()
        ac_loss.backward()
        ac_optimizer.step()

        
    
    # Plot results
    smoothed_rewards = pd.Series.rolling(pd.Series(all_rewards), 10).mean()
    smoothed_rewards = [elem for elem in smoothed_rewards]
    plt.plot(all_rewards)
    plt.plot(smoothed_rewards)
    plt.plot()
    plt.xlabel('Episode')
    plt.ylabel('Reward')
    plt.show()

    plt.plot(all_lengths)
    plt.plot(average_lengths)
    plt.xlabel('Episode')
    plt.ylabel('Episode length')
    plt.show()
    
    
def StartGames():
    env =  BrickBreakerEnv()
    a2c(env)
    
if __name__ == "__main__":
    env =  BrickBreakerEnv()
    a2c(env)  