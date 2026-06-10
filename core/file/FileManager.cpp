#include "FileManager.hpp"

#include "ImageIO.hpp"
#include "ImageLoader.hpp"


Image FileManager::openImage(const std::string &filepath) {
  return ImageLoader::loadPNG(filepath);
}

bool FileManager::saveImage(const Image &image, const std::string &filepath) {
  return ImageIO::savePNG(image, filepath);
}