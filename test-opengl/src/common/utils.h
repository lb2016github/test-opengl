#ifndef _UTILS_H
#define _UTILS_H
#include <string>


#define SAFE_DELETE(p) if(p) {delete p; p = NULL;}
#define ELEMENTS_COUNT(p) (sizeof(p) / sizeof(p[0]))

class PathManager 
{
public:
	std::string get_res_dir();
	std::string get_res_dir(std::string related_path);
	std::string get_shader_dir();
	std::string get_shader_dir(std::string related_path);
	static PathManager* instance();

private:
	PathManager();
	static PathManager* m_instance;
	std::string exe_dir;
	static const int MAX_PATH_LENGTH = 128;
};

#endif // !_UTILS_H
