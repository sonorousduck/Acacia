//#include "SoundBuffer.hpp"
//#include "sndfile.h"
//#include <inttypes.h>
//#include <AL/alext.h>
//
//
//SoundBuffer* SoundBuffer::get()
//{
//	static SoundBuffer* soundBuffer = new SoundBuffer();
//	return soundBuffer;
//}
//
//// OpenAL soft examples aiplay
//ALuint SoundBuffer::addSoundEffect(const char* filename)
//{
//	ALenum err = 0, format = 0;
//	ALuint buffer = 0;
//	SNDFILE* sndfile = nullptr;
//	SF_INFO sfinfo{};
//	short* membuf = nullptr;
//	sf_count_t num_frames = 0;
//	ALsizei num_bytes = 0;
//
//    /* Open the audio file and check that it's usable. */
//    sndfile = sf_open(filename, SFM_READ, &sfinfo);
//    if (!sndfile)
//    {
//        fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
//        return 0;
//    }
//    if (sfinfo.frames < 1)
//    {
//        fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
//        sf_close(sndfile);
//        return 0;
//    }
//
//    /* Figure out the OpenAL format from the file and desired sample type. */
//    format = AL_NONE;
//    if (sfinfo.channels == 1)
//    {
//        format = AL_FORMAT_MONO16;
//    }
//    else if (sfinfo.channels == 2)
//    {
//        format = AL_FORMAT_STEREO16;
//    }
//    else if (sfinfo.channels == 3)
//    {
//        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
//        {
//            format = AL_FORMAT_BFORMAT2D_16;
//        }
//    }
//    else if (sfinfo.channels == 4)
//    {
//        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
//        {
//                format = AL_FORMAT_BFORMAT3D_16;
//        }
//    }
//    if (!format)
//    {
//        fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
//        sf_close(sndfile);
//        return 0;
//    }
//    /* Decode the whole audio file to a buffer. */
//    membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));
//    num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
//
//    if (num_frames < 1)
//    {
//        free(membuf);
//        sf_close(sndfile);
//        fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filename, num_frames);
//        return 0;
//    }
//    num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);
//
//
//    /* Buffer the audio data into a new buffer object, then free the data and
//     * close the file.
//     */
//    buffer = 0;
//    alGenBuffers(1, &buffer);
//    alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);
//
//    free(membuf);
//    sf_close(sndfile);
//
//    /* Check if an error occurred, and clean up if so. */
//    err = alGetError();
//    if (err != AL_NO_ERROR)
//    {
//        fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
//        if (buffer && alIsBuffer(buffer))
//            alDeleteBuffers(1, &buffer);
//        return 0;
//    }
//
//    m_SoundEffectBuffers.push_back(buffer);
//
//    return buffer;
//}
//
//bool SoundBuffer::removeSoundEffect(const ALuint& buffer)
//{
//	auto it = m_SoundEffectBuffers.begin();
//
//	while (it != m_SoundEffectBuffers.end())
//	{
//		if (*it == buffer)
//		{
//			alDeleteBuffers(1, &*it);
//
//			it = m_SoundEffectBuffers.erase(it);
//
//			return true;
//		}
//		else
//		{
//			it++;
//		}
//	}
//
//	return false;
//}
//
//SoundBuffer::SoundBuffer()
//{
//	m_SoundEffectBuffers.clear();
//}
//
//SoundBuffer::~SoundBuffer()
//{
//	alDeleteBuffers(m_SoundEffectBuffers.size(), m_SoundEffectBuffers.data());
//	m_SoundEffectBuffers.clear();
//}