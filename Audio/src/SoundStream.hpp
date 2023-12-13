//#pragma once
//
//#include "SoundSource.hpp"
//
//namespace EbonyAudio
//{
//	class SoundStream
//	{
//	public:
//		SoundStream(std::unique_ptr<SoundSource> newSpeaker, ALuint sound) : sound(sound)
//		{
//			speaker = std::move(newSpeaker);
//			speaker->Play(sound);
//		}
//
//		std::unique_ptr<SoundSource> speaker;
//		ALuint sound;
//
//	private:
//	};
//}