/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <type_traits>
#include "gtest/gtest.h"

#define private public
#define protected public

#include "test/mock/core/common/mock_theme_manager.h"
#include "test/mock/core/pipeline/mock_pipeline_context.h"
#include "test/mock/core/common/mock_container.h"
#include "test/mock/core/render/mock_render_context.h"
#include "test/mock/core/rosen/mock_canvas.h"
#include "test/mock/core/rosen/testing_canvas.h"

#include "core/components/common/layout/constants.h"
#include "core/components/common/layout/grid_system_manager.h"
#include "core/components/common/properties/shadow_config.h"
#include "core/components/container_modal/container_modal_constants.h"
#include "core/components/select/select_theme.h"
#include "core/components/theme/shadow_theme.h"
#include "core/components_ng/base/view_stack_processor.h"
#include "core/components_ng/pattern/image/image_layout_property.h"
#include "core/components_ng/pattern/image/image_pattern.h"
#include "core/components_ng/pattern/menu/menu_item/menu_item_model_ng.h"
#include "core/components_ng/pattern/menu/menu_item/menu_item_pattern.h"
#include "core/components_ng/pattern/menu/menu_item_group/menu_item_group_pattern.h"
#include "core/components_ng/pattern/menu/menu_item_group/menu_item_group_view.h"
#include "core/components_ng/pattern/menu/menu_model_ng.h"
#include "core/components_ng/pattern/menu/menu_pattern.h"
#include "core/components_ng/pattern/menu/menu_theme.h"
#include "core/components_ng/pattern/menu/menu_view.h"
#include "core/components_ng/pattern/menu/multi_menu_layout_algorithm.h"
#include "core/components_ng/pattern/menu/preview/menu_preview_layout_algorithm.h"
#include "core/components_ng/pattern/menu/preview/menu_preview_pattern.h"
#include "core/components_ng/pattern/menu/sub_menu_layout_algorithm.h"
#include "core/components_ng/pattern/menu/wrapper/menu_wrapper_pattern.h"
#include "core/components_ng/pattern/pattern.h"
#include "core/components_ng/pattern/root/root_pattern.h"
#include "core/components_ng/pattern/scroll/scroll_pattern.h"
#include "core/components_ng/pattern/text/text_layout_property.h"
#include "core/components_ng/pattern/text/text_pattern.h"
#include "core/components_ng/property/border_property.h"
#include "core/components_ng/property/measure_property.h"
#include "core/components_ng/syntax/lazy_for_each_model.h"
#include "core/components_ng/syntax/lazy_layout_wrapper_builder.h"
#include "core/event/touch_event.h"

using namespace testing;
using namespace testing::ext;
using namespace OHOS::Ace::Framework;

namespace OHOS::Ace::NG {
namespace {
const InspectorFilter filter;
const std::string EMPTY_TEXT = "";
const std::string TEXT_TAG = "text";
const std::string MENU_TAG = "menu";
const std::string MENU_ITEM_TEXT = "menuItem";
const std::string MENU_ITEM_GROUP_TEXT = "menuItemGroup";
const std::string MENU_TOUCH_EVENT_TYPE = "1";
const std::string IMAGE_SRC_URL = "file://data/data/com.example.test/res/example.svg";
constexpr float FULL_SCREEN_WIDTH = 720.0f;
constexpr float FULL_SCREEN_HEIGHT = 1136.0f;
constexpr float TARGET_SIZE_WIDTH = 100.0f;
constexpr float TARGET_SIZE_HEIGHT = 100.0f;
constexpr float MENU_SIZE_WIDTH = 150.0f;
constexpr float MENU_SIZE_HEIGHT = 150.0f;
constexpr double MENU_OFFSET_X = 10.0;
constexpr double MENU_OFFSET_Y = 10.0;
constexpr float MENU_ITEM_SIZE_WIDTH = 100.0f;
constexpr float SUBMENU_SIZE_HEIGHT = 200.0f;
constexpr float SUBMENU_SIZE_HEIGHT_NEW = 90.0f;
constexpr float MENU_ITEM_WIDTH = 50.0f;
constexpr float MENU_ITEM_HEIGHT = 20.0f;
constexpr float MENU_ITEM_WRAPPER_HEIGHT = 300.0f;
constexpr float MENU_ITEM_WRAPPER_WIDTH = 200.0f;
constexpr float MENU_ITEM_WRAPPER_X = 100.0f;
constexpr float MENU_ITEM_WRAPPER_Y = 300.0f;
constexpr float MENU_ITEM_WRAPPER_Y_NEW = 100.0f;
constexpr float THREE_HUNDRED_FIFTY = 350.0f;
constexpr float THREE_HUNDRED_TEN = 310.0f;
constexpr float ONE = 1.0f;
constexpr float TWENTY = 20.0f;
constexpr float ZERO = 0.0f;
constexpr float FOUR = 4.0f;
constexpr float ONE_HUNDRED = 100.0f;
constexpr float NINETY = 90.0f;
constexpr float FORTY = 40.0f;
constexpr float HORIZONTAL_SIZE_WIDTH = 110.0f;
constexpr float HORIZONTAL_SIZE_HEIGHT = 200.0f;
constexpr float HORIZONTAL_SIZE_WIDTH_NEW = 200.0f;
constexpr float HORIZONTAL_WRAPPER_SIZE_WIDTH = 200.0f;
constexpr float MENU_ITEM_WIDTH_NEW = 50.0f;
constexpr float MENU_ITEM_HEIGHT_NEW = 20.0f;
constexpr int NODE_ID = 1;
const SizeF FULL_SCREEN_SIZE(FULL_SCREEN_WIDTH, FULL_SCREEN_HEIGHT);
const std::vector<std::string> FONT_FAMILY_VALUE = {"cursive"};
const std::vector<SelectParam> CREATE_VALUE = { { "content1", "icon1" }, { "content2", "" },
    { "", "icon3" }, { "", "" } };
const std::vector<SelectParam> CREATE_VALUE_NEW = { { "content1_new", "" }, { "", "icon4_new" },
    { "", "" }, { "", "icon4_new" } };
} // namespace
class SubMenuTestNg : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp() override;
    void TearDown() override;
    PaintWrapper* GetPaintWrapper(RefPtr<MenuPaintProperty> paintProperty);
    RefPtr<FrameNode> GetPreviewMenuWrapper(
        SizeF itemSize = SizeF(0.0f, 0.0f), std::optional<MenuPreviewAnimationOptions> scaleOptions = std::nullopt);
    RefPtr<FrameNode> menuFrameNode_;
    RefPtr<MenuAccessibilityProperty> menuAccessibilityProperty_;
    RefPtr<FrameNode> menuItemFrameNode_;
    RefPtr<MenuItemPattern> menuItemPattern_;
    RefPtr<MenuItemAccessibilityProperty> menuItemAccessibilityProperty_;
};

void SubMenuTestNg::SetUpTestCase() {}

void SubMenuTestNg::TearDownTestCase() {}

void SubMenuTestNg::SetUp()
{
    MockPipelineContext::SetUp();
    MockContainer::SetUp();
    auto themeManager = AceType::MakeRefPtr<MockThemeManager>();
    MockPipelineContext::GetCurrent()->SetThemeManager(themeManager);
    RefPtr<MenuTheme> menuTheme_ = AceType::MakeRefPtr<MenuTheme>();
    EXPECT_CALL(*themeManager, GetTheme(_)).WillRepeatedly([menuTheme_](ThemeType type) -> RefPtr<Theme> {
        if (type == TextTheme::TypeId()) {
            return AceType::MakeRefPtr<TextTheme>();
        } else if (type == IconTheme::TypeId()) {
            return AceType::MakeRefPtr<IconTheme>();
        } else if (type == SelectTheme::TypeId()) {
            return AceType::MakeRefPtr<SelectTheme>();
        } else {
            return menuTheme_;
        }
    });
}

void SubMenuTestNg::TearDown()
{
    MockPipelineContext::TearDown();
    menuFrameNode_ = nullptr;
    menuAccessibilityProperty_ = nullptr;
    menuItemFrameNode_ = nullptr;
    menuItemPattern_ = nullptr;
    menuItemAccessibilityProperty_ = nullptr;
    SystemProperties::SetDeviceType(DeviceType::PHONE);
    ScreenSystemManager::GetInstance().dipScale_ = 1.0;
    SystemProperties::orientation_ = DeviceOrientation::PORTRAIT;
    MockContainer::TearDown();
}

