#include "yuvTool.h"
YUVtool::YUVtool(int imgWidth, int imgHeight): width(imgWidth), height(imgHeight) {}

YUVtool::~YUVtool() {}

void YUVtool::convert(const std::string &inputFileName, const int width, const int height, const std::string &outputFileName) {
  std::vector<unsigned char> yuvData(width * height * 2); // yuv422p
  readFile(inputFileName, yuvData, yuvData.size());
  std::vector<unsigned char> grayData = yuvData; // yuv422p
  for (size_t i = width * height; i < yuvData.size(); ++i) {
    grayData.at(i) = 128; // to gray
  }
  writeFile(outputFileName, grayData);
}

std::vector<std::string> YUVtool::ListFilesInDirectory(std::string strRoot, std::string extn) {
  std::vector<std::string> vecFiles;
  DIR *dir;
  struct dirent *ent;
  struct stat st;

  if ((dir = opendir(strRoot.c_str())) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      const std::string filename = ent->d_name;
      const std::string fullpath = strRoot + "/" + filename;

      if (stat(fullpath.c_str(), &st) == 0 && S_ISREG(st.st_mode) && filename.find(extn) != std::string::npos) {
        vecFiles.push_back(fullpath);
      }
    }
    closedir(dir);
  }

  return vecFiles;
}

void YUVtool::readFile(const std::string &fileName, std::vector<unsigned char> &data, int expectedSize) {
  std::ifstream file(fileName, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Unable to open file: " + fileName);
  }
  
  file.seekg(0, std::ios::end);
  size_t dataSize = file.tellg();
  if (dataSize != expectedSize) {
    LOG_ERROR("Unexpected image size!");
  }
  file.seekg(0, std::ios::beg);
  std::vector<unsigned char> copy((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  data = copy;
  file.close();
}

void YUVtool::writeFile(const std::string &fileName, const std::vector<unsigned char> &data) {
  std::ofstream file(fileName, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Unable to open file: " + fileName);
  }
  file.write(reinterpret_cast<const char *>(data.data()), data.size());
  file.close();
}
