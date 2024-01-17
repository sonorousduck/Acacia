import numpy as np
import argparse
# from torch.utils.tensorboard import SummaryWriter
import datetime
import pickle
import random
import os

from .agent import SAC
import cpp_module


class ReplayMemory:
    def __init__(self, capacity, seed):
        random.seed(seed)
        self.capacity = capacity
        self.buffer = []
        self.position = 0

    def push(self, state, action, reward, next_state, done):
        if len(self.buffer) < self.capacity:
            self.buffer.append(None)
        self.buffer[self.position] = (state, action, reward, next_state, done)
        self.position = (self.position + 1) % self.capacity

    def sample(self, batch_size):
        batch = random.sample(self.buffer, batch_size)
        state, action, reward, next_state, done = map(np.stack, zip(*batch))
        return state, action, reward, next_state, done

    def __len__(self):
        return len(self.buffer)

    def save_buffer(self, env_name, suffix="", save_path=None):
        if not os.path.exists('checkpoints/'):
            os.makedirs('checkpoints/')

        if save_path is None:
            save_path = "checkpoints/sac_buffer_{}_{}".format(env_name, suffix)
        print('Saving buffer to {}'.format(save_path))

        with open(save_path, 'wb') as f:
            pickle.dump(self.buffer, f)

    def load_buffer(self, save_path):
        print('Loading buffer from {}'.format(save_path))

        with open(save_path, "rb") as f:
            self.buffer = pickle.load(f)
            self.position = len(self.buffer) % self.capacity


class SARSA:
    def __init__(self):
        self.state = []
        self.next_state = []
        self.reward = 0
        self.action = []
        self.total_steps = 0
        self.updates = 0
    
    def handle_received_info(self, state, reward, mask):
        # Convert state to just a plain list
        handled_state = []
        
        
        playerPosition = state.getPlayerPosition()
        playerPosition[0] /= 480
        playerPosition[1] /= 320
        playerPosition[2] /= 480
        playerPosition[3] /= 320
        
        score = reward.getReward()
        enemy_states = state.getEnemyPositions()[:5]
        bullet_states = state.getBulletInformation()[:5]
        
        while len(enemy_states) != 5:
            enemy_states.append(cpp_module.Box())
        
        while len(bullet_states) != 5:
            bullet_states.append(cpp_module.Box())
                
        reward_structure.setReward(score)

        
        
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
        
        rewardAmount = reward_structure.reward
        
        
        if (self.state == []):
            self.state = handled_state
        else:
            self.push_to_memory(handled_state, rewardAmount, mask)

    def set_action(self, action):
        self.action = action
        self.total_steps += 1

    def push_to_memory(self, next_state, reward, mask):
        memory.push(self.state, self.action, reward, next_state, mask)
        
        self.state = next_state
        self.reward = reward
        
        

class RewardStructure:
    def __init__(self):
        self.reward = 0
        self.last_reward = 0
        self.highest_reward = 0
        self.cumulative_reward = 0
    
    def setReward(self, newReward):
        self.last_reward = self.reward
        self.reward = newReward - self.last_reward
        self.cumulative_reward += self.reward
        
def randomAction():
    action = cpp_module.Box()
    
    shouldFlipGravity = 0.0 if np.random.random() < 0.5 else 1.0
    cursorAngle = np.random.randint(0, 360)
    random_int = np.random.randint(0, 3)
    speed_control = random_int - 1.0
    shouldShoot = 0.0 if np.random.random() < 0.5 else 1.0
    
    action.setBox(shouldFlipGravity, cursorAngle, speed_control, shouldShoot)
    
    return action

def Act():
    state = sarsa.state
    if args.start_steps > sarsa.total_steps:
        action = randomAction()  # Sample random action
    else:
        test = agent.select_action(state)
        action = cpp_module.Box()  # Sample action from policy
        action.setBox(*test)
    if len(memory) > args.batch_size:
        # Number of updates per step in environment
        for i in range(args.updates_per_step):
            # Update parameters of all the networks
            critic_1_loss, critic_2_loss, policy_loss, ent_loss, alpha = agent.update_parameters(memory, args.batch_size, sarsa.updates)
            # print(f"Loss/critic_1: ${critic_1_loss}")
            # print(f"Loss/critic_2: ${critic_2_loss}")
            # print(f"Loss/policy: ${policy_loss}")
            # print(f"Loss/entropy_loss: ${ent_loss}")
            # print(f"entropy_temprature/alpha: ${alpha}")
            
            # writer.add_scalar('loss/critic_1', critic_1_loss, updates)
            # writer.add_scalar('loss/critic_2', critic_2_loss, updates)
            # writer.add_scalar('loss/policy', policy_loss, updates)
            # writer.add_scalar('loss/entropy_loss', ent_loss, updates)
            # writer.add_scalar('entropy_temprature/alpha', alpha, updates)
            sarsa.updates += 1
    
    return action
    


