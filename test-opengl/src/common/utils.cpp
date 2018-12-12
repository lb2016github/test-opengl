#include "utils.h"
#include <windows.h>

PathManager* PathManager::m_instance = NULL;

PathManager::PathManager() {
	char tmp[MAX_PATH_LENGTH];
	GetModuleFileName(NULL, tmp, MAX_PATH_LENGTH);
	char dir[MAX_PATH_LENGTH];
	_splitpath(tmp, NULL, dir, NULL, NULL);
	exe_dir = std::string(dir);
}


std::string PathManager::get_res_dir() {
	return exe_dir + "/res/";
}
std::string PathManager::get_res_dir(std::string related_path) {
	return get_res_dir() + related_path;
}
std::string PathManager::get_shader_dir() {
	return exe_dir + "shaders/";
}
std::string PathManager::get_shader_dir(std::string related_path) {
	return get_shader_dir() + related_path;
}
PathManager* PathManager::instance() {
	if (m_instance == NULL) {
		m_instance = new PathManager();
	}
	return m_instance;
}