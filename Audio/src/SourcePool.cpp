//#include "SourcePool.hpp"
//
//#include <iostream>
//
//EbonyAudio::SourcePool::SourcePool(AudioType audioType, std::uint16_t sourceCount) : poolType(audioType), sourceCount(sourceCount)
//{
//
//
//}
//
//std::unique_ptr<SoundSource> EbonyAudio::SourcePool::GetSource()
//{
//	if (this->sources.empty())
//	{
//		return nullptr;
//	}
//
//	std::unique_ptr<SoundSource> source = std::move(this->sources.back());
//	this->sources.pop_back();
//	
//	return std::move(source);
//}
//
//void EbonyAudio::SourcePool::Init(AudioType type)
//{
//	for (std::uint16_t i = 0; i < sourceCount; i++)
//	{
//		this->sources.push_back(std::move(std::make_unique<SoundSource>(SoundSource(type))));
//	}
//
//}
//
//void EbonyAudio::SourcePool::ReleaseSource(std::unique_ptr<SoundSource> source)
//{
//	this->sources.push_back(std::move(source));
//}
//
//void EbonyAudio::SourcePool::AddNewSource(std::unique_ptr<SoundSource> source)
//{
//	this->sources.push_back(std::move(source));
//}
//
//void EbonyAudio::SourcePool::DeleteSourcePermanently()
//{
//	this->sources.pop_back();
//}