def Start():
    pass

# State will act as previous state of the last frame (since we just got the reward to go along with it, as well as the state for the current frame
def Update(state, reward):
    
    sarsa.handle_received_info(state, reward, 1)
    # testList = state.testPlayerPosition()
    action = Act()
    sarsa.set_action(action.getBox())
    
    
    # element 0: flipGravity (0.0 for don't, 1.0 for do)
    # element 1: angleOfCursor (0-360)
    # element 2: -1, 0, or 1 for slow, regular, fast speed
    # element 3: shoot (0.0 for don't, 1.0 for do)
    # action = randomAction()
    return action

def Reset(state, reward):
    # This will be called when the game terminates.
    print("RESETTING!") 
    sarsa.handle_received_info(state, reward, 0)
    
    
    
    reward_structure.last_reward = 0
    reward_structure.reward = 0
    print(reward_structure.cumulative_reward)
    print(reward_structure.highest_reward)
    
    if reward_structure.cumulative_reward > reward_structure.highest_reward:
        print(f"New highest reward so far: {reward_structure.cumulative_reward}")
        reward_structure.highest_reward = reward_structure.cumulative_reward
        agent.save_checkpoint("crypt", f"{int(reward_structure.highest_reward)}")
    reward_structure.cumulative_reward = 0
    # testList = state.testPlayerPosition()
    
    
    
    # element 0: flipGravity (0.0 for don't, 1.0 for do)
    # element 1: angleOfCursor (0-360)
    # element 2: -1, 0, or 1 for slow, regular, fast speed
    # element 3: shoot (0.0 for don't, 1.0 for do)
    # action = randomAction()


reward_structure = RewardStructure()

parser = argparse.ArgumentParser(description='PyTorch Soft Actor-Critic Args')
parser.add_argument('--env-name', default="HalfCheetah-v2",
                    help='Mujoco Gym environment (default: HalfCheetah-v2)')
parser.add_argument('--policy', default="Gaussian",
                    help='Policy Type: Gaussian | Deterministic (default: Gaussian)')
parser.add_argument('--eval', type=bool, default=True,
                    help='Evaluates a policy a policy every 10 episode (default: True)')
parser.add_argument('--gamma', type=float, default=0.99, metavar='G',
                    help='discount factor for reward (default: 0.99)')
parser.add_argument('--tau', type=float, default=0.005, metavar='G',
                    help='target smoothing coefficient(τ) (default: 0.005)')
parser.add_argument('--lr', type=float, default=0.0003, metavar='G',
                    help='learning rate (default: 0.0003)')
parser.add_argument('--alpha', type=float, default=0.2, metavar='G',
                    help='Temperature parameter α determines the relative importance of the entropy\
                            term against the reward (default: 0.2)')
parser.add_argument('--automatic_entropy_tuning', type=bool, default=False, metavar='G',
                    help='Automaically adjust α (default: False)')
parser.add_argument('--seed', type=int, default=123456, metavar='N',
                    help='random seed (default: 123456)')
parser.add_argument('--batch_size', type=int, default=256, metavar='N',
                    help='batch size (default: 256)')
parser.add_argument('--num_steps', type=int, default=1000001, metavar='N',
                    help='maximum number of steps (default: 1000000)')
parser.add_argument('--hidden_size', type=int, default=256, metavar='N',
                    help='hidden size (default: 256)')
parser.add_argument('--updates_per_step', type=int, default=1, metavar='N',
                    help='model updates per simulator step (default: 1)')
parser.add_argument('--start_steps', type=int, default=10000, metavar='N',
                    help='Steps sampling random actions (default: 10000)')
parser.add_argument('--target_update_interval', type=int, default=1, metavar='N',
                    help='Value target update per no. of updates per step (default: 1)')
parser.add_argument('--replay_size', type=int, default=1000000, metavar='N',
                    help='size of replay buffer (default: 10000000)')
parser.add_argument('--cuda', action="store_true",
                    help='run on CUDA (default: False)')
args = parser.parse_args()


agent = SAC(44, 4, args)
action = []
# writer = SummaryWriter('runs/{}_SAC_{}_{}_{}'.format(datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S"), args.env_name,
                                                            # args.policy, "autotune" if args.automatic_entropy_tuning else ""))
memory = ReplayMemory(args.replay_size, args.seed)

sarsa = SARSA()
