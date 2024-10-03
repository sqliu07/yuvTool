#include "yuvTool.h"
YUVtool::YUVtool(INT32 imgWidth, INT32 imgHeight): m_Width(imgWidth), m_Height(imgHeight) {}

YUVtool::~YUVtool() {}

VOID YUVtool::convert(const std::string &inputFileName, const INT32 m_Width, const INT32 m_Height, const std::string &outputFileName) {
  std::vector<unsigned char> yuvData(m_Width * m_Height * 2); // yuv422p
  bool ret = readFile(inputFileName, yuvData, yuvData.size());
  std::vector<unsigned char> grayData = yuvData; // yuv422p
  for (size_t i = m_Width * m_Height; i < yuvData.size(); ++i) {
    grayData.at(i) = 128; // to gray
  }
  writeFile(outputFileName, grayData, ret);
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

bool YUVtool::readFile(const std::string &fileName, std::vector<unsigned char> &data, INT32 expectedSize) {
  std::ifstream file(fileName, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Unable to open file: " + fileName);
  }
  
  file.seekg(0, std::ios::end);
  size_t dataSize = file.tellg();
  if (dataSize != expectedSize) {
    LOG_ERROR("Unexpected image size!");
    file.close();
    return S_ERROR; //error, just return false
  }
  file.seekg(0, std::ios::beg);
  std::vector<unsigned char> copy((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  data = copy;
  file.close();
  return S_OK;
}

VOID YUVtool::writeFile(const std::string &fileName, const std::vector<unsigned char> &data, bool readStatus) {
  if (readStatus) return;
  std::ofstream file(fileName, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Unable to open file: " + fileName);
  }
  file.write(reinterpret_cast<const char *>(data.data()), data.size());
  file.close();
}

INT32 YUVtool::yuv2jpeg(std::vector<unsigned char>& yuvData, std::vector<unsigned char>& jpegData, const INT32 width, const INT32 height) {
  return 0;
}
