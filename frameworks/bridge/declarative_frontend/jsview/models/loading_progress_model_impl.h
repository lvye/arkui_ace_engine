/*
 * Copyright (c) 2022-2025 Huawei Device Co., Ltd.
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

#ifndef FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_MODELS_LOADING_PROGRESS_MODEL_IMPL_H
#define FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_MODELS_LOADING_PROGRESS_MODEL_IMPL_H

#include "core/components_ng/pattern/loading_progress/loading_progress_model.h"
#include "bridge/declarative_frontend/jsview/js_interactable_view.h"

namespace OHOS::Ace::Framework {
class LoadingProgressModelImpl : public OHOS::Ace::LoadingProgressModel {
public:
    void Create() override;
    void SetColor(const Color& value) override;
    void SetColorByUser(bool isSetByUser) override {};
    void SetEnableLoading(bool enable) override {};
    void ResetColor() override {};
    void CreateWithResourceObj(LoadingProgressResourceType LoadingProgressResourceType, const RefPtr<ResourceObject>& resObj) override {};
};
} // namespace OHOS::Ace::Framework
#endif // FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_MODELS_LOADING_PROGRESS_MODEL_IMPL_H
