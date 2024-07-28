#ifndef   __YUVTOOL_H__
#define   __YUVTOOL_H__

#define  LOG_TAG "YUVTOOL"
#include <fstream>
#include <vector>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <cassert>

#include "os.h"
class YUVtool {
  public:
  YUVtool(int imgWidth, int imgHeight);
  ~YUVtool();
  void convert(const std::string& inputFileName, const int width, const int height, const std::string& outputFileName);
  std::vector<std::string> ListFilesInDirectory(std::string strRoot, std::string extn);
  private:
  void readFile(const std::string& fileName, std::vector<unsigned char>& data, int expectedSize);
  void writeFile(const std::string& fileName, const std::vector<unsigned char>& data);

  int width = 1920;
  int height = 1080; //default width and height
};



#endif // __YUVTOOL_H__