#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>

// mkdir
#include <sys/stat.h>

/**
 * @brief  判断指定的文件路径是否存在，如果不存在则创建
 * 
 * @param[in] pathname 文件路径
 * @return int32_t 0表示成功，-1表示失败
 */
int32_t OpendirAndMkdir(const char * pathname)
{
	int ret = 0;
	DIR * mydir = NULL;
    
	if ((mydir = opendir(pathname)) == NULL)  // 如果不存在则创建该folder
	{
		ret = mkdir(pathname, 0755);
		if (ret != 0) return -1;
 
		printf("%s created sucess!\n", pathname);
	}
	else
	{
		printf("%s exist!\n", pathname);
	}
	return ret;
}


bool
find_filelists(std::string& val_folder_path, std::vector<std::string>& file_lists)
{
    std::string txt_path = val_folder_path + "/wider_val.txt";
    std::ifstream ifs (txt_path.c_str(), std::ifstream::in);
    std::string s ;
    while(getline(ifs, s))
    {
        file_lists.push_back(s);
    }

    ifs.close();
    return file_lists.size() > 0 ? true : false;
}


int main(int argc, char* argv[])
{
    // std::string folder_path = "val";
    // std::vector<std::string> file_lists = {};
    // if(find_filelists(folder_path, file_lists))
    // {
    //     for(auto& path : file_lists)
    //     {
    //         std::cout << path << std::endl;
    //     }
    // }
    std::string image_name = "/1--Handshaking/1_Handshaking_Handshaking_1_35.txt";
    // find basename
    int found = image_name.rfind("/");
    if(found != std::string::npos)
    {

        std::cout << image_name.substr(0, image_name.size() - found ) << " " << image_name << std::endl;
    }

    // std::cout << image_name.substr(0, image_name.size()-4) << std::endl;

    // widerface_evaluate/widerface_txt
    // const std::string pathname = "widerface_evaluate/widerface_txt";
    // if(!OpendirAndMkdir(pathname.c_str()))
    // {
    //     std::cout << "mkdir sucessfully.\n" << std::endl;
    // }
    return 0;
}