/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2021 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#ifndef XCSOAR_ANDROID_DOWNLOAD_MANAGER_HPP
#define XCSOAR_ANDROID_DOWNLOAD_MANAGER_HPP

#include "java/Object.hxx"
#include "thread/Mutex.hxx"

#include <list>

class Path;
class Context;
namespace Net { class DownloadListener; }

class AndroidDownloadManager {
  Java::GlobalObject util;

  /**
   * Protects the #listeners attribute.
   */
  Mutex mutex;

  std::list<Net::DownloadListener *> listeners;

  AndroidDownloadManager(JNIEnv *env, jobject _util) noexcept;

public:
  ~AndroidDownloadManager() noexcept;

  static bool Initialise(JNIEnv *env) noexcept;
  static void Deinitialise(JNIEnv *env) noexcept;

  [[gnu::pure]]
  static bool IsAvailable() noexcept;

  static AndroidDownloadManager *Create(JNIEnv *env,
                                        Context &context) noexcept;

  void AddListener(Net::DownloadListener &listener) noexcept;
  void RemoveListener(Net::DownloadListener &listener) noexcept;
  void OnDownloadComplete(Path path_relative, bool success) noexcept;

  void Enumerate(JNIEnv *env, Net::DownloadListener &listener) noexcept;
  void Enqueue(JNIEnv *env, const char *uri, Path path_relative) noexcept;
  void Cancel(JNIEnv *env, Path path_relative) noexcept;
};

#endif
