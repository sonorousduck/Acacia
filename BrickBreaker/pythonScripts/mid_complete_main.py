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
from stable_baselines3.common.buffers import ReplayBuffer

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
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

    buffer_size = 10000
    gamma = 0.99
    tau = 1.0
    target_network_frequency = 500
    batch_size = 128
    start_e = 1
    end_e = 0.05
    exploration_fraction = 0.5
    learning_starts = 10000
    train_frequency = 10
    games_to_play = 100000


    q_network = QNetwork(env).to(device)
    optimizer = optim.Adam(q_network.parameters(), lr=2.5e-4)
    target_network = QNetwork(env).to(device)
    target_network.load_state_dict(q_network.state_dict())
    rb = ReplayBuffer(
        buffer_size,
        env.single_observation_space,
        env.single_action_space,
        device,
        handle_timeout_termination=False,
    )
    
    state, _ = env.reset()
    i = 0
    global_steps = 0
    while i != games_to_play:
        global_steps += 1
        epsilon = linear_schedule(start_e, end_e, exploration_fraction * games_to_play, i)
        print("Going to take an action")
        if random.random() < epsilon:
            action = env.single_action_space.sample()
            print(action)
        else:
            q_values = q_network(torch.Tensor(state).to(device))
            action = torch.argmax(q_values, dim=1).cpu().numpy()
        
        print("Have my action")
        next_state, reward, done, _, info = env.step(action) 
        print("Acted")
        rb.add([state], [next_state], [action], [reward], [done], [info])
        print("Got here at least")
        
        state = next_state

        if global_steps > learning_starts:
            if global_steps % train_frequency == 0:
                data = rb.sample(batch_size)
                
                with torch.no_grad():
                    target_max, _ = target_network(data.next_observations).max(dim=1)
                    td_target = data.rewards.flatten() + gamma * target_max * (1 - data.dones.flatten())
                old_val = q_network(data.observations).gather(1, data.actions).squeeze()
                loss = F.mse_loss(td_target, old_val)
                optimizer.zero_grad()
                loss.backward()
                optimizer.step()
                
            if global_steps % target_network_frequency == 0:
                for target_network_param, q_network_param in zip(target_network.parameters(), q_network.parameters()):
                    target_network_param.data.copy_(
                        tau * q_network_param.data + (1.0 - tau) * target_network_param.data
                    )  
        
        model_path = f"runs/{global_steps}.pth"
        torch.save(q_network.state_dict(), model_path)
        print(f"model saved to {model_path}")  
        
        if done:
            env.game.reset()
            env.game.python_init(rendering, True)
            i += 1
            state, _ = env.reset()
    
    # model = SAC("MlpPolicy", env)
    # model.learn(total_timesteps=10000, log_interval=4)
    # model.save("sac_pendulum")
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