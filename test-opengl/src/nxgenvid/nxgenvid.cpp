#include "nxgenvid.h"
#include <assert.h>

char* video_data = NULL;
int length = 0;
int window_width = 0;
int window_height = 0;
FUNC_READ_PIXELS func_read_pixel = NULL;

bool init_nxgenvid(const NxGenvidInitInfo& info)
{
	GenvidStatus gs = Genvid_Initialize();
	if (gs != GenvidStatus_Success) {
		// something is wrong when initializing genvid
		return false;
	}
	if (video_data != NULL)
	{
		delete video_data;
	}
	length = info.get_data_length();
	video_data = new char[length];
	func_read_pixel = info.func_read_pixel;
	window_width = info.width;
	window_height = info.height;

	// create video stream
	gs = Genvid_CreateStream(STREAM_VIDEO.c_str());
	if (gs != GenvidStatus_Success) {
		return false;
	}
	// specify video source
	gs = Genvid_SetParameterInt(STREAM_VIDEO.c_str(), "video.pixel_format", info.format);
	assert(gs == GenvidStatus_Success);
	gs = Genvid_SetParameterInt(STREAM_VIDEO.c_str(), "video.width", info.width);
	assert(gs == GenvidStatus_Success);
	gs = Genvid_SetParameterInt(STREAM_VIDEO.c_str(), "video.height", info.height);
	assert(gs == GenvidStatus_Success);
	gs = Genvid_SetParameterInt(STREAM_VIDEO.c_str(), "video.useopenglconvention", 1);
	assert(gs == GenvidStatus_Success);

	// create audio stream
	gs = Genvid_CreateStream(STREAM_AUDIO.c_str());
	if (gs != GenvidStatus_Success) {
		return false;
	}
	gs = Genvid_SetParameterInt(STREAM_AUDIO.c_str(), "Audio.Source.WASAPI", 1);
	if (gs != GenvidStatus_Success) {
		return false;
	}

	// create game data stream
	gs = Genvid_CreateStream(STREAM_GAME_DATA.c_str());
	if (gs != GenvidStatus_Success) {
		return false;
	}

	// todo subscribe events and commands

	return true;
}

bool destroy_nxgenvid()
{
	Genvid_DestroyStream(STREAM_GAME_DATA.c_str());
	Genvid_DestroyStream(STREAM_VIDEO.c_str());
	Genvid_DestroyStream(STREAM_AUDIO.c_str());
	// todo unsubscribe

	GenvidStatus gs = Genvid_Terminate();
	if (gs != GenvidStatus_Success)
	{
		return false;
	}
	return true;
}

bool on_game_loop()
{
	if (func_read_pixel == NULL) return false;
	func_read_pixel(video_data);
	GenvidTimecode tc = Genvid_GetCurrentTimecode();
	GenvidStatus gs = Genvid_SubmitVideoData(tc, STREAM_VIDEO.c_str(), video_data, length);
	if (gs != GenvidStatus_Success) {
		return false;
	}
	return true;
}

NxGenvidInitInfo& NxGenvidInitInfo::operator=(const NxGenvidInitInfo& info)
{
	if (this == &info) {
		return *this;
	}

	this->format = info.format;
	this->width = info.width;
	this->height = info.height;
	this->func_read_pixel = info.func_read_pixel;
	return *this;
}

int NxGenvidInitInfo::get_data_length() const
{
	int pixel_size = 0;
	switch (this->format)
	{
	case GenvidPixelFormat_B8G8R8A8:
		pixel_size = 4;
		break;
	case GenvidPixelFormat_R10G10B10A2:
		pixel_size = 4;
	case GenvidPixelFormat_R8G8B8:
		pixel_size = 3;
		break;
	case GenvidPixelFormat_R8G8B8A8:
		pixel_size = 4;
		break;
	default:
		pixel_size = 4;
		break;
	}
	return height * width * pixel_size;
}
