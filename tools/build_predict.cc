//
// Copyright RIME Developers
//
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <rime/common.h>
#include "predict_db.h"

using namespace rime;

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char* argv[]) {
  rime::predict::RawData data;

    std::ifstream file(argv[1]);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::vector<std::string> result= split(line, '\t');
            if (result.size() == 3)
            {
		//std::cout << "insert item " << result[0] << ':' << result[1] << ' ' << result[2] << std::endl;
                rime::predict::RawEntry entry;
		entry.text = result[1];
		entry.weight = std::stod(result[2]);
                data[result[0]].push_back(std::move(entry));
            }
        }
        file.close();
    } else {
        std::cerr << "can not open file" << std::endl;
    }

//   while (std::cin) {
//     string key;
//     std::cin >> key;
//     if (key.empty())
//       break;
//     rime::predict::RawEntry entry;
//     std::cin >> entry.text >> entry.weight;
//     data[key].push_back(std::move(entry));
//   }

  path file_path = argc > 1 ? path(argv[2]) : path{"predict.db"};
  PredictDb db(file_path);
  LOG(INFO) << "creating " << db.file_path();
  if (!db.Build(data) || !db.Save()) {
    LOG(ERROR) << "failed to build " << db.file_path();
    return 1;
  }
  LOG(INFO) << "created: " << db.file_path();
  return 0;
}
