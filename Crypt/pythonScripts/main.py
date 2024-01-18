import torch
import crypt


def Start():
    print("Starting up")
    
    game = crypt.Crypt(True, False)
    print("Game successfully created")


if __name__ == "__main__":
    print("Hello there!")