PaintWrapper* SubMenuTestNg::GetPaintWrapper(RefPtr<MenuPaintProperty> paintProperty)
{
    WeakPtr<RenderContext> renderContext;
    RefPtr<GeometryNode> geometryNode = AceType::MakeRefPtr<GeometryNode>();
    PaintWrapper* paintWrapper = new PaintWrapper(renderContext, geometryNode, paintProperty);
    return paintWrapper;
}

RefPtr<FrameNode> SubMenuTestNg::GetPreviewMenuWrapper(
    SizeF itemSize, std::optional<MenuPreviewAnimationOptions> scaleOptions)
{
    auto rootNode = FrameNode::CreateFrameNode(
        V2::ROOT_ETS_TAG, ElementRegister::GetInstance()->MakeUniqueId(), AceType::MakeRefPtr<RootPattern>());
    CHECK_NULL_RETURN(rootNode, nullptr);
    auto targetNode = FrameNode::CreateFrameNode(
        V2::TEXT_ETS_TAG, ElementRegister::GetInstance()->MakeUniqueId(), AceType::MakeRefPtr<TextPattern>());
    CHECK_NULL_RETURN(targetNode, nullptr);
    auto textNode = FrameNode::CreateFrameNode(
        V2::TEXT_ETS_TAG, ElementRegister::GetInstance()->MakeUniqueId(), AceType::MakeRefPtr<TextPattern>());
    CHECK_NULL_RETURN(textNode, nullptr);
    if (!(LessOrEqual(itemSize.Width(), 0.0) || LessOrEqual(itemSize.Height(), 0.0))) {
        auto itemGeometryNode = textNode->GetGeometryNode();
        CHECK_NULL_RETURN(itemGeometryNode, nullptr);
        itemGeometryNode->SetFrameSize(itemSize);
    }
    targetNode->MountToParent(rootNode);
    MenuParam menuParam;
    menuParam.type = MenuType::CONTEXT_MENU;
    menuParam.previewMode = MenuPreviewMode::CUSTOM;
    if (scaleOptions != std::nullopt) {
        menuParam.previewAnimationOptions = scaleOptions.value();
    }
    auto customNode = FrameNode::CreateFrameNode(
        V2::TEXT_ETS_TAG, ElementRegister::GetInstance()->MakeUniqueId(), AceType::MakeRefPtr<TextPattern>());
    CHECK_NULL_RETURN(customNode, nullptr);
    auto customGeometryNode = customNode->GetGeometryNode();
    CHECK_NULL_RETURN(customGeometryNode, nullptr);
    customGeometryNode->SetFrameSize(SizeF(TARGET_SIZE_WIDTH, TARGET_SIZE_HEIGHT));
    auto menuWrapperNode =
        MenuView::Create(textNode, targetNode->GetId(), V2::TEXT_ETS_TAG, menuParam, true, customNode);
    return menuWrapperNode;
}

/**
 * @tc.name: ShowSubMenu001
 * @tc.desc: Verify submenu layout when the layering parameter is true.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, ShowSubMenu001, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Create a menuWrapperNode and menuNodes.
     * @tc.expected: menuWrapperNode and menuNode are not null.
     */
    std::function<void()> buildFun = []() {
        MenuModelNG MenuModelInstance;
        MenuModelInstance.Create();
    };
    auto wrapperNode =
        FrameNode::CreateFrameNode(V2::MENU_WRAPPER_ETS_TAG, 1, AceType::MakeRefPtr<MenuWrapperPattern>(1));
    auto mainMenu = FrameNode::CreateFrameNode(
        V2::MENU_ETS_TAG, 2, AceType::MakeRefPtr<MenuPattern>(1, TEXT_TAG, MenuType::CONTEXT_MENU));
    auto subMenu = FrameNode::CreateFrameNode(
        V2::MENU_ETS_TAG, 3, AceType::MakeRefPtr<MenuPattern>(1, TEXT_TAG, MenuType::SUB_MENU));
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    menuItemNode->MountToParent(mainMenu);
    mainMenu->MountToParent(wrapperNode);
    subMenu->MountToParent(wrapperNode);
    auto menuItemPattern = menuItemNode->GetPattern<MenuItemPattern>();
    ASSERT_NE(menuItemPattern, nullptr);

    /**
     * @tc.steps: step2. Set sub menu shown and createLayoutAlgorithm.
     * @tc.expected: algorithm is not null.
     */
    menuItemPattern->ShowSubMenu();
    menuItemPattern->SetSubBuilder(buildFun);
    menuItemPattern->SetIsSubMenuShowed(false);
    auto mainMenuPattern = mainMenu->GetPattern<MenuPattern>();
    ASSERT_NE(mainMenuPattern, nullptr);
    mainMenuPattern->SetShowedSubMenu(subMenu);
    auto subMenuPattern = subMenu->GetPattern<MenuPattern>();
    ASSERT_NE(subMenuPattern, nullptr);
    auto algorithm = AceType::DynamicCast<SubMenuLayoutAlgorithm>(subMenuPattern->CreateLayoutAlgorithm());
    ASSERT_TRUE(algorithm);

    /**
     * @tc.cases: case1. the hierarchicalParameter is true.
     * @tc.expected: algorithm is not null and sub menu show correctly.
     */
    algorithm->canExpandCurrentWindow_ = true;
    subMenuPattern->SetParentMenuItem(menuItemNode);
    auto geometryNode = AceType::MakeRefPtr<GeometryNode>();
    geometryNode->SetFrameSize(SizeF(MENU_SIZE_WIDTH, MENU_SIZE_HEIGHT));
    auto layoutProp = AceType::MakeRefPtr<MenuLayoutProperty>();
    auto* wrapper = new LayoutWrapperNode(subMenu, geometryNode, layoutProp);
    algorithm->wrapperSize_ = SizeF(FULL_SCREEN_WIDTH, FULL_SCREEN_HEIGHT);
    algorithm->wrapperRect_ = Rect(0, 0, FULL_SCREEN_WIDTH, FULL_SCREEN_HEIGHT);
    algorithm->Layout(wrapper);
    ASSERT_TRUE(algorithm);

    // @tc.cases: case1. sub menu show on the right side of item
    algorithm->position_ = OffsetF(MENU_OFFSET_X + MENU_ITEM_SIZE_WIDTH, MENU_OFFSET_Y);
    algorithm->Layout(wrapper);
    EXPECT_EQ(wrapper->GetGeometryNode()->GetMarginFrameOffset().GetX(), 0);

    // @tc.cases: case2. sub menu show on the left side of item
    algorithm->position_ = OffsetF(FULL_SCREEN_WIDTH, MENU_OFFSET_Y);
    algorithm->Layout(wrapper);
    EXPECT_EQ(wrapper->GetGeometryNode()->GetMarginFrameOffset().GetX(), 0);
    EXPECT_EQ(wrapper->GetGeometryNode()->GetMarginFrameOffset().GetY(), 0);
}

