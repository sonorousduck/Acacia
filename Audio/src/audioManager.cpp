#include "audioManager.hpp"
#include "EmptySoundDevice.hpp"
#include <alext.h>
#include <inttypes.h>


// NOTE: It looks like 256 is a good amount of sources to allow playing at one time
// https://github.com/MonoGame/MonoGame/issues/4403

namespace EbonyAudio
{

    //AudioManager::UISourcePool = SourcePool(AudioType::UI, 8);
    //EntitySourcePool = SourcePool(AudioType::ENTITY, 128);
    //MusicSourcePool = SourcePool(AudioType::MUSIC, 4);
    SourcePool AudioManager::UISourcePool{ SourcePool(AudioType::UI, 1) };
    SourcePool AudioManager::EntitySourcePool{ SourcePool(AudioType::ENTITY, 1) };
    SourcePool AudioManager::MusicSourcePool{ SourcePool(AudioType::MUSIC, 1) };
    //std::unordered_map<std::string, ALuint> AudioManager::SoundEffectBuffers;
    std::vector<ALuint> AudioManager::SoundEffectBuffers;
    SoundDevice* AudioManager::device{ SoundDevice::get() };

    // Volume should be 0-100. May change this to be float, if it makes sense to
    std::vector<std::uint8_t> AudioManager::volumes{ 100, 100, 100 };


    AudioManager EbonyAudio::AudioManager::Init()
    {
	    return AudioManager();
    }

    // Might not get used. TODO: Delete this function if not used
    SoundSource AudioManager::createSoundSourceObject(SoundSource sound, AudioType type)
    {
	    return SoundSource();
    }

    ALuint AudioManager::LoadSound(const std::string& name, const char* filename)
    {
        ALenum err = 0, format = 0;
        ALuint buffer = 0;
        SNDFILE* sndfile = nullptr;
        SF_INFO sfinfo{};
        short* membuf = nullptr;
        sf_count_t num_frames = 0;
        ALsizei num_bytes = 0;

        /* Open the audio file and check that it's usable. */
        sndfile = sf_open(filename, SFM_READ, &sfinfo);
        if (!sndfile)
        {
            fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
            return 0;
        }
        if (sfinfo.frames < 1)
        {
            fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
            sf_close(sndfile);
            return 0;
        }

        /* Figure out the OpenAL format from the file and desired sample type. */
        format = AL_NONE;
        if (sfinfo.channels == 1)
        {
            format = AL_FORMAT_MONO16;
        }
        else if (sfinfo.channels == 2)
        {
            format = AL_FORMAT_STEREO16;
        }
        else if (sfinfo.channels == 3)
        {
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            {
                format = AL_FORMAT_BFORMAT2D_16;
            }
        }
        else if (sfinfo.channels == 4)
        {
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            {
                format = AL_FORMAT_BFORMAT3D_16;
            }
        }
        if (!format)
        {
            fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
            sf_close(sndfile);
            return 0;
        }
        /* Decode the whole audio file to a buffer. */
        membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));
        num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);

        if (num_frames < 1)
        {
            free(membuf);
            sf_close(sndfile);
            fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filename, num_frames);
            return 0;
        }
        num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);


        /* Buffer the audio data into a new buffer object, then free the data and
         * close the file.
         */
        buffer = 0;
        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

        free(membuf);
        sf_close(sndfile);

        /* Check if an error occurred, and clean up if so. */
        err = alGetError();
        if (err != AL_NO_ERROR)
        {
            fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
            if (buffer && alIsBuffer(buffer))
                alDeleteBuffers(1, &buffer);
            return 0;
        }

        //SoundEffectBuffers[name] = buffer;
        SoundEffectBuffers.push_back(buffer);

        return buffer;
    }





    std::unique_ptr<SoundSource> AudioManager::GetSource(AudioType type)
    {
        switch (type)
        {
        case EbonyAudio::ENTITY:
            return EntitySourcePool.GetSource();
        case EbonyAudio::UI:
            return UISourcePool.GetSource();
        case EbonyAudio::MUSIC:
            return MusicSourcePool.GetSource();
        default:
            break;
        }
    }

    void AudioManager::ReturnSource(std::unique_ptr<SoundSource> source, AudioType type)
    {
	    if (type & AudioType::ENTITY)
	    {
		    EntitySourcePool.ReleaseSource(std::move(source));
	    }
	    else if (type & AudioType::UI)
	    {
		    UISourcePool.ReleaseSource(std::move(source));

	    }
	    else if (type & AudioType::MUSIC)
	    {
		    MusicSourcePool.ReleaseSource(std::move(source));
	    }
    }

    void AudioManager::SetVolume(std::uint8_t newVolume, AudioType type)
    {
	    switch (type)
	    {
	    case EbonyAudio::ENTITY:
		    volumes[0] = newVolume;
		    break;
	    case EbonyAudio::UI:
            volumes[1] = newVolume;
		    break;
	    case EbonyAudio::MUSIC:
            volumes[2] = newVolume;
		    break;
	    default:
		    break;
	    }


    }

    void AudioManager::Mute(AudioType type)
    {
	    switch (type)
	    {
	    case EbonyAudio::ENTITY:
		    volumes[0] = 0;
		    break;
	    case EbonyAudio::UI:
		    volumes[1] = 0;
		    break;
	    case EbonyAudio::MUSIC:
		    volumes[2] = 0;
		    break;
	    default:
		    break;
	    }


    }

    void EbonyAudio::AudioManager::SetPlaylist()
    {
    }

    void EbonyAudio::AudioManager::PlayPlaylist()
    {
    }

    void EbonyAudio::AudioManager::PlaySound()
    {



    }

}
