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
import math
import matplotlib.pyplot as plt
from collections import namedtuple, deque
from itertools import count

# Self-implemented DQN

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
Transition = namedtuple('Transition',
                        ('state', 'action', 'next_state', 'reward'))
BATCH_SIZE = 128
GAMMA = 0.99
EPS_START = 0.9
EPS_END = 0.05
EPS_DECAY = 500000
TAU = 0.005
LR = 1e-4


class ReplayMemory(object):

    def __init__(self, capacity):
        self.memory = deque([], maxlen=capacity)

    def push(self, *args):
        """Save a transition"""
        self.memory.append(Transition(*args))

    def sample(self, batch_size):
        return random.sample(self.memory, batch_size)

    def __len__(self):
        return len(self.memory)
    

class DQN(nn.Module):

    def __init__(self, n_observations, n_actions):
        super(DQN, self).__init__()
        self.layer1 = nn.Linear(n_observations, 128)
        self.layer2 = nn.Linear(128, 128)
        self.layer3 = nn.Linear(128, n_actions)

    def forward(self, x):
        x = F.relu(self.layer1(x))
        x = F.relu(self.layer2(x))
        return self.layer3(x)

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
        
        self.update_time = 20000
    
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
        return state, {}
    
    def render(self, mode="human"):
        self.game.render(self.update_time)
        return
    
    def close(self):
        pass


class Agent:
    def __init__(self, env, num_observations, num_actions):
        self.steps_done = 0
        self.num_observations = num_observations
        self.num_actions = num_actions
        self.policy_net = DQN(self.num_observations, self.num_actions).to(device)
        self.target_net = DQN(self.num_observations, self.num_actions).to(device)
        self.target_net.load_state_dict(self.policy_net.state_dict())
        self.env = env
        self.optimizer = optim.AdamW(self.policy_net.parameters(), lr=LR, amsgrad=True)
        self.memory = ReplayMemory(100000)
        self.episode_durations = []
        self.rewards = []
    
    def select_action(self, state):
        sample = random.random()
        eps_threshold = EPS_END + (EPS_START - EPS_END) * \
            math.exp(-1. * self.steps_done / EPS_DECAY)
        print(eps_threshold)
        self.steps_done += 1
        if sample > eps_threshold:
            with torch.no_grad():
                # t.max(1) will return the largest column value of each row.
                # second column on max result is index of where max element was
                # found, so we pick action with the larger expected reward.
                # print("IS IT HERE?!")
                # test = self.policy_net(state)
                # print(test)
                # test1 = test.max(2)
                # print("Max: ")
                # print(test1)
                # print("Indicies")
                # test2 = test1.indices.squeeze().view(1, 1)
                # print(test2)
                return self.policy_net(state).max(2).indices.squeeze().view(1, 1)
            
        else:
            return torch.tensor([[self.env.action_space.sample()]], device=device, dtype=torch.long)

    def optimize_model(self):
        if len(self.memory) < BATCH_SIZE:
            return
        
        transitions = self.memory.sample(BATCH_SIZE)
        # Transpose the batch (see https://stackoverflow.com/a/19343/3343043 for
        # detailed explanation). This converts batch-array of Transitions
        # to Transition of batch-arrays.
        batch = Transition(*zip(*transitions))
        # Compute a mask of non-final states and concatenate the batch elements
        # (a final state would've been the one after which simulation ended)
        non_final_mask = torch.tensor(tuple(map(lambda s: s is not None,
                                            batch.next_state)), device=device, dtype=torch.bool)
        non_final_next_states = torch.cat([s for s in batch.next_state
                                                    if s is not None])
        state_batch = torch.cat(batch.state)
        action_batch = torch.cat(batch.action)
        reward_batch = torch.cat(batch.reward)

        # Compute Q(s_t, a) - the model computes Q(s_t), then we select the
        # columns of actions taken. These are the actions which would've been taken
        # for each batch state according to policy_net
        state_action_values = self.policy_net(state_batch).squeeze(1).gather(1, action_batch)

        # Compute V(s_{t+1}) for all next states.
        # Expected values of actions for non_final_next_states are computed based
        # on the "older" target_net; selecting their best reward with max(1).values
        # This is merged based on the mask, such that we'll have either the expected
        # state value or 0 in case the state was final.
        next_state_values = torch.zeros(BATCH_SIZE, device=device)
        with torch.no_grad():
            next_state_values[non_final_mask] = self.target_net(non_final_next_states).squeeze(1).max(1).values
        # Compute the expected Q values
        expected_state_action_values = (next_state_values * GAMMA) + reward_batch

        # Compute Huber loss
        criterion = nn.SmoothL1Loss()
        loss = criterion(state_action_values, expected_state_action_values.unsqueeze(1))

        # Optimize the model
        self.optimizer.zero_grad()
        loss.backward()
        # In-place gradient clipping
        torch.nn.utils.clip_grad_value_(self.policy_net.parameters(), 100)
        self.optimizer.step()
    
    
    def plot_durations(self, show_result=False):
        plt.figure(1)
        # durations_t = torch.tensor(self.episode_durations, dtype=torch.float)
        durations_t = torch.tensor(self.rewards, dtype=torch.float)
        
        if show_result:
            plt.title('Result')
        else:
            plt.clf()
            plt.title('Training...')
        plt.xlabel('Episode')
        plt.ylabel('Duration')
        plt.plot(durations_t.numpy())
        # Take 100 episode averages and plot them too
        if len(durations_t) >= 100:
            means = durations_t.unfold(0, 100, 1).mean(1).view(-1)
            means = torch.cat((torch.zeros(99), means))
            plt.plot(means.numpy())

        plt.pause(0.001)  # pause a bit so that plots are updated
        
def StartGames():
    # print("Starting up")
    
    rendering = True
    env = BrickBreakerEnv("human")

    num_actions = env.action_space.n
    state, _ = env.reset()
    num_observations = len(state[0])
    
    agent = Agent(env, num_observations, num_actions)
    
    epochs = 5000
    
    for i in range(epochs):
        state, _ = env.reset()
        state = torch.tensor(state, dtype=torch.float32, device=device).unsqueeze(0)    
        for t in count():
            action = agent.select_action(state)
            observation, reward, terminated, truncated, _ = env.step(action.item())
            
            env.render()
            reward = torch.tensor([reward], device=device)
            done = terminated or truncated
            if done:
                next_state = None
            else:
                next_state = torch.tensor(observation, dtype=torch.float32, device=device).unsqueeze(0)
            # Store the transition in memory
            agent.memory.push(state, action, next_state, reward)
            # Move to the next state
            state = next_state

            # Perform one step of the optimization (on the policy network)
            agent.optimize_model()
            # Soft update of the target network's weights
            # θ′ ← τ θ + (1 −τ )θ′
            target_net_state_dict = agent.target_net.state_dict()
            policy_net_state_dict = agent.policy_net.state_dict()
            for key in policy_net_state_dict:                
                target_net_state_dict[key] = policy_net_state_dict[key]*TAU + target_net_state_dict[key]*(1-TAU)
            agent.target_net.load_state_dict(target_net_state_dict)
            if done:
                agent.episode_durations.append(t + 1)
                agent.rewards.append(reward)
                agent.plot_durations()
                
                env.game.reset()
                env.game.python_init(rendering, True)
                break
    
    print("COMPLETE!")
    torch.save(agent.policy_net.state_dict(), "brick_breaker_agent.pth")
   
    agent.plot_durations(show_result=True)
    plt.ioff()
    plt.savefig("training.jpg")
    
    
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