/**
 * @tc.name: ShowSubMenu002
 * @tc.desc: Verify submenu layout when the layering parameter is false.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, ShowSubMenu002, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Create a menuWrapperNode and menuNodes.
     * @tc.expected: menuWrapperNode and menuNode are not null.
     */
    std::function<void()> buildFun = []() {
        MenuModelNG MenuModelInstance;
        MenuModelInstance.Create();
    };
    auto wrapperNode =
        FrameNode::CreateFrameNode(V2::MENU_WRAPPER_ETS_TAG, 1, AceType::MakeRefPtr<MenuWrapperPattern>(1));
    auto mainMenu = FrameNode::CreateFrameNode(
        V2::MENU_ETS_TAG, 2, AceType::MakeRefPtr<MenuPattern>(1, TEXT_TAG, MenuType::CONTEXT_MENU));
    auto subMenu = FrameNode::CreateFrameNode(
        V2::MENU_ETS_TAG, 3, AceType::MakeRefPtr<MenuPattern>(1, TEXT_TAG, MenuType::SUB_MENU));
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    menuItemNode->MountToParent(mainMenu);
    mainMenu->MountToParent(wrapperNode);
    subMenu->MountToParent(wrapperNode);
    auto menuItemPattern = menuItemNode->GetPattern<MenuItemPattern>();
    ASSERT_NE(menuItemPattern, nullptr);

    /**
     * @tc.steps: step2. Set sub menu shown and createLayoutAlgorithm.
     * @tc.expected: algorithm is not null.
     */
    menuItemPattern->ShowSubMenu();
    menuItemPattern->SetSubBuilder(buildFun);
    menuItemPattern->SetIsSubMenuShowed(false);
    auto mainMenuPattern = mainMenu->GetPattern<MenuPattern>();
    ASSERT_NE(mainMenuPattern, nullptr);
    mainMenuPattern->SetShowedSubMenu(subMenu);
    auto subMenuPattern = subMenu->GetPattern<MenuPattern>();
    ASSERT_NE(subMenuPattern, nullptr);
    auto algorithm = AceType::DynamicCast<SubMenuLayoutAlgorithm>(subMenuPattern->CreateLayoutAlgorithm());
    ASSERT_TRUE(algorithm);

    /**
     * @tc.cases: case1. the hierarchicalParameter is false.
     * @tc.expected: algorithm is not null and sub menu show correctly.
     */
    algorithm->canExpandCurrentWindow_ = false;
    subMenuPattern->SetParentMenuItem(menuItemNode);
    auto geometryNode = AceType::MakeRefPtr<GeometryNode>();
    geometryNode->SetFrameSize(SizeF(MENU_SIZE_WIDTH, MENU_SIZE_HEIGHT));
    auto layoutProp = AceType::MakeRefPtr<MenuLayoutProperty>();
    auto* wrapper = new LayoutWrapperNode(subMenu, geometryNode, layoutProp);
    algorithm->wrapperSize_ = SizeF(FULL_SCREEN_WIDTH, FULL_SCREEN_HEIGHT);
    algorithm->wrapperRect_ = Rect(0, 0, FULL_SCREEN_WIDTH, FULL_SCREEN_HEIGHT);
    algorithm->Layout(wrapper);
    ASSERT_TRUE(algorithm);

    // @tc.cases: case1. sub menu show on the right side of item
    algorithm->position_ = OffsetF(MENU_OFFSET_X + MENU_ITEM_SIZE_WIDTH, MENU_OFFSET_Y);
    algorithm->Layout(wrapper);
    EXPECT_EQ(wrapper->GetGeometryNode()->GetMarginFrameOffset().GetX(), 0);

    // @tc.cases: case2. sub menu show on the left side of item
    algorithm->position_ = OffsetF(FULL_SCREEN_WIDTH, MENU_OFFSET_Y);
    algorithm->Layout(wrapper);
    EXPECT_EQ(wrapper->GetGeometryNode()->GetMarginFrameOffset().GetX(), 0);
    EXPECT_EQ(wrapper->GetGeometryNode()->GetMarginFrameOffset().GetY(), 0);
}

/**
 * @tc.name: ShowSubMenu003
 * @tc.desc: Verify ShowSubMenu.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, ShowSubMenu003, TestSize.Level1)
{
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    auto mainMenu =
        FrameNode::CreateFrameNode(V2::MENU_ETS_TAG, 2, AceType::MakeRefPtr<MenuPattern>(1, TEXT_TAG, MenuType::MENU));
    auto menuItemPattern = menuItemNode->GetPattern<MenuItemPattern>();
    ASSERT_NE(menuItemPattern, nullptr);
    mainMenu->GetGeometryNode()->SetFrameSize(SizeF(100, 100));
    menuItemPattern->ShowSubMenu();
    menuItemPattern->isSubMenuShowed_ = false;
    menuItemPattern->expandingMode_ = SubMenuExpandingMode::EMBEDDED;
    menuItemPattern->ShowSubMenu();
}

/**
 * @tc.name: ShowSubMenu004
 * @tc.desc: Verify ShowSubMenu, CloseMenu.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, ShowSubMenu004, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create cascade menu condition
     * @tc.expected: wrapper and child pattern is not null
     */
    std::function<void()> buildFun = []() {
        MenuModelNG MenuModelInstance;
        MenuModelInstance.Create();
    };
    auto wrapperNode =
        FrameNode::CreateFrameNode(V2::MENU_WRAPPER_ETS_TAG, 1, AceType::MakeRefPtr<MenuWrapperPattern>(1));
    auto mainMenu =
        FrameNode::CreateFrameNode(V2::MENU_ETS_TAG, 2, AceType::MakeRefPtr<MenuPattern>(1, TEXT_TAG, MenuType::MENU));
    auto subMenu = FrameNode::CreateFrameNode(
        V2::MENU_ETS_TAG, 3, AceType::MakeRefPtr<MenuPattern>(1, TEXT_TAG, MenuType::SUB_MENU));
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    auto subMenuParent = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 5, AceType::MakeRefPtr<MenuItemPattern>());
    menuItemNode->MountToParent(mainMenu);
    mainMenu->MountToParent(wrapperNode);
    subMenu->MountToParent(wrapperNode);
    auto menuItemPattern = menuItemNode->GetPattern<MenuItemPattern>();
    ASSERT_NE(menuItemPattern, nullptr);
    menuItemPattern->ShowSubMenu();
    menuItemPattern->SetSubBuilder(buildFun);
    menuItemPattern->SetIsSubMenuShowed(false);
    auto mainMenuPattern = mainMenu->GetPattern<MenuPattern>();
    ASSERT_NE(mainMenuPattern, nullptr);
    mainMenuPattern->SetShowedSubMenu(subMenu);
    auto subMenuPattern = subMenu->GetPattern<MenuPattern>();
    ASSERT_NE(subMenuPattern, nullptr);
    subMenuPattern->SetParentMenuItem(subMenuParent);
    /**
     * @tc.steps: step2. prepare wrapperNode, menuNode, itemNode
     * @tc.expected: itemPattern is not null
     */
    menuItemPattern->ShowSubMenu();
    menuItemPattern->CloseMenu();
    EXPECT_EQ(wrapperNode->GetChildren().size(), 2);
}

/**
 * @tc.name: ShowSubMenu005
 * @tc.desc: Verify Submenu ShowInSubwindow params can follow outter menu.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, ShowSubMenu005, TestSize.Level1)
{
    auto wrapperNode =
        FrameNode::CreateFrameNode(V2::MENU_WRAPPER_ETS_TAG, 1, AceType::MakeRefPtr<MenuWrapperPattern>(1));
    /**
     * @tc.steps: step1. create outter menu and set show in subwindow true
     */
    auto outterMenuNode =
        FrameNode::CreateFrameNode(V2::MENU_ETS_TAG, 2, AceType::MakeRefPtr<MenuPattern>(1, TEXT_TAG, MenuType::MENU));
    auto outterMenuLayoutProps = outterMenuNode->GetLayoutProperty<MenuLayoutProperty>();
    outterMenuLayoutProps->UpdateShowInSubWindow(true);
    /**
     * @tc.steps: step2. create inner menu and set show in subwindow false
     */
    auto innerMenuNode =
        FrameNode::CreateFrameNode(V2::MENU_ETS_TAG, 2, AceType::MakeRefPtr<MenuPattern>(1, TEXT_TAG, MenuType::MENU));
    auto innerMenuLayoutProps = innerMenuNode->GetLayoutProperty<MenuLayoutProperty>();
    innerMenuLayoutProps->UpdateShowInSubWindow(false);
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    menuItemNode->MountToParent(innerMenuNode);
    innerMenuNode->MountToParent(outterMenuNode);
    outterMenuNode->MountToParent(wrapperNode);
    auto menuItemPattern = menuItemNode->GetPattern<MenuItemPattern>();

    /**
     * @tc.steps: step3. call ShowSubMenu to create submenu
     * @tc.expected: expect subMenu's showInSubwindow param is true
     */
    std::function<void()> buildFun = []() {
        MenuModelNG MenuModelInstance;
        MenuModelInstance.Create();
    };
    menuItemPattern->SetSubBuilder(buildFun);
    menuItemPattern->ShowSubMenu();
    auto outterMenuPattern = outterMenuNode->GetPattern<MenuPattern>();

    ASSERT_NE(outterMenuPattern->showedSubMenu_, nullptr);
    auto subMenuLayoutProperty = outterMenuPattern->showedSubMenu_->GetLayoutProperty<MenuLayoutProperty>();
    EXPECT_TRUE(subMenuLayoutProperty->GetShowInSubWindowValue(false));
    /**
     * @tc.steps: step4. clear subMenu and make outterMenu showInSubwindow param false, call ShowSubMenu again.
     * @tc.expected: expect subMenu's showInSubwindow param is false
     */
    menuItemPattern->isSubMenuShowed_ = false;
    outterMenuPattern->showedSubMenu_ = nullptr;
    outterMenuLayoutProps->UpdateShowInSubWindow(false);
    menuItemPattern->ShowSubMenu();
    ASSERT_NE(outterMenuPattern->showedSubMenu_, nullptr);
    subMenuLayoutProperty = outterMenuPattern->showedSubMenu_->GetLayoutProperty<MenuLayoutProperty>();
    EXPECT_FALSE(subMenuLayoutProperty->GetShowInSubWindowValue(false));
}

