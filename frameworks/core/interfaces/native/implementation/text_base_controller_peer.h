/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef FOUNDATION_ARKUI_ACE_ENGINE_FRAMEWORKS_CORE_INTERFACES_NATIVE_IMPL_TEXT_BASE_CONTROLLER_PEER_H
#define FOUNDATION_ARKUI_ACE_ENGINE_FRAMEWORKS_CORE_INTERFACES_NATIVE_IMPL_TEXT_BASE_CONTROLLER_PEER_H

#include "core/components_ng/pattern/rich_editor/rich_editor_model.h"

struct TextBaseControllerPeer {
    virtual ~TextBaseControllerPeer() = default;
    virtual void CloseSelectionMenu() = 0;
    virtual void SetSelection(int32_t selectionStart, int32_t selectionEnd,
        const std::optional<OHOS::Ace::SelectionOptions>& options = std::nullopt, bool isForward = false) = 0;
    virtual OHOS::Ace::WeakPtr<OHOS::Ace::NG::LayoutInfoInterface> GetLayoutInfoInterface() = 0;
};
#endif // FOUNDATION_ARKUI_ACE_ENGINE_FRAMEWORKS_CORE_INTERFACES_NATIVE_IMPL_TEXT_BASE_CONTROLLER_PEER_H