import gymnasium as gym
import numpy as np  
from gymnasium import spaces
from stable_baselines3.common.vec_env import DummyVecEnv
from stable_baselines3.common.env_util import make_vec_env
from stable_baselines3.common.env_checker import check_env
from stable_baselines3 import A2C, PPO, SAC

import gymnasium as gym
from gymnasium import spaces
import numpy as np

import crypt
import cpp_module

RENDERING = True
LOAD = True

class CryptEnv(gym.Env):
    metadata = {"render_modes": ["human"], "render_fps": 30}
    
    def __init__(self, render_mode=None):
        super(CryptEnv, self).__init__()
        # self.games = [brickbreaker.BrickBreaker(True, False)]
        
        self.game = crypt.Crypt(True, False)
        self.game.Init()
        self.game.LoadContent()
        self.observation_space = spaces.Box(low=0.0, high=1.0, shape=(1, 44), dtype=np.float32)
        self.action_space = spaces.Box(low=0.0, high=1.0, shape=(1, 4), dtype=np.float32)
        self.render_mode = render_mode
        done = False
        
        if (render_mode == "human"):
            self.game.python_init(True, False)
        else:
            self.game.python_init(RENDERING, False)
                
        
        self.update_time = 16000
    
    def handle_state(self, state):
        handled_state = np.zeros(shape=(1, 44), dtype=np.float32)
        playerPosition = state.getPlayerPosition()
        playerPosition[0] /= 480
        playerPosition[1] /= 320
        playerPosition[2] /= 480
        playerPosition[3] /= 320
        
        
        handled_state[0][0] = playerPosition[0]
        handled_state[0][1] = playerPosition[1]
        handled_state[0][2] = playerPosition[2]
        handled_state[0][3] = playerPosition[3]
        # score = reward.getReward()
        enemy_states = state.getEnemyPositions()[:5]
        bullet_states = state.getBulletInformation()[:5]
        enemy_states_array = []
        bullet_states_array = []
        
        for i in enemy_states:
            normalized_box = i.getBox()
            normalized_box[0] /= 480
            normalized_box[1] /= 320
            
            enemy_states_array.extend(normalized_box)
        
        for i in bullet_states:
            normalized_bullet = i.getBox()
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
       # 100 microseconds have passed
        
        action_for_engine = cpp_module.Box()
        action_for_engine.setBox(*(action[0]))      
        state, reward, done, _, _ = self.game.step(action_for_engine, self.update_time)
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
        print("Resetting!")
        self.game.reset()
        self.game.python_init(True if self.render_mode == "human" else RENDERING, True)
                       
        action = self.action_space.sample()[0]
        action_for_engine = cpp_module.Box()
        action_for_engine.setBox(*action)
        
        state, reward, done, _, _ = self.game.step(action_for_engine, self.update_time)
        self.reward = reward.getValue()
        state = self.handle_state(state)
        return state, {}
    
    def render(self, mode="human"):
        self.game.render(self.update_time)
        return
    
    def close(self):
        pass
    
    
def StartGames():
    # env = make_vec_env(CryptEnv, n_envs=4)

    # env = CryptEnv(render_mode="human")
    # print(check_env(env))
    # print("CHECKED ENV!")
    # state, _ = env.reset()
    
    # env.step(env.action_space.sample())
    
    # env = DummyVecEnv([lambda: CryptEnv("human")])
    # model = SAC("MlpPolicy", env, verbose=1)
    # model.learn(total_timesteps=50000)
    # model.save("stable_baselines3_crypt_sac")
    # # print("SAVED!")
    
    
    
    
    # del model
    # print("DELETED!")
    env = CryptEnv(render_mode="human")
    model = SAC.load("stable_baselines3_crypt_sac")
    obs, _ = env.reset()
    while True:
        action = env.action_space.sample()
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