/**
 * @tc.name: ShowSubMenu006
 * @tc.desc: Test long press MenuItem to open Stack SubMenu.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, ShowSubMenu006, TestSize.Level1)
{
    auto wrapperNode =
        FrameNode::CreateFrameNode(V2::MENU_WRAPPER_ETS_TAG, 1, AceType::MakeRefPtr<MenuWrapperPattern>(1));
    ASSERT_NE(wrapperNode, nullptr);
    /**
     * @tc.steps: step1. create outter menu and set show in subwindow true
     */
    auto outterMenuNode =
        FrameNode::CreateFrameNode(V2::MENU_ETS_TAG, 2, AceType::MakeRefPtr<MenuPattern>(1, TEXT_TAG, MenuType::MENU));
    ASSERT_NE(outterMenuNode, nullptr);
    auto outterMenuLayoutProps = outterMenuNode->GetLayoutProperty<MenuLayoutProperty>();
    ASSERT_NE(outterMenuLayoutProps, nullptr);
    outterMenuLayoutProps->UpdateShowInSubWindow(true);
    /**
     * @tc.steps: step2. create inner menu and set show in subwindow false
     */
    auto innerMenuNode =
        FrameNode::CreateFrameNode(V2::MENU_ETS_TAG, 2, AceType::MakeRefPtr<MenuPattern>(1, TEXT_TAG, MenuType::MENU));
    ASSERT_NE(innerMenuNode, nullptr);
    auto innerMenuLayoutProps = innerMenuNode->GetLayoutProperty<MenuLayoutProperty>();
    ASSERT_NE(innerMenuLayoutProps, nullptr);
    innerMenuLayoutProps->UpdateShowInSubWindow(false);
    innerMenuLayoutProps->UpdateExpandingMode(SubMenuExpandingMode::STACK);
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    ASSERT_NE(menuItemNode, nullptr);
    menuItemNode->MountToParent(innerMenuNode);
    innerMenuNode->MountToParent(outterMenuNode);
    outterMenuNode->MountToParent(wrapperNode);
    auto menuItemPattern = menuItemNode->GetPattern<MenuItemPattern>();
    ASSERT_NE(menuItemPattern, nullptr);
    menuItemPattern->expandingMode_ = innerMenuLayoutProps->GetExpandingMode().value_or(SubMenuExpandingMode::SIDE);

    /**
     * @tc.steps: step3. call ShowSubMenu to create submenu
     * @tc.expected: expect subMenu's showInSubwindow param is true
     */
    std::function<void()> buildFun = []() {
        MenuModelNG MenuModelInstance;
        MenuModelInstance.Create();
    };
    menuItemPattern->SetSubBuilder(buildFun);
    menuItemPattern->ShowSubMenu(ShowSubMenuType::LONG_PRESS);
    auto outterMenuPattern = outterMenuNode->GetPattern<MenuPattern>();
    ASSERT_NE(outterMenuPattern, nullptr);
    auto itemBgColor = menuItemPattern->GetBgBlendColor();
    ASSERT_EQ(itemBgColor, Color::TRANSPARENT);
}

/**
 * @tc.name: UpdateSubmenuExpandingMode001
 * @tc.desc: Verify UpdateSubmenuExpandingMode.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, UpdateSubmenuExpandingMode001, TestSize.Level1)
{
    MenuModelNG MneuModelInstance;
    MenuItemModelNG MneuItemModelInstance;
    MneuModelInstance.Create();
    MneuModelInstance.SetFontSize(Dimension(25.0));
    MneuModelInstance.SetFontColor(Color::RED);
    MneuModelInstance.SetFontWeight(FontWeight::BOLD);

    auto menuItemGroupPattern = AceType::MakeRefPtr<MenuItemPattern>();
    ASSERT_NE(menuItemGroupPattern, nullptr);
    RefPtr<NG::UINode> footerNode;
    footerNode = NG::ViewStackProcessor::GetInstance()->Finish();
    footerNode->tag_ = "V2::MENU_ETS_TAG";
    menuItemGroupPattern->UpdateSubmenuExpandingMode(footerNode);
    EXPECT_FALSE(menuItemGroupPattern->isSelected_);
}

/**
 * @tc.name: FindTouchedEmbeddedMenuItem001
 * @tc.desc: Verify FindTouchedEmbeddedMenuItem.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, FindTouchedEmbeddedMenuItem001, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create cascade menu condition
     * @tc.expected: wrapper and child pattern is not null
     */
    std::function<void()> buildFun = []() {
        MenuModelNG MenuModelInstance;
        MenuModelInstance.Create();
    };
    auto wrapperNode =
        FrameNode::CreateFrameNode(V2::MENU_WRAPPER_ETS_TAG, 1, AceType::MakeRefPtr<MenuWrapperPattern>(1));
    auto mainMenu =
        FrameNode::CreateFrameNode(V2::MENU_ETS_TAG, 2, AceType::MakeRefPtr<MenuPattern>(1, TEXT_TAG, MenuType::MENU));
    auto subMenu = FrameNode::CreateFrameNode(
        V2::MENU_ETS_TAG, 3, AceType::MakeRefPtr<MenuPattern>(1, TEXT_TAG, MenuType::SUB_MENU));
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    auto subMenuParent = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 5, AceType::MakeRefPtr<MenuItemPattern>());
    menuItemNode->MountToParent(mainMenu);
    mainMenu->MountToParent(wrapperNode);
    subMenu->MountToParent(wrapperNode);
    auto menuItemPattern = menuItemNode->GetPattern<MenuItemPattern>();
    ASSERT_NE(menuItemPattern, nullptr);
    menuItemPattern->ShowSubMenu();
    menuItemPattern->SetSubBuilder(buildFun);
    menuItemPattern->SetIsSubMenuShowed(false);
    auto mainMenuPattern = mainMenu->GetPattern<MenuPattern>();
    ASSERT_NE(mainMenuPattern, nullptr);
    mainMenuPattern->SetShowedSubMenu(subMenu);
    auto subMenuPattern = subMenu->GetPattern<MenuPattern>();
    ASSERT_NE(subMenuPattern, nullptr);
    subMenuPattern->SetParentMenuItem(subMenuParent);
    menuItemPattern->expandingMode_ = SubMenuExpandingMode::EMBEDDED;
    menuItemPattern->FindTouchedEmbeddedMenuItem(PointF(MENU_OFFSET_X, MENU_OFFSET_Y));
    menuItemPattern->expandingMode_ = SubMenuExpandingMode::STACK;
    menuItemPattern->isStackSubmenuHeader_ = true;
    menuItemPattern->OnClick();
    menuItemPattern->isExpanded_ = true;
    menuItemPattern->embeddedMenu_ = mainMenu;
    menuItemPattern->FindTouchedEmbeddedMenuItem(PointF(MENU_OFFSET_X, MENU_OFFSET_Y));
}

