/*
   Copyright (c) 2015, The Linux Foundation. All rights reserved.
   Copyright (C) 2020 The OmniRom Project.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <android-base/properties.h>
#include <android-base/logging.h>
#include <sys/resource.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

using android::base::GetProperty;

void property_override(const std::string& name, const std::string& value) {
	size_t valuelen = value.size();

	prop_info* pi = (prop_info*) __system_property_find(name.c_str());
	if (pi != nullptr) {
		__system_property_update(pi, value.c_str(), valuelen);
	} else {
		int rc = __system_property_add(name.c_str(), name.size(), value.c_str(), valuelen);
		if (rc < 0) {
			LOG(ERROR) << "property_set(\"" << name << "\", \"" << value << "\") failed: "
				<< "__system_property_add failed";
		}
	}
}

void model_property_override(const std::string& device, const std::string& name, const std::string& model) {
	property_override("ro.build.product", device);
	property_override("ro.product.device", device);
	property_override("ro.product.odm.device", device);
	property_override("ro.product.vendor.device", device);
	property_override("ro.product.product.device", device);
	property_override("ro.product.system_ext.device", device);
	property_override("ro.product.system.device", device);
	property_override("ro.product.bootimage.device", device);
	property_override("ro.product.name", name);
	property_override("ro.product.odm.name", name);
	property_override("ro.product.vendor.name", name);
	property_override("ro.product.product.name", name);
	property_override("ro.product.system_ext.name", name);
	property_override("ro.product.system.name", name);
	property_override("ro.product.model", model);
	property_override("ro.product.odm.model", model);
	property_override("ro.product.vendor.model", model);
	property_override("ro.product.product.model", model);
	property_override("ro.product.system_ext.model", model);
	property_override("ro.product.system.model", model);
}

void vendor_load_properties() {
	property_override("ro.bootimage.build.date.utc", "1609430400");
	property_override("ro.build.date.utc", "1609430400");
	const std::string sku = GetProperty("ro.boot.hardware.sku", "");
	if (sku == "mayfly") {
		model_property_override("mayfly", "mayfly", "Xiaomi 12S");
	} else if (sku == "unicorn") {
		model_property_override("unicorn", "unicorn", "Xiaomi 12S Pro");
	} else if (sku == "thor") {
		model_property_override("thor", "thor", "Xiaomi 12S Ultra");
	} else if (sku == "diting") {
		model_property_override("diting", "diting", "Redmi K50 Ultra");
	} else if (sku == "ditingp") {
		model_property_override("diting", "ditingp", "Xiaomi 12T Pro");
	} else {
		model_property_override("unknow", "unknow", "unknow name");
	}
}
