/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "wificond/scanning/scan_result.h"

#include <android-base/logging.h>

#include "wificond/logging_utils.h"
#include "wificond/parcelable_utils.h"

using android::status_t;
using android::OK;
using std::string;

namespace com {
namespace android {
namespace server {
namespace wifi {
namespace wificond {

NativeScanResult::NativeScanResult(std::vector<uint8_t>& ssid_,
                                   std::vector<uint8_t>& bssid_,
                                   std::vector<uint8_t>& info_element_,
                                   uint32_t frequency_,
                                   int32_t signal_mbm_,
                                   uint64_t tsf_,
                                   uint16_t capability_,
                                   bool associated_)
    : ssid(ssid_),
      bssid(bssid_),
      info_element(info_element_),
      frequency(frequency_),
      signal_mbm(signal_mbm_),
      tsf(tsf_),
      capability(capability_),
      associated(associated_) {
}

status_t NativeScanResult::writeToParcel(::android::Parcel* parcel) const {
  RETURN_IF_FAILED(parcel->writeByteVector(ssid));
  RETURN_IF_FAILED(parcel->writeByteVector(bssid));
  RETURN_IF_FAILED(parcel->writeByteVector(info_element));
  RETURN_IF_FAILED(parcel->writeUint32(frequency));
  RETURN_IF_FAILED(parcel->writeInt32(signal_mbm));
  RETURN_IF_FAILED(parcel->writeUint64(tsf));
  // There is no writeUint16() available.
  // Use writeUint32() instead.
  RETURN_IF_FAILED(parcel->writeUint32(capability));
  RETURN_IF_FAILED(parcel->writeInt32(associated ? 1 : 0));
  return ::android::OK;
}

status_t NativeScanResult::readFromParcel(const ::android::Parcel* parcel) {
  RETURN_IF_FAILED(parcel->readByteVector(&ssid));
  RETURN_IF_FAILED(parcel->readByteVector(&bssid));
  RETURN_IF_FAILED(parcel->readByteVector(&info_element));
  RETURN_IF_FAILED(parcel->readUint32(&frequency));
  RETURN_IF_FAILED(parcel->readInt32(&signal_mbm));
  RETURN_IF_FAILED(parcel->readUint64(&tsf));
  // There is no readUint16() available.
  // Use readUint32() instead.
  capability = static_cast<uint16_t>(parcel->readUint32());
  associated = (parcel->readInt32() != 0);
  return ::android::OK;
}

void NativeScanResult::DebugLog() {
  LOG(INFO) << "Scan result:";
  // |ssid| might be an encoded array but we just print it as ASCII here.
  string ssid_str(ssid.data(), ssid.data() + ssid.size());
  LOG(INFO) << "SSID: " << ssid_str;

  LOG(INFO) << "BSSID: "
            << ::android::wificond::LoggingUtils::GetMacString(bssid);
  LOG(INFO) << "FREQUENCY: " << frequency;
  LOG(INFO) << "SIGNAL: " << signal_mbm/100 << "dBm";
  LOG(INFO) << "TSF: " << tsf;
  LOG(INFO) << "CAPABILITY: " << capability;
  LOG(INFO) << "ASSOCIATED: " << associated;

}

}  // namespace wificond
}  // namespace wifi
}  // namespace server
}  // namespace android
}  // namespace com
