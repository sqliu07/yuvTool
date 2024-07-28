#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "yuvTool.h"
#include "os.h"

const int w = 6000;
const int h = 4000;

// 定义一个结构体来存储文件名和完成状态
struct FileData {
    std::string filename;
    bool processed = false;
    bool endOfFiles = false; // 新增标志，用于表示所有文件都已读取完毕
    FileData()
        : filename(""), processed(false), endOfFiles(false) {}
    FileData(const std::string& filename, bool processed, bool endOfFiles)
        : filename(filename), processed(processed), endOfFiles(endOfFiles) {}
};
// 读取文件并将其加入队列
void readFiles(std::queue<FileData>& fileQueue, const std::string& rootDir, const std::string& extension, std::mutex& mtx, std::condition_variable& cv) {
    YUVtool* yt = new YUVtool();
    
    auto files = yt->ListFilesInDirectory(rootDir, extension);
    
    for (auto file : files) {
        std::lock_guard<std::mutex> lock(mtx); // 自动释放锁
        FileData data;
        data.filename = file; // 显式地创建 FileData 对象并设置 filename
        fileQueue.push(data);
        LOG_VERBOSE("Read file: %s", file.c_str());
        cv.notify_one(); // 通知一个等待线程
    }
    
    // 标记所有文件读取完毕
    {
        std::lock_guard<std::mutex> lock(mtx); // 自动释放锁
        fileQueue.push(FileData{ "", false, true }); // 插入特殊值，表示所有文件都已读取完毕
        cv.notify_one(); // 通知一个等待线程
    }
    
    delete yt;
}

// 处理文件
void processFiles(std::queue<FileData>& fileQueue, std::mutex& mtx, std::condition_variable& cv) {
    YUVtool* yt = new YUVtool();
    
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return !fileQueue.empty(); }); // 等待直到队列非空
        if (fileQueue.empty()) continue; // 如果队列为空，继续等待
        
        FileData data = fileQueue.front();
        fileQueue.pop();
        
        lock.unlock(); // 解锁以便其他线程可以访问队列
        
        if (data.endOfFiles) {
            // 所有文件都已读取完毕，退出循环
            break;
        }
        
        LOG_VERBOSE("Processing file: %s", data.filename.c_str());
        yt->convert(data.filename, w, h, data.filename + "_wb.yuv");
        data.processed = true;
        lock.lock(); // 再次锁定以更新文件的状态
        cv.notify_all(); // 通知所有等待线程
    }
    
    delete yt;
}

int main(int argc, char* argvs[]) {
#ifdef _DEBUG
    std::string rootDir = "/home/lsq/workspace/project/cppProj/yuvProcess/input"; // 默认当前文件夹路径
#else
    if (argc != 2) {
        LOG_ERROR("Invalid input!");
        return S_ERROR;
    }
  std::string rootDir = argvs[1];  
#endif
    std::string extension = ".yuv"; // 默认文件后缀
    
    std::queue<FileData> fileQueue;
    std::mutex mtx;
    std::condition_variable cv;
    
    std::thread readerThread(readFiles, std::ref(fileQueue), rootDir, extension, std::ref(mtx), std::ref(cv));
    std::thread workerThread(processFiles, std::ref(fileQueue), std::ref(mtx), std::ref(cv));
    
    // 等待读取文件线程完成
    readerThread.join();
    
    // 主线程循环检查队列是否为空
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return fileQueue.empty(); }); // 等待直到队列为空
        if (fileQueue.empty()) break;
    }
    
    workerThread.join(); // 等待子线程结束
    LOG_VERBOSE("Process all done!");
    return S_OK;
}