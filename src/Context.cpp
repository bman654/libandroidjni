/*
 *      Copyright (C) 2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "Context.h"
#include "PackageManager.h"
#include <android/log.h>
#include "Intent.h"
#include "IntentFilter.h"
#include "ClassLoader.h"
#include "jutils-details.hpp"
#include "BroadcastReceiver.h"
#include "JNIThreading.h"
#include "ApplicationInfo.h"
#include "File.h"
#include "ContentResolver.h"
#include "BaseColumns.h"
#include "MediaStore.h"
#include "PowerManager.h"
#include "Cursor.h"
#include "ConnectivityManager.h"
#include "AudioFormat.h"
#include "AudioAttributes.h"
#include "AudioManager.h"
#include "AudioTrack.h"
#include "Surface.h"
#include "MediaCodec.h"
#include "MediaCodecInfo.h"
#include "MediaFormat.h"
#include "Window.h"
#include "View.h"
#include "Build.h"
#include "DisplayMetrics.h"
#include "Intent.h"
#include "KeyEvent.h"
#include "Settings.h"
#include "Environment.h"
#include "Document.h"
#include "RecognizerIntent.h"
#include "AudioDeviceInfo.h"
#include "MediaSync.h"
#include "IBinder.h"
#include "MediaMetadata.h"
#include "PlaybackState.h"
#include "URI.h"
#include "SpeechRecognizer.h"
#include "MediaCodecList.h"
#include "WindowManager.h"
#include "Resources.h"

#include <android/native_activity.h>

using namespace jni;

jhobject CJNIContext::m_context(0);

std::string CJNIContext::CONNECTIVITY_SERVICE;
std::string CJNIContext::NSD_SERVICE;
std::string CJNIContext::ACTIVITY_SERVICE;

CJNIContext::CJNIContext(const ANativeActivity *nativeActivity)
{
  m_context.reset(nativeActivity->clazz);
  xbmc_jni_on_load(nativeActivity->vm, nativeActivity->env);
  CJNIBase::SetSDKVersion(nativeActivity->sdkVersion);
  PopulateStaticFields();
}

CJNIContext::~CJNIContext()
{
}

void CJNIContext::PopulateStaticFields()
{
  jhclass clazz = find_class("android/content/Context");
  CONNECTIVITY_SERVICE = jcast<std::string>(get_static_field<jhstring>(clazz,"CONNECTIVITY_SERVICE"));
  NSD_SERVICE = jcast<std::string>(get_static_field<jhstring>(clazz,"NSD_SERVICE"));
  ACTIVITY_SERVICE = jcast<std::string>(get_static_field<jhstring>(clazz,"ACTIVITY_SERVICE"));

  CJNIBaseColumns::PopulateStaticFields();
  CJNIMediaStoreMediaColumns::PopulateStaticFields();
  CJNIMediaStore::PopulateStaticFields();
  CJNIPowerManager::PopulateStaticFields();
  CJNIPackageManager::PopulateStaticFields();
  CJNICursor::PopulateStaticFields();
  CJNIContentResolver::PopulateStaticFields();
  CJNIConnectivityManager::PopulateStaticFields();
  CJNIAudioFormat::PopulateStaticFields();
  CJNIAudioAttributes::PopulateStaticFields();
  CJNIAudioManager::PopulateStaticFields();
  CJNIAudioTrack::PopulateStaticFields();
  CJNISurface::PopulateStaticFields();
  CJNIMediaCodec::PopulateStaticFields();
  CJNIMediaCodecInfoCodecProfileLevel::PopulateStaticFields();
  CJNIMediaCodecInfoCodecCapabilities::PopulateStaticFields();
  CJNIMediaFormat::PopulateStaticFields();
  CJNIView::PopulateStaticFields();
  CJNIBuild::PopulateStaticFields();
  CJNIDisplayMetrics::PopulateStaticFields();
  CJNIIntent::PopulateStaticFields();
  CJNIKeyEvent::PopulateStaticFields();
  CJNISettings::PopulateStaticFields();
  CJNIEnvironment::PopulateStaticFields();
  CJNIDocument::PopulateStaticFields();
  CJNIRecognizerIntent::PopulateStaticFields();
  CJNIAudioDeviceInfo::PopulateStaticFields();
  CJNIViewInputDevice::PopulateStaticFields();
  CJNIMediaSync::PopulateStaticFields();
  CJNIIBinder::PopulateStaticFields();
  CJNIMediaMetadata::PopulateStaticFields();
  CJNIPlaybackState::PopulateStaticFields();
  CJNIURI::PopulateStaticFields();
  CJNISpeechRecognizer::PopulateStaticFields();
  CJNIMediaCodecList::PopulateStaticFields();
  CJNIWindowManagerLayoutParams::PopulateStaticFields();
}

CJNIPackageManager CJNIContext::GetPackageManager()
{
  return call_method<jhobject>(m_context,
    "getPackageManager", "()Landroid/content/pm/PackageManager;");
}

void CJNIContext::startActivity(const CJNIIntent &intent)
{
  call_method<void>(m_context,
    "startActivity", "(Landroid/content/Intent;)V",
    intent.get_raw());
}

int CJNIContext::checkCallingOrSelfPermission(const std::string &permission)
{
  return call_method<int>(m_context,
    "checkCallingOrSelfPermission", "(Ljava/lang/String;)I",
    jcast<jhstring>(permission));
}

jhobject CJNIContext::getSystemService(const std::string &service)
{
  return call_method<jhobject>(m_context,
    "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;",
    jcast<jhstring>(service));
}

CJNIIntent CJNIContext::registerReceiver(const CJNIBroadcastReceiver &receiver, const CJNIIntentFilter &filter)
{
  return call_method<jhobject>(m_context,
    "registerReceiver", "(Landroid/content/BroadcastReceiver;Landroid/content/IntentFilter;)Landroid/content/Intent;",
    receiver.get_raw(), filter.get_raw());
}

CJNIIntent CJNIContext::registerReceiver(const CJNIIntentFilter &filter)
{
  return call_method<jhobject>(m_context,
    "registerReceiver", "(Landroid/content/BroadcastReceiver;Landroid/content/IntentFilter;)Landroid/content/Intent;",
    (jobject)NULL, filter.get_raw());
}

void CJNIContext::unregisterReceiver(const CJNIBroadcastReceiver &receiver)
{
  call_method<void>(m_context,
    "unregisterReceiver", "(Landroid/content/BroadcastReceiver;)V",
    receiver.get_raw());
}

void CJNIContext::sendBroadcast(const CJNIIntent &intent)
{
  call_method<void>(m_context,
    "sendBroadcast", "(Landroid/content/Intent;)V",
    intent.get_raw());
}

CJNIIntent CJNIContext::getIntent()
{
  return call_method<jhobject>(m_context,
    "getIntent", "()Landroid/content/Intent;");
}

CJNIClassLoader CJNIContext::getClassLoader()
{
  return call_method<jhobject>(m_context,
    "getClassLoader", "()Ljava/lang/ClassLoader;");
}

CJNIApplicationInfo CJNIContext::getApplicationInfo()
{
  return call_method<jhobject>(m_context,
    "getApplicationInfo", "()Landroid/content/pm/ApplicationInfo;");
}

std::string CJNIContext::getPackageName()
{
  return jcast<std::string>(call_method<jhstring>(m_context,
    "getPackageName", "()Ljava/lang/String;"));
}

std::string CJNIContext::getPackageResourcePath()
{
  return jcast<std::string>(call_method<jhstring>(m_context,
    "getPackageResourcePath", "()Ljava/lang/String;"));
}

CJNIFile CJNIContext::getCacheDir()
{
  return call_method<jhobject>(m_context,
    "getCacheDir", "()Ljava/io/File;");
}

CJNIFile CJNIContext::getDir(const std::string &path, int mode)
{
  return call_method<jhobject>(m_context,
    "getDir", "(Ljava/lang/String;I)Ljava/io/File;",
    jcast<jhstring>(path), mode);
}

CJNIFile CJNIContext::getExternalFilesDir(const std::string &path)
{
  return call_method<jhobject>(m_context,
    "getExternalFilesDir", "(Ljava/lang/String;)Ljava/io/File;",
    jcast<jhstring>(path));
}

std::vector<CJNIFile> CJNIContext::getExternalFilesDirs(const std::string &type)
{
  JNIEnv *env = xbmc_jnienv();
  jhobjectArray oFiles = call_method<jhobjectArray>(m_context,
    "getExternalFilesDirs", "(Ljava/lang/String;)[Ljava/io/File;",
    jcast<jhstring>(type));
  
  jsize size = env->GetArrayLength(oFiles.get());
  std::vector<CJNIFile> files;
  files.reserve(size);
  for(int i = 0; i < size; i++)
    files.push_back(CJNIFile(jhobject(env->GetObjectArrayElement(oFiles.get(), i))));

  return files;
}

CJNIContentResolver CJNIContext::getContentResolver()
{
  return call_method<jhobject>(m_context,
    "getContentResolver", "()Landroid/content/ContentResolver;");
}

CJNIWindow CJNIContext::getWindow()
{
  return call_method<jhobject>(m_context,
    "getWindow", "()Landroid/view/Window;");
}

CJNIResourcesTheme CJNIContext::getTheme()
{
  return call_method<jhobject>(m_context, "getTheme", "()Landroid/content/res/Resources$Theme;");
}

CJNIResources CJNIContext::getResources()
{
  return call_method<jhobject>(m_context,
    "getResources", "()Landroid/content/res/Resources;");
}
