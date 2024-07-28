#ifndef   __YUVTOOL_H__
#define   __YUVTOOL_H__

#define  LOG_TAG "YUVTOOL"
#include <fstream>
#include <vector>
#include <string>
#include <dirent.h>
#include <sys/stat.h>

class YUVtool {
  public:
  YUVtool();
  ~YUVtool();
  void convert(const std::string& inputFileName, const int width, const int height, const std::string& outputFileName);
  std::vector<std::string> ListFilesInDirectory(std::string strRoot, std::string extn);
  private:
  void readFile(const std::string& fileName, std::vector<unsigned char>& data);
  void writeFile(const std::string& fileName, const std::vector<unsigned char>& data);
};



#endif // __YUVTOOL_H__