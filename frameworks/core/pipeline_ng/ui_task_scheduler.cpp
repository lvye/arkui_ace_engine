/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#include "core/pipeline_ng/ui_task_scheduler.h"

#include <unistd.h>

#ifdef FFRT_EXISTS
#include "base/longframe/long_frame_report.h"
#endif
#include "base/perfmonitor/perf_monitor.h"
#include "core/pipeline_ng/pipeline_context.h"

namespace OHOS::Ace::NG {
namespace {
constexpr char LIBFFRT_LIB64_PATH[] = "/system/lib64/ndk/libffrt.z.so";
constexpr int32_t ENDORSE_LAYOUT_COUNT = 2;

#ifndef IS_RELEASE_VERSION
constexpr int32_t SINGLE_FRAME_TIME_NANOSEC = 16600000;
constexpr int32_t NANO_TO_MICRO = 1000;
#endif
} // namespace
uint64_t UITaskScheduler::frameId_ = 0;

UITaskScheduler::UITaskScheduler()
{
    if (access(LIBFFRT_LIB64_PATH, F_OK) == -1) {
        return;
    }
    is64BitSystem_ = true;
}

UITaskScheduler::~UITaskScheduler()
{
    persistAfterLayoutTasks_.clear();
}

void UITaskScheduler::AddDirtyLayoutNode(const RefPtr<FrameNode>& dirty)
{
    CHECK_RUN_ON(UI);
    CHECK_NULL_VOID(dirty);
    dirtyLayoutNodes_.emplace_back(dirty);
}

void UITaskScheduler::AddIgnoreLayoutSafeAreaBundle(IgnoreLayoutSafeAreaBundle&& bundle)
{
    CHECK_RUN_ON(UI);
    ignoreLayoutSafeAreaBundles_.emplace_back(std::move(bundle));
}

void UITaskScheduler::AddLayoutNode(const RefPtr<FrameNode>& layoutNode)
{
    CHECK_RUN_ON(UI);
    CHECK_NULL_VOID(layoutNode);
    layoutNodes_.emplace_back(layoutNode);
}

void UITaskScheduler::SetLayoutNodeRect()
{
    if (layoutNodes_.empty()) {
        return;
    }
    auto layoutNodes = std::move(layoutNodes_);
    LayoutNodesSet layoutNodesSet(layoutNodes.begin(), layoutNodes.end());

    for (auto& layoutNode : layoutNodesSet) {
        if (layoutNode->GetIsFind()) {
            layoutNode->SetIsFind(false);
            continue;
        }
        std::list<RefPtr<FrameNode>> children;
        OffsetF offset;
        layoutNode->GetOneDepthVisibleFrameWithOffset(children, offset);
        for (auto& child : children) {
            child->GetRenderContext()->SetExtraOffset(offset);
        }
    }
}

void UITaskScheduler::AddDirtyRenderNode(const RefPtr<FrameNode>& dirty)
{
    CHECK_RUN_ON(UI);
    CHECK_NULL_VOID(dirty);
    auto result = dirtyRenderNodes_[dirty->GetPageId()].emplace(dirty);
    if (!result.second) {
        LOGW("Fail to emplace %{public}s render node", dirty->GetTag().c_str());
    }
}

void UITaskScheduler::ExpandSafeArea()
{
    auto pipeline = PipelineContext::GetCurrentContext();
    CHECK_NULL_VOID(pipeline);
    auto safeAreaManager = pipeline->GetSafeAreaManager();
    CHECK_NULL_VOID(safeAreaManager);
    safeAreaManager->ExpandSafeArea();
}

void UITaskScheduler::FlushSyncGeometryNodeTasks()
{
    ACE_LAYOUT_SCOPED_TRACE("FlushSyncGeometryNodeTasks");
    ExpandSafeArea();
    SetLayoutNodeRect();
    auto tasks = std::move(syncGeometryNodeTasks_);
    for (auto& task : tasks) {
        if (task) {
            task();
        }
    }
}

void UITaskScheduler::FlushLayoutTask(bool forceUseMainThread)
{
    CHECK_RUN_ON(UI);
    ACE_FUNCTION_TRACE_COMMERCIAL();
    if (dirtyLayoutNodes_.empty()) {
        return;
    }
    if (isLayouting_ && SystemProperties::GetLayoutDetectEnabled()) {
        LOGF_ABORT("you are already in flushing layout!");
    }

#ifdef FFRT_EXISTS
    // Pause GC during long frame
    std::unique_ptr<ILongFrame> longFrame = std::make_unique<ILongFrame>();
    if (is64BitSystem_) {
        ACE_LAYOUT_SCOPED_TRACE("ReportStartEvent");
        longFrame->ReportStartEvent();
    }
#endif

    isLayouting_ = true;
    auto dirtyLayoutNodes = std::move(dirtyLayoutNodes_);
    PageDirtySet dirtyLayoutNodesSet(dirtyLayoutNodes.begin(), dirtyLayoutNodes.end());

    // Priority task creation
    int64_t time = 0;
#ifndef IS_RELEASE_VERSION
    int64_t duration = 0;
#endif
    for (auto&& node : dirtyLayoutNodesSet) {
        // need to check the node is destroying or not before CreateLayoutTask
        if (!node || node->IsInDestroying()) {
            continue;
        }
        time = GetSysTimestamp();
        node->CreateLayoutTask(forceUseMainThread);
        time = GetSysTimestamp() - time;
        if (frameInfo_ != nullptr) {
            frameInfo_->AddTaskInfo(node->GetTag(), node->GetId(), time, FrameInfo::TaskType::LAYOUT);
        }
#ifndef IS_RELEASE_VERSION
        duration += time;
#endif
    }

    while (!ignoreLayoutSafeAreaBundles_.empty()) {
        FlushPostponedLayoutTask(forceUseMainThread);
    }

    FlushSyncGeometryNodeTasks();
#ifdef FFRT_EXISTS
    if (is64BitSystem_) {
        ACE_LAYOUT_SCOPED_TRACE("ReportEndEvent");
        longFrame->ReportEndEvent();
    }
#endif

    isLayouting_ = false;
#ifndef IS_RELEASE_VERSION
    if (duration > SINGLE_FRAME_TIME_NANOSEC) {
        PerfMonitor::GetPerfMonitor()->SetSubHealthInfo("SUBHEALTH", "FlushLayoutTask", duration / NANO_TO_MICRO);
    }
#endif
}

void UITaskScheduler::FlushPostponedLayoutTask(bool forceUseMainThread)
{
    auto ignoreLayoutSafeAreaBundles = std::move(ignoreLayoutSafeAreaBundles_);
    for (auto&& bundle = ignoreLayoutSafeAreaBundles.rbegin(); bundle != ignoreLayoutSafeAreaBundles.rend();
        ++bundle) {
        for (auto&& node : bundle->first) {
            if (!node || node->IsInDestroying()) {
                continue;
            }
            node->CreateLayoutTask(forceUseMainThread, LayoutType::MEASURE_FOR_IGNORE);
        }
        auto&& container = bundle->second;
        if (!container || container->IsInDestroying()) {
            continue;
        }
        container->CreateLayoutTask(forceUseMainThread, LayoutType::LAYOUT_FOR_IGNORE);
    }
}

void UITaskScheduler::FlushRenderTask(bool forceUseMainThread)
{
    CHECK_RUN_ON(UI);
    if (FrameReport::GetInstance().GetEnable()) {
        FrameReport::GetInstance().BeginFlushRender();
    }

    auto dirtyRenderNodes = std::move(dirtyRenderNodes_);
    // Priority task creation
    int64_t time = 0;
    for (auto&& pageNodes : dirtyRenderNodes) {
        ACE_SCOPED_TRACE_COMMERCIAL("FlushRenderTask %zu", pageNodes.second.size());
        for (auto&& node : pageNodes.second) {
            if (!node) {
                continue;
            }
            if (node->IsInDestroying()) {
                // reset RenderDirtyMarked for recycle node
                node->ResetRenderDirtyMarked(false);
                continue;
            }
            time = GetSysTimestamp();
            auto task = node->CreateRenderTask(forceUseMainThread);
            if (task) {
                if (forceUseMainThread || (task->GetTaskThreadType() == MAIN_TASK)) {
                    (*task)();
                    time = GetSysTimestamp() - time;
                    if (frameInfo_ != nullptr) {
                        frameInfo_->AddTaskInfo(node->GetTag(), node->GetId(), time, FrameInfo::TaskType::RENDER);
                    }
                }
            }
        }
    }
}

bool UITaskScheduler::NeedAdditionalLayout()
{
    bool ret = false;
    ElementRegister::GetInstance()->ReSyncGeometryTransition();

    RootDirtyMap dirtyLayoutNodesMap;
    for (auto&& dirty : dirtyLayoutNodes_) {
        dirtyLayoutNodesMap[dirty->GetPageId()].emplace(dirty);
    }

    for (auto&& pageNodes : dirtyLayoutNodesMap) {
        for (auto&& node : pageNodes.second) {
            if (!node || node->IsInDestroying() || !node->GetLayoutProperty()) {
                continue;
            }
            const auto& geometryTransition = node->GetLayoutProperty()->GetGeometryTransition();
            if (geometryTransition != nullptr) {
                ret |= geometryTransition->OnAdditionalLayout(node);
            }
        }
    }
    return ret;
}

void UITaskScheduler::FlushTaskWithCheck(bool triggeredByImplicitAnimation)
{
    layoutWithImplicitAnimation_.push(triggeredByImplicitAnimation);
    if (IsLayouting()) {
        multiLayoutCount_++;
        return;
    }
    FlushTask();
}

void UITaskScheduler::FlushTask()
{
    CHECK_RUN_ON(UI);
    ACE_SCOPED_TRACE("UITaskScheduler::FlushTask");
    // update for first entry from flushVSync
    // and reset to avoid infinite add
    int32_t layoutedCount = 0;
    multiLayoutCount_ = 1;
    singleDirtyNodesToFlush_.clear();
    do {
        if (layoutedCount >= ENDORSE_LAYOUT_COUNT && RequestFrameOnLayoutCountExceeds()) {
            break;
        }
        FlushLayoutTask();
        if (NeedAdditionalLayout()) {
            FlushLayoutTask();
        }
        if (!afterLayoutTasks_.empty()) {
            FlushAfterLayoutTask();
        }
        layoutedCount++;
        multiLayoutCount_--;
        FlushSafeAreaPaddingProcess();
        auto triggeredByImplicitAnimation =
            layoutWithImplicitAnimation_.empty() ? false : layoutWithImplicitAnimation_.front();
        if (!triggeredByImplicitAnimation && !afterLayoutCallbacksInImplicitAnimationTask_.empty()) {
            FlushAfterLayoutCallbackInImplicitAnimationTask();
        }
        if (!layoutWithImplicitAnimation_.empty()) {
            layoutWithImplicitAnimation_.pop();
        }
    } while (multiLayoutCount_ > 0);
    // abandon unused params
    layoutWithImplicitAnimation_ = std::queue<bool>();
    FlushAllSingleNodeTasks();
    multiLayoutCount_ = 0;
    ElementRegister::GetInstance()->ClearPendingRemoveNodes();
    FlushRenderTask();
}

void UITaskScheduler::FlushAllSingleNodeTasks()
{
    // handle case of components executing FlushUITaskWithSingleDirtyNode during FlushLayoutTask
    if (singleDirtyNodesToFlush_.empty()) {
        return;
    }
    ACE_SCOPED_TRACE("Flush after-layout singleNode task, count %zu", singleDirtyNodesToFlush_.size());
    auto pipeline = PipelineContext::GetCurrentContextPtrSafelyWithCheck();
    CHECK_NULL_VOID(pipeline);
    auto singleDirtyNodes = std::move(singleDirtyNodesToFlush_);
    for (const auto& node : singleDirtyNodes) {
        // skip if already flushed by any previous tasks
        if (!node || (!node->IsLayoutDirtyMarked() && !node->CheckNeedForceMeasureAndLayout())) {
            continue;
        }
        pipeline->FlushUITaskWithSingleDirtyNode(node);
    }
}

void UITaskScheduler::AddSingleNodeToFlush(const RefPtr<FrameNode>& dirtyNode)
{
    singleDirtyNodesToFlush_.insert(dirtyNode);
}

bool UITaskScheduler::RequestFrameOnLayoutCountExceeds()
{
    auto pipeline = PipelineContext::GetCurrentContextPtrSafelyWithCheck();
    if (pipeline) {
        pipeline->RequestFrame();
    }
    return true;
}

void UITaskScheduler::AddSafeAreaPaddingProcessTask(FrameNode* node)
{
    safeAreaPaddingProcessTasks_.insert(node);
}

void UITaskScheduler::RemoveSafeAreaPaddingProcessTask(FrameNode* node)
{
    safeAreaPaddingProcessTasks_.erase(node);
}

void UITaskScheduler::FlushSafeAreaPaddingProcess()
{
    if (safeAreaPaddingProcessTasks_.empty()) {
        return;
    }
    auto safeAreaPaddingProcessTasks = safeAreaPaddingProcessTasks_;
    auto iter = safeAreaPaddingProcessTasks.begin();
    while (iter != safeAreaPaddingProcessTasks.end()) {
        auto node = *iter;
        if (!node) {
            iter = safeAreaPaddingProcessTasks.erase(iter);
        } else {
            node->ProcessSafeAreaPadding();
            ++iter;
        }
    }
    // clear caches after all process tasks
    iter = safeAreaPaddingProcessTasks.begin();
    while (iter != safeAreaPaddingProcessTasks.end()) {
        auto node = *iter;
        if (node) {
            const auto& geometryNode = node->GetGeometryNode();
            if (geometryNode) {
                geometryNode->ResetAccumulatedSafeAreaPadding();
            }
        }
        ++iter;
    }

    auto eraseIter = safeAreaPaddingProcessTasks_.begin();
    while (eraseIter != safeAreaPaddingProcessTasks_.end()) {
        auto node = *eraseIter;
        if (!node) {
            eraseIter = safeAreaPaddingProcessTasks_.erase(eraseIter);
        } else {
            ++eraseIter;
        }
    }
}

void UITaskScheduler::AddPredictTask(PredictTask&& task)
{
    predictTask_.push_back(std::move(task));
}

void UITaskScheduler::FlushPredictTask(int64_t deadline, bool canUseLongPredictTask)
{
    decltype(predictTask_) tasks(std::move(predictTask_));
    for (const auto& task : tasks) {
        if (task) {
            task(deadline, canUseLongPredictTask);
        }
    }
}

void UITaskScheduler::CleanUp()
{
    dirtyLayoutNodes_.clear();
    dirtyRenderNodes_.clear();
}

bool UITaskScheduler::isEmpty()
{
    return dirtyLayoutNodes_.empty() && dirtyRenderNodes_.empty();
}

bool UITaskScheduler::IsPredictTaskEmpty()
{
    return predictTask_.empty();
}

void UITaskScheduler::AddAfterLayoutTask(std::function<void()>&& task, bool isFlushInImplicitAnimationTask)
{
    if (isFlushInImplicitAnimationTask) {
        afterLayoutCallbacksInImplicitAnimationTask_.emplace_back(std::move(task));
    } else {
        afterLayoutTasks_.emplace_back(std::move(task));
    }
}

void UITaskScheduler::AddPersistAfterLayoutTask(std::function<void()>&& task)
{
    persistAfterLayoutTasks_.emplace_back(std::move(task));
    LOGI("AddPersistAfterLayoutTask size: %{public}u", static_cast<uint32_t>(persistAfterLayoutTasks_.size()));
}

void UITaskScheduler::FlushAfterLayoutTask()
{
    decltype(afterLayoutTasks_) tasks(std::move(afterLayoutTasks_));
    for (const auto& task : tasks) {
        if (task) {
            task();
        }
    }
    // flush correct rect again and flush dirty node again
    FlushPersistAfterLayoutTask();
}

void UITaskScheduler::FlushAfterLayoutCallbackInImplicitAnimationTask()
{
    decltype(afterLayoutCallbacksInImplicitAnimationTask_) tasks(
        std::move(afterLayoutCallbacksInImplicitAnimationTask_));
    for (const auto& task : tasks) {
        if (task) {
            task();
        }
    }
}

void UITaskScheduler::FlushPersistAfterLayoutTask()
{
    // only execute after layout
    if (persistAfterLayoutTasks_.empty()) {
        return;
    }
    ACE_SCOPED_TRACE("UITaskScheduler::FlushPersistAfterLayoutTask");
    for (const auto& task : persistAfterLayoutTasks_) {
        if (task) {
            task();
        }
    }
}

void UITaskScheduler::AddAfterRenderTask(std::function<void()>&& task)
{
    afterRenderTasks_.emplace_back(std::move(task));
}

void UITaskScheduler::FlushAfterRenderTask()
{
    ACE_SCOPED_TRACE("UITaskScheduler::FlushAfterRenderTask");
    decltype(afterRenderTasks_) tasks(std::move(afterRenderTasks_));
    for (const auto& task : tasks) {
        if (task) {
            task();
        }
    }
}

} // namespace OHOS::Ace::NG
