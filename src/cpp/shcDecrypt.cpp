#include <dirent.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
// 获取当前工作目录
std::string get_current_dir()
{
    char *cwd = getcwd(NULL, 0);
    std::string current_dir(cwd);
    free(cwd);
    std::cout << current_dir << "\n";
    return current_dir;
}

// 更改文件所有者和权限
void change_owner_and_permissions(const std::string &file)
{
    chown(file.c_str(), 0, 0);  // 更改所有者为 root
    chmod(file.c_str(), 0777);  // 更改权限为可执行
}

// 后台执行程序
void execute_in_background(const std::string &file, const std::string &log_file)
{
    std::string command = "nohup " + file + " >" + log_file + " 2>&1 &";
    system(command.c_str());
}

// 获取进程ID
std::vector<pid_t> get_process_ids(const std::string &process_name)
{
    std::vector<pid_t> pids;
    int n = 1;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("/proc")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            int id = atoi(ent->d_name);
            if (id > 0) {
                std::string cmd_path =
                    std::string("/proc/") + ent->d_name + "/cmdline";
                std::ifstream cmd_file(cmd_path.c_str());
                std::string cmd_line;
                std::getline(cmd_file, cmd_line);
                if (cmd_line.find(process_name) != std::string::npos) {
                    pids.push_back(id);
#define 抓取的cmdline数量 10
                    if (n < 抓取的cmdline数量) {
                        n++;
                    }
                    if (n == 抓取的cmdline数量) {
                        closedir(dir);
                        return pids;
                    }
                }
            }
        }
        closedir(dir);
    }
    return pids;
}

// 读取并保存进程的命令行参数
void save_process_cmdline(pid_t pid, const std::string &output_dir)
{
    std::string cmdline_file = "/proc/" + std::to_string(pid) + "/cmdline";
    std::ifstream ifs(cmdline_file);
    std::string cmdline((std::istreambuf_iterator<char>(ifs)),
                        std::istreambuf_iterator<char>());
    ifs.close();

    std::string output_file =
        output_dir + "/shc解密" + std::to_string(pid) + ".txt";
    std::ofstream ofs(output_file);
    ofs << cmdline;
    ofs.close();
}

// 杀死进程
void kill_process(const std::string &process_name, int signal)
{
    std::string command =
        "pkill -" + std::to_string(signal) + " " + process_name;
    system(command.c_str());
}

int main()
{
    std::system("rm -rf $(pwd)/shc*.txt");
    std::string current_dir = get_current_dir();
    std::string shcfile = current_dir + "/iris_helper";
    std::string base_name_shcfile = "iris_helper";  // 假设文件名为 iris_helper
    std::system(("killall -15 " + base_name_shcfile).c_str());
    change_owner_and_permissions(shcfile);
    execute_in_background(shcfile, current_dir + "/log.log");

    // sleep(1); // 等待程序启动并获取PID

    std::vector<pid_t> pids = get_process_ids(base_name_shcfile);
    for (const auto &pid : pids) {
        save_process_cmdline(pid, current_dir);
    }

    kill_process(base_name_shcfile, SIGTERM);

    return 0;
}
