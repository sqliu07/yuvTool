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
  YUVtool(INT32 imgWidth, INT32 imgHeight);
  ~YUVtool();
  VOID convert(const std::string& inputFileName, const INT32 width, const INT32 height, const std::string& outputFileName);
  std::vector<std::string> ListFilesInDirectory(std::string strRoot, std::string extn);
  private:
  bool readFile(const std::string& fileName, std::vector<unsigned char>& data, INT32 expectedSize);
  VOID writeFile(const std::string& fileName, const std::vector<unsigned char>& data, bool readStatus = false);
  INT32 yuv2jpeg(std::vector<unsigned char>& yuvData, std::vector<unsigned char>& jpegData, const INT32 width, const INT32 height);

  INT32 m_Width = 1920;
  INT32 m_Height = 1080; //default width and height
};




#endif // __YUVTOOL_H__