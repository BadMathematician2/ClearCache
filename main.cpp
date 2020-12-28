#include <sys/stat.h>
#include "filesystem"
#include "string"

namespace fs = std::filesystem;

long timeToLastUpdate(const std::string& filename) {
    struct stat attrib{};
    stat(filename.c_str(), &attrib);
    time_t current_time = time(nullptr);

    return mktime(gmtime(&current_time)) - mktime(gmtime(&(attrib.st_mtime)));
}

void clearCache(const std::string& directory, long max_time) {
    for (const auto & entry : fs::directory_iterator(directory)) {
        std::string file = entry.path();
        std::string last = std::string(file, file.rfind('/'));

        if (last.find('.') == -1) {
            clearCache(file, max_time);
        } else {
            if (timeToLastUpdate(file) > max_time) {
                remove(file.c_str());
            }
        }
    }
}

int main(int arg, char** args) {
    if (3 == arg) {
        std::string path = args[1];
        std::string max_time = args[2];

        clearCache(path, std::stol(max_time));
    }

    return 0;
}



