#ifndef _NX_GENVID_H
#define _NX_GENVID_H

#include "genvid/genvid.h"
#include <string>

/*
init nxgenvid const vars;
*/

static const std::string STREAM_AUDIO = "STREAM_AUDIO";
static const std::string STREAM_VIDEO = "STREAM_VIDEO";
static const std::string STREAM_GAME_DATA = "STREAM_GAME_DATA";

typedef void(*FUNC_READ_PIXELS)(char*);

struct NxGenvidInitInfo {

	int width;
	int height;
	GenvidPixelFormat format;
	FUNC_READ_PIXELS func_read_pixel;

	NxGenvidInitInfo& operator=(const NxGenvidInitInfo& info);
	int get_data_length() const;	// 返回数据长度
};

struct VideoData {
	const void* data;
	const int size;
};

bool init_nxgenvid(const NxGenvidInitInfo& desc);

bool destroy_nxgenvid();

bool on_game_loop();

#endif
