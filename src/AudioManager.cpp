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

#include "AudioManager.h"
#include "Activity.h"
#include "ClassLoader.h"

#include "jutils-details.hpp"

#include <algorithm>

using namespace jni;

int CJNIAudioManager::STREAM_MUSIC(3);

int CJNIAudioManager::AUDIOFOCUS_GAIN(0x00000001);
int CJNIAudioManager::AUDIOFOCUS_GAIN_TRANSIENT(0x00000002);
int CJNIAudioManager::AUDIOFOCUS_GAIN_TRANSIENT_EXCLUSIVE(0x00000004);
int CJNIAudioManager::AUDIOFOCUS_GAIN_TRANSIENT_MAY_DUCK(0x00000003);
int CJNIAudioManager::AUDIOFOCUS_LOSS(0xffffffff);
int CJNIAudioManager::AUDIOFOCUS_LOSS_TRANSIENT(0xfffffffe);
int CJNIAudioManager::AUDIOFOCUS_LOSS_TRANSIENT_CAN_DUCK(0xfffffffd);
int CJNIAudioManager::AUDIOFOCUS_REQUEST_GRANTED(0x00000001);
int CJNIAudioManager::AUDIOFOCUS_REQUEST_FAILED(0x00000000);

int CJNIAudioManager::GET_DEVICES_ALL(0x00000003);
int CJNIAudioManager::GET_DEVICES_INPUTS(0x00000001);
int CJNIAudioManager::GET_DEVICES_OUTPUTS(0x00000002);

int CJNIAudioManager::AUDIO_SESSION_ID_GENERATE(0);

void CJNIAudioManager::PopulateStaticFields()
{
  jhclass clazz = find_class("android/media/AudioManager");
  STREAM_MUSIC = (get_static_field<int>(clazz, "STREAM_MUSIC"));
  AUDIOFOCUS_GAIN = (get_static_field<int>(clazz, "AUDIOFOCUS_GAIN"));
  AUDIOFOCUS_GAIN_TRANSIENT = (get_static_field<int>(clazz, "AUDIOFOCUS_GAIN_TRANSIENT"));
  AUDIOFOCUS_GAIN_TRANSIENT_EXCLUSIVE = (get_static_field<int>(clazz, "AUDIOFOCUS_GAIN_TRANSIENT_EXCLUSIVE"));
  AUDIOFOCUS_GAIN_TRANSIENT_MAY_DUCK = (get_static_field<int>(clazz, "AUDIOFOCUS_GAIN_TRANSIENT_MAY_DUCK"));
  AUDIOFOCUS_LOSS = (get_static_field<int>(clazz, "AUDIOFOCUS_LOSS"));
  AUDIOFOCUS_LOSS_TRANSIENT = (get_static_field<int>(clazz, "AUDIOFOCUS_LOSS_TRANSIENT"));
  AUDIOFOCUS_LOSS_TRANSIENT_CAN_DUCK = (get_static_field<int>(clazz, "AUDIOFOCUS_LOSS_TRANSIENT_CAN_DUCK"));
  AUDIOFOCUS_REQUEST_GRANTED = (get_static_field<int>(clazz, "AUDIOFOCUS_REQUEST_GRANTED"));
  AUDIOFOCUS_REQUEST_FAILED = (get_static_field<int>(clazz, "AUDIOFOCUS_REQUEST_FAILED"));

  int sdk = CJNIBase::GetSDKVersion();
  if (sdk >=21)
  {
    AUDIO_SESSION_ID_GENERATE = (get_static_field<int>(clazz, "AUDIO_SESSION_ID_GENERATE"));
  }

  if (sdk >= 23)
  {
    GET_DEVICES_ALL = (get_static_field<int>(clazz, "GET_DEVICES_ALL"));
    GET_DEVICES_INPUTS = (get_static_field<int>(clazz, "GET_DEVICES_INPUTS"));
    GET_DEVICES_OUTPUTS = (get_static_field<int>(clazz, "GET_DEVICES_OUTPUTS"));
  }
}

int CJNIAudioManager::getStreamMaxVolume()
{
  return call_method<jint>(m_object,
    "getStreamMaxVolume", "(I)I",
    STREAM_MUSIC);
}

int CJNIAudioManager::getStreamVolume()
{
  return call_method<int>(m_object,
    "getStreamVolume", "(I)I",
    STREAM_MUSIC);
}

void CJNIAudioManager::setStreamVolume(int index /* 0 */, int flags /* NONE */)
{
  call_method<void>(m_object,
    "setStreamVolume", "(III)V",
                    STREAM_MUSIC, index, flags);
}

int CJNIAudioManager::requestAudioFocus(const CJNIAudioFocusRequestClass& request)
{
  return call_method<int>(m_object,
                          "requestAudioFocus",
                          "(Landroid/media/AudioFocusRequest;)I", request.get_raw());
}

int CJNIAudioManager::requestAudioFocus(const CJNIAudioManagerAudioFocusChangeListener& listener, int streamType, int durationHint)
{
  return call_method<int>(m_object,
                          "requestAudioFocus",
                          "(Landroid/media/AudioManager$OnAudioFocusChangeListener;II)I", listener.get_raw(), streamType, durationHint);
}

int CJNIAudioManager::abandonAudioFocusRequest(const CJNIAudioFocusRequestClass& request)
{
  return call_method<int>(m_object,
                          "abandonAudioFocusRequest",
                          "(Landroid/media/AudioFocusRequest;)I", request.get_raw());
}

int CJNIAudioManager::abandonAudioFocus(const CJNIAudioManagerAudioFocusChangeListener& listener)
{
  return call_method<int>(m_object,
                          "abandonAudioFocus",
                          "(Landroid/media/AudioManager$OnAudioFocusChangeListener;)I", listener.get_raw());
}

bool CJNIAudioManager::isBluetoothA2dpOn()
{
  return call_method<jboolean>(m_object,
                               "isBluetoothA2dpOn",
                               "()Z");
}

bool CJNIAudioManager::isWiredHeadsetOn()
{
  return call_method<jboolean>(m_object,
                               "isWiredHeadsetOn",
                               "()Z");
}

CJNIAudioDeviceInfos CJNIAudioManager::getDevices(int flags)
{
  return jcast<CJNIAudioDeviceInfos>(call_method<jhobjectArray>(m_object,
                                "getDevices", "(I)[Landroid/media/AudioDeviceInfo;", flags));
}
