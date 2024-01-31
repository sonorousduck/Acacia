import gymnasium as gym
from gymnasium import spaces
import numpy as np
import random

from stable_baselines3 import A2C
from stable_baselines3.common.env_checker import check_env


class BrickBreaker:
    def __init__(self):
        self.sample_observation_space = spaces.Box(low=0.0, high=1.0, shape=(1,36), dtype=np.float32)
        self.action_space = spaces.Discrete(4)
        pass
    
    def step(self, action, timestep):
        return self.sample_observation_space.sample(), 0, False, False, {}
    
    def render(self, timestep):
        return


class BrickBreakerEnv(gym.Env):
    metadata = {"render_modes": ["human"], "render_fps": 30}
    
    def __init__(self, render_mode=None):
        super(BrickBreakerEnv, self).__init__()
        self.observation_space = spaces.Box(low=0.0, high=1.0, shape=(1,36), dtype=np.float32)
        self.action_space = spaces.Discrete(4)
        self.game = BrickBreaker()
    
    def handle_state(self, state):
        handled_state = np.zeros(shape=(1, 36), dtype=np.float32)
        paddlePosition = state[0][:4]
        paddlePosition[0] /= 480
        paddlePosition[1] /= 320
        
        ballPosition = state[0][4:8]
        
        ballPosition[0] /= 480
        ballPosition[1] /= 320
        # score = reward.getReward()
        brick_states = state[:][8:28]
        powerup_states = state[:][28:]

        handled_state[0][0] = paddlePosition[0]
        handled_state[0][1] = paddlePosition[1]
        handled_state[0][4] = ballPosition[0]
        handled_state[0][5] = ballPosition[1]
        
           
        for i, box in enumerate(brick_states):
            normalized_box = box
            normalized_box[0] /= 480
            normalized_box[1] /= 320
            
            index = i * 4 + 7
            
            handled_state[0][index] = normalized_box[0]
            handled_state[0][index + 1] = normalized_box[1]
                    
        for i, box in enumerate(powerup_states):
            normalized_box = box
            normalized_box[0] /= 480
            normalized_box[1] /= 320
            
            index = i * 4 + 27
            
            handled_state[0][index] = normalized_box[0]
            handled_state[0][index + 1] = normalized_box[1]
            
        return handled_state
    
    
    def step(self, action):
        print("Starting checking step")
        state, reward, done, _, _ = self.game.step(action, 10000)
        state = self.handle_state(state)
        print("Checked step!")
        
        return state, reward, done, done, {}

    def reset(self, seed=None, options=None):
        super().reset(seed=seed)
        
        state, reward, done, _, _ = self.game.step(self.action_space.sample(), 10000)
        
        state = self.handle_state(state)
        
        return state, {}
    
    def render(self, mode="human"):
        self.game.render(10000)
    
    def close(self):
        pass
    

if __name__ == "__main__":
    env = BrickBreakerEnv("human")
    
    # model = A2C("MlpPolicy", env)
    # model.learn(total_timesteps=10000, log_interval=4)
    # model.save("a2c_brickbreaker")
    print("Checking env")
    check_env(env)
    print("Checked!")