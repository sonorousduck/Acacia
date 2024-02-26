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
from stable_baselines3.common.callbacks import CheckpointCallback
import spaceguy
import cpp_module

RENDERING = True
LOAD = True

class SpaceGuyEnv(gym.Env):
    metadata = {"render_modes": ["human"], "render_fps": 30}
    
    def __init__(self, render_mode=None):
        super(SpaceGuyEnv, self).__init__()
        # self.games = [brickbreaker.BrickBreaker(True, False)]
        
        self.game = spaceguy.SpaceGuy(True, False)
        self.game.Init()
        self.game.LoadContent()
        self.observation_space = spaces.Box(low=0.0, high=1.0, shape=(1, 45), dtype=np.float32)
        self.action_space = spaces.Box(low=0.0, high=1.0, shape=(1, 5), dtype=np.float32)
        self.render_mode = render_mode
        done = False
        
        self.previous_reward = 0
        
        if (render_mode == "human"):
            self.game.python_init(True, False)
        else:
            self.game.python_init(RENDERING, False)
        
        self.update_time = 20000
    
    def handle_state(self, state):
        handled_state = np.zeros(shape=(1, 45), dtype=np.float32)
        playerPosition = state.getPlayerPosition()
        keyPosition = state.getKeyInformation()
        
        playerPosition[0] /= 3000
        playerPosition[1] /= 800
        playerPosition[2] /= 360
        
        
        keyPosition[0] /= 3000
        keyPosition[1] /= 800
        
        
        
        handled_state[0][0] = playerPosition[0]
        handled_state[0][1] = playerPosition[1]
        handled_state[0][2] = playerPosition[2]
        handled_state[0][3] = keyPosition[0]
        handled_state[0][4] = keyPosition[1]
        # score = reward.getReward()
        enemy_states = state.getEnemyPositions()[:5]
        bullet_states = state.getBulletInformation()[:5]
        enemy_states_array = []
        bullet_states_array = []
        
        for i in enemy_states:
            normalized_box = i.getBox()
            normalized_box[0] /= 3000
            normalized_box[1] /= 800
            
            enemy_states_array.extend(normalized_box)
        
        for i in bullet_states:
            normalized_bullet = i.getBox()
            normalized_bullet[0] /= 3000
            normalized_bullet[1] /= 800
            normalized_bullet[2] /= 200
            normalized_bullet[3] /= 200
            
            bullet_states_array.extend(normalized_bullet)
        
        while len(enemy_states_array) != 20:
            enemy_states_array.append(0.0)
                    
        
        while len(bullet_states_array) != 20:
            bullet_states_array.append(0.0)
        
        for i, box in enumerate(enemy_states_array):
            index = i + len(playerPosition) + 1
            handled_state[0][index] = box
        
        for i, box in enumerate(bullet_states_array):
            index = i + len(playerPosition) + len(bullet_states_array) # Add player position amount and add enemy state amount
            
            handled_state[0][index] = box
        return handled_state[0]
        
        
        
    
    def step(self, action):
       # 100 microseconds have passed
        
        action_for_engine = cpp_module.Action()
        action_for_engine.setAction(*(action[0]))      
        state, reward, done, _, _ = self.game.step(action_for_engine, self.update_time)
        state = self.handle_state(state)
        
        reward = reward.getValue()
        
        temp_reward = reward - self.previous_reward
        self.previous_reward = reward
        self.reward = temp_reward
        
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
        return state, self.reward, done, done, {}
        
        
    
    def reset(self, seed=None, options=None):
        super().reset(seed=seed)
        self.game.reset()
        self.game.python_init(True if self.render_mode == "human" else RENDERING, True)
        action = self.action_space.sample()[0]
        action_for_engine = cpp_module.Action()
        action_for_engine.setAction(*action)
        
        state, reward, done, _, _ = self.game.step(action_for_engine, self.update_time)
        self.reward = reward.getValue()
        self.previous_reward = self.reward
        state = self.handle_state(state)
        return state, {}
    
    def render(self, mode="human"):
        self.game.render(self.update_time)
        return
    
    def close(self):
        pass
    
    
def StartGames():
    # checkpoint_callback = CheckpointCallback(
    #     save_freq=50000,
    #     save_path="./logs/",
    #     name_prefix="rl_model",
    #     save_replay_buffer=True,
    #     save_vecnormalize=True,
    # )
    
    # env = make_vec_env(SpaceGuyEnv, n_envs=6)
    # model = PPO("MlpPolicy", env, verbose=1, tensorboard_log="./spaceguy_tensorboard/")
    
    # model.learn(total_timesteps=10_000_000, callback=checkpoint_callback)
    # model.save("stable_baselines3_spaceguy_ppo_new_frame_reward")
    # print("SAVED!")
    
    
    # model = PPO.load("logs/rl_model_7400000_steps")
    # print("Loaded")
    # model.set_env(env)
    # # model = SAC("MlpPolicy", env, verbose=1)
    # env = CryptEnv(render_mode="human")
    # print(check_env(env))
    # print("CHECKED ENV!")
    # state, _ = env.reset()
    
    # env.step(env.action_space.sample())
    
    # env = DummyVecEnv([lambda: CryptEnv("human")])

    
    
    
    
    # del model
    # print("DELETED!")
    env = SpaceGuyEnv(render_mode="human")
    # model = PPO.load("logs/rl_model_960000_steps")
    model = PPO.load("logs/rl_model_1500000_steps")
    
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