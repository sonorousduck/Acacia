import torch
import crypt
import datetime

def StartGames():
    print("Starting up")
    
    game = crypt.Crypt(True, False)
    print("Game successfully created")
    
    test_time = datetime.timedelta(microseconds=100)
    
    game.ProcessInput(test_time)


def Start():
   pass


if __name__ == "__main__":
    print("Hello there!")