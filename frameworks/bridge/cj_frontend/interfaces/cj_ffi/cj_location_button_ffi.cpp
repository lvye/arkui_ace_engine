/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cj_lambda.h"
#include "bridge/cj_frontend/interfaces/cj_ffi/cj_location_button_ffi.h"
#include "core/components_ng/base/view_abstract_model.h"
#include "core/components_ng/pattern/security_component/location_button/location_button_common.h"
#include "core/components_ng/pattern/security_component/location_button/location_button_model_ng.h"
#include "core/components_ng/pattern/security_component/security_component_theme.h"

using namespace OHOS::Ace;
using namespace OHOS::Ace::Framework;
using OHOS::Ace::NG::LocationButtonModelNG;
using OHOS::Ace::NG::SecurityComponentTheme;

extern "C" {
void FfiOHOSAceFrameworkLocationButtonCreate()
{
    LocationButtonModelNG::GetInstance()->Create(
        static_cast<int32_t>(LocationButtonLocationDescription::CURRENT_LOCATION),
        static_cast<int32_t>(LocationButtonIconStyle::ICON_LINE),
        static_cast<int32_t>(ButtonType::CAPSULE), false);
}

void FfiOHOSAceFrameworkLocationButtonCreateWithButtonOptions(int32_t icon, int32_t text, int32_t buttonType)
{
    LocationButtonModelNG::GetInstance()->Create(text, icon, buttonType, false);
}
}