/**
 * @tc.name: OnClick001
 * @tc.desc: Verify OnClick to show subMenu.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, OnClick001, TestSize.Level1)
{
    std::function<void()> buildFun = []() {
        MenuModelNG MenuModelInstance;
        MenuModelInstance.Create();
    };
    auto wrapperNode =
        FrameNode::CreateFrameNode(V2::MENU_WRAPPER_ETS_TAG, 1, AceType::MakeRefPtr<MenuWrapperPattern>(1));
    ASSERT_NE(wrapperNode, nullptr);
    auto mainMenu =
        FrameNode::CreateFrameNode(V2::MENU_ETS_TAG, 2, AceType::MakeRefPtr<MenuPattern>(1, TEXT_TAG, MenuType::MENU));
    ASSERT_NE(mainMenu, nullptr);
    auto subMenu = FrameNode::CreateFrameNode(
        V2::MENU_ETS_TAG, 3, AceType::MakeRefPtr<MenuPattern>(1, TEXT_TAG, MenuType::SUB_MENU));
    ASSERT_NE(subMenu, nullptr);
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    ASSERT_NE(menuItemNode, nullptr);
    auto subMenuParent = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 5, AceType::MakeRefPtr<MenuItemPattern>());
    ASSERT_NE(subMenuParent, nullptr);
    menuItemNode->MountToParent(mainMenu);
    mainMenu->MountToParent(wrapperNode);
    subMenu->MountToParent(wrapperNode);
    auto menuItemPattern = menuItemNode->GetPattern<MenuItemPattern>();
    ASSERT_NE(menuItemPattern, nullptr);
    menuItemPattern->ShowSubMenu();
    menuItemPattern->SetSubBuilder(buildFun);
    menuItemPattern->SetIsSubMenuShowed(false);
    auto mainMenuPattern = mainMenu->GetPattern<MenuPattern>();
    ASSERT_NE(mainMenuPattern, nullptr);
    mainMenuPattern->SetShowedSubMenu(subMenu);
    auto subMenuPattern = subMenu->GetPattern<MenuPattern>();
    ASSERT_NE(subMenuPattern, nullptr);
    subMenuPattern->SetParentMenuItem(subMenuParent);
    menuItemPattern->expandingMode_ = SubMenuExpandingMode::EMBEDDED;
    menuItemPattern->FindTouchedEmbeddedMenuItem(PointF(MENU_OFFSET_X, MENU_OFFSET_Y));
    menuItemPattern->expandingMode_ = SubMenuExpandingMode::STACK;
    menuItemPattern->OnClick();
    menuItemPattern->isExpanded_ = true;
    menuItemPattern->embeddedMenu_ = mainMenu;
    menuItemPattern->FindTouchedEmbeddedMenuItem(PointF(MENU_OFFSET_X, MENU_OFFSET_Y));
    EXPECT_FALSE(menuItemPattern->isStackSubmenuHeader_);
}

/**
 * @tc.name: OnKeyEvent001
 * @tc.desc: Verify OnKeyEvent to show subMenu.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, OnKeyEvent001, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create item node
     * @tc.expected: pattern is not null
     */
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    auto menuItemPattern = menuItemNode->GetPattern<MenuItemPattern>();
    ASSERT_NE(menuItemPattern, nullptr);
    /**
     * @tc.steps: step2. update event type, excute OnKeyEvent
     * @tc.expected: result as expected
     */
    // use longPressEvent
    KeyEvent longPressEvent(KeyCode::KEY_ESCAPE, KeyAction::LONG_PRESS);
    EXPECT_FALSE(menuItemPattern->OnKeyEvent(longPressEvent));
    // use enterEvent
    KeyEvent enterEvent(KeyCode::KEY_ENTER, KeyAction::DOWN);
    EXPECT_TRUE(menuItemPattern->OnKeyEvent(enterEvent));
    // use rightEvent
    KeyEvent rightEvent(KeyCode::KEY_DPAD_RIGHT, KeyAction::DOWN);
    std::function<void()> buildFun = []() {};
    menuItemPattern->SetSubBuilder(buildFun);
    menuItemPattern->SetIsSubMenuShowed(false);
    EXPECT_TRUE(menuItemPattern->OnKeyEvent(rightEvent));
    menuItemPattern->SetIsSubMenuShowed(true);
    EXPECT_FALSE(menuItemPattern->OnKeyEvent(rightEvent));
    menuItemPattern->SetSubBuilder(nullptr);
    menuItemPattern->SetIsSubMenuShowed(true);
    EXPECT_FALSE(menuItemPattern->OnKeyEvent(rightEvent));
}

/**
 * @tc.name: SubMenuDelayTest001
 * @tc.desc: Verify PostHoverSubMenuTask to show subMenu.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, SubMenuDelayTest001, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create item node
     * @tc.expected: pattern is not null
     */
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    auto menuItemPattern = menuItemNode->GetPattern<MenuItemPattern>();
    ASSERT_NE(menuItemPattern, nullptr);
    /**
     * @tc.steps: step2. call PostHoverSubMenuTask
     * @tc.expected: IsSubMenuShowed is false
     */
    menuItemPattern->PostHoverSubMenuTask();
    EXPECT_FALSE(menuItemPattern->IsSubMenuShowed());
    /**
     * @tc.steps: step3. set build func and call PostHoverSubMenuTask
     * @tc.expected: IsSubMenuShowed is false
     */
    std::function<void()> buildFun = []() {};
    menuItemPattern->SetSubBuilder(buildFun);
    menuItemPattern->PostHoverSubMenuTask();
    EXPECT_FALSE(menuItemPattern->IsSubMenuShowed());
    /**
     * @tc.steps: step4. make isSubMenuShowed_ true and call PostHoverSubMenuTask
     * @tc.expected: IsSubMenuShowed is true
     */
    menuItemPattern->isSubMenuShowed_ = true;
    menuItemPattern->PostHoverSubMenuTask();
    menuItemPattern->showTask_.impl_->callback_();
    EXPECT_TRUE(menuItemPattern->IsSubMenuShowed());
    /**
     * @tc.steps: step5. make isHover_ true and call PostHoverSubMenuTask
     * @tc.expected: IsSubMenuShowed is false
     */
    menuItemPattern->isHover_  = true;
    menuItemPattern->PostHoverSubMenuTask();
    menuItemPattern->showTask_.impl_->callback_();
    menuItemPattern->isSubMenuShowed_ = false;
    menuItemPattern->showTask_.impl_->callback_();
    EXPECT_FALSE(menuItemPattern->IsSubMenuShowed());
    /**
     * @tc.steps: step6. change post time and call PostHoverSubMenuTask
     * @tc.expected: IsSubMenuShowed is false
     */
    auto pipeline = PipelineBase::GetCurrentContext();
    ASSERT_NE(pipeline, nullptr);
    auto menuTheme = pipeline->GetTheme<MenuTheme>();
    menuTheme->subMenuShowDelayDuration_ = 0;
    menuItemPattern->PostHoverSubMenuTask();
    EXPECT_FALSE(menuItemPattern->IsSubMenuShowed());
}

/**
 * @tc.name: SubMenuDelayTest002
 * @tc.desc: Verify CheckHideSubMenuTask to hide subMenu.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, SubMenuDelayTest002, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create item node
     * @tc.expected: pattern is not null
     */
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    auto menuItemPattern = menuItemNode->GetPattern<MenuItemPattern>();
    menuItemPattern->isSubMenuShowed_ = true;
    ASSERT_NE(menuItemPattern, nullptr);
    /**
     * @tc.steps: step2. set lastInnerPosition_
     * @tc.expected: sub menu will hide when call CheckHideSubMenu
     */
    menuItemPattern->isSubMenuShowed_ = true;
    auto mousePoint = PointF(MENU_OFFSET_X, TWENTY);
    auto menuZone = RectF(TWENTY, ZERO, TWENTY, TWENTY);
    menuItemPattern->CheckHideSubMenu([]() {}, mousePoint, menuZone);
    EXPECT_FALSE(menuItemPattern->IsSubMenuShowed());
    /**
     * @tc.steps: step3. change the mouse point
     * @tc.expected: task will post in this condition
     */
    menuItemPattern->isSubMenuShowed_ = true;
    menuItemPattern->lastInnerPosition_ = PointF();
    menuItemPattern->CheckHideSubMenu([]() {}, mousePoint, menuZone);
    EXPECT_FALSE(menuItemPattern->IsSubMenuShowed());
    /**
     * @tc.steps: step4. change the mouse point
     * @tc.expected: task will post in this condition
     */
    menuItemPattern->isSubMenuShowed_ = true;
    menuItemPattern->lastInnerPosition_ = PointF();
    mousePoint = PointF(MENU_OFFSET_X, FOUR);
    menuItemPattern->CheckHideSubMenu([]() {}, mousePoint, menuZone);
    EXPECT_TRUE(menuItemPattern->hideTask_);
    EXPECT_TRUE(menuItemPattern->isSubMenuShowed_);
    /**
     * @tc.steps: step5. change mousePoint
     * @tc.expected: sub menu will hide when call CheckHideSubMenu
     */
    menuItemPattern->lastOutterPosition_ = mousePoint;
    mousePoint = PointF(FOUR, MENU_OFFSET_Y);
    menuItemPattern->CheckHideSubMenu([]() {}, mousePoint, menuZone);
    EXPECT_FALSE(menuItemPattern->isSubMenuShowed_);
    /**
     * @tc.steps: step6. make subMenuHideDelayDuration_ zero
     * @tc.expected: sub menu will hide when call CheckHideSubMenu
     */
    menuItemPattern->hideTask_.Cancel();
    auto pipeline = PipelineBase::GetCurrentContext();
    ASSERT_NE(pipeline, nullptr);
    auto menuTheme = pipeline->GetTheme<MenuTheme>();
    menuTheme->subMenuHideDelayDuration_ = 0;
    menuItemPattern->CheckHideSubMenu([]() {}, mousePoint, menuZone);
    EXPECT_FALSE(menuItemPattern->isSubMenuShowed_);
    /**
     * @tc.steps: step7. change expandingMode_
     * @tc.expected: sub menu will hide when call CheckHideSubMenu
     */
    menuItemPattern->expandingMode_ = SubMenuExpandingMode::EMBEDDED;
    menuItemPattern->isSubMenuShowed_ = true;
    menuItemPattern->CheckHideSubMenu([]() {}, mousePoint, menuZone);
    EXPECT_FALSE(menuItemPattern->IsSubMenuShowed());
}

