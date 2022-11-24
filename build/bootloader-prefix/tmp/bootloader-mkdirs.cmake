# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Espressif/frameworks/esp-idf-v4.4.2/components/bootloader/subproject"
  "F:/Persional/ESP32/user_project/lvgl_example/build/bootloader"
  "F:/Persional/ESP32/user_project/lvgl_example/build/bootloader-prefix"
  "F:/Persional/ESP32/user_project/lvgl_example/build/bootloader-prefix/tmp"
  "F:/Persional/ESP32/user_project/lvgl_example/build/bootloader-prefix/src/bootloader-stamp"
  "F:/Persional/ESP32/user_project/lvgl_example/build/bootloader-prefix/src"
  "F:/Persional/ESP32/user_project/lvgl_example/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "F:/Persional/ESP32/user_project/lvgl_example/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
