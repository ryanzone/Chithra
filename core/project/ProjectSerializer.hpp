#pragma once

#include "../document/Document.hpp"

#include <string>

class ProjectSerializer {
public:
  static bool save(const Document &document, const std::string &path);

  static bool load(Document &document, const std::string &path);
};