/**
 * @tc.name: GetSubMenuPosition001
 * @tc.desc: Verify GetSubMenuPosition.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, GetSubMenuPosition001, TestSize.Level1)
{
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    ASSERT_NE(menuItemNode, nullptr);
    auto menuItemPattern = menuItemNode->GetPattern<MenuItemPattern>();
    ASSERT_NE(menuItemPattern, nullptr);
    auto pipelineContext = menuItemNode->GetContextWithCheck();
    ASSERT_NE(pipelineContext, nullptr);
    pipelineContext->windowModal_ = WindowModal::NORMAL;
    SubMenuLayoutAlgorithm subMenuLayoutAlgorithm;
    EXPECT_EQ(subMenuLayoutAlgorithm.GetSubMenuPosition(menuItemNode, SubMenuExpandingMode::STACK).x_, ZERO);
}

/**
 * @tc.name: VerticalLayoutSubMenuHalfScreen001
 * @tc.desc: Verify VerticalLayoutSubMenuHalfScreen.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, VerticalLayoutSubMenuHalfScreen001, TestSize.Level1)
{
    auto pipelineContext = PipelineContext::GetMainPipelineContext();
    ASSERT_NE(pipelineContext, nullptr);
    auto safeAreaManager = pipelineContext->GetSafeAreaManager();
    ASSERT_NE(safeAreaManager, nullptr);
    SizeF size = SizeT(TARGET_SIZE_WIDTH, SUBMENU_SIZE_HEIGHT);
    SizeF menuItemSize = SizeT(MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
    SubMenuLayoutAlgorithm subMenuLayoutAlgorithm;
    subMenuLayoutAlgorithm.position_.y_ = MENU_ITEM_WRAPPER_WIDTH;
    subMenuLayoutAlgorithm.wrapperSize_ = SizeT(TARGET_SIZE_WIDTH, MENU_ITEM_WRAPPER_HEIGHT);
    float position = ONE;
    EXPECT_EQ(subMenuLayoutAlgorithm.VerticalLayoutSubMenuHalfScreen(
            size, position, menuItemSize, nullptr, false, nullptr), TARGET_SIZE_WIDTH);
}

/**
 * @tc.name: VerticalLayoutSubMenuHalfScreen002
 * @tc.desc: Verify VerticalLayoutSubMenuHalfScreen.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, VerticalLayoutSubMenuHalfScreen002, TestSize.Level1)
{
    auto pipelineContext = PipelineContext::GetMainPipelineContext();
    ASSERT_NE(pipelineContext, nullptr);
    auto safeAreaManager = pipelineContext->GetSafeAreaManager();
    ASSERT_NE(safeAreaManager, nullptr);
    SizeF size = SizeT(TARGET_SIZE_WIDTH, SUBMENU_SIZE_HEIGHT);
    SizeF menuItemSize = SizeT(MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
    SubMenuLayoutAlgorithm subMenuLayoutAlgorithm;
    subMenuLayoutAlgorithm.wrapperSize_ = SizeT(TARGET_SIZE_WIDTH, TARGET_SIZE_HEIGHT);
    float position = ONE;
    EXPECT_EQ(subMenuLayoutAlgorithm.VerticalLayoutSubMenuHalfScreen(
        size, position, menuItemSize, nullptr, false, nullptr), ZERO);
}

/**
 * @tc.name: VerticalLayoutSubMenu001
 * @tc.desc: Verify VerticalLayoutSubMenu.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, VerticalLayoutSubMenu001, TestSize.Level1)
{
    auto pipelineContext = PipelineContext::GetMainPipelineContext();
    ASSERT_NE(pipelineContext, nullptr);
    auto safeAreaManager = pipelineContext->GetSafeAreaManager();
    ASSERT_NE(safeAreaManager, nullptr);
    SizeF size = SizeT(TARGET_SIZE_WIDTH, SUBMENU_SIZE_HEIGHT_NEW);
    SizeF menuItemSize = SizeT(MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
    SubMenuLayoutAlgorithm subMenuLayoutAlgorithm;
    subMenuLayoutAlgorithm.wrapperRect_ =
        Rect(MENU_ITEM_WRAPPER_X, MENU_ITEM_WRAPPER_Y, MENU_ITEM_WRAPPER_WIDTH, TARGET_SIZE_HEIGHT);
    float position = THREE_HUNDRED_FIFTY;
    EXPECT_EQ(subMenuLayoutAlgorithm.VerticalLayoutSubMenu(size, position, menuItemSize, nullptr, false, nullptr),
        THREE_HUNDRED_TEN);
}

/**
 * @tc.name: VerticalLayoutSubMenu002
 * @tc.desc: Verify VerticalLayoutSubMenu.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, VerticalLayoutSubMenu002, TestSize.Level1)
{
    auto pipelineContext = PipelineContext::GetMainPipelineContext();
    ASSERT_NE(pipelineContext, nullptr);
    auto safeAreaManager = pipelineContext->GetSafeAreaManager();
    ASSERT_NE(safeAreaManager, nullptr);
    SizeF size = SizeT(TARGET_SIZE_WIDTH, SUBMENU_SIZE_HEIGHT);
    SizeF menuItemSize = SizeT(MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
    SubMenuLayoutAlgorithm subMenuLayoutAlgorithm;
    subMenuLayoutAlgorithm.wrapperRect_ =
        Rect(MENU_ITEM_WRAPPER_X, MENU_ITEM_WRAPPER_Y_NEW, MENU_ITEM_WRAPPER_WIDTH, TARGET_SIZE_HEIGHT);
    float position = ONE;
    EXPECT_EQ(subMenuLayoutAlgorithm.VerticalLayoutSubMenu(size, position, menuItemSize, nullptr, false, nullptr),
        TARGET_SIZE_WIDTH);
}

/**
 * @tc.name: HorizontalLayoutSubMenu001
 * @tc.desc: Verify HorizontalLayoutSubMenu.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, HorizontalLayoutSubMenu001, TestSize.Level1)
{
    auto mainPipeline = PipelineContext::GetMainPipelineContext();
    ASSERT_NE(mainPipeline, nullptr);
    auto safeAreaManager = mainPipeline->GetSafeAreaManager();
    ASSERT_NE(safeAreaManager, nullptr);
    SizeF size = SizeT(HORIZONTAL_SIZE_WIDTH, HORIZONTAL_SIZE_HEIGHT);
    SizeF menuItemSize = SizeT(MENU_ITEM_WIDTH_NEW, MENU_ITEM_HEIGHT_NEW);
    auto frameNode = AceType::MakeRefPtr<FrameNode>("test1", NODE_ID, AceType::MakeRefPtr<Pattern>());
    ASSERT_NE(frameNode, nullptr);
    SubMenuLayoutAlgorithm subMenuLayoutAlgorithm;
    subMenuLayoutAlgorithm.wrapperSize_ = SizeT(HORIZONTAL_WRAPPER_SIZE_WIDTH, TARGET_SIZE_HEIGHT);
    float position = ONE_HUNDRED;
    auto refLayoutWrapper = frameNode->CreateLayoutWrapper();
    ASSERT_NE(refLayoutWrapper, nullptr);
    LayoutWrapper* layoutWrapper = Referenced::RawPtr(refLayoutWrapper);
    ASSERT_NE(layoutWrapper, nullptr);
    EXPECT_EQ(subMenuLayoutAlgorithm.HorizontalLayoutSubMenu(size, position, menuItemSize, layoutWrapper), NINETY);
}

/**
 * @tc.name: HorizontalLayoutSubMenu002
 * @tc.desc: Verify HorizontalLayoutSubMenu.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, HorizontalLayoutSubMenu002, TestSize.Level1)
{
    auto mainPipeline = PipelineContext::GetMainPipelineContext();
    ASSERT_NE(mainPipeline, nullptr);
    auto safeAreaManager = mainPipeline->GetSafeAreaManager();
    ASSERT_NE(safeAreaManager, nullptr);
    SizeF size = SizeT(HORIZONTAL_SIZE_WIDTH, HORIZONTAL_SIZE_HEIGHT);
    SizeF menuItemSize = SizeT(MENU_ITEM_WIDTH_NEW, MENU_ITEM_HEIGHT_NEW);
    auto frameNode = AceType::MakeRefPtr<FrameNode>("test1", NODE_ID, AceType::MakeRefPtr<Pattern>());
    ASSERT_NE(frameNode, nullptr);
    SubMenuLayoutAlgorithm subMenuLayoutAlgorithm;
    subMenuLayoutAlgorithm.wrapperSize_ = SizeT(HORIZONTAL_WRAPPER_SIZE_WIDTH, TARGET_SIZE_HEIGHT);
    float position = HORIZONTAL_WRAPPER_SIZE_WIDTH;
    auto refLayoutWrapper = frameNode->CreateLayoutWrapper();
    ASSERT_NE(refLayoutWrapper, nullptr);
    LayoutWrapper* layoutWrapper = Referenced::RawPtr(refLayoutWrapper);
    ASSERT_NE(layoutWrapper, nullptr);
    EXPECT_EQ(subMenuLayoutAlgorithm.HorizontalLayoutSubMenu(size, position, menuItemSize, layoutWrapper), FORTY);
}

/**
 * @tc.name: HorizontalLayoutSubMenu003
 * @tc.desc: Verify HorizontalLayoutSubMenu.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, HorizontalLayoutSubMenu003, TestSize.Level1)
{
    auto mainPipeline = PipelineContext::GetMainPipelineContext();
    ASSERT_NE(mainPipeline, nullptr);
    auto safeAreaManager = mainPipeline->GetSafeAreaManager();
    ASSERT_NE(safeAreaManager, nullptr);
    SizeF size = SizeT(HORIZONTAL_SIZE_WIDTH_NEW, HORIZONTAL_SIZE_HEIGHT);
    SizeF menuItemSize = SizeT(MENU_ITEM_WIDTH_NEW, MENU_ITEM_HEIGHT_NEW);
    auto frameNode = AceType::MakeRefPtr<FrameNode>("test1", NODE_ID, AceType::MakeRefPtr<Pattern>());
    ASSERT_NE(frameNode, nullptr);
    SubMenuLayoutAlgorithm subMenuLayoutAlgorithm;
    subMenuLayoutAlgorithm.wrapperSize_ = SizeT(TARGET_SIZE_WIDTH, TARGET_SIZE_HEIGHT);
    float position = ONE_HUNDRED;
    auto refLayoutWrapper = frameNode->CreateLayoutWrapper();
    ASSERT_NE(refLayoutWrapper, nullptr);
    LayoutWrapper* layoutWrapper = Referenced::RawPtr(refLayoutWrapper);
    ASSERT_NE(layoutWrapper, nullptr);
    EXPECT_EQ(subMenuLayoutAlgorithm.HorizontalLayoutSubMenu(size, position, menuItemSize, layoutWrapper), ZERO);
}

/**
 * @tc.name: ModifySubMenuWrapper001
 * @tc.desc: Verify ModifySubMenuWrapper.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, ModifySubMenuWrapper001, TestSize.Level1)
{
    auto frameNode = AceType::MakeRefPtr<FrameNode>("test1", NODE_ID, AceType::MakeRefPtr<Pattern>());
    ASSERT_NE(frameNode, nullptr);
    auto refLayoutWrapper = frameNode->CreateLayoutWrapper();
    ASSERT_NE(refLayoutWrapper, nullptr);
    LayoutWrapper* layoutWrapper = Referenced::RawPtr(refLayoutWrapper);
    ASSERT_NE(layoutWrapper, nullptr);
    SubMenuLayoutAlgorithm subMenuLayoutAlgorithm;
    subMenuLayoutAlgorithm.canExpandCurrentWindow_ = false;
    subMenuLayoutAlgorithm.param_.menuWindowRect.width_ = TWENTY;
    subMenuLayoutAlgorithm.ModifySubMenuWrapper(layoutWrapper);
    EXPECT_EQ(subMenuLayoutAlgorithm.wrapperSize_.width_, TWENTY);
}

/**
 * @tc.name: InitializePadding001
 * @tc.desc: Verify InitializePadding.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, InitializePadding001, TestSize.Level1)
{
    auto frameNode = AceType::MakeRefPtr<FrameNode>(
        "test1", NODE_ID, AceType::MakeRefPtr<MenuPattern>(NODE_ID, TEXT_TAG, MenuType::MENU));
    ASSERT_NE(frameNode, nullptr);
    auto refLayoutWrapper = frameNode->CreateLayoutWrapper();
    ASSERT_NE(refLayoutWrapper, nullptr);
    LayoutWrapper* layoutWrapper = Referenced::RawPtr(refLayoutWrapper);
    ASSERT_NE(layoutWrapper, nullptr);
    ASSERT_NE(layoutWrapper->GetHostNode(), nullptr);
    auto menuPattern = layoutWrapper->GetHostNode()->GetPattern<MenuPattern>();
    ASSERT_NE(menuPattern, nullptr);
    menuPattern->frameNode_ = AceType::WeakClaim(AceType::RawPtr(frameNode));
    auto themeManager = AceType::MakeRefPtr<MockThemeManager>();
    ASSERT_NE(themeManager, nullptr);
    MockPipelineContext::GetCurrent()->SetThemeManager(themeManager);
    EXPECT_CALL(*themeManager, GetTheme(_)).WillRepeatedly(Return(AceType::MakeRefPtr<SelectTheme>()));
    EXPECT_CALL(*themeManager, GetTheme(_, _)).WillRepeatedly(Return(AceType::MakeRefPtr<SelectTheme>()));
    menuPattern->type_ = MenuType::SUB_MENU;
    SubMenuLayoutAlgorithm subMenuLayoutAlgorithm;
    AceApplicationInfo::GetInstance().SetApiTargetVersion(static_cast<int32_t>(PlatformVersion::VERSION_TWELVE));
    subMenuLayoutAlgorithm.InitializePadding(layoutWrapper);
    EXPECT_EQ(subMenuLayoutAlgorithm.paddingTop_, ZERO);
}

/**
 * @tc.name: GetSubMenuPosition002
 * @tc.desc: Verify GetSubMenuPosition.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, GetSubMenuPosition002, TestSize.Level1)
{
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    ASSERT_NE(menuItemNode, nullptr);
    auto menuItemPattern = menuItemNode->GetPattern<MenuItemPattern>();
    ASSERT_NE(menuItemPattern, nullptr);
    auto pipelineContext = menuItemNode->GetContextWithCheck();
    ASSERT_NE(pipelineContext, nullptr);
    pipelineContext->windowModal_ = WindowModal::NORMAL;
    SubMenuLayoutAlgorithm subMenuLayoutAlgorithm;
    menuItemNode->geometryNode_->SetFrameSize(SizeF(TWENTY, TWENTY));

    EXPECT_FLOAT_EQ(subMenuLayoutAlgorithm.GetSubMenuPosition(menuItemNode, SubMenuExpandingMode::EMBEDDED).x_, TWENTY);
}

/**
 * @tc.name: CalcStackSubMenuPositionYHalfScreen001
 * @tc.desc: Verify CalcStackSubMenuPositionYHalfScreen.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, CalcStackSubMenuPositionYHalfScreen001, TestSize.Level1)
{
    auto frameNode = AceType::MakeRefPtr<FrameNode>(
        "test1", NODE_ID, AceType::MakeRefPtr<MenuPattern>(NODE_ID, TEXT_TAG, MenuType::MENU));
    ASSERT_NE(frameNode, nullptr);
    auto menuGeometryNode = frameNode->GetGeometryNode();
    ASSERT_NE(menuGeometryNode, nullptr);
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    ASSERT_NE(menuItemNode, nullptr);
    auto menuItemGeometryNode = menuItemNode->GetGeometryNode();
    ASSERT_NE(menuItemGeometryNode, nullptr);
    menuItemNode->MountToParent(frameNode);
    auto refLayoutWrapper = frameNode->CreateLayoutWrapper();
    ASSERT_NE(refLayoutWrapper, nullptr);
    LayoutWrapper* layoutWrapper = Referenced::RawPtr(refLayoutWrapper);
    ASSERT_NE(layoutWrapper, nullptr);
    ASSERT_NE(layoutWrapper->GetHostNode(), nullptr);
    auto menuPattern = layoutWrapper->GetHostNode()->GetPattern<MenuPattern>();
    ASSERT_NE(menuPattern, nullptr);
    menuPattern->frameNode_ = AceType::WeakClaim(AceType::RawPtr(frameNode));
    menuPattern->type_ = MenuType::SUB_MENU;
    menuPattern->UpdateLastPlacement(Placement::TOP_RIGHT);
 
    SubMenuLayoutAlgorithm subMenuLayoutAlgorithm;
    subMenuLayoutAlgorithm.wrapperSize_ = SizeT(HORIZONTAL_WRAPPER_SIZE_WIDTH, TARGET_SIZE_HEIGHT);
    subMenuLayoutAlgorithm.position_ = OffsetF(0.0f, 0.0f);
 
    SizeF size = SizeF(100.0f, 100.0f);
    subMenuLayoutAlgorithm.CalcStackSubMenuPositionYHalfScreen(size, frameNode, menuItemNode);
    EXPECT_EQ(menuPattern->GetLastPlacement(), Placement::TOP_RIGHT);
 
    menuGeometryNode->SetFrameSize(SizeF(100.0f, 200.0f));
    EXPECT_EQ(subMenuLayoutAlgorithm.CalcStackSubMenuPositionYHalfScreen(size, frameNode, menuItemNode), 0.0f);
 
    menuPattern->UpdateLastPlacement(Placement::BOTTOM_RIGHT);
    subMenuLayoutAlgorithm.CalcStackSubMenuPositionYHalfScreen(size, frameNode, menuItemNode);
    EXPECT_EQ(menuPattern->GetLastPlacement(), Placement::BOTTOM_RIGHT);
 
    auto context = MockPipelineContext::GetCurrent();
    ASSERT_NE(context, nullptr);
    subMenuLayoutAlgorithm.canExpandCurrentWindow_ = false;
    context->SetWindowModal(WindowModal::CONTAINER_MODAL);
    subMenuLayoutAlgorithm.wrapperRect_ = Rect(0.0f, 200.0f, 100.0f, 300.0f);
    subMenuLayoutAlgorithm.CalcStackSubMenuPositionYHalfScreen(size, frameNode, menuItemNode);
    EXPECT_EQ(menuPattern->GetLastPlacement(), Placement::BOTTOM_RIGHT);
}
 
/**
 * @tc.name: CalcStackSubMenuPositionYHalfScreenWithPreview001
 * @tc.desc: Verify CalcStackSubMenuPositionYHalfScreenWithPreview.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, CalcStackSubMenuPositionYHalfScreenWithPreview001, TestSize.Level1)
{
    auto frameNode = AceType::MakeRefPtr<FrameNode>(
        "test1", NODE_ID, AceType::MakeRefPtr<MenuPattern>(NODE_ID, TEXT_TAG, MenuType::MENU));
    ASSERT_NE(frameNode, nullptr);
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    ASSERT_NE(menuItemNode, nullptr);
    menuItemNode->MountToParent(frameNode);
    auto refLayoutWrapper = frameNode->CreateLayoutWrapper();
    ASSERT_NE(refLayoutWrapper, nullptr);
    LayoutWrapper* layoutWrapper = Referenced::RawPtr(refLayoutWrapper);
    ASSERT_NE(layoutWrapper, nullptr);
    ASSERT_NE(layoutWrapper->GetHostNode(), nullptr);
    auto menuPattern = layoutWrapper->GetHostNode()->GetPattern<MenuPattern>();
    ASSERT_NE(menuPattern, nullptr);
    menuPattern->frameNode_ = AceType::WeakClaim(AceType::RawPtr(frameNode));
    menuPattern->type_ = MenuType::SUB_MENU;
 
    SubMenuLayoutAlgorithm subMenuLayoutAlgorithm;
    subMenuLayoutAlgorithm.wrapperSize_ = SizeT(HORIZONTAL_WRAPPER_SIZE_WIDTH, TARGET_SIZE_HEIGHT);
    subMenuLayoutAlgorithm.position_ = OffsetF(0.0f, 0.0f);
 
    SizeF size = SizeF(100.0f, 100.0f);
    menuPattern->UpdateLastPlacement(Placement::TOP_RIGHT);
    subMenuLayoutAlgorithm.CalcStackSubMenuPositionYHalfScreenWithPreview(size, frameNode, layoutWrapper);
    EXPECT_EQ(menuPattern->GetLastPlacement(), Placement::TOP_RIGHT);
 
    menuPattern->UpdateLastPlacement(Placement::BOTTOM_RIGHT);
    subMenuLayoutAlgorithm.CalcStackSubMenuPositionYHalfScreenWithPreview(size, frameNode, layoutWrapper);
    EXPECT_EQ(menuPattern->GetLastPlacement(), Placement::BOTTOM_RIGHT);
}
 
/**
 * @tc.name: UpdateHoverRegion001
 * @tc.desc: Verify UpdateHoverRegion.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, UpdateHoverRegion001, TestSize.Level1)
{
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    ASSERT_NE(menuItemNode, nullptr);
    SizeF size(MENU_SIZE_WIDTH, MENU_SIZE_HEIGHT);
    SubMenuLayoutAlgorithm subMenuLayoutAlgorithm;
    subMenuLayoutAlgorithm.canExpandCurrentWindow_ = false;
    auto pipelineContext = menuItemNode->GetContextWithCheck();
    ASSERT_NE(pipelineContext, nullptr);
    pipelineContext->windowModal_ = WindowModal::CONTAINER_MODAL;
    subMenuLayoutAlgorithm.UpdateHoverRegion(menuItemNode, OffsetF(0.0f, 0.0f), size);
 
    auto menuItemPattern = menuItemNode->GetPattern<MenuItemPattern>();
    ASSERT_NE(menuItemPattern, nullptr);
    ASSERT_FALSE(menuItemPattern->hoverRegions_.empty());
}

/**
 * @tc.name: NormalizePositionY001
 * @tc.desc: Verify NormalizePositionY.
 * @tc.type: FUNC
 */
HWTEST_F(SubMenuTestNg, NormalizePositionY001, TestSize.Level1)
{
    auto menuItemNode = FrameNode::CreateFrameNode(V2::MENU_ITEM_ETS_TAG, 4, AceType::MakeRefPtr<MenuItemPattern>());
    ASSERT_NE(menuItemNode, nullptr);
    auto themeManager = AceType::MakeRefPtr<MockThemeManager>();
    ASSERT_NE(themeManager, nullptr);
    MockPipelineContext::GetCurrent()->SetThemeManager(themeManager);
    EXPECT_CALL(*themeManager, GetTheme(_)).WillRepeatedly([](ThemeType type) -> RefPtr<Theme> {
        auto theme = AceType::MakeRefPtr<SelectTheme>();
        theme->expandDisplay_ = true;
        return theme;
    });
    SubMenuLayoutAlgorithm subMenuLayoutAlgorithm;
    EXPECT_EQ(subMenuLayoutAlgorithm.NormalizePositionY(menuItemNode, 0.0f, 10.0f), 9.5f);
}
} // namespace OHOS::Ace::NG
