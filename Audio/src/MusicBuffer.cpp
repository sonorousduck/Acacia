#include "MusicBuffer.hpp"
#include <cstddef>
#include <alext.h>
#include <malloc.h>

MusicBuffer::MusicBuffer(const char* filename)
{
	alGenSources(1, &source);
	alGenBuffers(NUM_BUFFERS, buffers);

	std::size_t frame_size{};

	sndFile = sf_open(filename, SFM_READ, &sfInfo);

	if (!sndFile)
	{
		throw("Could not open provided music file. Check path");
	}

	// Get the sound format and figure out the OpenAL format
	if (sfInfo.channels == 1)
	{
		format = AL_FORMAT_MONO16;
	}
	else if (sfInfo.channels == 2)
	{
		format = AL_FORMAT_STEREO16;
	}
	else if (sfInfo.channels == 3)
	{
		if (sf_command(sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
		{
			format = AL_FORMAT_BFORMAT3D_16;
		}
	}
	else if (sfInfo.channels == 4)
	{
		if (sf_command(sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
		{
			format = AL_FORMAT_BFORMAT3D_16;
		}
	}

	if (!format)
	{
		sf_close(sndFile);
		sndFile = NULL;
		throw("Unsupported channel count from file");
	}

	frame_size = ((static_cast<size_t>(BUFFER_SAMPLES) * static_cast<size_t>(sfInfo.channels))) * sizeof(short);
	memBuf = static_cast<short*>(malloc(frame_size));
}

MusicBuffer::~MusicBuffer()
{
	alDeleteSources(1, &source);

	if (sndFile)
	{
		sf_close(sndFile);
	}

	sndFile = nullptr;
	free(memBuf);
	alDeleteBuffers(NUM_BUFFERS, buffers);
}


ALint MusicBuffer::getSource()
{
	return source;
}

void MusicBuffer::Play()
{

	// Clear any AL errors
	alGetError();

	// Rewind the source position and clear the buffer queue
	alSourceRewind(source);
	alSourcei(source, AL_BUFFER, 0);


	for (ALsizei i = 0; i < NUM_BUFFERS; i++)
	{
		sf_count_t slen = sf_readf_short(sndFile, memBuf, BUFFER_SAMPLES);
		if (slen < 1) break;

		slen *= sfInfo.channels * static_cast<sf_count_t>(sizeof(short));
		alBufferData(buffers[i], format, memBuf, static_cast<ALsizei>(slen), sfInfo.samplerate);
	}
	if (alGetError() != AL_NO_ERROR)
	{
		throw("Error Buffering for playback");
	}

	/* Now queue and start playback! */
	alSourceQueueBuffers(source, NUM_BUFFERS, buffers);
	alSourcePlay(source);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("Error starting playback");
	}
}

void MusicBuffer::UpdateBufferStream()
{
	ALint processed = 0, state = 0;

	// Clear errors
	alGetError();

	// Get relevant source info
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

	if (alGetError() != AL_NO_ERROR)
	{
		throw("error checking music source state");
	}

	// Unqueue and handle each processed buffer
	while (processed > 0)
	{
		ALuint bufid = 0;
		sf_count_t slen = 0;

		alSourceUnqueueBuffers(source, 1, &bufid);
		processed--;

		/* Read the next chunk of data, refill the buffer, and queue it
		 * back on the source */
		slen = sf_readf_short(sndFile, memBuf, BUFFER_SAMPLES);
		if (slen > 0)
		{
			slen *= sfInfo.channels * (sf_count_t)sizeof(short);
			alBufferData(bufid, format, memBuf, (ALsizei)slen,
				sfInfo.samplerate);
			alSourceQueueBuffers(source, 1, &bufid);
		}
		if (alGetError() != AL_NO_ERROR)
		{
			throw("error buffering music data");
		}
	}

	/* Make sure the source hasn't underrun */
	if (state != AL_PLAYING && state != AL_PAUSED)
	{
		ALint queued;

		/* If no buffers are queued, playback is finished */
		alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
		if (queued == 0)
			return;

		alSourcePlay(source);
		if (alGetError() != AL_NO_ERROR)
		{
			throw("error restarting music playback");
		}
	}

}