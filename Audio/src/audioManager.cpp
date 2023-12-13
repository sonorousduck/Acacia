//#include "audioManager.hpp"
//#include "EmptySoundDevice.hpp"
//#include <alext.h>
//#include <inttypes.h>
//#include "SoundStream.hpp"
//
//
//// NOTE: It looks like 256 is a good amount of sources to allow playing at one time
//// https://github.com/MonoGame/MonoGame/issues/4403
//
//namespace EbonyAudio
//{
//
//    //AudioManager::UISourcePool = SourcePool(AudioType::UI, 8);
//    //EntitySourcePool = SourcePool(AudioType::ENTITY, 128);
//    //MusicSourcePool = SourcePool(AudioType::MUSIC, 4);
//    SourcePool AudioManager::UISourcePool{ SourcePool(AudioType::UI, 8) };
//    SourcePool AudioManager::EntitySourcePool{ SourcePool(AudioType::ENTITY, 118) };
//    SourcePool AudioManager::MusicSourcePool{ SourcePool(AudioType::MUSIC, 4) };
//    std::unordered_map<std::string, ALuint> AudioManager::SoundEffectBuffers;
//    std::vector<std::shared_ptr<SoundStream>> AudioManager::sourcesPlaying{};
//    std::vector<std::shared_ptr<MusicSource>> AudioManager::musicPlaying{};
//    std::unordered_map<std::string, std::shared_ptr<MusicSource>> AudioManager::MusicSources;
//    //std::vector<ALuint> AudioManager::SoundEffectBuffers;
//    SoundDevice* AudioManager::device{ SoundDevice::get() };
//    ALCdevice* AudioManager::s_AudioDevice = nullptr;
//
//    // Volume should be 0-100. May change this to be float, if it makes sense to
//    std::vector<std::uint8_t> AudioManager::volumes{ 100, 100, 100 };
//
//
//    void EbonyAudio::AudioManager::Init()
//    {
//        AudioManager::UISourcePool.Init(AudioType::UI);
//        AudioManager::EntitySourcePool.Init(AudioType::ENTITY);
//        AudioManager::MusicSourcePool.Init(AudioType::MUSIC);
//
//
//        // Music Initialization
//        // =============================================================
//        //if (InitAL(s_AudioDevice, nullptr, 0) != 0)
//        //{
//        //    std::cout << "Audio Device error!" << std::endl;
//        //}
//
//        //// Init listener
//        //ALfloat listenerPos[] = { 0.0, 0.0, 0.0 };
//        //ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
//        //ALfloat listenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };
//        //alListenerfv(AL_POSITION, listenerPos);
//        //alListenerfv(AL_VELOCITY, listenerVel);
//        //alListenerfv(AL_ORIENTATION, listenerOri);
//        // =============================================================
//
//	    //return AudioManager();
//    }
//
//    void AudioManager::Update()
//    {
//        sourcesPlaying.erase(
//            std::remove_if(
//                sourcesPlaying.begin(),
//                sourcesPlaying.end(),
//                [&](const auto& source)
//                {
//                    ALint state = AL_PLAYING;
//                    alGetSourcei(source->sound, AL_SOURCE_STATE, &state);
//
//                    if (state == AL_INITIAL)
//                    {
//                        std::cout << "Returning source" << std::endl;
//                        ReturnSource(std::move(source->speaker), source->speaker->type);
//                    }
//
//                    return (state != AL_PLAYING);
//                }
//            ),
//            sourcesPlaying.end()
//        );
//
//        
//
//        // If the song has finished playing, it will get removed from the music playing automatically, else, it will just update the buffer stream.
//        // This makes it hard to tell what is going on.
//        musicPlaying.erase(std::remove_if(musicPlaying.begin(), musicPlaying.end(), [](std::shared_ptr<MusicSource> musicSource)
//            {
//                return !musicSource->UpdateBufferStream();
//            }), musicPlaying.end());
//
//
//        //for (std::size_t i = 0; i < musicPlaying.size(); i++)
//        //{
//        //    bool hasNotCompleted = musicPlaying[i]->UpdateBufferStream();
//        //    if (!hasNotCompleted)
//        //    {
//        //        // Do something eventually with this
//        //        //std::cout << "Song has completed" << std::endl;
//        //    }
//        //}
//
//
//
//    }
//
//    ALuint AudioManager::LoadSound(const std::string& name, const char* filename)
//    {
//        ALenum err = 0, format = 0;
//        ALuint buffer = 0;
//        SNDFILE* sndfile = nullptr;
//        SF_INFO sfinfo{};
//        short* membuf = nullptr;
//        sf_count_t num_frames = 0;
//        ALsizei num_bytes = 0;
//
//        /* Open the audio file and check that it's usable. */
//        sndfile = sf_open(filename, SFM_READ, &sfinfo);
//        if (!sndfile)
//        {
//            fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
//            return 0;
//        }
//        if (sfinfo.frames < 1)
//        {
//            fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
//            sf_close(sndfile);
//            return 0;
//        }
//
//        /* Figure out the OpenAL format from the file and desired sample type. */
//        format = AL_NONE;
//        if (sfinfo.channels == 1)
//        {
//            format = AL_FORMAT_MONO16;
//        }
//        else if (sfinfo.channels == 2)
//        {
//            format = AL_FORMAT_STEREO16;
//        }
//        else if (sfinfo.channels == 3)
//        {
//            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
//            {
//                format = AL_FORMAT_BFORMAT2D_16;
//            }
//        }
//        else if (sfinfo.channels == 4)
//        {
//            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
//            {
//                format = AL_FORMAT_BFORMAT3D_16;
//            }
//        }
//        if (!format)
//        {
//            fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
//            sf_close(sndfile);
//            return 0;
//        }
//        /* Decode the whole audio file to a buffer. */
//        membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));
//        num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
//
//        if (num_frames < 1)
//        {
//            free(membuf);
//            sf_close(sndfile);
//            fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filename, num_frames);
//            return 0;
//        }
//        num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);
//
//
//        /* Buffer the audio data into a new buffer object, then free the data and
//         * close the file.
//         */
//        buffer = 0;
//        alGenBuffers(1, &buffer);
//        alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);
//
//        free(membuf);
//        sf_close(sndfile);
//
//        /* Check if an error occurred, and clean up if so. */
//        err = alGetError();
//        if (err != AL_NO_ERROR)
//        {
//            fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
//            if (buffer && alIsBuffer(buffer))
//                alDeleteBuffers(1, &buffer);
//            return 0;
//        }
//
//        SoundEffectBuffers[name] = buffer;
//
//        return buffer;
//    }
//
//    std::shared_ptr<MusicSource> AudioManager::LoadMusic(const std::string& name, const char* filename)
//    {
//        std::shared_ptr<EbonyAudio::MusicSource> musicSource = std::make_shared<EbonyAudio::MusicSource>();
//        musicSource->fileFormat = EbonyAudio::AudioFileFormat::OTHER;
//
//        //alGenSources(1, &musicSource->source);
//        alGenBuffers(musicSource->NUM_BUFFERS, musicSource->buffers);
//
//        std::size_t frame_size{};
//
//        musicSource->sndFile = sf_open(filename, SFM_READ, &musicSource->sfInfo);
//
//        if (!musicSource->sndFile)
//        {
//            throw("Could not open provided music file. Check path");
//        }
//
//        // Get the sound format and figure out the OpenAL format
//
//        switch (musicSource->sfInfo.channels)
//        {
//        case 1:
//            musicSource->format = AL_FORMAT_MONO16;
//            break;
//        case 2:
//            musicSource->format = AL_FORMAT_STEREO16;
//            break;
//        case 3:
//            if (sf_command(musicSource->sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
//            {
//                musicSource->format = AL_FORMAT_BFORMAT3D_16;
//            }
//            break;
//        case 4:
//            if (sf_command(musicSource->sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
//            {
//                musicSource->format = AL_FORMAT_BFORMAT3D_16;
//            }
//            break;
//        default:
//            throw("Unsupported number of channels");
//            break;
//        }
//
//
//        if (!musicSource->format)
//        {
//            sf_close(musicSource->sndFile);
//            musicSource->sndFile = NULL;
//            throw("Unsupported channel count from file");
//        }
//
//        frame_size = ((static_cast<size_t>(musicSource->BUFFER_SAMPLES) * static_cast<size_t>(musicSource->sfInfo.channels))) * sizeof(short);
//        musicSource->memBuf = static_cast<short*>(malloc(frame_size));
//
//
//        MusicSources[name] = musicSource;
//
//        return musicSource;
//    }
//
//
//    ALuint AudioManager::GetSound(const std::string& name)
//    {
//        return SoundEffectBuffers[name];
//    }
//
//    std::shared_ptr<EbonyAudio::MusicSource> AudioManager::GetMusic(const std::string& name)
//    {
//        return MusicSources[name];
//    }
//
//
//    std::unique_ptr<SoundSource> AudioManager::GetSource(AudioType type)
//    {
//        switch (type)
//        {
//        case EbonyAudio::ENTITY:
//            return EntitySourcePool.GetSource();
//        case EbonyAudio::UI:
//            return UISourcePool.GetSource();
//        case EbonyAudio::MUSIC:
//            return MusicSourcePool.GetSource();
//        default:
//            return nullptr;
//        }
//    }
//
//    void AudioManager::ReturnSource(std::unique_ptr<SoundSource> source, AudioType type)
//    {
//	    if (type & AudioType::ENTITY)
//	    {
//		    EntitySourcePool.ReleaseSource(std::move(source));
//	    }
//	    else if (type & AudioType::UI)
//	    {
//            std::cout << UISourcePool.GetSize() << std::endl;
//		    UISourcePool.ReleaseSource(std::move(source));
//            std::cout << UISourcePool.GetSize() << std::endl;
//
//	    }
//	    else if (type & AudioType::MUSIC)
//	    {
//		    MusicSourcePool.ReleaseSource(std::move(source));
//	    }
//    }
//
//    void AudioManager::SetVolume(std::uint8_t newVolume, AudioType type)
//    {
//	    switch (type)
//	    {
//	    case EbonyAudio::ENTITY:
//		    volumes[0] = newVolume;
//		    break;
//	    case EbonyAudio::UI:
//            volumes[1] = newVolume;
//		    break;
//	    case EbonyAudio::MUSIC:
//            volumes[2] = newVolume;
//		    break;
//	    default:
//		    break;
//	    }
//
//
//    }
//
//    void AudioManager::Mute(AudioType type)
//    {
//	    switch (type)
//	    {
//	    case EbonyAudio::ENTITY:
//		    volumes[0] = 0;
//		    break;
//	    case EbonyAudio::UI:
//		    volumes[1] = 0;
//		    break;
//	    case EbonyAudio::MUSIC:
//		    volumes[2] = 0;
//		    break;
//	    default:
//		    break;
//	    }
//    }
//
//    void EbonyAudio::AudioManager::StopAll()
//    {
//        for (auto& source : musicPlaying)
//        {
//            source->Stop();
//        }
//    }
//
//    void EbonyAudio::AudioManager::PauseAll()
//    {
//        for (auto& source : musicPlaying)
//        {
//            if (source->currentState & Playing)
//            {
//                source->Pause();
//            }
//        }
//    }
//
//    void EbonyAudio::AudioManager::UnpauseAll()
//    {
//        for (auto& source : musicPlaying)
//        {
//            if (source->currentState & Paused)
//            {
//                source->Resume();
//            }
//        }
//    }
//
//    void EbonyAudio::AudioManager::SetPlaylist()
//    {
//    }
//
//    void EbonyAudio::AudioManager::PlayPlaylist()
//    {
//    }
//
//    std::shared_ptr<SoundStream> EbonyAudio::AudioManager::PlaySound(ALuint sound, AudioType type)
//    {
//        std::unique_ptr<SoundSource> speaker = GetSource(type);
//
//        if (speaker != nullptr)
//        {
//            std::shared_ptr<SoundStream> sharedStream = std::make_shared<SoundStream>(std::move(speaker), sound);
//            sourcesPlaying.push_back(sharedStream);
//
//            return sharedStream;
//        }
//        else
//        {
//            std::cout << "Not enough sources in " << type << " to play the sound effect" << std::endl;
//            return nullptr;
//        }
//    }
//
//    void AudioManager::PlayMusic(std::shared_ptr<MusicSource> musicSource)
//    {
//        std::unique_ptr<SoundSource> speaker = GetSource(AudioType::MUSIC);
//        musicSource->speaker = std::move(speaker);
//        musicSource->currentState = Ebony::Playing;
//        //Clear any AL errors
//        alGetError();
//
//        // Rewind the source position and clear the buffer queue
//        alSourceRewind(musicSource->speaker->source);
//        alSourcei(musicSource->speaker->source, AL_BUFFER, 0);
//
//
//        for (ALsizei i = 0; i < musicSource->NUM_BUFFERS; i++)
//        {
//            sf_count_t slen = sf_readf_short(musicSource->sndFile, musicSource->memBuf, musicSource->BUFFER_SAMPLES);
//            if (slen < 1) break;
//
//            slen *= musicSource->sfInfo.channels * static_cast<sf_count_t>(sizeof(short));
//            alBufferData(musicSource->buffers[i], musicSource->format, musicSource->memBuf, static_cast<ALsizei>(slen), musicSource->sfInfo.samplerate);
//        }
//        if (alGetError() != AL_NO_ERROR)
//        {
//            throw("Error Buffering for playback");
//        }
//
//        /* Now queue and start playback! */
//        alSourceQueueBuffers(musicSource->speaker->source, musicSource->NUM_BUFFERS, musicSource->buffers);
//        alSourcePlay(musicSource->speaker->source);
//        if (alGetError() != AL_NO_ERROR)
//        {
//            throw("Error starting playback");
//        }
//
//        musicPlaying.push_back(musicSource);
//    }
//
//}
