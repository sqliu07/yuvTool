/**<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
  *<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
  *<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
  *<<<<<<<<<<<<<2023-2024 Siqi Liu All rights reserved.>>>>>>>>>>>>>*
  *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>*
  *<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
  *<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <cstdlib>
#include <condition_variable>

#include "yuvTool.h"
#include "os.h"

static int w = 0;
static int h = 0;

struct FileData {
    std::string filename;
    bool processed = false;
    bool endOfFiles = false; 
    FileData()
        : filename(""), processed(false), endOfFiles(false) {}
    FileData(const std::string& filename, bool processed, bool endOfFiles)
        : filename(filename), processed(processed), endOfFiles(endOfFiles) {}
};

void readFiles(std::queue<FileData>& fileQueue, const std::string& rootDir, const std::string& extension, int width, int height, std::mutex& mtx, std::condition_variable& cv) {
    YUVtool* yt = new YUVtool(width, height);
    
    auto files = yt->ListFilesInDirectory(rootDir, extension);
    
    for (auto file : files) {
        std::lock_guard<std::mutex> lock(mtx);
        FileData data;
        data.filename = file;
        fileQueue.push(data);
        LOG_VERBOSE("Read file: %s", file.c_str());
        cv.notify_one(); 
    }
    
    {
        std::lock_guard<std::mutex> lock(mtx);
        fileQueue.push(FileData{ "", false, true });
        cv.notify_one();
    }
    
    delete yt;
}

// 处理文件
void processFiles(int width, int height, std::queue<FileData>& fileQueue, std::mutex& mtx, std::condition_variable& cv) {
    YUVtool* yt = new YUVtool(width, height);
    
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return !fileQueue.empty(); });
        if (fileQueue.empty()) continue;
        
        FileData data = fileQueue.front();
        fileQueue.pop();
        
        lock.unlock();
        
        if (data.endOfFiles) {
            break;
        }
        
        LOG_VERBOSE("Processing file: %s", data.filename.c_str());
        yt->convert(data.filename, width, height, data.filename + "_wb.yuv");
        data.processed = true;
        lock.lock();
        cv.notify_all();
    }
    
    delete yt;
}

int main(int argc, char* argvs[]) {
#ifdef _DEBUG
    std::string rootDir = "/home/lsq/workspace/project/cppProj/yuvProcess/input";
#else
    if (argc < 2) {
        LOG_ERROR("Invalid input!");
        return S_ERROR;
    }
  std::string rootDir = argvs[1];  
  w = atoi(argvs[2]);
  h = atoi(argvs[3]);
#endif
    std::string extension = ".yuv";
    
    std::queue<FileData> fileQueue;
    std::mutex mtx;
    std::condition_variable cv;
    
    std::thread readerThread(readFiles, std::ref(fileQueue), rootDir, extension, w, h, std::ref(mtx), std::ref(cv));
    std::thread workerThread(processFiles, w, h, std::ref(fileQueue), std::ref(mtx), std::ref(cv));
    
    readerThread.join();

    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return fileQueue.empty(); });
        if (fileQueue.empty()) break;
    }
    
    workerThread.join();
    LOG_VERBOSE("Process all done!");
    return S_OK;
}